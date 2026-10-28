#include <doctest/doctest.h>

#include "mash/fixtures.hh"
#include "treyarch/mash/array_replacement.hh"

namespace mash = treyarch::mash;
using namespace tests::mash_support;

// array_replacement<record_t, 3> is a pointer to exactly three records sitting back to back.
// the 3 is part of the type itself, so unlike a vector, loading it never reads a count from the shared stream.
// all three records are read in one go, and only then does each record's unmash method run.
TEST_SUITE("mash") {
    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "a fixed array reads all of its elements before unmashing any of them") {

        for (auto selected : {mash::NORMAL_BUFFER, mash::SHARED_BUFFER}) {
            CAPTURE(selected);

            image_t<256> image;
            image.header(128);

            // the array itself is just its four byte pointer, at bytes 16 to 19.
            trace_t trace;
            auto*   values = image.make<mash::array_replacement<record_t, 3>>(16);

            const u32 start = selected == mash::NORMAL_BUFFER ? 20 : 128;
            put_record(image, start,      1, trace);
            put_record(image, start + 8,  2, trace);
            put_record(image, start + 16, 3, trace);

            mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 256);
            reader.advance_buffer(mash::NORMAL_BUFFER, 4);
            values->unmash(&reader, &trace, selected);

            // every unmash call sees the cursor already past all three records, and gets no parent.
            CHECK(values->array == image.at<record_t>(start));
            REQUIRE(trace.events.size() == 3);

            for (u32 index = 0; index < 3; ++index) {
                CAPTURE(index);

                CHECK(trace.events[index].id == index + 1);
                CHECK(trace.events[index].parent == nullptr);
                CHECK(trace.events[index].buffer == selected);
                CHECK(trace.events[index].normal_used == (selected == mash::NORMAL_BUFFER ? 44 : 20));
                CHECK(trace.events[index].shared_used == (selected == mash::SHARED_BUFFER ? 24 : 0));
            }

            // [] reaches straight into the image.
            (*values)[1].id = 12;

            CHECK(values->array[1].id == 12);

            // construction and teardown both go front to back, unlike a vector's teardown.
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
                      "a fixed array with a null pointer does nothing at all") {

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
                      "an empty fixed array still aligns and points at the cursor") {

        // an empty vector ends up with a null pointer, but an empty fixed array still points at a real spot in the image.
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
                      "an owned fixed array goes by its hidden count, destroying back to front") {

        // when c++ makes an array with new[], it stores the element count in the four bytes right before the first element.
        // that hidden count is often called an array cookie, and delete[] uses it to know how many destructors to run.
        trace_t trace;
        void*   allocation = treyarch::mm::allocate(4 + 3 * sizeof(record_t));

        *(u32*)allocation = 3;

        auto* elements = (record_t*)((u8*)allocation + 4);

        for (u32 index = 0; index < 3; ++index)
            ::new (elements + index) record_t{index + 1, &trace};

        {
            // the 2 in the type is on purpose; teardown follows the cookie's 3 instead.
            // the allocation is freed from the cookie's address, since that's where it really starts.
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
                      "an owned fixed array with a cookie of 0 still frees its allocation") {

        void* allocation = treyarch::mm::allocate(4);

        *(u32*)allocation = 0;

        mash::array_replacement_base<record_t> values{(record_t*)((u8*)allocation + 4)};
        values.finalize_helper_allocated(99);

        CHECK(free_count(tests::heap_allocator) == 1);
        CHECK(find(allocation)->released);
    }
}
