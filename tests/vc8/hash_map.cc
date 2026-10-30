#include <algorithm>
#include <map>

#include "vc8/fixtures.hh"
#include "game/arch_base_vhandle.hh"

namespace vc8 = treyarch::vc8;
using namespace tests::vc8_support;

TEST_SUITE("vc8") {
    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "hash map starts with one bucket and nine bounds") {

        integer_hash_map values;
        const auto      &layout = hash_layout(values);

        CHECK(values.empty());
        CHECK(values.begin() == values.end());
        CHECK(layout.mask == 1);
        CHECK(layout.bucket_count == 1);
        CHECK(layout.bounds.size() == 9);
        CHECK(layout.bounds.capacity() == 9);
        CHECK(check_hash(values).empty());
        CHECK(values.find(123) == values.end());
        CHECK(allocations.size() == 2);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "hash map insert keeps the first value of a key") {

        integer_hash_map values;

        auto first = values.insert({ 10, 100 });
        auto same  = values.insert({ 10, 200 });

        CHECK(first.second);
        CHECK_FALSE(same.second);
        CHECK(same.first == first.first);
        CHECK(same.first->value.value == 100);
        CHECK(check_hash(values) == std::vector<u32> { 10 });
        CHECK(allocations.size() == 3);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "hash map orders a bucket by unsigned key") {

        vc8::hash_map<u32, u32, collision_hash_t> values;

        for (u32 key : { 0xFFFFFFFFu, 7u, 0x80000000u, 0u, 0x7FFFFFFFu, 9u })
            values.insert({ key, key ^ 0x1234 });

        CHECK(check_hash(values, collision_hash_t {}) == std::vector<u32> { 0, 7, 9, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF });

        for (u32 key : { 1u, 8u, 10u, 0xFFFFFFFEu })
            CHECK(values.find(key) == values.end());
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "hash map first split moves the odd keys to bucket one") {

        struct split_t {
            std::vector<u32> keys;
            std::vector<u32> order;
        };

        for (const split_t &split : { split_t { { 1, 3, 5, 7, 8 }, { 8, 1, 3, 5, 7 } },
                                      split_t { { 0, 2, 4, 6, 8 }, { 0, 2, 4, 6, 8 } },
                                      split_t { { 1, 2, 4, 7, 8 }, { 2, 4, 8, 1, 7 } },
                                      split_t { { 0, 1, 2, 3, 8 }, { 0, 2, 8, 1, 3 } },
                                      split_t { { 0, 1, 3, 5, 2 }, { 0, 2, 1, 3, 5 } } }) {

            CAPTURE(split.keys);

            integer_hash_map                     values;
            std::vector<decltype(values.head())> nodes;

            for (u32 key : split.keys)
                nodes.push_back(values.insert({ key, key + 100 }).first);

            CHECK(hash_layout(values).bucket_count == 2);
            CHECK(check_hash(values) == split.order);

            for (auto* node : nodes)
                CHECK(values.find(node->value.key) == node);
        }
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "hash map grows before looking for a duplicate") {

        integer_hash_map values;

        for (u32 key = 0; key < 4; ++key)
            values.insert({ key, key + 10 });

        auto* original = values.find(1);
        u32   before   = allocations.size();

        auto* duplicate = values.insert({ 1, 999 }).first;

        CHECK(duplicate == original);
        CHECK(duplicate->value.value == 11);
        CHECK(hash_layout(values).bucket_count == 2);
        CHECK(check_hash(values) == std::vector<u32> { 0, 2, 1, 3 });
        CHECK(allocations.size() == before);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "hash map widens its mask and bounds as buckets are added") {

        integer_hash_map values;

        for (u32 key = 0; key < 65; ++key) {
            CAPTURE(key);

            values.insert({ key, key + 1000 });

            const auto &layout = hash_layout(values);

            if (key == 3) {
                CHECK(layout.bucket_count == 1);
                CHECK(layout.mask == 1);
            } else if (key == 4) {
                CHECK(layout.bucket_count == 2);
                CHECK(layout.mask == 1);
            } else if (key == 8) {
                CHECK(layout.bucket_count == 3);
                CHECK(layout.mask == 3);
            } else if (key == 16) {
                CHECK(layout.bucket_count == 5);
                CHECK(layout.mask == 7);
            } else if (key == 31) {
                CHECK(layout.bucket_count == 8);
                CHECK(layout.bounds.size() == 9);
            } else if (key == 32) {
                CHECK(layout.bucket_count == 9);
                CHECK(layout.mask == 15);
                CHECK(layout.bounds.size() == 17);
            } else if (key == 64) {
                CHECK(layout.bucket_count == 17);
                CHECK(layout.mask == 31);
                CHECK(layout.bounds.size() == 33);
            }

            CHECK(check_hash(values).size() == key + 1);
        }

        CHECK(allocations[1].released);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "hash map erase repairs the bucket bounds") {

        for (u32 removed : { 0u, 2u, 6u, 1u }) {
            CAPTURE(removed);

            integer_hash_map values;
            std::vector<u32> order { 0, 2, 4, 6, 1 };

            for (u32 key : order)
                values.insert({ key, key });

            auto* node = values.find(removed);
            auto* next = node->next;

            CHECK(values.erase(node) == next);
            CHECK(values.find(removed) == values.end());

            std::vector<u32> remaining = order;
            remaining.erase(std::find(remaining.begin(), remaining.end(), removed));

            CHECK(check_hash(values) == remaining);

            auto* replacement = values.insert({ removed, 99 }).first;

            CHECK(replacement->value.value == 99);
            CHECK(check_hash(values) == order);

            while (!values.empty())
                values.erase(values.begin());

            CHECK(check_hash(values).empty());
            CHECK(hash_layout(values).bucket_count == 2);
        }
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "hash map clear resets the buckets and keeps the bound storage") {

        integer_hash_map values;

        for (u32 key = 0; key < 65; ++key)
            values.insert({ key, key });

        const auto &layout = hash_layout(values);
        auto*       bounds = layout.bounds.begin();
        auto*       head   = values.head();
        u32         before = allocations.size();

        values.clear();
        values.clear();

        CHECK(check_hash(values).empty());
        CHECK(values.head() == head);
        CHECK(layout.bounds.begin() == bounds);
        CHECK(layout.bounds.size() == 9);
        CHECK(layout.bounds.capacity() == 33);
        CHECK(layout.mask == 1);
        CHECK(layout.bucket_count == 1);
        CHECK(allocations.size() == before);
        CHECK(live_count(tests::heap_allocator) == 2);

        for (u32 key = 0; key < 65; ++key)
            values.insert({ key, key + 1 });

        CHECK(check_hash(values).size() == 65);
        CHECK(layout.bounds.begin() == bounds);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "hash map splits relink nodes without copying values") {

        value_trace_t trace;
        {
            tracked_value_t seed(7, trace);

            vc8::hash_map<u32, tracked_value_t, identity_hash_t> values;
            std::vector<decltype(values.head())>                 nodes;

            for (u32 key = 0; key < 40; ++key)
                nodes.push_back(values.insert({ key, seed }).first);

            for (u32 key = 0; key < 40; ++key)
                CHECK(values.find(key) == nodes[key]);

            // the pair temporary and the node each take a copy
            CHECK(trace.count(value_copied) == 80);
            CHECK(trace.count(value_moved) == 0);
            CHECK(trace.live == 41);

            trace.events.clear();
            values.clear();

            CHECK(trace.count(value_destroyed) == 40);
            CHECK(trace.live == 1);
        }

        CHECK(trace.live == 0);
        CHECK(live_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "hash map default traits use the key's hash and ordering") {

        vc8::hash_map<treyarch::arch_base_vhandle, u32> values;

        for (u32 key : { 0xFFFFFFFFu, 0u, 0x80000000u, 5u, 17u })
            values.insert({ treyarch::arch_base_vhandle(key), key ^ 123 });

        for (u32 key : { 0xFFFFFFFFu, 0u, 0x80000000u, 5u, 17u }) {
            CAPTURE(key);

            auto* position = values.find(treyarch::arch_base_vhandle(key));

            REQUIRE(position != values.end());
            CHECK(position->value.value == (key ^ 123));
        }

        CHECK(values.find(treyarch::arch_base_vhandle(3)) == values.end());
        CHECK(values.size() == 5);
    }

    TEST_CASE("hash compare mixes unsigned keys like vc8's integral hash") {
        vc8::hash_compare<u32> traits;

        CHECK(traits(0)          == 0x60D22BD1);
        CHECK(traits(1)          == 0x60D1EA2A);
        CHECK(traits(0xDEADBEEF) == 0);
        CHECK(traits(0xFFFFFFFF) == 0x1F2D9287);
        CHECK(traits(0x12345678) == 0x0C74F724);
        CHECK(traits.less(1, 0xFFFFFFFF));
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "hash map destructor frees the entries sentinel and bounds") {

        {
            integer_hash_map values;

            for (u32 key = 0; key < 50; ++key)
                values.insert({ key, key });

            CHECK(check_hash(values).size() == 50);
        }

        CHECK(live_count(tests::heap_allocator) == 0);
        CHECK(free_count(tests::plain_allocator) == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "hash map random edits match std::map") {

        integer_hash_map   values;
        std::map<u32, u32> expected;
        random_t           random { 0xBADC0FFE };

        for (u32 step = 0; step < 900; ++step) {
            CAPTURE(step);

            u32 roll = random.next();
            u32 key  = (roll >> 8) % 100;

            if (roll & 3) {
                values.insert({ key, step });
                expected.emplace(key, step);
            } else {
                auto* node = values.find(key);

                if (node != values.end())
                    values.erase(node);

                expected.erase(key);
            }

            std::vector<u32> keys = check_hash(values);
            std::vector<u32> expected_keys;

            std::sort(keys.begin(), keys.end());

            for (const auto &[expected_key, expected_value] : expected) {
                expected_keys.push_back(expected_key);

                auto* node = values.find(expected_key);

                REQUIRE(node != values.end());
                CHECK(node->value.value == expected_value);
            }

            CHECK(keys == expected_keys);
        }
    }
}
