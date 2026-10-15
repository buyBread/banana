#include <map>

#include "dinkumware/fixtures.hh"
#include "treyarch/shared/arch_base_vhandle.hh"

namespace dinkumware = treyarch::dinkumware;
using namespace tests::container_support;

// the hash map keeps all entries in one linked list, with boundaries separating the buckets searched by find.
// growth splits one bucket at a time; a split moves existing nodes rather than building a replacement table.
TEST_SUITE("dinkumware") {
    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "hash map starts with one bucket and nine sentinel boundaries") {

        integer_hash_map values;
        const auto       state  = hash_state(values);

        CHECK(values.empty());
        CHECK(values.size() == 0);
        CHECK(values.begin() == values.end());
        CHECK(state.mask == 1);
        CHECK(state.bucket_count == 1);
        CHECK(state.bound_count == 9);
        CHECK(state.bound_capacity == 9);

        for (u32 index = 0; index < 9; ++index)
            CHECK(state.bounds[index] == values.head());

        CHECK(check_hash(values).empty());
        CHECK(values.find(123) == values.end());
        CHECK(allocation_count(tests::heap_allocator) == 2);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "hash map inserts unique keys without replacing duplicate values") {

        integer_hash_map values;
        auto*            first  = values.insert(10, 100);
        auto*            same   = values.insert(10, 200);

        CHECK(first == same);
        CHECK(values.size() == 1);
        CHECK(same->value.value == 100);
        CHECK(check_hash(values) == std::vector<u32> { 10 });
        CHECK(allocation_count(tests::heap_allocator) == 3);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "hash map orders colliding keys as unsigned integers") {

        dinkumware::hash_map<u32, u32, collision_hash_t> values;

        for (u32 key : {0xFFFFFFFFu, 7u, 0x80000000u, 0u, 0x7FFFFFFFu, 9u})
            values.insert(key, key ^ 0x1234);

        CHECK(check_hash(values, collision_hash_t{}) ==
            std::vector<u32> {0, 7, 9, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF});

        for (u32 key : {1u, 8u, 10u, 0xFFFFFFFEu})
            CHECK(values.find(key) == values.end());
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "hash map splitting consecutive tail nodes preserves the live bucket boundary") {

        integer_hash_map                     values;
        std::vector<decltype(values.head())> nodes;

        for (u32 key : { 0u, 1u, 3u, 5u })
            nodes.push_back(values.insert(key, key + 100));

        // the fifth insertion splits bucket zero; its three consecutive odd tail nodes move to bucket one.
        // the old bucket's end changes during those moves, so retaining its original end would lose the stopping point.
        values.insert(2, 102);

        CHECK(hash_state(values).bucket_count == 2);
        CHECK(check_hash(values) == std::vector<u32> { 0, 2, 1, 3, 5 });

        for (auto* node : nodes)
            CHECK(values.find(node->value.key) == node);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "hash map splits buckets when all some or none of their nodes move") {

        for (const auto &keys : { std::vector<u32> { 1, 3, 5, 7 },
                                  std::vector<u32> { 0, 2, 4, 6 },
                                  std::vector<u32> { 1, 2, 4, 7 },
                                  std::vector<u32> { 0, 1, 2, 3 } }) {
 
            integer_hash_map values;

            for (u32 key : keys)
                values.insert(key, key);

            values.insert(8, 8);
            auto observed = check_hash(values);
            std::sort(observed.begin(), observed.end());

            auto expected = keys;
            expected.push_back(8);
            std::sort(expected.begin(), expected.end());

            CHECK(observed == expected);
            CHECK(hash_state(values).bucket_count == 2);
        }
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "hash map grows before checking a duplicate at the split threshold") {

        integer_hash_map values;
        for (u32 key = 0; key < 4; ++key)
            values.insert(key, key + 10);

        auto* original           = values.find(1);
        u32   allocations_before = allocation_count(tests::heap_allocator);
        auto* duplicate          = values.insert(1, 999);

        CHECK(duplicate == original);
        CHECK(duplicate->value.value == 11);
        CHECK(values.size() == 4);
        CHECK(hash_state(values).bucket_count == 2);
        CHECK(allocation_count(tests::heap_allocator) == allocations_before);
        CHECK(check_hash(values) == std::vector<u32> { 0, 2, 1, 3 });
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "hash map crosses mask and boundary vector growth thresholds") {

        integer_hash_map values;
        const size_t original_allocation = allocations.size() - 1;

        for (u32 key = 0; key < 65; ++key) {
            values.insert(key, key + 1000);
            const auto state = hash_state(values);

            if (key == 3) {
                CHECK(state.bucket_count == 1);
                CHECK(state.mask == 1);
            } else if (key == 4) {
                CHECK(state.bucket_count == 2);
                CHECK(state.mask == 1);
            } else if (key == 8) {
                CHECK(state.bucket_count == 3);
                CHECK(state.mask == 3);
            } else if (key == 16) {
                CHECK(state.bucket_count == 5);
                CHECK(state.mask == 7);
            } else if (key == 32) {
                CHECK(state.bucket_count == 9);
                CHECK(state.mask == 15);
                CHECK(state.bound_count == 17);
            } else if (key == 64) {
                CHECK(state.bucket_count == 17);
                CHECK(state.mask == 31);
                CHECK(state.bound_count == 33);
            }

            CAPTURE(key);
            CHECK(check_hash(values).size() == key + 1);
        }

        CHECK(allocations[original_allocation].released);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "hash map erase repairs boundaries for first middle last and only bucket entries") {

        for (u32 removed : { 0u, 2u, 4u, 6u }) {
            integer_hash_map values;
            for (u32 key : { 0u, 2u, 4u, 6u, 1u })
                values.insert(key, key);

            auto* node = values.find(removed);
            auto* next = node->next;

            CHECK(values.erase(node) == next);
            CHECK(values.find(removed) == values.end());
            CHECK(check_hash(values).size() == 4);

            while (!values.empty())
                values.erase(values.begin());

            CHECK(check_hash(values).empty());
            CHECK(hash_state(values).bucket_count == 2);
            CHECK(hash_state(values).bounds[0] == values.head());
        }
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "hash map erased keys can be inserted again with a new value") {

        integer_hash_map values;
        values.insert(0, 10);
        values.insert(1, 20);
        values.erase(values.find(0));
        auto* replacement = values.insert(0, 30);

        CHECK(replacement->value.value == 30);
        CHECK(values.size() == 2);
        CHECK(check_hash(values) == std::vector<u32> { 0, 1 });
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "hash map clear resets buckets while retaining the expanded boundary allocation") {

        integer_hash_map values;
        for (u32 key = 0; key < 65; ++key)
            values.insert(key, key);

        const auto grown            = hash_state(values);
        auto*      head             = values.head();
        u32        allocation_total = allocation_count(tests::heap_allocator);
        values.clear();
        values.clear();
        const auto cleared = hash_state(values);

        CHECK(check_hash(values).empty());
        CHECK(values.head() == head);
        CHECK(cleared.bounds == grown.bounds);
        CHECK(cleared.bound_capacity == grown.bound_capacity);
        CHECK(cleared.bound_count == 9);
        CHECK(cleared.mask == 1);
        CHECK(cleared.bucket_count == 1);
        CHECK(allocation_count(tests::heap_allocator) == allocation_total);
        CHECK(live_count(tests::heap_allocator) == 2);

        for (u32 key = 0; key < 65; ++key)
            values.insert(key, key + 1);

        CHECK(hash_state(values).bounds == grown.bounds);
        CHECK(check_hash(values).size() == 65);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "hash map splitting preserves addresses and does not copy stored values") {

        value_trace_t trace;
        {
            tracked_value_t seed(7, trace);
            dinkumware::hash_map<u32, tracked_value_t, identity_hash_t> values;
            std::vector<decltype(values.head())>                        nodes;

            for (u32 key = 0; key < 40; ++key)
                nodes.push_back(values.insert(key, seed));

            for (u32 key = 0; key < nodes.size(); ++key)
                CHECK(values.find(key) == nodes[key]);

            // each insertion copies through a temporary pair; bucket splits themselves add no value copies.
            CHECK(trace.count(value_copied) == 80);
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
                      "hash map mixed insertion and erasure keeps every remaining key reachable") {

        integer_hash_map   values;
        std::map<u32, u32> expected;
        u32                state    = 0xBADC0FFE;

        for (u32 step = 0; step < 900; ++step) {
            state = state * 1664525u + 1013904223u;
            const u32 key = (state >> 8) % 100;

            if ((state & 3) != 0) {
                values.insert(key, step);
                expected.emplace(key, step);
            } else {
                auto* node = values.find(key);

                if (node != values.end())
                    values.erase(node);

                expected.erase(key);
            }

            CAPTURE(step);
            auto observed = check_hash(values);
            std::sort(observed.begin(), observed.end());
            std::vector<u32> expected_keys;

            for (const auto &pair : expected) {
                expected_keys.push_back(pair.first);
                auto* node = values.find(pair.first);
                REQUIRE(node != values.end());
                CHECK(node->value.value == pair.second);
            }

            CHECK(observed == expected_keys);
        }
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "hash map default traits use the real handle hash and unsigned comparison") {

        dinkumware::hash_map<treyarch::arch_base_vhandle, u32> values;

        for (u32 key : { 0xFFFFFFFFu, 0u, 0x80000000u, 5u, 17u })
            values.insert(treyarch::arch_base_vhandle(key), key ^ 123);

        for (u32 key : { 0xFFFFFFFFu, 0u, 0x80000000u, 5u, 17u }) {
            auto* position = values.find(treyarch::arch_base_vhandle(key));
            REQUIRE(position != values.end());
            CHECK(position->value.value == (key ^ 123));
        }

        CHECK(values.find(treyarch::arch_base_vhandle(3)) == values.end());
        CHECK(values.size() == 5);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "hash map destruction releases entries sentinel and boundary storage") {

        {
            integer_hash_map values;
            
            for (u32 key = 0; key < 50; ++key)
                values.insert(key, key);

            CHECK(check_hash(values).size() == 50);
        }

        CHECK(live_count(tests::heap_allocator) == 0);
        CHECK(free_count(tests::plain_allocator) == 0);
    }
}
