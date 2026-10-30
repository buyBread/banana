#include <doctest/doctest.h>

#include "mash/fixtures.hh"
#include "treyarch/mash/vector.hh"
#include "treyarch/mash/vector_basic.hh"
#include "treyarch/mash/vector_basic_ref_counted.hh"
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
        CHECK(values.max_size == 0);
        CHECK(values.array == nullptr);
        CHECK(values.destruct_contents);
        CHECK(values.begin() == nullptr);
        CHECK(values.end() == nullptr);

        const auto &constant = values;

        CHECK(constant.begin() == nullptr);
        CHECK(constant.end() == nullptr);
        CHECK(values.get_mash_sizeof() == 20);

        // initialize(ALLOCATED) is the reset for a vector that isn't from an image.
        values.destruct_contents = false;
        values.max_size          = 8;
        values.initialize(mash::ALLOCATED);

        CHECK(values.destruct_contents);
        CHECK(values.max_size == 0);
        CHECK(values.array == nullptr);
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
            CHECK(values->max_size == 2);
            CHECK(values->array == image.at<record_t*>(table));
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
            CHECK(values->max_size == 0);
            CHECK(values->mash_image_offset == 0);
            CHECK(values->array == nullptr);
            CHECK(free_count(tests::heap_allocator) == 0);
        }
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "an empty pointer vector reads no slots but still measures its image bytes") {

        image_t<128> image;
        image.header(80);

        auto* values = image.make<mash::vector<record_t>>(16);

        values->max_size = 99;
        values->array    = image.at<record_t*>(64);

        mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 128);
        reader.advance_buffer(mash::NORMAL_BUFFER, 20);
        values->unmash(&reader, nullptr, mash::NORMAL_BUFFER);

        CHECK(values->array == nullptr);
        CHECK(values->max_size == 0);
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

        REQUIRE(values->array == image.at<record_t*>(36));
        CHECK(values->array[0] == element);
        CHECK(values->max_size == 1);
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
        values->array = table;
        values->size  = values->max_size = 4;

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
        CHECK(values->array == nullptr);
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
            values.array = table;
            values.size  = values.max_size = 1;

            // destruct_contents only decides whether clear destroys the elements; construction still visits them.
            values.destruct_contents = false;
            values.construct_mashed_class();

            REQUIRE(trace.events.size() == 1);
            CHECK(trace.events[0].kind == event_construct);

            values.clear();

            CHECK_FALSE(values.destruct_contents);
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
            values.array    = (mash::mash_virtual_base**)treyarch::mm::allocate(4);
            values.array[0] = new c_virtual_record(8, &trace);
            values.size     = values.max_size = 1;
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
            CHECK(values->max_size == 2);
            CHECK(values->array == image.at<pair_t>(data_offset));
            CHECK((size_t)values->array % 4 == 0);
            CHECK(values->array[0].first == 1);
            CHECK(values->array[1].second == 4);
            CHECK(values->mash_image_offset == data_offset + 8 - 16);

            values->construct_mashed_class();

            CHECK(values->size == 2);

            values->destruct_mashed_class();

            CHECK(values->array == nullptr);
            CHECK(values->max_size == 0);
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

        values->array    = image.at<u32>(64);
        values->max_size = 12;

        mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 128);
        reader.advance_buffer(mash::NORMAL_BUFFER, 16);
        values->unmash(&reader, nullptr, mash::NORMAL_BUFFER);

        CHECK(values->array == nullptr);
        CHECK(values->max_size == 0);
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

        REQUIRE(values->array == image.at<u64>(40));
        CHECK(values->array[0] == 0x1122334455667788);
        CHECK(values->array[1] == 0x8877665544332211);
        CHECK(values->max_size == 2);
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
            values.array    = (u32*)treyarch::tl_mem_alloc(12, 4, 0);
            values.size     = values.max_size = 3;
            values.array[0] = 10;
            values.clear();

            CHECK(values.array == nullptr);
            CHECK(values.size == 0);
            CHECK(values.max_size == 0);
            CHECK(values.mash_image_offset == 0);
        }

        CHECK(free_count(tests::plain_allocator) == 1);
        CHECK(free_count(tests::heap_allocator) == 0);
        CHECK(live_count(tests::plain_allocator) == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "erasing from a pointer vector destroys the element, slides the rest down and nulls the old last slot") {

        image_t<256> image;

        trace_t trace;
        auto*   values = image.make<mash::vector<record_t>>(16);
        auto**  table  = (record_t**)treyarch::mm::allocate(3 * sizeof(record_t*));

        table[0] = put_record(image, 64, 1, trace);
        table[1] = put_record(image, 72, 2, trace);
        table[2] = put_record(image, 80, 3, trace);

        values->array             = table;
        values->size              = values->max_size = 3;
        values->mash_image_offset = 80;

        // erase hands back the spot it was given, which now holds the element that came after it.
        auto next = values->erase(values->begin() + 1);

        CHECK(next == table + 1);
        REQUIRE(trace.events.size() == 1);
        CHECK(trace.events[0].kind == event_destruct_mashed);
        CHECK(trace.events[0].id == 2);
        CHECK(values->size == 2);
        CHECK(table[0] == image.at<record_t>(64));
        CHECK(table[1] == image.at<record_t>(80));
        CHECK(table[2] == nullptr);

        // with destruct_contents off the element stays alive; erasing the last one has nothing to slide.
        values->destruct_contents = false;
        values->erase(values->begin() + 1);

        CHECK(trace.events.size() == 1);
        CHECK(values->size == 1);
        CHECK(table[1] == image.at<record_t>(80));

        values->clear();

        CHECK(free_count(tests::heap_allocator) == 1);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "resizing a pointer vector down destroys what's past the new size, even when it doesn't own its elements") {

        image_t<256> image;

        trace_t trace;
        auto*   values = image.make<mash::vector<record_t>>(16);
        auto**  table  = (record_t**)treyarch::mm::allocate(3 * sizeof(record_t*));

        table[0] = put_record(image, 64, 1, trace);
        table[1] = put_record(image, 72, 2, trace);
        table[2] = put_record(image, 80, 3, trace);

        values->array             = table;
        values->size              = values->max_size = 3;
        values->mash_image_offset = 80;
        values->destruct_contents = false;

        record_t fill{9, &trace};
        values->resize(1, fill);

        REQUIRE(trace.events.size() == 2);
        CHECK(trace.events[0].kind == event_destruct_mashed);
        CHECK(trace.events[0].id == 3);
        CHECK(trace.events[1].id == 2);
        CHECK(values->size == 1);
        CHECK(values->max_size == 3);
        CHECK(table[1] == nullptr);
        CHECK(table[2] == nullptr);

        values->clear();
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "resizing a pointer vector up points every new slot at the one fill object") {

        trace_t  trace;
        record_t fill{9, &trace};

        {
            mash::vector<record_t> values;
            values.destruct_contents = false;
            values.resize(3, fill);

            CHECK(values.size == 3);
            CHECK(values.max_size == 3);
            CHECK(values.array[0] == &fill);
            CHECK(values.array[1] == &fill);
            CHECK(values.array[2] == &fill);
            CHECK(allocations.size() == 1);
        }

        CHECK(trace.events.empty());
        CHECK(free_count(tests::heap_allocator) == 1);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "erasing from a basic vector slides the rest down and leaves the old last value behind") {

        mash::vector_basic<u32> values {};
        values.array = (u32*)treyarch::tl_mem_alloc(16, 4, 0);
        values.size  = values.max_size = 4;

        values.array[0] = 10;
        values.array[1] = 20;
        values.array[2] = 30;
        values.array[3] = 40;

        auto next = values.erase(values.begin() + 1);

        CHECK(next == values.array + 1);
        CHECK(values.size == 3);
        CHECK(values.array[0] == 10);
        CHECK(values.array[1] == 30);
        CHECK(values.array[2] == 40);
        CHECK(values.array[3] == 40);

        values.erase(values.begin() + 2);

        CHECK(values.size == 2);
        CHECK(values.array[1] == 30);

        values.clear();

        CHECK(free_count(tests::plain_allocator) == 1);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "a ref-counted vector reads its internals and their vector from shared, then tears them down in place") {

        bool reset_ref_count = false;
        c_reference_binding<bool> binding(mash::references::unk_01126c84, &reset_ref_count);

        // only the four byte pointer sits in normal; the presence byte, the internals, the vector body, its count and its values are all in shared.
        // 128 is the presence byte, the internals are 132 to 139, the vector body 140 to 155, the count 156 and the values 160 to 167.
        image_t<256> image;
        image.header(128);
        image.put(128, mash::member_class_mashed);
        image.put(136, u16{0});
        image.put(138, u16{7});
        image.put(156, u32{2});
        image.put(160, u32{10});
        image.put(164, u32{20});

        auto* values = image.make<mash::vector_basic_ref_counted<u32>>(16);

        mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 256);
        values->unmash(&reader, nullptr, mash::NORMAL_BUFFER);

        auto* internals = image.at<mash::vector_basic_ref_count_internals<u32>>(132);
        auto* data      = image.at<mash::vector_basic<u32>>(140);

        REQUIRE(values->internals == internals);
        REQUIRE(internals->data == data);
        CHECK(data->array == image.at<u32>(160));
        CHECK(data->mash_image_offset == 28);
        CHECK(values->size() == 2);
        CHECK((*values)[1] == 20);
        CHECK(reader.get_buffer_size_used(mash::NORMAL_BUFFER) == 16);
        CHECK(reader.get_buffer_size_used(mash::SHARED_BUFFER) == 40);

        // constructing counts one more owner and marks the internals as not allocated.
        values->construct_mashed_class();

        CHECK(internals->ref_count == 1);
        CHECK(internals->dynamic == 0);

        // the last owner tears the vector down, but its values lie inside the image, so nothing is freed.
        values->destruct_mashed_class();

        CHECK(values->internals == nullptr);
        CHECK(internals->ref_count == 0);
        CHECK(internals->data == nullptr);
        CHECK(data->array == nullptr);
        CHECK(data->size == 0);
        CHECK(data->mash_image_offset == 0);
        CHECK(frees.empty());
    }

    TEST_CASE("a ref-counted vector with no presence byte has no internals") {
        image_t<128> image;
        image.header(64);

        auto* values      = image.make<mash::vector_basic_ref_counted<u32>>(16);
        values->internals = image.at<mash::vector_basic_ref_count_internals<u32>>(32);

        mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 128);
        values->unmash(&reader, nullptr, mash::NORMAL_BUFFER);

        CHECK(values->internals == nullptr);
        CHECK(reader.get_buffer_size_used(mash::SHARED_BUFFER) == 1);

        values->construct_mashed_class();

        CHECK(values->internals == nullptr);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "owners sharing ref-counted internals only tear them down when the last one lets go") {

        bool reset_ref_count = false;
        c_reference_binding<bool> binding(mash::references::unk_01126c84, &reset_ref_count);

        image_t<128> image;

        auto* internals = image.at<mash::vector_basic_ref_count_internals<u32>>(32);
        auto* data      = image.make<mash::vector_basic<u32>>(48);

        internals->data      = data;
        internals->ref_count = 0;
        internals->dynamic   = 0;

        auto* first  = image.make<mash::vector_basic_ref_counted<u32>>(16);
        auto* second = image.make<mash::vector_basic_ref_counted<u32>>(20);

        first ->internals = internals;
        second->internals = internals;

        first ->construct_mashed_class();
        second->construct_mashed_class();

        CHECK(internals->ref_count == 2);

        first->destruct_mashed_class();

        CHECK(first->internals == nullptr);
        CHECK(internals->ref_count == 1);
        CHECK(internals->data == data);

        second->destruct_mashed_class();

        CHECK(second->internals == nullptr);
        CHECK(internals->ref_count == 0);
        CHECK(internals->data == nullptr);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "allocated ref-counted internals free their values, their vector and themselves") {

        bool reset_ref_count = false;
        c_reference_binding<bool> binding(mash::references::unk_01126c84, &reset_ref_count);

        // the values go back through the plain allocator like any basic vector's; the vector and the internals through the heap.
        auto* data = (mash::vector_basic<u32>*)treyarch::mm::allocate(sizeof(mash::vector_basic<u32>));
        std::memset(data, 0, sizeof(*data));
        data->array = (u32*)treyarch::tl_mem_alloc(8, 4, 0);
        data->size  = data->max_size = 2;

        auto* internals = (mash::vector_basic_ref_count_internals<u32>*)treyarch::mm::allocate(sizeof(mash::vector_basic_ref_count_internals<u32>));
        internals->data      = data;
        internals->ref_count = 1;
        internals->dynamic   = 1;

        {
            mash::vector_basic_ref_counted<u32> values {};
            values.internals = internals;
        }

        CHECK(free_count(tests::plain_allocator) == 1);
        CHECK(free_count(tests::heap_allocator) == 2);
        CHECK(live_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "with the reset flag set, the internals forget their other owners before tearing down") {

        bool reset_ref_count = true;
        c_reference_binding<bool> binding(mash::references::unk_01126c84, &reset_ref_count);

        image_t<128> image;

        auto* internals = image.at<mash::vector_basic_ref_count_internals<u32>>(32);
        auto* data      = image.make<mash::vector_basic<u32>>(48);

        internals->data      = data;
        internals->ref_count = 3;
        internals->dynamic   = 0;

        // destruct_mashed_class on the internals is what the owner's own teardown calls once the count reaches zero.
        internals->destruct_mashed_class();

        CHECK(internals->ref_count == 0);
        CHECK(internals->data == nullptr);
    }
}
