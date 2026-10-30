#include <utility>
#include <doctest/doctest.h>

#include "mash/fixtures.hh"

namespace mash = treyarch::mash;
using namespace tests::mash_support;

// an object with virtual methods keeps a pointer to its class's vtable in its first four bytes.
// a vtable is a table of addresses of functions, and the game's addresses are different every time it runs, so the tools can't save one.
// the image holds a type key in its place instead, a number that names the class.
// fixup_vtable looks that key up in the vtable registry and writes the real vtable address over it.
// until that's done, calling any virtual method on the object would jump to a garbage address.
// each test sets up its own registry through c_registry_fixture, so none of this needs the game running.
TEST_SUITE("mash") {
    TEST_CASE("a type key names the exact class, while subclass checks also accept its parents") {
        // mash_virtual_base's own key is a hash of its name; the hash ignores letter case.
        mash::mash_virtual_base base;

        CHECK(base.get_virtual_type_key() == 0x0668FEE9);
        CHECK(base.get_mash_sizeof() == 4);
        CHECK(mash::mash_virtual_base::generate_virtual_types_key_from_string("MASH_VIRTUAL_BASE") == 0x0668FEE9);
        CHECK(base.is_or_is_subclass_of(0x0668FEE9));
        CHECK_FALSE(base.is_subclass_of(0x0668FEE9));
        CHECK_FALSE(base.is_or_is_subclass_of(virtual_record_key));

        c_virtual_record derived(7, nullptr);

        CHECK(derived.is_or_is_subclass_of(virtual_record_key));
        CHECK(derived.is_or_is_subclass_of(virtual_record_parent_key));
        CHECK_FALSE(derived.is_subclass_of(virtual_record_key));
        CHECK_FALSE(derived.is_or_is_subclass_of(0x1002));

        // the base class's mash methods do nothing at all.
        base.construct_mashed_class();
        base.destruct_mashed_class();
        base.unmash(nullptr, nullptr, mash::NORMAL_BUFFER);
    }

    TEST_CASE("the base copy_values only checks that both types match, it doesn't copy anything") {
        c_virtual_record source(1, nullptr);
        c_virtual_record destination(2, nullptr);
        destination.copy_values(&source);

        CHECK(destination.id == 2);
        CHECK(destination.get_virtual_type_key() == source.get_virtual_type_key());
    }

    TEST_CASE("with an empty registry, lookups find nothing and saved keys are left as they are") {
        c_registry_fixture registry;
        u32 key = 0x12345678;

        CHECK(mash::mash_virtual_base::lookup_vtable_entry(key) == mash::invalid_vtable_value);

        mash::mash_virtual_base::fixup_vtable(&key);

        CHECK(key == 0x12345678);
    }

    TEST_CASE("lookup finds every key in a shared bucket and compares keys as unsigned") {
        c_registry_fixture registry;

        // each row is a bucket, a type key and the vtable that key should map to.
        // a bucket mask of 0 sends every key to bucket 0, so all five share it.
        // keys from 0x80000000 up would count as negative if they were compared as signed numbers.
        registry.set({ {0, 0,          11},
                       {0, 10,         22},
                       {0, 20,         33},
                       {0, 0x80000000, 44},
                       {0, 0xFFFFFFFF, 55} }, 0, 1);

        for (const auto &[key, vtable] : { std::pair<u32, u32>{ 0,          11 },
                                                              { 10,         22 },
                                                              { 20,         33 },
                                                              { 0x80000000, 44 },
                                                              { 0xFFFFFFFF, 55 } }) {

            CAPTURE(key);

            CHECK(mash::mash_virtual_base::lookup_vtable_entry(key) == vtable);
        }

        for (u32 key : {1u, 9u, 11u, 19u, 21u, 0x7FFFFFFFu, 0xFFFFFFFEu})
            CHECK(mash::mash_virtual_base::lookup_vtable_entry(key) == mash::invalid_vtable_value);
    }

    TEST_CASE("picking a bucket treats keys as signed for the math, even past the high bit") {
        // before picking a bucket, lookup scrambles the key with some arithmetic that treats it as a signed number.
        // the expected buckets are written out by hand, so the fixture can't repeat a mistake in that arithmetic.
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
                       { 7, 0xFFFFFFFF, 110 } });

        for (const auto &[key, vtable] : { std::pair<u32, u32>{ 0x7FFFFFFF, 101 },
                                                              { 0x80000000, 102 },
                                                              { 0xDEADBEEF, 103 },
                                                              { 0,          104 },
                                                              { 1,          105 },
                                                              { 2,          106 },
                                                              { 3,          107 },
                                                              { 4,          108 },
                                                              { 5,          109 },
                                                              { 0xFFFFFFFF, 110 } }) {

            CAPTURE(key);

            CHECK(mash::mash_virtual_base::lookup_vtable_entry(key) == vtable);
        }
    }

    TEST_CASE("a bucket number past the bucket count folds back into the lower half") {
        // a mask of 7 can pick any bucket from 0 to 7, but only five exist here.
        // picking 5, 6 or 7 moves down by four, to 1, 2 or 3.
        c_registry_fixture registry;

        registry.set({ {1, 0,          11},
                       {1, 4,          12},
                       {2, 1,          21},
                       {2, 5,          22},
                       {3, 2,          31},
                       {3, 0xFFFFFFFF, 32} }, 7, 5);

        for (const auto &[key, vtable] : { std::pair<u32, u32>{ 0,          11 },
                                                              { 4,          12 },
                                                              { 1,          21 },
                                                              { 5,          22 },
                                                              { 2,          31 },
                                                              { 0xFFFFFFFF, 32 } }) {

            CAPTURE(key);

            CHECK(mash::mash_virtual_base::lookup_vtable_entry(key) == vtable);
        }

        CHECK(mash::mash_virtual_base::lookup_vtable_entry(u32{3}) == mash::invalid_vtable_value);
    }

    TEST_CASE("lookup stops at the end of its bucket instead of running into the next one") {
        // 0x80000000 belongs in bucket 0, which ends right before the bucket that holds key 0.
        c_registry_fixture registry;
        registry.set({{0, 0x7FFFFFFF, 1}, {1, 0, 99}});

        CHECK(mash::mash_virtual_base::lookup_vtable_entry(u32{0x80000000}) == mash::invalid_vtable_value);
    }

    TEST_CASE("verifying accepts anything, fixing swaps a key for its vtable, and a vtable of 0 is never written") {
        // before fixup_vtable, the first word holds a key like 20; after it, it holds a vtable address like 0x5678.
        // verify_valid_vtable doesn't look at the object at all, so it passes before and after the fix.
        c_registry_fixture registry;
        registry.set({{0, 10, 0x1234}, {0, 20, 0x5678}, {0, 30, mash::invalid_vtable_value}}, 0, 1);

        u32 value = 20;

        CHECK(mash::mash_virtual_base::verify_valid_vtable(&value));

        mash::mash_virtual_base::fixup_vtable(&value);

        CHECK(mash::mash_virtual_base::verify_valid_vtable(&value));

        CHECK(value == 0x5678);

        value = 30;
        mash::mash_virtual_base::fixup_vtable(&value);

        CHECK(value == 30);
    }

    TEST_CASE("construct_class_helper just calls construct_mashed_class, without any lookup") {
        // this object was made by its normal constructor, so its vtable is already real and the empty registry never comes up.
        c_registry_fixture registry;

        trace_t trace;
        c_virtual_record value(9, &trace);

        CHECK(mash::mash_virtual_base::construct_class_helper(&value) == &value);
        REQUIRE(trace.events.size() == 1);
        CHECK(trace.events[0].kind == event_construct);

        value.trace = nullptr;
    }
}
