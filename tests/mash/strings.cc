#include <string>
#include <doctest/doctest.h>

#include "mash/fixtures.hh"

namespace mash = treyarch::mash;
using namespace tests::mash_support;

// a mash string has a length, a character pointer and a field called m_source_slab.
// an ordinary string owns its allocated characters; a loaded string can point at characters already in the image.
// m_source_slab labels those borrowed characters by matching the engine's current source_slab address.
// cleanup compares those addresses and leaves matching storage alone, since freeing it would free part of the image.
// the label is not reset by every edit; the mutation tests below show new allocations being left unfreed as a result.
TEST_SUITE("mash") {
    TEST_CASE_FIXTURE(c_string_fixture,
                      "empty string constructors share null storage and balance the live count") {

        {
            c_string_slot empty;
            c_string_slot null((const char*)nullptr);
            c_string_slot literal("");

            CHECK(string_count == 3);
            for (auto* value : {&*empty, &*null, &*literal}) {
                CHECK(value->empty());
                CHECK(value->size() == 0);
                CHECK(value->length() == 0);
                CHECK(value->c_str() == empty_storage.data() + 16);
                CHECK(value->data() == value->c_str());
                CHECK(value->get_mash_sizeof() == 12);
            }

            CHECK(allocations.empty());
        }

        CHECK(string_count == 0);
        CHECK(free_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "ordinary string construction leaves the source slab word untouched") {

        image_t<64> image;
        void* marker = &image;
        image.put(8, marker);
        auto* value = ::new (image.at<mash::string>(0)) mash::string;

        CHECK(value->m_source_slab == marker);
        CHECK(value->m_data == empty_storage.data() + 16);
        CHECK(value->m_size == 0);

        value->~string();
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "from mash string construction preserves all serialized fields") {

        image_t<128> image;
        image.put(0, i32{5});
        image.put(4, image.at<char>(64));
        image.put(8, source_slab);
        std::memcpy(image.bytes.data() + 64, "hello", 6);
        std::array<u8, 12> original;
        std::memcpy(original.data(), image.bytes.data(), 12);
        auto* value = ::new (image.at<mash::string>(0))
            mash::string((mash::from_mash_in_place_constructor*)nullptr);

        CHECK(string_count == 1);
        CHECK(std::memcmp(original.data(), image.bytes.data(), 12) == 0);

        value->destruct_mashed_class();

        CHECK(string_count == 0);
        CHECK(free_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "string unmash borrows characters and aligns the selected stream") {

        for (auto selected : {mash::NORMAL_BUFFER, mash::SHARED_BUFFER}) {
            for (i32 length : {1, 2, 3, 4, 5}) {
                CAPTURE(selected);
                CAPTURE(length);

                image_t<256> image;
                image.header(128);
                image.put(128, length);
                auto*     value = image.at<mash::string>(16);
                const u32 start = selected == mash::NORMAL_BUFFER ? 28 : 132;
                std::memcpy(image.bytes.data() + start, "abcde", length);
                image.bytes[start + length] = 0;

                mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 256);
                reader.advance_buffer(mash::NORMAL_BUFFER, 12);

                // the length always comes from shared storage; characters plus their terminator use the selected stream.
                value->unmash(&reader, nullptr, selected);

                CHECK(string_count == 0);
                CHECK(value->m_size == length);
                CHECK(value->m_data == image.at<char>(start));
                CHECK(value->m_source_slab == source_slab);

                const u32 end = (start + length + 1 + 3) / 4 * 4;

                CHECK(reader.get_buffer_size_used(mash::NORMAL_BUFFER) ==
                    (selected == mash::NORMAL_BUFFER ? (i32)end : 28));
                CHECK(reader.get_buffer_size_used(mash::SHARED_BUFFER) ==
                    (selected == mash::SHARED_BUFFER ? (i32)end - 128 : 4));

                // repair alone leaves the live count unchanged; only the construction method registers a live string.
                value->construct_mashed_class();

                CHECK(string_count == 1);

                value->destruct_mashed_class();

                CHECK(string_count == 0);
                CHECK(value->m_data == empty_storage.data() + 16);
                CHECK(value->m_size == length);
                CHECK(value->m_source_slab == source_slab);
                CHECK(allocations.empty());
            }
        }
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "nonpositive mashed string lengths preserve the length and use null storage") {

        for (i32 length : {0, -1, -7}) {
            image_t<128> image;
            image.header(80);
            image.put(80, length);
            auto* value = image.at<mash::string>(16);

            mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 128);
            reader.advance_buffer(mash::NORMAL_BUFFER, 12);
            value->unmash(&reader, nullptr, mash::NORMAL_BUFFER);

            CHECK(value->m_size == length);
            CHECK(value->m_data == empty_storage.data() + 16);
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
                      "owned strings copy characters and release their heap storage") {

        char input[] = "hello";
        {
            c_string_slot value(input);

            CHECK(value->m_size == 5);
            CHECK(value->m_data != input);
            CHECK(doctest::String(value->c_str()) == "hello");

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
                      "copy construction uses the terminator while string assignment uses stored size") {

        // an embedded terminator separates the visible text length from the four bytes stored in the source.
        // these overloads disagree about which length to copy, so ordinary text alone would miss the difference.
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
                      "string substring constructors clamp count to the remaining suffix") {

        c_string_slot source("abcdef");
        for (i32 count : {mash::string::npos, 20, 4}) {
            c_string_slot result(*source, 2, count);

            CHECK(result->size() == 4);
            CHECK(doctest::String(result->c_str()) == "cdef");
        }
        c_string_slot middle(*source, 2, 2);
        c_string_slot at_end(*source, 6, mash::string::npos);

        CHECK(doctest::String(middle->c_str()) == "cd");
        CHECK(at_end->empty());
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "numeric and formatted string constructors use the recovered formatting") {

        c_string_slot negative(i32{-123});
        c_string_slot integer(i32{0});
        c_string_slot number(f32{1.25f});
        c_string_slot negative_zero(-0.0f);
        c_string_slot formatted(mash::string::fmt, "%s:%04d", "item", 7);

        CHECK(doctest::String(negative->c_str()) == "-123");
        CHECK(doctest::String(integer->c_str()) == "0");
        CHECK(doctest::String(number->c_str()) == "1.250");
        CHECK(doctest::String(negative_zero->c_str()) == "-0.000");
        CHECK(doctest::String(formatted->c_str()) == "item:0007");
        CHECK(mash::string::from_int(42) == "42");
        CHECK(mash::string::from_float(-2.5f) == "-2.500");
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "owned string updates reuse storage until the stored length grows") {

        c_string_slot value("abcdef");
        char* original = value->data();
        value->update_guts("xy", 2);

        CHECK(value->data() == original);
        CHECK(value->size() == 2);
        CHECK(value->data()[2] == '\0');
        CHECK(allocation_count(tests::heap_allocator) == 1);

        // reuse compares against the current length of two, not the room originally allocated for six characters.
        value->update_guts("12345");

        CHECK(value->size() == 5);
        CHECK(allocation_count(tests::heap_allocator) == 2);
        CHECK(free_count(tests::heap_allocator) == 1);
        CHECK(doctest::String(value->c_str()) == "12345");
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "zero and negative explicit copy lengths retain their signed value") {

        for (i32 length : {0, -2, -3}) {
            c_string_slot value("abcdef");
            value->copy("ignored", length);

            CHECK(value->size() == length);
            CHECK(value->c_str() == empty_storage.data() + 16);
            CHECK(value->empty() == (length == 0));
        }

        CHECK(live_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "self assignment is inert and character assignment returns the same object") {

        c_string_slot value("hello");
        char* original = value->data();

        CHECK(&(*value = *value) == &*value);
        CHECK(value->data() == original);
        CHECK(allocation_count(tests::heap_allocator) == 1);
        CHECK(&(*value = "bye") == &*value);
        CHECK(value->data() == original);
        CHECK(doctest::String(value->c_str()) == "bye");
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "borrowed string mutation retains its slab marker and heap allocations") {

        // this local array stands in for characters inside an image; the matching label makes the string borrow it.
        char borrowed[] = "borrowed";
        c_string_slot slot((mash::from_mash_in_place_constructor*)nullptr);
        slot->m_size        = 8;
        slot->m_data        = borrowed;
        slot->m_source_slab = source_slab;

        slot->update_guts("x");

        CHECK(slot->m_data != borrowed);
        CHECK(slot->m_source_slab == source_slab);
        CHECK(doctest::String(slot->c_str()) == "x");
        CHECK(doctest::String(borrowed) == "borrowed");

        slot->update_guts("y");

        CHECK(allocation_count(tests::heap_allocator) == 2);
        CHECK(free_count(tests::heap_allocator) == 0);

        // the retained marker also prevents teardown from freeing the new heap bytes.
        // the fixture reclaims those leftovers after the test has checked them.
        slot.release()->destruct_mashed_class();

        CHECK(string_count == 0);
        CHECK(live_count(tests::heap_allocator) == 2);
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "an empty borrowed update still allocates without freeing the new storage") {

        // editing a borrowed string first allocates room for the new text, even when that text is empty.
        // empty text then selects the common empty-string address; the new one-byte allocation is left behind.
        char borrowed[] = "a";
        c_string_slot slot((mash::from_mash_in_place_constructor*)nullptr);
        slot->m_size        = 1;
        slot->m_data        = borrowed;
        slot->m_source_slab = source_slab;

        slot->copy("", 0);

        REQUIRE(allocations.size() == 1);
        CHECK(allocations[0].size == 1);
        CHECK(slot->m_data == empty_storage.data() + 16);
        CHECK(slot->m_source_slab == source_slab);
        CHECK(free_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "appending to a borrowed string retains the marker and leaves the image unchanged") {

        char borrowed[] = "abc";
        c_string_slot value((mash::from_mash_in_place_constructor*)nullptr);
        value->m_size        = 3;
        value->m_data        = borrowed;
        value->m_source_slab = source_slab;

        value->append("de");

        CHECK(value->size() == 5);
        CHECK(doctest::String(value->c_str()) == "abcde");
        CHECK(doctest::String(borrowed) == "abc");
        CHECK(value->m_source_slab == source_slab);

        value.release()->destruct_mashed_class();

        CHECK(live_count(tests::heap_allocator) == 1);
        CHECK(free_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "clearing borrowed strings edits their image without allocating") {

        char borrowed[] = "abc";
        c_string_slot value((mash::from_mash_in_place_constructor*)nullptr);
        value->m_size        = 3;
        value->m_data        = borrowed;
        value->m_source_slab = source_slab;

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
                      "finalize ignores its scope and balances construction exactly once") {

        for (auto scope : {mash::ALLOCATED, mash::FROM_MASH}) {
            c_string_slot slot("owned");
            mash::string* value = slot.release();

            CHECK(string_count == 1);

            value->finalize(scope);

            CHECK(string_count == 0);
            CHECK(value->m_data == empty_storage.data() + 16);
            CHECK(value->m_size == 5);
        }

        CHECK(free_count(tests::heap_allocator) == 2);
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "append always reallocates except for a zero count") {

        c_string_slot value("ab");
        char* original = value->data();
        value->append("ignored", 0);

        CHECK(value->data() == original);
        CHECK(allocation_count(tests::heap_allocator) == 1);

        value->append("cdef", 2);

        CHECK(doctest::String(value->c_str()) == "abcd");
        CHECK(value->size() == 4);
        CHECK(allocation_count(tests::heap_allocator) == 2);
        CHECK(free_count(tests::heap_allocator) == 1);

        value->append('\0');

        CHECK(value->size() == 4);
        CHECK(allocation_count(tests::heap_allocator) == 2);

        value->append('!');

        CHECK(doctest::String(value->c_str()) == "abcd!");
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "append counts the requested bytes even when the source terminates early") {

        // the recorded size uses the requested count, while appending characters stops at the source terminator.
        // this leaves size larger than the visible text length without reading past the source string.
        c_string_slot value("x");
        value->append("ab", 4);

        CHECK(doctest::String(value->c_str()) == "xab");
        CHECK(value->size() == 5);
        CHECK(value->data()[5] == '\0');
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "append supports self and interior sources before releasing old bytes") {

        c_string_slot value("abc");
        *value += *value;

        CHECK(doctest::String(value->c_str()) == "abcabc");

        value->append(value->data() + 3);

        CHECK(doctest::String(value->c_str()) == "abcabcabc");
        CHECK(value->size() == 9);
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "string append overloads use the source terminator rather than its stored size") {

        const char bytes[]{'a', '\0', 'b', 'c'};
        c_string_slot source;
        source->copy(bytes, 4);
        c_string_slot destination("x");
        destination->append(*source);

        CHECK(destination->size() == 2);
        CHECK(doctest::String(destination->c_str()) == "xa");

        *destination += *source;

        CHECK(destination->size() == 3);
        CHECK(doctest::String(destination->c_str()) == "xaa");
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "all concatenation overloads preserve their inputs") {

        c_string_slot left("ab");
        c_string_slot right("cd");

        CHECK((*left + *right) == "abcd");
        CHECK((*left + "cd") == "abcd");
        CHECK(("ab" + *right) == "abcd");
        CHECK(doctest::String(left->c_str()) == "ab");
        CHECK(doctest::String(right->c_str()) == "cd");
        CHECK(&(*left += "e") == &*left);
        CHECK(&(*left += 'f') == &*left);
        CHECK(doctest::String(left->c_str()) == "abef");
        CHECK(&(*left += *right) == &*left);
        CHECK(doctest::String(left->c_str()) == "abefcd");
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "clear and truncate edit storage in place") {

        c_string_slot value("abcdef");
        char* original = value->data();
        {
            // truncate changes the original buffer and also returns a separate string containing the shortened text.
            mash::string result = value->truncate(3);

            CHECK(result == "abc");
            CHECK(value->size() == 3);
            CHECK(value->data() == original);
            CHECK(result.data() != original);
        }

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
                      "negative clear lengths are preserved and address their negative byte offset") {

        // empty_storage has guard bytes before its empty string, so this backward write stays inside the fixture's array.
        c_string_slot value;
        value->m_size = -3;
        value->clear();

        CHECK(value->m_size == -3);
        CHECK(value->m_data == empty_storage.data() + 16);
        CHECK(empty_storage[13] == '\0');
        CHECK(empty_storage[14] == '?');
        CHECK(empty_storage[15] == '?');
        CHECK(empty_storage[16] == '\0');
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "string comparison uses reversed signs and handles prefixes") {

        // compare returns positive for less and negative for greater; the relational operators follow those signs.
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
                      "string equality compares only the first 65535 bytes") {

        std::string first(65536, 'a');
        std::string second = first;

        first[65535]  = 'x';
        second[65535] = 'y';
        c_string_slot left(first.c_str());
        c_string_slot right(second.c_str());

        CHECK(left->is_equal(second.c_str()));
        CHECK(*left == *right);
        CHECK_FALSE(*left != *right);

        // equality stops at its fixed comparison limit, but compare still reaches the differing character.
        CHECK(left->compare(second.c_str()) == 1);
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "string search offsets and terminators follow each overload") {

        c_string_slot value("ababa");

        CHECK(value->find("aba") == 0);
        CHECK(value->find("aba", 1) == 2);
        CHECK(value->find("missing") == mash::string::npos);
        CHECK(value->find("", 5) == 5);
        CHECK(value->find(mash::string::pos_t{1}, 'a') == 2);
        CHECK(value->find(mash::string::pos_t{0}, '\0') == mash::string::npos);
        CHECK(value->find(mash::string::pos_t{5}, 'a') == mash::string::npos);
        CHECK(value->rfind('a') == 4);
        CHECK(value->rfind('a', 3) == 2);
        CHECK(value->rfind('x') == mash::string::npos);
        CHECK(value->rfind('\0') == mash::string::npos);
        CHECK(value->rfind('\0', 5) == 5);
        CHECK(value->rfind("aba") == 2);
        CHECK(value->rfind("ababa") == 0);
        CHECK(value->rfind("ababab") == mash::string::npos);
        CHECK(value->rfind("") == 5);

        c_string_slot empty;

        CHECK(empty->rfind('a') == mash::string::npos);
        CHECK(empty->rfind("") == 0);
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "case conversion uses the upper table and leaves zero entries unchanged") {

        const u8 bytes[]{'a', 'Z', '1', 0xE0, 0xF7, 0xFF, 0};
        c_string_slot value((const char*)bytes);
        char* original = value->data();

        CHECK(&value->to_upper() == &*value);
        CHECK(value->data() == original);
        CHECK(value->size() == 6);
        CHECK((u8)value->data()[0] == 'A');
        CHECK((u8)value->data()[1] == 'Z');
        CHECK((u8)value->data()[2] == '1');
        CHECK((u8)value->data()[3] == 0xC0);
        CHECK((u8)value->data()[4] == 0xF7);
        CHECK((u8)value->data()[5] == 0xDF);

        // changing the fixture table makes its use observable even for a character that was already uppercase.
        upper_table['A'] = 'Q';
        value->to_upper();

        CHECK(value->data()[0] == 'Q');

        c_string_slot ascii("AbZ09!");

        CHECK(&ascii->to_lower() == &*ascii);
        CHECK(doctest::String(ascii->c_str()) == "abz09!");
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "substring and slice preserve their distinct count conventions") {

        c_string_slot value("abcdef");

        CHECK(value->substr() == "abcdef");
        CHECK(value->substr(2, 2) == "cd");
        CHECK(value->substr(2) == "cdef");
        CHECK(value->substr(6) == "");
        CHECK(value->substr(2, 0) == "");

        // slice takes an exclusive end position rather than a count; negative positions count back from the end.
        CHECK(value->slice(1, 4) == "bcd");
        CHECK(value->slice(-3, -1) == "de");
        CHECK(value->slice(0, -1) == "abcde");
        CHECK(value->slice(0, 6) == "abcdef");
        CHECK(doctest::String(value->c_str()) == "abcdef");
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "trimming uses character sets and handles empty null and fully matching inputs") {

        c_string_slot value("xxabyy");

        CHECK(&value->remove_leading("x") == &*value);
        CHECK(doctest::String(value->c_str()) == "abyy");
        CHECK(&value->remove_trailing("y") == &*value);
        CHECK(doctest::String(value->c_str()) == "ab");

        value->remove_leading(nullptr);
        value->remove_trailing(nullptr);
        value->remove_leading("");
        value->remove_trailing("");

        CHECK(doctest::String(value->c_str()) == "ab");

        c_string_slot all("aaaa");
        all->remove_leading("a");

        CHECK(all->empty());

        *all = "aaaa";
        all->remove_trailing("a");

        CHECK(all->empty());

        c_string_slot spaces(" \n\t\rtext\r\t\n ");

        CHECK(&spaces->remove_surrounding_whitespace() == &*spaces);
        CHECK(doctest::String(spaces->c_str()) == "text");

        c_string_slot empty;
        empty->remove_surrounding_whitespace();

        CHECK(empty->empty());
    }

    TEST_CASE_FIXTURE(c_string_fixture,
                      "numeric parsing accepts prefixes and stops at trailing text") {

        c_string_slot integer("  -42rest");
        c_string_slot number(" +12.5rest");
        c_string_slot invalid("word");

        CHECK(integer->to_int() == -42);
        CHECK(number->to_float() == 12.5f);
        CHECK(invalid->to_int() == 0);
        CHECK(invalid->to_float() == 0.0f);
    }
}
