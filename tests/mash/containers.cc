#include <doctest/doctest.h>

#include "mash/fixtures.hh"
#include "treyarch/shared/mash/vector.hh"
#include "treyarch/shared/mash/vector_basic.hh"
#include "treyarch/shared/memory/heap.hh"
#include "treyarch/shared/memory/memory.hh"

namespace mash = treyarch::mash;
using namespace tests::mash_support;

// mash has two vector types with different element layouts.
// vector<record_t> stores record_t pointers; its image has the pointer slots first, followed by the record bodies.
// unmashing replaces each saved slot with the address of its record and calls that record's unmash method.
// vector_basic<pair_t> stores pair_t values next to each other, so it needs no pointer table or per-value unmash call.
// both can use storage inside the image; their ownership checks keep clear from freeing those borrowed bytes.
namespace tests { namespace mash_support {
    struct alignas(4) pair_t {
        u16 first;
        u16 second;
    };
}}

TEST_SUITE("mash") {
    TEST_CASE("container ownership includes both image boundaries") {
        image_t<128> image;
        auto* value = ::new (image.at<mash::container_base>(16)) mash::container_base;

        CHECK(value->empty());
        CHECK_FALSE(value->from_mash());
        CHECK(value->is_pointer_in_mash_image(value));

        // mash_image_offset is a distance from this container to the end of its borrowed range.
        // the container is at byte 16, so a distance of 64 includes pointers from byte 16 through byte 80.
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

    TEST_CASE("container counts come from aligned shared bytes") {
        image_t<128> image;
        image.header(80);
        image.put(84, u32{7});
        auto* value = ::new (image.at<mash::container_base>(16)) mash::container_base;

        value->size              = 99;
        value->mash_image_offset = 41;

        mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 128);
        reader.advance_buffer(mash::SHARED_BUFFER, 1);
        value->unmash(&reader, nullptr, mash::NORMAL_BUFFER);

        CHECK(value->size == 7);
        CHECK(value->mash_image_offset == 41);
        CHECK(reader.get_buffer_size_used(mash::NORMAL_BUFFER) == 16);
        CHECK(reader.get_buffer_size_used(mash::SHARED_BUFFER) == 8);

        value->custom_unmash_base(&reader, mash::SHARED_BUFFER);

        CHECK(value->mash_image_offset == 72);

        value->clear();

        CHECK(value->size == 0);
        CHECK(value->mash_image_offset == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "pointer vectors initialize owned state and null iterators") {

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

        values.destroy_elements = false;
        values.capacity         = 8;
        values.initialize(mash::ALLOCATED);

        CHECK(values.destroy_elements);
        CHECK(values.capacity == 0);
        CHECK(values.data == nullptr);
        CHECK(allocations.empty());
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "pointer vectors replace table entries and traverse objects in either stream") {

        for (auto selected : {mash::NORMAL_BUFFER, mash::SHARED_BUFFER}) {
            CAPTURE(selected);

            image_t<256> image;
            image.header(128);
            image.put(128, u32{2});

            trace_t trace;
            auto* values = ::new (image.at<mash::vector<record_t>>(16)) mash::vector<record_t>;

            values->size = 99;

            const u32 table = selected == mash::NORMAL_BUFFER ? 36 : 132;

            // the vector body occupies bytes 16 through 35; two four-byte pointer slots follow it at byte 36.
            // two eight-byte records follow those slots, so their addresses replace the deliberately invalid slot values.
            // the shared-stream variant uses the same layout after its four-byte count, starting at byte 132 instead.
            image.put(table, u32{0xDEAD0001});
            image.put(table + 4, u32{0xDEAD0002});
            record_t* first  = put_record(image, table + 8, 1, trace);
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

            // these records point into the image; cleanup calls their destruct_mashed_class methods without freeing them.
            // the event list also shows that cleanup visits the last record first.
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
                      "empty vectors record ownership without consuming element storage") {

        image_t<128> image;
        image.header(80);
        auto* values = ::new (image.at<mash::vector<record_t>>(16)) mash::vector<record_t>;

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
                      "custom pointer vector unmash uses the existing count without reading shared metadata") {

        // unmash normally reads the count first, then delegates to custom_unmash for the table and elements.
        // calling that second stage directly must leave the deliberately different shared count unread.
        image_t<128> image;
        image.header(96);
        image.put(96, u32{99});

        trace_t trace;
        auto* values = ::new (image.at<mash::vector<record_t>>(16)) mash::vector<record_t>;

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
                      "pointer vector clear distinguishes borrowed owned and null elements") {

        image_t<256> image;

        trace_t   trace;
        auto*     values = ::new (image.at<mash::vector<record_t>>(16)) mash::vector<record_t>;
        record_t* first  = put_record(image, 64, 1, trace);
        record_t* last   = put_record(image, 96, 3, trace);
        auto*     owned  = ::new (treyarch::memory::heap::allocate(sizeof(record_t))) record_t{2, &trace};
        auto**    table  = (record_t**)treyarch::memory::heap::allocate(4 * sizeof(record_t*));

        table[0]     = first;
        table[1]     = owned;
        table[2]     = nullptr;
        table[3]     = last;
        values->data = table;
        values->size = values->capacity = 4;

        // the two image records fall inside this range; the allocated record and pointer table do not.
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

        // the fixture saved the table just before freeing it, so this also checks that clear nulled every slot.
        CHECK(find(table)->released_bytes == std::vector<u8>(16, 0));
        CHECK(values->data == nullptr);
        CHECK(values->size == 0);
        CHECK(values->mash_image_offset == 0);

        values->clear();

        CHECK(free_count(tests::heap_allocator) == 2);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "nonowning pointer vectors leave elements alive while releasing their table") {

        trace_t trace;
        auto*  owned = ::new (treyarch::memory::heap::allocate(sizeof(record_t))) record_t{1, &trace};
        auto** table = (record_t**)treyarch::memory::heap::allocate(sizeof(record_t*));

        table[0] = owned;
        {
            mash::vector<record_t> values;
            values.data = table;
            values.size = values.capacity = 1;

            // this flag controls element destruction; construction still visits the element and clear still frees the table.
            values.destroy_elements = false;
            values.construct_mashed_class();

            REQUIRE(trace.events.size() == 1);
            CHECK(trace.events[0].kind == event_construct);

            values.clear();

            CHECK_FALSE(values.destroy_elements);
            CHECK(values.empty());
            CHECK(trace.events.size() == 1);
            CHECK(find(owned)->released == false);
            CHECK(find(table)->released);
        }
        owned->~record_t();
        treyarch::memory::heap::free(owned);

        CHECK(free_count(tests::heap_allocator) == 2);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "pointer vector destruction dispatches virtual deleting destructors") {

        trace_t trace;
        {
            mash::vector<mash::mash_virtual_base> values;
            values.data    = (mash::mash_virtual_base**)treyarch::memory::heap::allocate(4);
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
                      "an element starting at the inclusive image end is borrowed") {

        image_t<128> image;

        trace_t trace;
        auto* values = ::new (image.at<mash::vector<record_t>>(16)) mash::vector<record_t>;

        values->mash_image_offset = 48;

        record_t* element = put_record(image, 64, 4, trace);
        values->destroy_element(element);

        CHECK(element == nullptr);
        REQUIRE(trace.events.size() == 1);
        CHECK(trace.events[0].kind == event_destruct_mashed);
        CHECK(free_count(tests::heap_allocator) == 0);

        values->destroy_element(element);

        CHECK(trace.events.size() == 1);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "basic vectors borrow contiguous values from either stream") {

        for (auto selected : {mash::NORMAL_BUFFER, mash::SHARED_BUFFER}) {
            image_t<256> image;
            image.header(128);
            image.put(128, u32{2});
            auto* values = ::new (image.at<mash::vector_basic<pair_t>>(16))
                mash::vector_basic<pair_t> {};
            values->size = 99;

            const u32 data_offset = selected == mash::NORMAL_BUFFER ? 32 : 132;
            image.put(data_offset, pair_t{1, 2});
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
                      "empty basic vectors null stale pointers without reading a payload") {

        image_t<128> image;
        image.header(80);
        auto* values = ::new (image.at<mash::vector_basic<u32>>(16)) mash::vector_basic<u32> {};

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
                      "custom basic vector unmash respects the element alignment without child traversal") {

        image_t<128> image;
        image.header(96);
        image.put(96, u32{99});
        auto* values = ::new (image.at<mash::vector_basic<u64>>(16)) mash::vector_basic<u64> {};

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
                      "basic vector destruction uses the plain allocator once") {

        // the two vector types use different allocation families; the fixture checks that free uses the matching one.
        {
            mash::vector_basic<u32> values {};
            values.data    = (u32*)treyarch::memory::allocate(12, 4, 0);
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
