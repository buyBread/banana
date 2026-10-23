#include "vc8/fixtures.hh"

namespace vc8 = treyarch::vc8;
using namespace tests::vc8_support;

TEST_SUITE("vc8") {
    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "list starts with a sentinel linked to itself") {

        {
            vc8::list<u64> values;

            CHECK(values.empty());
            CHECK(values.size() == 0);
            CHECK(values.begin() == values.end());
            CHECK(values.head()->next == values.head());
            CHECK(values.head()->previous == values.head());

            REQUIRE(allocations.size() == 1);
            CHECK(allocations[0].size == sizeof(vc8::list_node<u64>));
        }

        CHECK(live_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "list insert_before links the node in place") {

        vc8::list<u32> values;

        auto* second = values.push_back(2);
        auto* first  = values.insert_before(second, 1);
        auto* fourth = values.push_back(4);
        auto* third  = values.insert_before(fourth, 3);

        CHECK(list_values(values) == std::vector<u32> { 1, 2, 3, 4 });
        CHECK(values.begin() == first);
        CHECK(first->next == second);
        CHECK(second->next == third);
        CHECK(third->next == fourth);
        CHECK(values.head()->previous == fourth);
        CHECK(allocations.size() == 5);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "list erase frees the node and returns its successor") {

        vc8::list<u32> values;

        auto* first  = values.push_back(1);
        auto* second = values.push_back(2);
        auto* third  = values.push_back(3);
        auto* fourth = values.push_back(4);

        CHECK(values.erase(second) == third);
        CHECK(list_values(values) == std::vector<u32> { 1, 3, 4 });
        CHECK(values.erase(first) == third);
        CHECK(list_values(values) == std::vector<u32> { 3, 4 });
        CHECK(values.erase(fourth) == values.end());
        CHECK(list_values(values) == std::vector<u32> { 3 });
        CHECK(values.erase(third) == values.end());
        CHECK(list_values(values).empty());

        for (auto* node : { first, second, third, fourth })
            CHECK(find(node)->released);

        CHECK(live_count(tests::heap_allocator) == 1);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "list erase destroys the value") {

        value_trace_t trace;
        {
            tracked_value_t seed(7, trace);

            vc8::list<tracked_value_t> values;
            auto*                      node = values.push_back(seed);

            trace.events.clear();
            values.erase(node);

            CHECK(trace.events == std::vector<value_event_t> { { value_destroyed, 7 } });
        }

        CHECK(trace.live == 0);
        CHECK(live_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "list clear destroys front to back and keeps the sentinel") {

        value_trace_t trace;
        {
            tracked_value_t seed(0, trace);

            vc8::list<tracked_value_t> values;

            for (i32 value : { 1, 2, 3 }) {
                seed.value = value;
                values.push_back(seed);
            }

            auto* head = values.head();
            trace.events.clear();

            values.clear();
            values.clear();

            CHECK(trace.events == std::vector<value_event_t> { { value_destroyed, 1 },
                                                               { value_destroyed, 2 },
                                                               { value_destroyed, 3 } });
            CHECK(values.size() == 0);
            CHECK(values.head() == head);
            CHECK(head->next == head);
            CHECK(head->previous == head);
            CHECK(live_count(tests::heap_allocator) == 1);

            values.push_back(seed);

            CHECK(values.size() == 1);
            CHECK(values.begin()->value.value == 3);
        }

        CHECK(trace.live == 0);
        CHECK(live_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "list copy duplicates the nodes in order") {

        vc8::list<u32> source;
        source.push_back(3);
        source.push_back(1);
        source.push_back(3);

        vc8::list<u32> copied(source);

        CHECK(list_values(copied) == std::vector<u32> { 3, 1, 3 });
        CHECK(copied.head() != source.head());
        CHECK(copied.begin() != source.begin());

        copied.begin()->next->value = 88;
        source.clear();

        CHECK(list_values(copied) == std::vector<u32> { 3, 88, 3 });

        u32            before = allocations.size();
        vc8::list<u32> empty(source);

        CHECK(list_values(empty).empty());
        CHECK(empty.head() != source.head());
        CHECK(allocations.size() == before + 1);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "list copy copies every value") {

        value_trace_t trace;
        {
            tracked_value_t seed(8, trace);

            vc8::list<tracked_value_t> source;
            source.push_back(seed);
            source.push_back(seed);
            trace.events.clear();

            {
                vc8::list<tracked_value_t> copied(source);
                copied.begin()->value.value = 22;

                CHECK(trace.events == std::vector<value_event_t> { { value_copied, 8 },
                                                                   { value_copied, 8 } });
                CHECK(source.begin()->value.value == 8);
            }

            CHECK(trace.values(value_destroyed) == std::vector<i32> { 22, 8 });
        }

        CHECK(trace.live == 0);
        CHECK(live_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "list relink_before moves nodes without allocating") {

        vc8::list<u32> values;

        auto* first  = values.push_back(1);
        auto* second = values.push_back(2);
        auto* third  = values.push_back(3);
        auto* fourth = values.push_back(4);

        values.relink_before(first, values.end());

        CHECK(list_values(values) == std::vector<u32> { 2, 3, 4, 1 });

        values.relink_before(fourth, second);

        CHECK(list_values(values) == std::vector<u32> { 4, 2, 3, 1 });

        values.relink_before(third, second);

        CHECK(list_values(values) == std::vector<u32> { 4, 3, 2, 1 });

        values.relink_before(third, second);

        CHECK(list_values(values) == std::vector<u32> { 4, 3, 2, 1 });
        CHECK(values.begin() == fourth);
        CHECK(values.head()->previous == first);
        CHECK(allocations.size() == 5);
        CHECK(frees.empty());
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "list increment_size rejects counts past its limit") {

        vc8::list<u32> values;
        values.increment_size(0x3FFFFFFF);

        CHECK(values.size() == 0x3FFFFFFF);
        CHECK_THROWS_WITH_AS(values.increment_size(1), "list<T> too long", std::length_error);
        CHECK(values.size() == 0x3FFFFFFF);

        values.clear();

        CHECK(list_values(values).empty());
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "list destructor frees every node and the sentinel") {

        {
            vc8::list<u32> values;

            for (u32 value = 0; value < 20; ++value)
                values.push_back(value);

            CHECK(live_count(tests::heap_allocator) == 21);
        }

        CHECK(live_count(tests::heap_allocator) == 0);
        CHECK(free_count(tests::heap_allocator) == 21);
    }
}
