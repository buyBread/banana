#include <doctest/doctest.h>

#include "mash/fixtures.hh"
#include "treyarch/shared/mash/array_replacement.hh"
#include "treyarch/shared/mash/unmash.hh"
#include "treyarch/shared/mash/vector.hh"

namespace mash = treyarch::mash;
using namespace tests::mash_support;

// the reader finds byte ranges; class traversal uses those ranges as objects of a particular type.
// unmash_class first moves past the object's body, then calls its unmash method to follow any further members.
// unmash_class_in_place visits a member whose body was already included in its parent's bytes.
// construct_class is a later step; the event lists check that pointer repair finishes before construction starts.
namespace tests { namespace mash_support {
    struct alignas(16) wide_record_t : record_t {
        u32 tail[2];
    };

    // a tiny parent and child let us see pointer repair finish before either construction hook runs.
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
    TEST_CASE("plain class traversal honors natural alignment and forwards its parent") {
        image_t<128> image;

        trace_t trace;
        put_record(image, 32, 1, trace);

        mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 128, false);
        reader.advance_buffer(mash::NORMAL_BUFFER, 17);
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

    TEST_CASE("in place member traversal reserves no second object body") {
        // an inline member is stored inside its parent's body, so the reader has already stepped past its bytes.
        // visiting it can repair its pointers without reserving that same body a second time.
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

    TEST_CASE("root traversal performs nested unmash before ownership construction") {
        for (bool headered : {false, true}) {
            image_t<128> image;
            image.header(96);

            trace_t   trace;
            const u32 start = headered ? 16 : 0;
            image.put(start, u32{1});
            image.put(start + 4, &trace);

            // this placeholder is not a usable pointer; unmashing replaces it with the following child's address.
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

    TEST_CASE("polymorphic traversal fixes the vtable and consumes the dynamic tail before unmash") {
        c_registry_fixture registry;
        c_virtual_record prototype(0, nullptr);

        // construct a real object to obtain a callable virtual table; the image only holds its numeric type key.
        registry.set({{0, 0x1001, *(u32*)&prototype}}, 0, 1);
        image_t<128> image;
        image.bytes.fill(0x55);
        std::fill(image.bytes.begin() + 17, image.bytes.begin() + 32, 0xA1);

        trace_t trace;
        image.put(32, u32{0x1001});
        image.put(36, u32{7});
        image.put(40, &trace);

        mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 128, false);
        reader.advance_buffer(mash::NORMAL_BUFFER, 17);
        mash::mash_virtual_base* value = nullptr;

        // the base pointer's type accounts for four bytes, but this record actually occupies twelve.
        // restoring its virtual table lets get_mash_sizeof report twelve before the record's unmash method runs.
        reader.unmash_class(value, &trace, mash::NORMAL_BUFFER);

        CHECK(value == image.at<mash::mash_virtual_base>(32));
        CHECK(*(u32*)value == *(u32*)&prototype);
        CHECK(reader.get_buffer_size_used(mash::NORMAL_BUFFER) == 44);
        REQUIRE(trace.events.size() == 1);
        CHECK(trace.events[0].id == 7);
        CHECK(trace.events[0].parent == &trace);
        CHECK(trace.events[0].normal_used == 44);

        mash::mash_info_struct::construct_class(value);

        REQUIRE(trace.events.size() == 2);
        CHECK(trace.events[1].kind == event_construct);
    }

    TEST_CASE("polymorphic in place members fix their vtables without reserving body bytes") {
        c_registry_fixture registry;
        c_virtual_record prototype(0, nullptr);
        registry.set({{0, 0x1001, *(u32*)&prototype}}, 0, 1);
        image_t<128> image;

        trace_t trace;
        image.put(64, u32{0x1001});
        image.put(68, u32{8});
        image.put(72, &trace);

        mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 128, false);
        reader.advance_buffer(mash::NORMAL_BUFFER, 20);
        auto* value = image.at<c_virtual_record>(64);
        reader.unmash_class_in_place(*value, &trace, mash::SHARED_BUFFER);

        CHECK(*(u32*)value == *(u32*)&prototype);
        CHECK(reader.get_buffer_size_used(mash::NORMAL_BUFFER) == 20);
        CHECK(reader.get_buffer_size_used(mash::SHARED_BUFFER) == 0);
        REQUIRE(trace.events.size() == 1);
        CHECK(trace.events[0].object == value);
        CHECK(trace.events[0].buffer == mash::SHARED_BUFFER);
    }

    TEST_CASE("the root helper constructs a polymorphic root exactly once") {
        c_registry_fixture registry;
        c_virtual_record prototype(0, nullptr);
        registry.set({{0, 0x1001, *(u32*)&prototype}}, 0, 1);
        image_t<128> image;
        image.header(96);

        trace_t trace;
        image.put(16, u32{0x1001});
        image.put(20, u32{9});
        image.put(24, &trace);
        auto* root = mash::unmash_in_place<mash::mash_virtual_base>(image.bytes.data(), 128);

        CHECK(root == image.at<mash::mash_virtual_base>(16));
        REQUIRE(trace.events.size() == 2);
        CHECK(trace.events[0].kind == event_unmash);
        CHECK(trace.events[0].normal_used == 28);
        CHECK(trace.events[1].kind == event_construct);

        // the root helper repairs and constructs objects; the resource handler is what manages this flag.
        CHECK(image.at<mash::mash_info_struct::mash_header>(0)->in_use == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "pointer vectors restore each polymorphic object and preserve the image") {

        c_registry_fixture registry;
        c_virtual_record prototype(0, nullptr);
        registry.set({{0, 0x1001, *(u32*)&prototype}}, 0, 1);
        image_t<256> image;
        image.header(128);
        image.put(128, u32{2});

        trace_t trace;
        auto* values = ::new (image.at<mash::vector<mash::mash_virtual_base>>(16))
            mash::vector<mash::mash_virtual_base>;
        for (u32 index = 0; index < 2; ++index) {
            image.put(44 + 12 * index, u32{0x1001});
            image.put(48 + 12 * index, index + 1);
            image.put(52 + 12 * index, &trace);
        }
        mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 256);
        reader.advance_buffer(mash::NORMAL_BUFFER, 20);
        values->unmash(&reader, &trace, mash::NORMAL_BUFFER);

        CHECK(values->data[0] == image.at<mash::mash_virtual_base>(44));
        CHECK(values->data[1] == image.at<mash::mash_virtual_base>(56));
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
                      "fixed arrays restore polymorphic vtables after reserving all elements") {

        c_registry_fixture registry;
        c_virtual_record prototype(0, nullptr);
        registry.set({{0, 0x1001, *(u32*)&prototype}}, 0, 1);
        image_t<128> image;

        trace_t trace;
        for (u32 index = 0; index < 2; ++index) {
            image.put(16 + 12 * index, u32{0x1001});
            image.put(20 + 12 * index, index + 1);
            image.put(24 + 12 * index, &trace);
        }
        mash::array_replacement_base<c_virtual_record> values {};

        mash::mash_info_struct reader(mash::UNMASH_MODE, image.bytes.data(), 128, false);
        reader.advance_buffer(mash::NORMAL_BUFFER, 16);
        values.custom_unmash_helper(2, &reader, &trace, mash::NORMAL_BUFFER);

        REQUIRE(trace.events.size() == 2);
        CHECK(trace.events[0].normal_used == 40);
        CHECK(trace.events[1].normal_used == 40);
        CHECK(trace.events[0].parent == nullptr);

        values.initialize_helper_mash(2);
        values.finalize_helper_mash(2);

        REQUIRE(trace.events.size() == 6);
        CHECK(trace.events[4].id == 1);
        CHECK(trace.events[5].id == 2);
        CHECK(free_count(tests::heap_allocator) == 0);
    }
}
