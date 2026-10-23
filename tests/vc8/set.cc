#include "vc8/fixtures.hh"

namespace vc8 = treyarch::vc8;
using namespace tests::vc8_support;

TEST_SUITE("vc8") {
    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "set custom comparison controls the order") {

        vc8::set<u32, std::greater<u32>> values;

        for (u32 key : { 2u, 6u, 4u, 0u, 8u })
            values.insert(key);

        check_tree(values, { 8, 6, 4, 2, 0 });

        CHECK(values.lower_bound(9)->value == 8);
        CHECK(values.lower_bound(7)->value == 6);
        CHECK(values.lower_bound(4)->value == 4);
        CHECK(values.lower_bound(1)->value == 0);
        CHECK(values.find(3) == values.end());

        values.erase(values.find(4));

        check_tree(values, { 8, 6, 2, 0 });
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "set equality follows the comparator") {

        struct decade_less_t {
            bool operator()(u32 left, u32 right) const { return left / 10 < right / 10; }
        };

        vc8::set<u32, decade_less_t> values;

        auto first = values.insert(12);
        auto same  = values.insert(19);

        values.insert(25);

        CHECK_FALSE(same.second);
        CHECK(same.first == first.first);
        CHECK(values.find(18) == first.first);
        CHECK(values.lower_bound(11) == first.first);
        CHECK(values.find(29)->value == 25);
        CHECK(values.find(30) == values.end());
        check_tree(values, { 12, 25 });
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "set destructor frees every node and the head") {

        {
            vc8::set<u32> values;

            for (u32 key : shuffled_keys(100, 99))
                values.insert(key);

            CHECK(live_count(tests::heap_allocator) == 101);
        }

        CHECK(live_count(tests::heap_allocator) == 0);
        CHECK(free_count(tests::heap_allocator) == 101);
        CHECK(free_count(tests::plain_allocator) == 0);
    }
}
