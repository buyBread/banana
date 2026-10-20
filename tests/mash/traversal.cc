#include <doctest/doctest.h>

#include "mash/fixtures.hh"
#include "treyarch/shared/mash/array_replacement.hh"
#include "treyarch/shared/mash/unmash.hh"
#include "treyarch/shared/mash/vector.hh"

namespace mash = treyarch::mash;
using namespace tests::mash_support;

// reader.cc hands out raw byte ranges; this file turns those ranges into objects.
// unmash_class(pointer) reads one object's worth of bytes, points the pointer at them and then calls that object's unmash method.
// that unmash method is where an object reads whatever it points to, so loading spreads out from the first object, the root.
// unmash_class_in_place does the same for an object that's stored inside another one, so its bytes were already read with its parent.
// construct_class runs construct_mashed_class afterwards; the traces check that every pointer is fixed before anything gets constructed.
namespace tests { namespace mash_support {
    // a record with a sixteen byte alignment requirement, to see the reader honor it.
    struct alignas(16) wide_record_t : record_t {
        u32 tail[2];
    };

    // a parent with one child pointer, the smallest case of loading spreading from one object to the next.
    struct nested_record_t {
        u32       id;
        trace_t*  trace;
        record_t* child;

        void unmash(mash::mash_info_struct* reader,
                    void*                   parent,
                    mash::buffer_type       buffer);

        void construct_mashed_class();
    };
}}

void tests::mash_support::nested_record_t::unmash(mash::mash_info_struct* reader,
                                                  void*                   parent,
                                                  mash::buffer_type       buffer) {

    trace->push(event_unmash, id, this, parent, buffer, reader);
    reader->unmash_class(child, this, buffer);
}

void tests::mash_support::nested_record_t::construct_mashed_class() {
    trace->push(event_construct, id, this);
    mash::mash_info_struct::construct_class(child);
}

