#include "dinkumware/fixtures.hh"

namespace dinkumware = treyarch::dinkumware;
using namespace tests::container_support;

// map uses the same tree as set, but orders pairs by their key while keeping the mapped value independently editable.
TEST_SUITE("dinkumware") {
    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "map orders pairs by key rather than mapped value") {

        dinkumware::map<u32, u32> values;
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
                      "map duplicate keys retain their original mapped value") {

        dinkumware::map<u32, u32> values;
        auto                      first  = values.insert({ 4, 40 });
        auto                      again  = values.insert({ 4, 400 });

        CHECK(first.second);
        CHECK_FALSE(again.second);
        CHECK(again.first == first.first);
        CHECK(again.first->value.second == 40);
        CHECK(allocation_count(tests::heap_allocator) == 2);
        check_tree(values, { 4 });
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "map two-child erase relinks the successor without assigning its const key") {

        dinkumware::map<u32, u32> values;
        for (u32 key : { 4u, 2u, 7u, 1u, 3u, 5u, 8u, 6u })
            values.insert({ key, key * 100 });

        auto* erased    = values.find(4);
        auto* successor = values.find(5);
        auto* stable    = values.find(7);

        CHECK(values.erase(erased) == successor);
        CHECK(values.find(5) == successor);
        CHECK(successor->value.second == 500);
        CHECK(values.find(7) == stable);
        CHECK(stable->value.second == 700);
        check_tree(values, { 1, 2, 3, 5, 6, 7, 8 });
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "map preserves pair lifetimes across rotations erasure and destruction") {

        value_trace_t trace;
        {
            tracked_value_t seed(17, trace);
            {
                dinkumware::map<u32, tracked_value_t> values;

                for (u32 key : { 3u, 1u, 5u, 0u, 2u, 4u, 6u })
                    values.insert({key, seed});

                CHECK(trace.live == 8);
                trace.events.clear();
                values.erase(values.find(3));

                CHECK(trace.events == std::vector<value_event_t> {{value_destroyed, 17}});
                CHECK(trace.live == 7);
                check_tree(values, { 0, 1, 2, 4, 5, 6 });

                trace.events.clear();
                values.erase(values.begin(), values.end());

                CHECK(trace.count(value_destroyed) == 6);
                CHECK(trace.live == 1);
                CHECK(live_count(tests::heap_allocator) == 1);
            }

            CHECK(trace.live == 1);
        }

        CHECK(trace.live == 0);
        CHECK(live_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "map whole tree cleanup destroys mapped values in descending key order") {

        value_trace_t trace;
        {
            tracked_value_t seed(0, trace);
            dinkumware::map<u32, tracked_value_t> values;

            for (u32 key : { 4u, 2u, 6u, 1u, 3u, 5u, 7u }) {
                seed.value = key;
                values.insert({key, seed});
            }

            trace.events.clear();
            values.erase(values.begin(), values.end());

            CHECK(trace.values(value_destroyed) == std::vector<i32> { 7, 6, 5, 4, 3, 2, 1 });
            CHECK(trace.live == 1);
            check_tree(values, {});
        }

        CHECK(trace.live == 0);
        CHECK(live_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "map custom comparison controls lookup and traversal") {

        dinkumware::map<u32, u32, std::greater<u32>> values;
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
    }
}
