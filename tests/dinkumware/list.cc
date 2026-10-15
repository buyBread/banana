#include "dinkumware/fixtures.hh"

namespace dinkumware = treyarch::dinkumware;
using namespace tests::container_support;

// the sentinel is a permanent end marker; an empty list links that marker back to itself in both directions.
// element nodes are separate allocations, so inserting or moving one does not move the values in other nodes.
TEST_SUITE("dinkumware") {
    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "list allocates an empty sentinel without constructing an element") {

        value_trace_t trace;
        {
            dinkumware::list<tracked_value_t> values;

            CHECK(values.empty());
            CHECK(values.size() == 0);
            CHECK(values.begin() == values.end());
            CHECK(values.head()->next == values.head());
            CHECK(values.head()->previous == values.head());
            CHECK(trace.events.empty());
            REQUIRE(allocations.size() == 1);
            CHECK(allocations[0].size == sizeof(dinkumware::list_node<tracked_value_t>));
        }

        CHECK(trace.events.empty());
        CHECK(live_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "list insertion repairs forward and backward links at every position") {

        dinkumware::list<u32> values;
        auto*                 middle = values.push_back(2);
        auto*                 first  = values.insert_before(middle, 1);
        auto*                 last   = values.push_back(4);
        auto*                 third  = values.insert_before(last, 3);

        CHECK(list_values(values) == std::vector<u32> { 1, 2, 3, 4 });
        CHECK(values.begin() == first);
        CHECK(values.head()->previous == last);
        CHECK(first->next == middle);
        CHECK(middle->next == third);
        CHECK(third->next == last);
        CHECK(allocation_count(tests::heap_allocator) == 5);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "list erase returns the surviving successor for first middle and last nodes") {

        dinkumware::list<u32> values;
        auto*                 first  = values.push_back(1);
        auto*                 second = values.push_back(2);
        auto*                 third  = values.push_back(3);
        auto*                 fourth = values.push_back(4);

        CHECK(values.erase(second) == third);
        CHECK(list_values(values) == std::vector<u32> { 1, 3, 4 });
        CHECK(values.erase(first) == third);
        CHECK(list_values(values) == std::vector<u32> { 3, 4 });
        CHECK(values.erase(fourth) == values.end());
        CHECK(list_values(values) == std::vector<u32> { 3 });
        CHECK(values.erase(third) == values.end());
        CHECK(list_values(values).empty());
        CHECK(values.empty());
        CHECK(live_count(tests::heap_allocator) == 1);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "list erase destroys its value and releases its node") {

        value_trace_t trace;
        {
            tracked_value_t seed(7, trace);
            dinkumware::list<tracked_value_t> values;
            auto*                             node = values.push_back(seed);
            trace.events.clear();

            values.erase(node);

            CHECK(trace.events == std::vector<value_event_t> { { value_destroyed, 7 } });
            CHECK(find(node)->released);
            CHECK(trace.live == 1);
        }

        CHECK(trace.live == 0);
        CHECK(live_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "list clear destroys in forward order and retains its sentinel") {

        value_trace_t trace;
        {
            tracked_value_t seed(0, trace);
            dinkumware::list<tracked_value_t> values;

            for (i32 value : {1, 2, 3}) {
                seed.value = value;
                values.push_back(seed);
            }

            auto* head = values.head();
            trace.events.clear();
            values.clear();
            values.clear();

            CHECK(trace.values(value_destroyed) == std::vector<i32> { 1, 2, 3 });
            CHECK(values.head() == head);
            CHECK(values.head()->next == head);
            CHECK(values.head()->previous == head);
            CHECK(values.size() == 0);
            CHECK(live_count(tests::heap_allocator) == 1);

            values.push_back(seed);

            CHECK(values.size() == 1);
            CHECK(values.begin()->value.value == 3);
            CHECK(values.head() == head);
        }

        CHECK(trace.live == 0);
        CHECK(live_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "list copying duplicates nodes and preserves source order") {

        dinkumware::list<u32> source;
        source.push_back(3);
        source.push_back(1);
        source.push_back(3);

        dinkumware::list<u32> copied(source);

        CHECK(list_values(copied) == std::vector<u32> { 3, 1, 3 });
        CHECK(copied.head() != source.head());
        CHECK(copied.begin() != source.begin());

        source.begin()->value = 77;
        copied.begin()->next->value = 88;

        CHECK(list_values(source) == std::vector<u32> { 77, 1, 3 });
        CHECK(list_values(copied) == std::vector<u32> { 3, 88, 3 });

        source.clear();

        CHECK(list_values(copied) == std::vector<u32> { 3, 88, 3 });
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "list copying an empty list creates only its own sentinel") {

        dinkumware::list<u32> source;
        dinkumware::list<u32> copied(source);

        CHECK(copied.empty());
        CHECK(copied.head() != source.head());
        CHECK(list_values(copied).empty());
        CHECK(allocation_count(tests::heap_allocator) == 2);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "list relinking moves existing nodes without copying values or allocating") {

        dinkumware::list<u32> values;
        auto*                 first  = values.push_back(1);
        auto*                 second = values.push_back(2);
        auto*                 third  = values.push_back(3);
        auto*                 fourth = values.push_back(4);

        values.relink_before(first, values.end());
        CHECK(list_values(values) == std::vector<u32> { 2, 3, 4, 1 });

        values.relink_before(fourth, second);
        CHECK(list_values(values) == std::vector<u32> { 4, 2, 3, 1 });

        values.relink_before(third, second);
        CHECK(list_values(values) == std::vector<u32> { 4, 3, 2, 1 });

        values.relink_before(third, second);
        CHECK(list_values(values) == std::vector<u32> { 4, 3, 2, 1 });
        CHECK(values.size() == 4);
        CHECK(allocation_count(tests::heap_allocator) == 5);
        CHECK(frees.empty());
        CHECK(first->value == 1);
        CHECK(second->value == 2);
        CHECK(third->value == 3);
        CHECK(fourth->value == 4);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "list size limit rejects overflow without changing its count") {

        dinkumware::list<u32> values;
        values.increment_size(0x3FFFFFFF);

        CHECK(values.size() == 0x3FFFFFFF);
        CHECK_THROWS_WITH_AS(values.increment_size(1), "list<T> too long", std::length_error);
        CHECK(values.size() == 0x3FFFFFFF);

        // exercise the count helper directly without manufacturing a billion nodes.
        // clear restores its ordinary empty state afterward.
        values.clear();

        CHECK(values.size() == 0);
        CHECK(list_values(values).empty());
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "list copies construct independent values and leave the sentinel unconstructed") {

        value_trace_t trace;
        {
            tracked_value_t seed(8, trace);
            dinkumware::list<tracked_value_t> source;
            source.push_back(seed);
            source.push_back(seed);
            trace.events.clear();

            {
                dinkumware::list<tracked_value_t> copied(source);

                CHECK(trace.count(value_copied) == 2);
                CHECK(trace.live == 5);
                copied.begin()->value.value = 22;
                CHECK(source.begin()->value.value == 8);
            }

            CHECK(trace.values(value_destroyed) == std::vector<i32> { 22, 8 });
            CHECK(trace.live == 3);
        }

        CHECK(trace.live == 0);
        CHECK(live_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "list destructor releases all nodes and its sentinel") {

        {
            dinkumware::list<u32> values;
            for (u32 value = 0; value < 20; ++value)
                values.push_back(value);

            CHECK(live_count(tests::heap_allocator) == 21);
        }

        CHECK(live_count(tests::heap_allocator) == 0);
        CHECK(free_count(tests::heap_allocator) == 21);
    }
}