TEST_SUITE("mash") {
    TEST_CASE("loading a plain object respects its alignment and passes its parent along") {
        image_t<128> image;

        trace_t trace;
        put_record(image, 32, 1, trace);

        mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 128, false);
        reader.advance_buffer(mash::NORMAL_BUFFER, 17);

        // the cursor is at 17, so the sixteen byte alignment moves the record to 32 and its sixteen bytes end at 48.
        wide_record_t* value = nullptr;
        reader.unmash_class(value, &trace, mash::NORMAL_BUFFER);

        CHECK(value == image.at<wide_record_t>(32));
        CHECK(reader.get_buffer_size_used(mash::NORMAL_BUFFER) == 48);
        REQUIRE(trace.events.size() == 1);
        CHECK(trace.events[0].kind == event_unmash);
        CHECK(trace.events[0].parent == &trace);
        CHECK(trace.events[0].normal_used == 48);

        mash::mash_info_struct::construct_class(value);

        REQUIRE(trace.events.size() == 2);
        CHECK(trace.events[1].kind == event_construct);
    }

    TEST_CASE("an object stored inside its parent doesn't read any bytes of its own") {
        // its parent's read already covered these bytes, so only its unmash method runs.
        image_t<128> image;

        trace_t   trace;
        record_t* value = put_record(image, 64, 1, trace);

        mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 128, false);
        reader.set_buffer(mash::SHARED_BUFFER, image.bytes.data() + 96, 32);
        reader.advance_buffer(mash::NORMAL_BUFFER, 20);
        reader.advance_buffer(mash::SHARED_BUFFER, 3);
        reader.unmash_class_in_place(*value, &trace, mash::SHARED_BUFFER);

        CHECK(reader.get_buffer_size_used(mash::NORMAL_BUFFER) == 20);
        CHECK(reader.get_buffer_size_used(mash::SHARED_BUFFER) == 3);
        REQUIRE(trace.events.size() == 1);
        CHECK(trace.events[0].object == value);
        CHECK(trace.events[0].parent == &trace);
        CHECK(trace.events[0].buffer == mash::SHARED_BUFFER);
    }

    TEST_CASE("unmash_in_place unmashes the whole tree of objects before constructing any of them") {
        for (bool headered : {false, true}) {
            CAPTURE(headered);

            image_t<128> image;
            image.header(96);

            // the root goes first in the normal stream, so right after the header or at 0 when there isn't one.
            trace_t   trace;
            const u32 start = headered ? 16 : 0;
            image.put(start,     u32{1});
            image.put(start + 4, &trace);

            // this is the saved child pointer; it's junk until unmashing points it at the child that comes right after the root.
            image.put(start + 8, u32{0x12345678});

            record_t* child = put_record(image, start + 12, 2, trace);
            auto*     root  = mash::unmash_in_place<nested_record_t>(image.bytes.data(), 128, headered);

            CHECK(root == image.at<nested_record_t>(start));
            CHECK(root->child == child);
            REQUIRE(trace.events.size() == 4);
            CHECK(trace.events[0].kind == event_unmash);
            CHECK(trace.events[0].id == 1);
            CHECK(trace.events[0].parent == nullptr);
            CHECK(trace.events[1].kind == event_unmash);
            CHECK(trace.events[1].parent == root);
            CHECK(trace.events[1].normal_used == start + 20);
            CHECK(trace.events[2].kind == event_construct);
            CHECK(trace.events[2].id == 1);
            CHECK(trace.events[3].kind == event_construct);
            CHECK(trace.events[3].id == 2);
        }
    }

    TEST_CASE("loading a virtual object fixes its vtable first, so it can say how big it really is") {
        c_registry_fixture registry;
        registry.register_virtual_record();

        // the bytes before the record are 0xa1 filler; a virtual read finds where the object starts by skipping them.
        image_t<128> image;
        image.bytes.fill(0x55);
        std::fill(image.bytes.begin() + 17, image.bytes.begin() + 32, mash::alignment_padding);

        trace_t trace;
        put_virtual_record(image, 32, 7, trace);

        mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 128, false);
        reader.advance_buffer(mash::NORMAL_BUFFER, 17);

        // a mash_virtual_base pointer only knows about four bytes, but the object behind it is a twelve byte c_virtual_record.
        // once the vtable is in place, get_mash_sizeof can be asked for the real size; the reader skips the other eight bytes before unmash runs.
        mash::mash_virtual_base* value = nullptr;
        reader.unmash_class(value, &trace, mash::NORMAL_BUFFER);

        CHECK(value == image.at<mash::mash_virtual_base>(32));
        CHECK(first_word(value) == registry.virtual_record_vtable);
        CHECK(reader.get_buffer_size_used(mash::NORMAL_BUFFER) == 44);
        REQUIRE(trace.events.size() == 1);
        CHECK(trace.events[0].id == 7);
        CHECK(trace.events[0].parent == &trace);
        CHECK(trace.events[0].normal_used == 44);

        mash::mash_info_struct::construct_class(value);

        REQUIRE(trace.events.size() == 2);
        CHECK(trace.events[1].kind == event_construct);
    }

    TEST_CASE("a virtual object stored inside its parent gets its vtable without reading any bytes") {
        c_registry_fixture registry;
        registry.register_virtual_record();

        image_t<128> image;

        trace_t trace;
        auto*   value = put_virtual_record(image, 64, 8, trace);

        mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 128, false);
        reader.advance_buffer(mash::NORMAL_BUFFER, 20);
        reader.unmash_class_in_place(*value, &trace, mash::SHARED_BUFFER);

        CHECK(first_word(value) == registry.virtual_record_vtable);
        CHECK(reader.get_buffer_size_used(mash::NORMAL_BUFFER) == 20);
        CHECK(reader.get_buffer_size_used(mash::SHARED_BUFFER) == 0);
        REQUIRE(trace.events.size() == 1);
        CHECK(trace.events[0].object == value);
        CHECK(trace.events[0].buffer == mash::SHARED_BUFFER);
    }

    TEST_CASE("unmash_in_place constructs a virtual root exactly once") {
        c_registry_fixture registry;
        registry.register_virtual_record();

        image_t<128> image;
        image.header(96);

        trace_t trace;
        put_virtual_record(image, 16, 9, trace);

        auto* root = mash::unmash_in_place<mash::mash_virtual_base>(image.bytes.data(), 128);

        CHECK(root == image.at<mash::mash_virtual_base>(16));
        REQUIRE(trace.events.size() == 2);
        CHECK(trace.events[0].kind == event_unmash);
        CHECK(trace.events[0].normal_used == 28);
        CHECK(trace.events[1].kind == event_construct);

        // in_use belongs to resource loading, which graphs.cc covers; unmash_in_place never touches it.
        CHECK(image.at<mash::mash_info_struct::mash_header>(0)->in_use == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "a vector of virtual objects fixes every vtable and frees nothing from the image") {

        // containers.cc covers vectors in depth; this only checks that each element goes through the virtual loading path.
        c_registry_fixture registry;
        registry.register_virtual_record();

        image_t<256> image;
        image.header(128);
        image.put(128, u32{2});

        trace_t trace;
        auto*   values = image.make<mash::vector<mash::mash_virtual_base>>(16);

        // the vector's body is bytes 16 to 35, its two pointer slots are bytes 36 to 43 and the two records follow.
        put_virtual_record(image, 44, 1, trace);
        put_virtual_record(image, 56, 2, trace);

        mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 256);
        reader.advance_buffer(mash::NORMAL_BUFFER, 20);
        values->unmash(&reader, &trace, mash::NORMAL_BUFFER);

        CHECK(values->data[0] == image.at<mash::mash_virtual_base>(44));
        CHECK(values->data[1] == image.at<mash::mash_virtual_base>(56));
        CHECK(first_word(values->data[1]) == registry.virtual_record_vtable);
        CHECK(reader.get_buffer_size_used(mash::NORMAL_BUFFER) == 68);

        values->construct_mashed_class();
        values->destruct_mashed_class();

        REQUIRE(trace.events.size() == 6);
        CHECK(trace.events[4].kind == event_destruct_mashed);
        CHECK(trace.events[4].id == 2);
        CHECK(trace.events[5].id == 1);
        CHECK(free_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "a fixed array of virtual objects reads every element before fixing their vtables") {

        // arrays.cc covers fixed arrays in depth; the elements here sit back to back, so their bytes are read in one go.
        c_registry_fixture registry;
        registry.register_virtual_record();

        image_t<128> image;

        trace_t trace;
        put_virtual_record(image, 16, 1, trace);
        put_virtual_record(image, 28, 2, trace);

        mash::array_replacement_base<c_virtual_record> values {};

        mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 128, false);
        reader.advance_buffer(mash::NORMAL_BUFFER, 16);
        values.custom_unmash_helper(2, &reader, &trace, mash::NORMAL_BUFFER);

        REQUIRE(trace.events.size() == 2);
        CHECK(trace.events[0].normal_used == 40);
        CHECK(trace.events[1].normal_used == 40);
        CHECK(trace.events[0].parent == nullptr);
        CHECK(first_word(&values.array[1]) == registry.virtual_record_vtable);

        values.initialize_helper_mash(2);
        values.finalize_helper_mash(2);

        REQUIRE(trace.events.size() == 6);
        CHECK(trace.events[4].id == 1);
        CHECK(trace.events[5].id == 2);
        CHECK(free_count(tests::heap_allocator) == 0);
    }
}
