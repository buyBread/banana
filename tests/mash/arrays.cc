#include <doctest/doctest.h>

#include "mash/fixtures.hh"
#include "treyarch/shared/mash/array_replacement.hh"

namespace mash = treyarch::mash;
using namespace tests::mash_support;

// array_replacement<record_t, 3> gives a fixed array a pointer to three adjacent records in the image.
// three is part of its type; loading it does not read an element count from the shared stream.
// the reader steps past all three record bodies before visiting the pointers or other members inside each record.
TEST_SUITE("mash") {
    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "fixed arrays reserve the whole body before walking their members") {

        for (auto selected : {mash::NORMAL_BUFFER, mash::SHARED_BUFFER}) {
            image_t<256> image;
            image.header(128);

            trace_t trace;
            auto* values = ::new (image.at<mash::array_replacement<record_t, 3>>(16))
                mash::array_replacement<record_t, 3> {};

            const u32 start = selected == mash::NORMAL_BUFFER ? 20 : 128;
            put_record(image, start,      1, trace);
            put_record(image, start + 8,  2, trace);
            put_record(image, start + 16, 3, trace);

            mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 256);
            reader.advance_buffer(mash::NORMAL_BUFFER, 4);
            values->unmash(&reader, &trace, selected);

            // every member callback sees the cursor after the whole array body, and receives a null parent.
            CHECK(values->array == image.at<record_t>(start));
            REQUIRE(trace.events.size() == 3);

            for (u32 index = 0; index < 3; ++index) {
                CHECK(trace.events[index].id == index + 1);
                CHECK(trace.events[index].parent == nullptr);
                CHECK(trace.events[index].buffer == selected);
                CHECK(trace.events[index].normal_used == (selected == mash::NORMAL_BUFFER ? 44 : 20));
                CHECK(trace.events[index].shared_used == (selected == mash::SHARED_BUFFER ? 24 : 0));
            }
            
            (*values)[1].id = 12;

            CHECK(values->array[1].id == 12);

            values->construct_mashed_class();
            values->destruct_mashed_class();

            REQUIRE(trace.events.size() == 9);
            CHECK(trace.events[3].kind == event_construct);
            CHECK(trace.events[4].id == 12);
            CHECK(trace.events[6].kind == event_destruct_mashed);
            CHECK(trace.events[6].id == 1);
            CHECK(trace.events[7].id == 12);
            CHECK(trace.events[8].id == 3);
            CHECK(values->array == image.at<record_t>(start));
            CHECK(free_count(tests::heap_allocator) == 0);
        }
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "null fixed array helpers perform no lifecycle work") {

        mash::array_replacement<record_t, 3> values {};
        values.construct_mashed_class();
        values.destruct_mashed_class();
        values.initialize_helper_mash(3);
        values.finalize_helper_mash(3);
        values.finalize_helper_allocated(3);

        CHECK(values.array == nullptr);
        CHECK(allocations.empty());
        CHECK(frees.empty());
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "zero length fixed arrays still align and keep the cursor address") {

        // an empty fixed array keeps a position in the stream; empty vectors instead get a null data pointer.
        image_t<64> image;

        mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 64, false);
        reader.advance_buffer(mash::NORMAL_BUFFER, 1);
        auto* values = image.at<mash::array_replacement<record_t, 0>>(32);

        values->array = nullptr;
        values->custom_unmash(&reader, nullptr, mash::NORMAL_BUFFER);

        CHECK(values->array == image.at<record_t>(4));
        CHECK(reader.get_buffer_size_used(mash::NORMAL_BUFFER) == 4);

        values->construct_mashed_class();
        values->destruct_mashed_class();

        CHECK(allocations.empty());
        CHECK(frees.empty());
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "owned fixed arrays use their allocation cookie and destroy in reverse") {

        trace_t trace;
        void* allocation = treyarch::memory::heap::allocate(4 + 3 * sizeof(record_t));

        // the four bytes before the first element hold the allocated element count, often called an array cookie.
        *(u32*)allocation = 3;

        auto* elements = (record_t*)((u8*)allocation + 4);
        for (u32 index = 0; index < 3; ++index)
            ::new (elements + index) record_t{index + 1, &trace};

        {
            // the deliberately different template count shows that owned teardown follows the cookie instead.
            // it destroys elements in reverse and frees the allocation at the cookie, not the element pointer.
            mash::array_replacement<record_t, 2> values {};
            values.array = elements;
        }

        REQUIRE(trace.events.size() == 3);
        CHECK(trace.events[0].id == 3);
        CHECK(trace.events[1].id == 2);
        CHECK(trace.events[2].id == 1);
        CHECK(trace.events[0].kind == event_destructor);
        CHECK(trace.destructor_saw_live_storage);
        CHECK(free_count(tests::heap_allocator) == 1);
        CHECK(find(allocation)->released);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "an owned array with a zero element cookie still frees its allocation") {

        void* allocation = treyarch::memory::heap::allocate(4);

        *(u32*)allocation = 0;
        mash::array_replacement_base<record_t> values{(record_t*)((u8*)allocation + 4)};
        values.finalize_helper_allocated(99);

        CHECK(free_count(tests::heap_allocator) == 1);
        CHECK(find(allocation)->released);
    }
}
