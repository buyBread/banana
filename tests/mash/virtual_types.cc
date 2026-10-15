#include <doctest/doctest.h>

#include "mash/fixtures.hh"

namespace mash = treyarch::mash;
using namespace tests::mash_support;

// c++ objects with virtual methods carry a pointer to a table of functions.
// the saved image has a numeric type key in that pointer's place; it cannot be used to call a function yet.
// lookup finds the live table for that key, and fixup_vtable writes its address into the object's first four bytes.
// only then can virtual calls reach the correct unmash and construction methods for the loaded type.
// fixtures supply the registry entries; traversal.cc shows these operations while loading a complete object.
TEST_SUITE("mash") {
    TEST_CASE("base virtual type identity and subclass predicates are distinct") {
        mash::mash_virtual_base base;

        CHECK(base.get_virtual_type_key() == 0x0668FEE9);
        CHECK(base.get_mash_sizeof() == 4);
        CHECK(mash::mash_virtual_base::generate_virtual_types_key_from_string("MASH_VIRTUAL_BASE") == 0x0668FEE9);
        CHECK(base.is_or_is_subclass_of(0x0668FEE9));
        CHECK_FALSE(base.is_subclass_of(0x0668FEE9));
        CHECK_FALSE(base.is_or_is_subclass_of(0x1001));

        c_virtual_record derived(7, nullptr);

        CHECK(derived.is_or_is_subclass_of(0x1001));
        CHECK(derived.is_or_is_subclass_of(0x1000));
        CHECK_FALSE(derived.is_subclass_of(0x1001));
        CHECK_FALSE(derived.is_or_is_subclass_of(0x1002));

        base.construct_mashed_class();
        base.destruct_mashed_class();
        base.unmash(nullptr, nullptr, mash::NORMAL_BUFFER);
    }

    TEST_CASE("base copy values validates compatible types without copying fields") {
        c_virtual_record source(1, nullptr);
        c_virtual_record destination(2, nullptr);
        destination.copy_values(&source);

        CHECK(destination.id == 2);
        CHECK(destination.get_virtual_type_key() == source.get_virtual_type_key());
    }

    TEST_CASE("empty vtable registries report misses without changing serialized keys") {
        c_registry_fixture registry;
        u32 key = 0x12345678;

        CHECK(mash::mash_virtual_base::lookup_vtable_entry(key) == 0);
        CHECK_FALSE(mash::mash_virtual_base::verify_valid_vtable(&key));
        CHECK_FALSE(mash::mash_virtual_base::verify_valid_vtable_map(&key));

        mash::mash_virtual_base::fixup_vtable(&key);

        CHECK(key == 0x12345678);
    }

    TEST_CASE("vtable lookup handles collisions and unsigned key ordering") {
        c_registry_fixture registry;

        // a bucket is a search range within the registry's list; mask zero sends every key to this one bucket.
        // each row supplies the bucket number, type key and value returned for that key.
        registry.set({ {0, 0,          11},
                       {0, 10,         22},
                       {0, 20,         33},
                       {0, 0x80000000, 44},
                       {0, 0xFFFFFFFF, 55} }, 0, 1);

        for (const auto &pair : { std::pair<u32, u32>{ 0,          11 },
                                                     { 10,         22 },
                                                     { 20,         33 },
                                                     { 0x80000000, 44 },
                                                     { 0xFFFFFFFF, 55 } })

            CHECK(mash::mash_virtual_base::lookup_vtable_entry(pair.first) == pair.second);

        for (u32 key : {1u, 9u, 11u, 19u, 21u, 0x7FFFFFFFu, 0xFFFFFFFEu})
            CHECK(mash::mash_virtual_base::lookup_vtable_entry(key) == 0);
    }

    TEST_CASE("vtable buckets use signed division for keys across the high bit") {
        // lookup mixes a key into a bucket number; interpreting its high bit as a sign changes that calculation.
        // expected bucket numbers are written into these rows directly, so the fixture cannot repeat the same mistake.
        c_registry_fixture registry;

        registry.set({ { 0, 0x7FFFFFFF, 101 },
                       { 0, 0x80000000, 102 },
                       { 0, 0xDEADBEEF, 103 },
                       { 1, 0,          104 },
                       { 2, 1,          105 },
                       { 3, 2,          106 },
                       { 4, 3,          107 },
                       { 5, 4,          108 },
                       { 6, 5,          109 },
                       { 7, 0xFFFFFFFF, 110 } } );

        for (const auto &pair : { std::pair<u32, u32>{ 0x7FFFFFFF, 101 },
                                                     { 0x80000000, 102 },
                                                     { 0xDEADBEEF, 103 },
                                                     { 0,          104 },
                                                     { 1,          105 },
                                                     { 2,          106 },
                                                     { 3,          107 },
                                                     { 4,          108 },
                                                     { 5,          109 },
                                                     { 0xFFFFFFFF, 110 } }) {

            CAPTURE(pair.first);

            CHECK(mash::mash_virtual_base::lookup_vtable_entry(pair.first) == pair.second);
        }
    }

    TEST_CASE("vtable buckets above the active count fold into the lower half") {
        // a mask of seven can select eight buckets, but only five are active here.
        // selections past that count are moved down by four before the bucket is searched.
        c_registry_fixture registry;

        registry.set({ {1, 0,          11},
                       {1, 4,          12},
                       {2, 1,          21},
                       {2, 5,          22},
                       {3, 2,          31},
                       {3, 0xFFFFFFFF, 32} }, 7, 5);

        for (const auto &pair : { std::pair<u32, u32>{ 0,          11 },
                                                     { 4,          12 },
                                                     { 1,          21 },
                                                     { 5,          22 },
                                                     { 2,          31 },
                                                     { 0xFFFFFFFF, 32 } })

            CHECK(mash::mash_virtual_base::lookup_vtable_entry(pair.first) == pair.second);

        CHECK(mash::mash_virtual_base::lookup_vtable_entry(u32{3}) == 0);
    }

    TEST_CASE("lookup cannot walk through a bucket boundary to another bucket") {
        c_registry_fixture registry;
        registry.set({{0, 0x7FFFFFFF, 1}, {1, 0, 99}});

        CHECK(mash::mash_virtual_base::lookup_vtable_entry(u32{0x80000000}) == 0);
    }

    TEST_CASE("vtable verification scans values and fixup only replaces nonzero entries") {
        // fixup searches by key, but verification searches the registered table addresses.
        // before fixup, 20 is a key; afterward, 0x5678 is the table address that verification accepts.
        c_registry_fixture registry;
        registry.set({{0, 10, 0x1234}, {0, 20, 0x5678}, {0, 30, 0}}, 0, 1);
        u32 value = 0x5678;

        CHECK(mash::mash_virtual_base::verify_valid_vtable_map(&value));
        CHECK(mash::mash_virtual_base::verify_valid_vtable(&value));

        value = 20;

        CHECK_FALSE(mash::mash_virtual_base::verify_valid_vtable(&value));

        mash::mash_virtual_base::fixup_vtable(&value);

        CHECK(value == 0x5678);

        value = 30;
        mash::mash_virtual_base::fixup_vtable(&value);

        CHECK(value == 30);
    }

    TEST_CASE("construct helper invokes ownership construction without looking up a key") {
        // this object already has a live virtual table, so construction works even with an empty registry.
        c_registry_fixture registry;

        trace_t trace;
        c_virtual_record value(9, &trace);

        CHECK(mash::mash_virtual_base::construct_class_helper(&value) == &value);
        REQUIRE(trace.events.size() == 1);
        CHECK(trace.events[0].kind == event_construct);

        value.trace = nullptr;
    }

    TEST_CASE("creating a subclass fixes its vtable before constructing it in place") {
        c_registry_fixture registry;
        c_virtual_record prototype(0, nullptr);
        registry.set({{0, 0x1001, *(u32*)&prototype}}, 0, 1);

        image_t<64> image;
        trace_t     trace;
        image.put(4, u32{12});
        image.put(8, &trace);
        auto* original = image.at<mash::mash_virtual_base>(0);
        auto* result   = mash::mash_virtual_base::create_subclass_in_place(0x1001, original);

        CHECK(result == original);
        CHECK(result->get_virtual_type_key() == 0x1001);
        REQUIRE(trace.events.size() == 1);
        CHECK(trace.events[0].kind == event_construct);
        CHECK(trace.events[0].id == 12);
    }
}
