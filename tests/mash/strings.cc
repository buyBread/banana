#include <string>
#include <doctest/doctest.h>

#include "mash/fixtures.hh"

namespace mash = treyarch::mash;
using namespace tests::mash_support;

// a mash string is three fields; its length, a pointer to its characters and m_source_slab.
// a string made at runtime owns its characters, which it allocates and frees itself.
// a loaded string just points at characters sitting in the image, so it has to make sure never to free them.
// m_source_slab is how it tells the two apart; a loaded string carries the source_slab address, and a string that does never frees its characters.
// that mark isn't cleared when such a string gets edited, so a few tests below catch newly allocated characters that never get freed.
// the class also has a few other odd habits, mostly around negative lengths and which length a copy goes by; those get pointed out where they show up.
TEST_SUITE("mash") {
    TEST_CASE_FIXTURE(c_string_fixture,
                      "empty strings all share one empty string and are counted while alive") {

        {
            c_string_slot empty;
            c_string_slot null((const char*)nullptr);
            c_string_slot literal("");

            CHECK(string_count == 3);

            for (auto* value : {&*empty, &*null, &*literal}) {
                CHECK(value->empty());
                CHECK(value->size() == 0);
                CHECK(value->length() == 0);
                CHECK(value->c_str() == empty_string());
                CHECK(value->data() == value->c_str());
                CHECK(value->get_mash_sizeof() == 12);
            }

            CHECK(allocations.empty());
        }

        CHECK(string_count == 0);
        CHECK(free_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "an ordinary constructor never touches the source slab field") {

        // that's the reason c_string_slot exists; a string on the stack would start out with junk in this field.
        image_t<64> image;
        void*       marker = &image;
        image.put(8, marker);

        auto* value = ::new (image.at<mash::string>(0)) mash::string;

        CHECK(value->m_source_slab == marker);
        CHECK(value->m_data == empty_string());
        CHECK(value->m_size == 0);

        value->~string();
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "the from-mash constructor leaves every loaded field alone and only counts the string") {

        // the null pointer argument is only a tag that picks this constructor.
        image_t<128> image;
        image.put(0, i32{5});
        image.put(4, image.at<char>(64));
        image.put(8, source_slab);
        std::memcpy(image.bytes.data() + 64, "hello", 6);

        std::array<u8, 12> original;
        std::memcpy(original.data(), image.bytes.data(), 12);

        auto* value = ::new (image.at<mash::string>(0)) mash::string((mash::from_mash_in_place_constructor*)nullptr);

        CHECK(string_count == 1);
        CHECK(std::memcmp(original.data(), image.bytes.data(), 12) == 0);

        value->destruct_mashed_class();

        CHECK(string_count == 0);
        CHECK(free_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "loading a string borrows its characters from the image and realigns the stream after them") {

        for (auto selected : {mash::NORMAL_BUFFER, mash::SHARED_BUFFER}) {
            for (i32 length : {1, 2, 3, 4, 5}) {
                CAPTURE(selected);
                CAPTURE(length);

                // the string's twelve bytes are at 16; its characters follow at 28, or after its length in shared at 132.
                image_t<256> image;
                image.header(128);
                image.put(128, length);

                auto*     value = image.at<mash::string>(16);
                const u32 start = selected == mash::NORMAL_BUFFER ? 28 : 132;
                std::memcpy(image.bytes.data() + start, "abcde", length);
                image.bytes[start + length] = 0;

                mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 256);
                reader.advance_buffer(mash::NORMAL_BUFFER, 12);

                // the length always comes from the shared stream; the characters and their terminating zero come from the selected one.
                value->unmash(&reader, nullptr, selected);

                CHECK(string_count == 0);
                CHECK(value->m_size == length);
                CHECK(value->m_data == image.at<char>(start));
                CHECK(value->m_source_slab == source_slab);

                // after the characters, the cursor rounds up to the next multiple of four.
                const u32 end = (start + length + 1 + 3) / 4 * 4;

                CHECK(reader.get_buffer_size_used(mash::NORMAL_BUFFER) ==
                    (selected == mash::NORMAL_BUFFER ? (i32)end : 28));
                CHECK(reader.get_buffer_size_used(mash::SHARED_BUFFER) ==
                    (selected == mash::SHARED_BUFFER ? (i32)end - 128 : 4));

                // unmash alone doesn't count the string as alive; construct_mashed_class does that.
                value->construct_mashed_class();

                CHECK(string_count == 1);

                value->destruct_mashed_class();

                CHECK(string_count == 0);
                CHECK(value->m_data == empty_string());
                CHECK(value->m_size == length);
                CHECK(value->m_source_slab == source_slab);
                CHECK(allocations.empty());
            }
        }
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "a loaded length of zero or less reads no characters and keeps that length") {

        for (i32 length : {0, -1, -7}) {
            CAPTURE(length);

            image_t<128> image;
            image.header(80);
            image.put(80, length);

            auto* value = image.at<mash::string>(16);

            mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 128);
            reader.advance_buffer(mash::NORMAL_BUFFER, 12);
            value->unmash(&reader, nullptr, mash::NORMAL_BUFFER);

            // a negative length isn't treated as empty, since empty only checks for exactly 0.
            CHECK(value->m_size == length);
            CHECK(value->m_data == empty_string());
            CHECK(value->m_source_slab == nullptr);
            CHECK(value->empty() == (length == 0));
            CHECK(reader.get_buffer_size_used(mash::NORMAL_BUFFER) == 28);
            CHECK(reader.get_buffer_size_used(mash::SHARED_BUFFER) == 4);

            value->construct_mashed_class();
            value->destruct_mashed_class();
        }

        CHECK(allocations.empty());
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "a string made from text copies it and frees the copy when it goes away") {

        char input[] = "hello";

        {
            c_string_slot value(input);

            CHECK(value->m_size == 5);
            CHECK(value->m_data != input);
            CHECK(*value == "hello");

            // changing the original text doesn't change the copy.
            input[0] = 'x';

            CHECK((*value)[0] == 'h');
            CHECK(value->at(5) == '\0');

            const mash::string &constant = *value;

            CHECK(constant.data() == value->data());
            REQUIRE(allocations.size() == 1);
            CHECK(allocations[0].size == 6);
        }

        CHECK(free_count(tests::heap_allocator) == 1);
        CHECK(live_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "the copy constructor stops at the first zero, while assignment and copy go by the stored length") {

        // a string can hold a zero in the middle, so how many characters it holds and where its text ends can differ.
        // this one holds 4 characters, but its text ends after 1.
        const char bytes[]{'a', '\0', 'b', 'c'};

        c_string_slot source;
        source->copy(bytes, 4);

        c_string_slot copied(*source);
        c_string_slot assigned;
        *assigned = *source;
        c_string_slot copied_explicitly;
        copied_explicitly->copy(*source);

        CHECK(copied->size() == 1);
        CHECK(assigned->size() == 4);
        CHECK(copied_explicitly->size() == 4);
        CHECK(std::memcmp(assigned->data(), bytes, 4) == 0);
        CHECK(std::memcmp(copied_explicitly->data(), bytes, 4) == 0);
        CHECK(copied->data() != source->data());
        CHECK(assigned->data() != source->data());
        CHECK(assigned->data()[4] == '\0');
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "the substring constructor cuts the count down to whatever's left") {

        c_string_slot source("abcdef");

        for (i32 count : {mash::string::npos, 20, 4}) {
            CAPTURE(count);

            c_string_slot result(*source, 2, count);

            CHECK(result->size() == 4);
            CHECK(*result == "cdef");
        }

        c_string_slot middle(*source, 2, 2);
        c_string_slot at_end(*source, 6, mash::string::npos);

        CHECK(*middle == "cd");
        CHECK(at_end->empty());
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "number and format constructors print like printf, with floats at three decimals") {

        c_string_slot negative(i32{-123});
        c_string_slot integer(i32{0});
        c_string_slot number(f32{1.25f});
        c_string_slot negative_zero(-0.0f);
        c_string_slot formatted(mash::string::fmt, "%s:%04d", "item", 7);

        CHECK(*negative == "-123");
        CHECK(*integer == "0");
        CHECK(*number == "1.250");
        CHECK(*negative_zero == "-0.000");
        CHECK(*formatted == "item:0007");
        CHECK(mash::string::from_int(42) == "42");
        CHECK(mash::string::from_float(-2.5f) == "-2.500");
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "an owned string reuses its storage until the new text is longer than its current length") {

        c_string_slot value("abcdef");
        char*         original = value->data();
        value->update_guts("xy", 2);

        CHECK(value->data() == original);
        CHECK(value->size() == 2);
        CHECK(value->data()[2] == '\0');
        CHECK(allocation_count(tests::heap_allocator) == 1);

        // the check is against the current length of 2, not the 6 characters there's actually room for, so this reallocates.
        value->update_guts("12345");

        CHECK(value->size() == 5);
        CHECK(allocation_count(tests::heap_allocator) == 2);
        CHECK(free_count(tests::heap_allocator) == 1);
        CHECK(*value == "12345");
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "copying zero or a negative count keeps that count as the length") {

        for (i32 length : {0, -2, -3}) {
            CAPTURE(length);

            c_string_slot value("abcdef");
            value->copy("ignored", length);

            CHECK(value->size() == length);
            CHECK(value->c_str() == empty_string());
            CHECK(value->empty() == (length == 0));
        }

        CHECK(live_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "assigning a string to itself does nothing, and assignment hands back the same string") {

        c_string_slot value("hello");
        char*         original = value->data();

        CHECK(&(*value = *value) == &*value);
        CHECK(value->data() == original);
        CHECK(allocation_count(tests::heap_allocator) == 1);
        CHECK(&(*value = "bye") == &*value);
        CHECK(value->data() == original);
        CHECK(*value == "bye");
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "editing a borrowed string allocates new characters, but the borrowed mark stays and they're never freed") {

        // a local array stands in for characters inside an image.
        char          borrowed[] = "borrowed";
        c_string_slot value((mash::from_mash_in_place_constructor*)nullptr);
        borrow(*value, borrowed);

        value->update_guts("x");

        CHECK(value->m_data != borrowed);
        CHECK(value->m_source_slab == source_slab);
        CHECK(*value == "x");
        CHECK(doctest::String(borrowed) == "borrowed");

        // still marked as borrowed, so this allocates yet again instead of reusing, and the previous allocation is dropped without a free.
        value->update_guts("y");

        CHECK(allocation_count(tests::heap_allocator) == 2);
        CHECK(free_count(tests::heap_allocator) == 0);

        // teardown skips freeing as well; the memory fixture cleans the leftovers up after the test.
        value.release()->destruct_mashed_class();

        CHECK(string_count == 0);
        CHECK(live_count(tests::heap_allocator) == 2);
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "emptying a borrowed string still allocates first, then drops that allocation") {

        // the new characters get allocated before anything checks whether there are any.
        // empty text then switches to the shared empty string, and the one byte allocation is left behind.
        char          borrowed[] = "a";
        c_string_slot value((mash::from_mash_in_place_constructor*)nullptr);
        borrow(*value, borrowed);

        value->copy("", 0);

        REQUIRE(allocations.size() == 1);
        CHECK(allocations[0].size == 1);
        CHECK(value->m_data == empty_string());
        CHECK(value->m_source_slab == source_slab);
        CHECK(free_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "appending to a borrowed string keeps the borrowed mark and leaves the image alone") {

        char          borrowed[] = "abc";
        c_string_slot value((mash::from_mash_in_place_constructor*)nullptr);
        borrow(*value, borrowed);

        value->append("de");

        CHECK(value->size() == 5);
        CHECK(*value == "abcde");
        CHECK(doctest::String(borrowed) == "abc");
        CHECK(value->m_source_slab == source_slab);

        value.release()->destruct_mashed_class();

        CHECK(live_count(tests::heap_allocator) == 1);
        CHECK(free_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "clearing a borrowed string writes into the image instead of allocating") {

        char          borrowed[] = "abc";
        c_string_slot value((mash::from_mash_in_place_constructor*)nullptr);
        borrow(*value, borrowed);

        value->clear();

        CHECK(value->empty());
        CHECK(value->m_data == borrowed);
        CHECK(value->m_source_slab == source_slab);
        CHECK(borrowed[0] == '\0');
        CHECK(borrowed[1] == 'b');
        CHECK(allocations.empty());
        CHECK(frees.empty());
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "finalize works the same either way and uncounts the string once") {

        for (auto scope : {mash::ALLOCATED, mash::FROM_MASH}) {
            CAPTURE(scope);

            c_string_slot slot("owned");
            mash::string* value = slot.release();

            CHECK(string_count == 1);

            value->finalize(scope);

            CHECK(string_count == 0);
            CHECK(value->m_data == empty_string());
            CHECK(value->m_size == 5);
        }

        CHECK(free_count(tests::heap_allocator) == 2);
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "appending always allocates new characters, unless there's nothing to append") {

        c_string_slot value("ab");
        char*         original = value->data();
        value->append("ignored", 0);

        CHECK(value->data() == original);
        CHECK(allocation_count(tests::heap_allocator) == 1);

        value->append("cdef", 2);

        CHECK(*value == "abcd");
        CHECK(value->size() == 4);
        CHECK(allocation_count(tests::heap_allocator) == 2);
        CHECK(free_count(tests::heap_allocator) == 1);

        // a zero character is empty text, so it counts as nothing to append.
        value->append('\0');

        CHECK(value->size() == 4);
        CHECK(allocation_count(tests::heap_allocator) == 2);

        value->append('!');

        CHECK(*value == "abcd!");
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "appending adds the full count to the length, even when the text ends sooner") {

        // only "ab" exists to copy, so the text is "xab", but the length still grows by the 4 that were asked for.
        c_string_slot value("x");
        value->append("ab", 4);

        CHECK(*value == "xab");
        CHECK(value->size() == 5);
        CHECK(value->data()[5] == '\0');
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "a string can append itself or a piece of itself") {

        // the old characters are only freed after the new ones are built, so the source is still there while it's copied.
        c_string_slot value("abc");
        *value += *value;

        CHECK(*value == "abcabc");

        value->append(value->data() + 3);

        CHECK(*value == "abcabcabc");
        CHECK(value->size() == 9);
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "appending another string stops at its first zero, not its stored length") {

        const char bytes[]{'a', '\0', 'b', 'c'};

        c_string_slot source;
        source->copy(bytes, 4);

        c_string_slot destination("x");
        destination->append(*source);

        CHECK(destination->size() == 2);
        CHECK(*destination == "xa");

        *destination += *source;

        CHECK(destination->size() == 3);
        CHECK(*destination == "xaa");
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "every way of joining strings leaves the originals alone") {

        c_string_slot left("ab");
        c_string_slot right("cd");

        CHECK((*left + *right) == "abcd");
        CHECK((*left + "cd") == "abcd");
        CHECK(("ab" + *right) == "abcd");
        CHECK(*left == "ab");
        CHECK(*right == "cd");
        CHECK(&(*left += "e") == &*left);
        CHECK(&(*left += 'f') == &*left);
        CHECK(*left == "abef");
        CHECK(&(*left += *right) == &*left);
        CHECK(*left == "abefcd");
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "clear and truncate shorten the string where it is") {

        c_string_slot value("abcdef");
        char*         original = value->data();

        {
            // truncate shortens the string itself and also hands back a separate copy of the result.
            mash::string result = value->truncate(3);

            CHECK(result == "abc");
            CHECK(value->size() == 3);
            CHECK(value->data() == original);
            CHECK(result.data() != original);
        }

        // truncate keeps the new length between 0 and the current length.
        CHECK(value->truncate(99) == "abc");
        CHECK(value->truncate(-2) == "");
        CHECK(value->data() == original);
        CHECK(value->size() == 0);

        value->clear();

        CHECK(value->data() == original);

        *value   = "xy";
        original = value->data();
        value->clear();

        CHECK(value->data() == original);
        CHECK(value->size() == 0);
        CHECK(*original == '\0');
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "clearing a negative length keeps it, and writes a zero that many bytes before the text") {

        // the zero goes to m_data[-3]; empty_storage has room before the empty string, so it lands inside the fixture.
        c_string_slot value;
        value->m_size = -3;
        value->clear();

        CHECK(value->m_size == -3);
        CHECK(value->m_data == empty_string());
        CHECK(empty_string()[-3] == '\0');
        CHECK(empty_string()[-2] == '?');
        CHECK(empty_string()[-1] == '?');
        CHECK(empty_string()[0] == '\0');
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "compare has its signs flipped compared to strcmp, and handles one string being a prefix of the other") {

        // compare gives 1 when this string sorts before the argument and -1 when it sorts after, the opposite of strcmp.
        // the < and > operators are built on it, and they come out the usual way around.
        struct comparison_t {
            const char* left;
            const char* right;
            i32         expected;
        };

        for (const auto &comparison : { comparison_t {"",    "",     0},
                                                     {"",    "a",    1},
                                                     {"a",   "",    -1},
                                                     {"ab",  "abc",  1},
                                                     {"abc", "ab",  -1},
                                                     {"abc", "abd",  1},
                                                     {"abd", "abc", -1},
                                                     {"abc", "abc",  0} }) {

            CAPTURE(comparison.left);
            CAPTURE(comparison.right);

            c_string_slot left(comparison.left);
            c_string_slot right(comparison.right);

            CHECK(left->compare(comparison.right) == comparison.expected);
            CHECK((*left < *right) == (comparison.expected == 1));
            CHECK((*left > *right) == (comparison.expected == -1));
            CHECK((*left <= *right) == (comparison.expected != -1));
            CHECK((*left >= *right) == (comparison.expected != 1));
            CHECK((*left == *right) == (comparison.expected == 0));
            CHECK((*left != *right) == (comparison.expected != 0));
            CHECK((*left == comparison.right) == (comparison.expected == 0));
            CHECK((comparison.right == *left) == (comparison.expected == 0));
            CHECK((*left != comparison.right) == (comparison.expected != 0));
            CHECK((comparison.right != *left) == (comparison.expected != 0));
        }
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "== only looks at the first 65535 characters") {

        std::string first(65536, 'a');
        std::string second = first;

        first[65535]  = 'x';
        second[65535] = 'y';

        c_string_slot left(first.c_str());
        c_string_slot right(second.c_str());

        CHECK(left->is_equal(second.c_str()));
        CHECK(*left == *right);
        CHECK_FALSE(*left != *right);

        // compare has no such limit and does see the difference.
        CHECK(left->compare(second.c_str()) == 1);
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "each search function has its own take on start positions and the terminating zero") {

        c_string_slot value("ababa");

        // find with text, and an optional position to start from.
        CHECK(value->find("aba") == 0);
        CHECK(value->find("aba", 1) == 2);
        CHECK(value->find("missing") == mash::string::npos);
        CHECK(value->find("", 5) == 5);

        // find with a character takes its start position first, wrapped in pos_t, and never matches the terminating zero.
        CHECK(value->find(mash::string::pos_t{1}, 'a') == 2);
        CHECK(value->find(mash::string::pos_t{0}, '\0') == mash::string::npos);
        CHECK(value->find(mash::string::pos_t{5}, 'a') == mash::string::npos);

        // rfind with a character searches backwards, from the last character or a given start; the zero is only found if you start on it.
        CHECK(value->rfind('a') == 4);
        CHECK(value->rfind('a', 3) == 2);
        CHECK(value->rfind('x') == mash::string::npos);
        CHECK(value->rfind('\0') == mash::string::npos);
        CHECK(value->rfind('\0', 5) == 5);

        // rfind with text gives the start of the last match.
        CHECK(value->rfind("aba") == 2);
        CHECK(value->rfind("ababa") == 0);
        CHECK(value->rfind("ababab") == mash::string::npos);
        CHECK(value->rfind("") == 5);

        c_string_slot empty;

        CHECK(empty->rfind('a') == mash::string::npos);
        CHECK(empty->rfind("") == 0);
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "to_upper goes through the upper case table, skipping bytes it has no entry for") {

        // the fixture's table maps a to z and the accented latin-1 letters, except 0xf7 (the division sign).
        const u8      bytes[]{'a', 'Z', '1', 0xE0, 0xF7, 0xFF, 0};
        c_string_slot value((const char*)bytes);
        char*         original = value->data();

        CHECK(&value->to_upper() == &*value);
        CHECK(value->data() == original);
        CHECK(value->size() == 6);
        CHECK((u8)value->data()[0] == 'A');
        CHECK((u8)value->data()[1] == 'Z');
        CHECK((u8)value->data()[2] == '1');
        CHECK((u8)value->data()[3] == 0xC0);
        CHECK((u8)value->data()[4] == 0xF7);
        CHECK((u8)value->data()[5] == 0xDF);

        // swapping a table entry proves the table is what's being used, even for a letter that's already upper case.
        upper_table['A'] = 'Q';
        value->to_upper();

        CHECK(value->data()[0] == 'Q');

        // to_lower uses the c library's tolower instead of a table.
        c_string_slot ascii("AbZ09!");

        CHECK(&ascii->to_lower() == &*ascii);
        CHECK(*ascii == "abz09!");
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "substr takes a count while slice takes an end position") {

        c_string_slot value("abcdef");

        CHECK(value->substr() == "abcdef");
        CHECK(value->substr(2, 2) == "cd");
        CHECK(value->substr(2) == "cdef");
        CHECK(value->substr(6) == "");
        CHECK(value->substr(2, 0) == "");

        // slice's end isn't included, and negative positions count back from the end of the string.
        CHECK(value->slice(1, 4) == "bcd");
        CHECK(value->slice(-3, -1) == "de");
        CHECK(value->slice(0, -1) == "abcde");
        CHECK(value->slice(0, 6) == "abcdef");
        CHECK(*value == "abcdef");
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "trimming removes any of the given characters, and copes with empty, null and everything matching") {

        c_string_slot value("xxabyy");

        CHECK(&value->remove_leading("x") == &*value);
        CHECK(*value == "abyy");
        CHECK(&value->remove_trailing("y") == &*value);
        CHECK(*value == "ab");

        value->remove_leading(nullptr);
        value->remove_trailing(nullptr);
        value->remove_leading("");
        value->remove_trailing("");

        CHECK(*value == "ab");

        c_string_slot all("aaaa");
        all->remove_leading("a");

        CHECK(all->empty());

        *all = "aaaa";
        all->remove_trailing("a");

        CHECK(all->empty());

        c_string_slot spaces(" \n\t\rtext\r\t\n ");

        CHECK(&spaces->remove_surrounding_whitespace() == &*spaces);
        CHECK(*spaces == "text");

        c_string_slot empty;
        empty->remove_surrounding_whitespace();

        CHECK(empty->empty());
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "number parsing reads as much as it can and ignores the rest") {

        c_string_slot integer("  -42rest");
        c_string_slot number(" +12.5rest");
        c_string_slot invalid("word");

        CHECK(integer->to_int() == -42);
        CHECK(number->to_float() == 12.5f);
        CHECK(invalid->to_int() == 0);
        CHECK(invalid->to_float() == 0.0f);
    }
}
