#include <algorithm>

#include "vc8/fixtures.hh"

namespace vc8 = treyarch::vc8;
using namespace tests::vc8_support;

TEST_SUITE("vc8") {
    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "map orders pairs by key") {

        vc8::map<u32, u32> values;
        values.insert({ 7, 1 });
        values.insert({ 2, 99 });
        values.insert({ 5, 50 });

        check_tree(values, { 2, 5, 7 });

        CHECK(values.find(2)->value.second == 99);
        CHECK(values.find(5)->value.second == 50);
        CHECK(values.find(7)->value.second == 1);

        values.find(5)->value.second = 123;

        CHECK(values.find(5)->value.second == 123);
        check_tree(values, { 2, 5, 7 });
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "map insert keeps the first mapped value") {

        vc8::map<u32, u32> values;

        auto first = values.insert({ 4, 40 });
        auto again = values.insert({ 4, 400 });

        CHECK(first.second);
        CHECK_FALSE(again.second);
        CHECK(again.first == first.first);
        CHECK(again.first->value.second == 40);
        CHECK(allocations.size() == 2);
        check_tree(values, { 4 });
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "map subscript inserts a missing key once") {

        vc8::map<u32, u32> values;

        values[5] = 50;
        values[9] += 9;

        u32 &two = values[2];

        CHECK(two == 0);
        CHECK(&two == &values.find(2)->value.second);
        CHECK(values[5] == 50);
        CHECK(values[9] == 9);
        CHECK(allocations.size() == 4);
        check_tree(values, { 2, 5, 9 });

        std::vector<u32> expected { 2, 5, 9 };

        for (u32 key : shuffled_keys(64, 11)) {
            CAPTURE(key);

            values[key + 10] = key;
            expected.push_back(key + 10);

            std::sort(expected.begin(), expected.end());
            check_tree(values, expected);
        }

        for (u32 key = 0; key < 64; ++key)
            CHECK(values[key + 10] == key);

        CHECK(allocations.size() == 68);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "map erase relinks the successor instead of copying its pair") {

        vc8::map<u32, u32> values;

        for (u32 key : { 4u, 2u, 7u, 1u, 3u, 5u, 8u, 6u })
            values.insert({ key, key * 100 });

        auto* erased    = values.find(4);
        auto* successor = values.find(5);
        auto* stable    = values.find(7);

        CHECK(values.erase(erased) == successor);
        CHECK(values.find(5) == successor);
        CHECK(values.find(7) == stable);
        CHECK(successor->value.second == 500);
        CHECK(stable->value.second == 700);
        check_tree(values, { 1, 2, 3, 5, 6, 7, 8 });
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "map erase destroys the mapped values") {

        value_trace_t trace;
        {
            tracked_value_t seed(0, trace);

            {
                vc8::map<u32, tracked_value_t> values;

                for (u32 key : { 4u, 2u, 6u, 1u, 3u, 5u, 7u, 0u }) {
                    seed.value = key;
                    values.insert({ key, seed });
                }

                CHECK(trace.live == 9);

                trace.events.clear();
                values.erase(values.find(0));

                CHECK(trace.events == std::vector<value_event_t> { { value_destroyed, 0 } });
                check_tree(values, { 1, 2, 3, 4, 5, 6, 7 });

                trace.events.clear();
                values.erase(values.begin(), values.end());

                CHECK(trace.values(value_destroyed) == std::vector<i32> { 7, 6, 5, 4, 3, 2, 1 });
                CHECK(trace.live == 1);
                check_tree(values, {});

                seed.value = 8;
                values.insert({ 8, seed });
                trace.events.clear();
            }

            CHECK(trace.events == std::vector<value_event_t> { { value_destroyed, 8 } });
        }

        CHECK(trace.live == 0);
        CHECK(live_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "map custom comparison controls the order") {

        vc8::map<u32, u32, std::greater<u32>> values;
        values.insert({ 1, 10 });
        values.insert({ 5, 50 });
        values.insert({ 3, 30 });

        check_tree(values, { 5, 3, 1 });

        CHECK(values.lower_bound(6)->value.first == 5);
        CHECK(values.lower_bound(4)->value.first == 3);
        CHECK(values.lower_bound(2)->value.first == 1);
        CHECK(values.lower_bound(0) == values.end());
        CHECK(values.find(3)->value.second == 30);
        CHECK(values.find(2) == values.end());
        CHECK(values[4] == 0);
        check_tree(values, { 5, 4, 3, 1 });
    }
}
