#include <doctest/doctest.h>

#include "mash/fixtures.hh"
#include "treyarch/mash/vector.hh"
#include "treyarch/mash/vector_basic.hh"
#include "treyarch/mm/mm.hh"
#include "treyarch/base/tl_system.hh"

namespace mash = treyarch::mash;
using namespace tests::mash_support;

// mash has two kinds of vectors, and both can keep their contents inside the image instead of in memory of their own.
// vector<record_t> holds pointers to records; in the image, a table of pointer slots comes first and the records follow it.
// loading rewrites each slot to point at its record and runs that record's unmash method.
// vector_basic<pair_t> holds the values themselves back to back, so there are no pointers to fix and no unmash calls.
// either way, the element count isn't kept in the vector's own bytes, it's read from the shared stream.
// a vector remembers how far its image bytes reach, so clearing it later never frees memory that belongs to the image.
namespace tests { namespace mash_support {
    struct alignas(4) pair_t {
        u16 first;
        u16 second;
    };
}}

TEST_SUITE("mash") {
    TEST_CASE("a container counts anything from itself to the end of its image bytes as borrowed, both ends included") {
        image_t<128> image;
        auto*        value = image.make<mash::container_base>(16);

        CHECK(value->empty());
        CHECK_FALSE(value->from_mash());
        CHECK(value->is_pointer_in_mash_image(value));

        // mash_image_offset is the distance from the container to the end of its image bytes.
        // the container sits at 16, so a distance of 64 covers everything from byte 16 up to byte 80.
        value->mash_image_offset = 64;
        value->size              = 3;

        CHECK(value->from_mash());
        CHECK_FALSE(value->empty());
        CHECK_FALSE(value->is_pointer_in_mash_image(image.bytes.data() + 15));
        CHECK(value->is_pointer_in_mash_image(image.bytes.data() + 16));
        CHECK(value->is_pointer_in_mash_image(image.bytes.data() + 48));
        CHECK(value->is_pointer_in_mash_image(image.bytes.data() + 80));
        CHECK_FALSE(value->is_pointer_in_mash_image(image.bytes.data() + 81));
        CHECK_FALSE(value->is_pointer_in_mash_image(nullptr));

        value->construct_mashed_class();

        CHECK(value->size == 3);
        CHECK(value->get_mash_sizeof() == 8);
        CHECK(value->get_mash_image_offset() == 64);

        value->destruct_mashed_class();

        CHECK(value->empty());
        CHECK_FALSE(value->from_mash());
    }

    TEST_CASE("a container's count comes from the shared stream, aligned to four") {
        image_t<128> image;
        image.header(80);
        image.put(84, u32{7});

        auto* value = image.make<mash::container_base>(16);

        // the 99 here is junk, like a saved count would be.
        value->size              = 99;
        value->mash_image_offset = 41;

        mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 128);
        reader.advance_buffer(mash::SHARED_BUFFER, 1);
        value->unmash(&reader, nullptr, mash::NORMAL_BUFFER);

        CHECK(value->size == 7);
        CHECK(value->mash_image_offset == 41);
        CHECK(reader.get_buffer_size_used(mash::NORMAL_BUFFER) == 16);
        CHECK(reader.get_buffer_size_used(mash::SHARED_BUFFER) == 8);

        // custom_unmash_base is what each container calls once it's read everything it owns; it measures up to the cursor.
        value->custom_unmash_base(&reader, mash::SHARED_BUFFER);

        CHECK(value->mash_image_offset == 72);

        value->clear();

        CHECK(value->size == 0);
        CHECK(value->mash_image_offset == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "a new pointer vector is empty, owns its elements and iterates over nothing") {

        mash::vector<record_t> values;

        CHECK(values.size == 0);
        CHECK(values.capacity == 0);
        CHECK(values.data == nullptr);
        CHECK(values.destroy_elements);
        CHECK(values.begin() == nullptr);
        CHECK(values.end() == nullptr);

        const auto &constant = values;

        CHECK(constant.begin() == nullptr);
        CHECK(constant.end() == nullptr);
        CHECK(values.get_mash_sizeof() == 20);

        // initialize(ALLOCATED) is the reset for a vector that isn't from an image.
        values.destroy_elements = false;
        values.capacity         = 8;
        values.initialize(mash::ALLOCATED);

        CHECK(values.destroy_elements);
        CHECK(values.capacity == 0);
        CHECK(values.data == nullptr);
        CHECK(allocations.empty());
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "loading a pointer vector points every slot at its record, in either stream") {

        for (auto selected : {mash::NORMAL_BUFFER, mash::SHARED_BUFFER}) {
            CAPTURE(selected);

            image_t<256> image;
            image.header(128);
            image.put(128, u32{2});

            trace_t trace;
            auto*   values = image.make<mash::vector<record_t>>(16);

            values->size = 99;

            // the vector's body is bytes 16 to 35; when it's loaded from the normal stream, its two pointer slots follow at 36.
            // the shared stream starts with the count at 128, so the slots go to 132 when that's the selected stream instead.
            // the two records come right after the slots, and the slots hold junk until loading replaces them.
            const u32 table = selected == mash::NORMAL_BUFFER ? 36 : 132;

            image.put(table,     u32{0xDEAD0001});
            image.put(table + 4, u32{0xDEAD0002});

            record_t* first  = put_record(image, table + 8,  1, trace);
            record_t* second = put_record(image, table + 16, 2, trace);

            mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 256);
            reader.advance_buffer(mash::NORMAL_BUFFER, 20);

            void* parent = &trace;
            values->unmash(&reader, parent, selected);

            CHECK(values->size == 2);
            CHECK(values->capacity == 2);
            CHECK(values->data == image.at<record_t*>(table));
            CHECK((*values)[0] == first);
            CHECK((*values)[1] == second);
            CHECK(values->end() == values->begin() + 2);

            const auto &constant = *values;

            CHECK(constant[1] == second);
            CHECK(constant.end() == constant.begin() + 2);
            CHECK(values->mash_image_offset == table + 24 - 16);
            REQUIRE(trace.events.size() == 2);
            CHECK(trace.events[0].id == 1);
            CHECK(trace.events[1].id == 2);
            CHECK(trace.events[0].parent == parent);
            CHECK(trace.events[1].buffer == selected);
            CHECK(reader.get_buffer_size_used(mash::NORMAL_BUFFER) ==
                (selected == mash::NORMAL_BUFFER ? 60 : 36));
            CHECK(reader.get_buffer_size_used(mash::SHARED_BUFFER) ==
                (selected == mash::SHARED_BUFFER ? 28 : 4));

            values->construct_mashed_class();

            REQUIRE(trace.events.size() == 4);
            CHECK(trace.events[2].kind == event_construct);
            CHECK(trace.events[2].id == 1);
            CHECK(trace.events[3].id == 2);

            // the records live in the image, so teardown calls their destruct_mashed_class and frees nothing.
            // it also goes through them back to front.
            values->destruct_mashed_class();

            REQUIRE(trace.events.size() == 6);
            CHECK(trace.events[4].kind == event_destruct_mashed);
            CHECK(trace.events[4].id == 2);
            CHECK(trace.events[5].id == 1);
            CHECK(*image.at<record_t*>(table)     == nullptr);
            CHECK(*image.at<record_t*>(table + 4) == nullptr);
            CHECK(values->size == 0);
            CHECK(values->capacity == 0);
            CHECK(values->mash_image_offset == 0);
            CHECK(values->data == nullptr);
            CHECK(free_count(tests::heap_allocator) == 0);
        }
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "an empty pointer vector reads no slots but still measures its image bytes") {

        image_t<128> image;
        image.header(80);

        auto* values = image.make<mash::vector<record_t>>(16);

        values->capacity = 99;
        values->data     = image.at<record_t*>(64);

        mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 128);
        reader.advance_buffer(mash::NORMAL_BUFFER, 20);
        values->unmash(&reader, nullptr, mash::NORMAL_BUFFER);

        CHECK(values->data == nullptr);
        CHECK(values->capacity == 0);
        CHECK(values->mash_image_offset == 20);
        CHECK(reader.get_buffer_size_used(mash::NORMAL_BUFFER) == 36);
        CHECK(reader.get_buffer_size_used(mash::SHARED_BUFFER) == 4);

        values->initialize(mash::FROM_MASH);
        values->clear();

        CHECK(allocations.empty());
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "custom_unmash on its own uses the count already in the vector") {

        // unmash reads the count from the shared stream and then hands off to custom_unmash for the slots and elements.
        // calling custom_unmash directly skips that first step, so the 99 waiting in the shared stream is never read.
        image_t<128> image;
        image.header(96);
        image.put(96, u32{99});

        trace_t trace;
        auto*   values = image.make<mash::vector<record_t>>(16);

        values->size = 1;

        record_t* element = put_record(image, 40, 1, trace);

        mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 128);
        reader.advance_buffer(mash::NORMAL_BUFFER, 20);
        values->custom_unmash(&reader, &trace, mash::NORMAL_BUFFER);

        REQUIRE(values->data == image.at<record_t*>(36));
        CHECK(values->data[0] == element);
        CHECK(values->capacity == 1);
        CHECK(reader.get_buffer_size_used(mash::NORMAL_BUFFER) == 48);
        CHECK(reader.get_buffer_size_used(mash::SHARED_BUFFER) == 0);

        values->clear();
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "clearing a pointer vector handles borrowed, owned and null elements differently") {

        // borrowed elements get destruct_mashed_class, owned ones get their destructor and get freed, and null slots are skipped.
        image_t<256> image;

        trace_t   trace;
        auto*     values = image.make<mash::vector<record_t>>(16);
        record_t* first  = put_record(image, 64, 1, trace);
        record_t* last   = put_record(image, 96, 3, trace);
        auto*     owned  = ::new (treyarch::mm::allocate(sizeof(record_t))) record_t{2, &trace};
        auto**    table  = (record_t**)treyarch::mm::allocate(4 * sizeof(record_t*));

        table[0]     = first;
        table[1]     = owned;
        table[2]     = nullptr;
        table[3]     = last;
        values->data = table;
        values->size = values->capacity = 4;

        // this range covers both image records, but neither the owned record nor the table.
        values->mash_image_offset = 96;
        values->clear();

        REQUIRE(trace.events.size() == 3);
        CHECK(trace.events[0].id == 3);
        CHECK(trace.events[0].kind == event_destruct_mashed);
        CHECK(trace.events[1].id == 2);
        CHECK(trace.events[1].kind == event_destructor);
        CHECK(trace.events[2].id == 1);
        CHECK(trace.events[2].kind == event_destruct_mashed);
        CHECK(trace.destructor_saw_live_storage);
        CHECK(free_count(tests::heap_allocator) == 2);
        REQUIRE(find(table) != nullptr);

        // the memory fixture saves a copy of the table right before it's freed, which shows every slot was nulled first.
        CHECK(find(table)->released_bytes == std::vector<u8>(16, 0));
        CHECK(values->data == nullptr);
        CHECK(values->size == 0);
        CHECK(values->mash_image_offset == 0);

        // clearing twice is harmless.
        values->clear();

        CHECK(free_count(tests::heap_allocator) == 2);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "a vector that doesn't own its elements leaves them alive, but still frees its table") {

        trace_t trace;
        auto*   owned = ::new (treyarch::mm::allocate(sizeof(record_t))) record_t{1, &trace};
        auto**  table = (record_t**)treyarch::mm::allocate(sizeof(record_t*));

        table[0] = owned;

        {
            mash::vector<record_t> values;
            values.data = table;
            values.size = values.capacity = 1;

            // destroy_elements only decides whether clear destroys the elements; construction still visits them.
            values.destroy_elements = false;
            values.construct_mashed_class();

            REQUIRE(trace.events.size() == 1);
            CHECK(trace.events[0].kind == event_construct);

            values.clear();

            CHECK_FALSE(values.destroy_elements);
            CHECK(values.empty());
            CHECK(trace.events.size() == 1);
            CHECK_FALSE(find(owned)->released);
            CHECK(find(table)->released);
        }

        owned->~record_t();
        treyarch::mm::free(owned);

        CHECK(free_count(tests::heap_allocator) == 2);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "owned elements with a virtual destructor are deleted through it") {

        // delete on a class with a virtual destructor goes through its vtable, which lets the class free itself its own way.
        trace_t trace;

        {
            mash::vector<mash::mash_virtual_base> values;
            values.data    = (mash::mash_virtual_base**)treyarch::mm::allocate(4);
            values.data[0] = new c_virtual_record(8, &trace);
            values.size    = values.capacity = 1;
        }

        REQUIRE(trace.events.size() == 1);
        CHECK(trace.events[0].kind == event_destructor);
        CHECK(trace.events[0].id == 8);
        CHECK(trace.destructor_saw_live_storage);
        CHECK(free_count(tests::heap_allocator) == 2);
        CHECK(live_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "an element sitting exactly at the end of the image bytes still counts as borrowed") {

        image_t<128> image;

        trace_t trace;
        auto*   values = image.make<mash::vector<record_t>>(16);

        // 16 plus 48 puts the end of the image bytes at 64, exactly where the element starts.
        values->mash_image_offset = 48;

        record_t* element = put_record(image, 64, 4, trace);
        values->destroy_element(element);

        CHECK(element == nullptr);
        REQUIRE(trace.events.size() == 1);
        CHECK(trace.events[0].kind == event_destruct_mashed);
        CHECK(free_count(tests::heap_allocator) == 0);

        // destroy_element nulls the pointer it's given, so a second call has nothing to do.
        values->destroy_element(element);

        CHECK(trace.events.size() == 1);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "loading a basic vector borrows its values in place, from either stream") {

        for (auto selected : {mash::NORMAL_BUFFER, mash::SHARED_BUFFER}) {
            CAPTURE(selected);

            image_t<256> image;
            image.header(128);
            image.put(128, u32{2});

            // a basic vector's body is sixteen bytes, 16 to 31 here; the values come right after it, or after the count in shared.
            auto* values = image.make<mash::vector_basic<pair_t>>(16);
            values->size = 99;

            const u32 data_offset = selected == mash::NORMAL_BUFFER ? 32 : 132;
            image.put(data_offset,     pair_t{1, 2});
            image.put(data_offset + 4, pair_t{3, 4});

            mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 256);
            reader.advance_buffer(mash::NORMAL_BUFFER, 16);
            values->unmash(&reader, nullptr, selected);

            CHECK(values->size == 2);
            CHECK(values->capacity == 2);
            CHECK(values->data == image.at<pair_t>(data_offset));
            CHECK((size_t)values->data % 4 == 0);
            CHECK(values->data[0].first == 1);
            CHECK(values->data[1].second == 4);
            CHECK(values->mash_image_offset == data_offset + 8 - 16);

            values->construct_mashed_class();

            CHECK(values->size == 2);

            values->destruct_mashed_class();

            CHECK(values->data == nullptr);
            CHECK(values->capacity == 0);
            CHECK(values->size == 0);
            CHECK(values->mash_image_offset == 0);
            CHECK(free_count(tests::plain_allocator) == 0);
            CHECK(free_count(tests::heap_allocator) == 0);
        }
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "an empty basic vector drops its old data pointer and reads no values") {

        image_t<128> image;
        image.header(80);

        auto* values = image.make<mash::vector_basic<u32>>(16);

        values->data     = image.at<u32>(64);
        values->capacity = 12;

        mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 128);
        reader.advance_buffer(mash::NORMAL_BUFFER, 16);
        values->unmash(&reader, nullptr, mash::NORMAL_BUFFER);

        CHECK(values->data == nullptr);
        CHECK(values->capacity == 0);
        CHECK(values->mash_image_offset == 16);
        CHECK(reader.get_buffer_size_used(mash::NORMAL_BUFFER) == 32);
        CHECK(reader.get_buffer_size_used(mash::SHARED_BUFFER) == 4);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "a basic vector aligns its values by their type and calls nothing on them") {

        // a u64 needs eight byte alignment, so the cursor at 33 skips to 40 before the values are read.
        // custom_unmash is called directly here as well, so the 99 in shared goes unread like before.
        image_t<128> image;
        image.header(96);
        image.put(96, u32{99});

        auto* values = image.make<mash::vector_basic<u64>>(16);

        values->size = 2;
        image.put(40, u64{0x1122334455667788});
        image.put(48, u64{0x8877665544332211});

        mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 128);
        reader.advance_buffer(mash::NORMAL_BUFFER, 17);
        values->custom_unmash(&reader, nullptr, mash::NORMAL_BUFFER);

        REQUIRE(values->data == image.at<u64>(40));
        CHECK(values->data[0] == 0x1122334455667788);
        CHECK(values->data[1] == 0x8877665544332211);
        CHECK(values->capacity == 2);
        CHECK(reader.get_buffer_size_used(mash::NORMAL_BUFFER) == 56);
        CHECK(reader.get_buffer_size_used(mash::SHARED_BUFFER) == 0);

        values->clear();

        CHECK(free_count(tests::plain_allocator) == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "a basic vector frees its own storage once, through the plain allocator") {

        // the engine has two allocators; pointer vectors free through the heap one, basic vectors through the plain one.
        // the memory fixture flags any free that goes through the wrong one.
        {
            mash::vector_basic<u32> values {};
            values.data    = (u32*)treyarch::tl_mem_alloc(12, 4, 0);
            values.size    = values.capacity = 3;
            values.data[0] = 10;
            values.clear();

            CHECK(values.data == nullptr);
            CHECK(values.size == 0);
            CHECK(values.capacity == 0);
            CHECK(values.mash_image_offset == 0);
        }

        CHECK(free_count(tests::plain_allocator) == 1);
        CHECK(free_count(tests::heap_allocator) == 0);
        CHECK(live_count(tests::plain_allocator) == 0);
    }
}
