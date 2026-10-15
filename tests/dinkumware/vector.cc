#include "dinkumware/fixtures.hh"

namespace dinkumware = treyarch::dinkumware;
using namespace tests::container_support;

// a vector owns one continuous allocation; size counts live elements and capacity counts available element slots.
// clear destroys the elements but keeps that allocation; destroy also gives the memory back to the heap.
TEST_SUITE("dinkumware") {
    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "vector starts empty without allocating storage") {

        dinkumware::vector<u32> values;
        const auto &constant = values;

        CHECK(values.empty());
        CHECK(values.size() == 0);
        CHECK(values.capacity() == 0);
        CHECK(values.begin() == nullptr);
        CHECK(values.end() == nullptr);
        CHECK(constant.begin() == nullptr);
        CHECK(constant.end() == nullptr);

        values.reserve(0);
        values.clear();
        values.destroy();

        CHECK(allocations.empty());
        CHECK(frees.empty());
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "vector reserve requests exact capacity without constructing elements") {

        value_trace_t trace;
        {
            dinkumware::vector<tracked_value_t> values;
            values.reserve(7);
            auto* storage = values.begin();

            REQUIRE(allocations.size() == 1);
            CHECK(allocations[0].size == 7 * sizeof(tracked_value_t));
            CHECK(values.capacity() == 7);
            CHECK(values.size() == 0);
            CHECK(values.end() == storage);
            CHECK(trace.events.empty());

            values.reserve(3);
            values.reserve(7);

            CHECK(values.begin() == storage);
            CHECK(allocation_count(tests::heap_allocator) == 1);
        }

        CHECK(live_count(tests::heap_allocator) == 0);
        CHECK(trace.live == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "vector append follows its one and a half capacity growth") {

        dinkumware::vector<u32> values;
        const u32 capacities[] { 1, 2, 3, 4, 6, 6, 9, 9, 9, 13, 13, 13 };

        for (u32 index = 0; index < 12; ++index) {
            CAPTURE(index);
            values.push_back(100 + index);

            CHECK(values.size() == index + 1);
            CHECK(values.capacity() == capacities[index]);
            CHECK(values.back() == 100 + index);

            for (u32 prior = 0; prior <= index; ++prior)
                CHECK(values[prior] == 100 + prior);
        }

        values[0] = 77;
        const auto &constant = values;

        CHECK(constant[0] == 77);
        CHECK(constant.back() == 111);
        CHECK(constant.end() - constant.begin() == 12);
        CHECK(free_count(tests::plain_allocator) == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "vector reserve copies existing elements before destroying old storage") {

        value_trace_t trace;
        {
            tracked_value_t first(1, trace);
            tracked_value_t second(2, trace);
            dinkumware::vector<tracked_value_t> values;

            values.reserve(2);
            values.push_back(first);
            values.push_back(second);
            auto* old_storage = values.begin();
            trace.events.clear();

            values.reserve(5);

            // explicit reserve copies the live values, while automatic append growth moves them.
            CHECK(trace.events == std::vector<value_event_t> { { value_copied, 1 },
                                                               { value_copied, 2 },
                                                               { value_destroyed, 2 },
                                                               { value_destroyed, 1 } });
            CHECK(values.size() == 2);
            CHECK(values.capacity() == 5);
            CHECK(values[0].value == 1);
            CHECK(values[1].value == 2);
            CHECK(find(old_storage)->released);
            CHECK(trace.live == 4);
        }

        CHECK(trace.live == 0);
        CHECK(live_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "vector automatic growth moves old elements and copies the appended value") {

        value_trace_t trace;
        {
            tracked_value_t first(1, trace);
            tracked_value_t second(2, trace);
            dinkumware::vector<tracked_value_t> values;
            values.push_back(first);
            trace.events.clear();

            values.push_back(second);

            CHECK(trace.events == std::vector<value_event_t> { { value_moved, 1 },
                                                               { value_destroyed, -1 },
                                                               { value_copied, 2 } });
            CHECK(values[0].value == 1);
            CHECK(values[1].value == 2);
            CHECK(first.value == 1);
            CHECK(second.value == 2);
        }

        CHECK(trace.live == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "vector clear destroys in reverse and retains reusable storage") {

        value_trace_t trace;
        {
            tracked_value_t seed(0, trace);
            dinkumware::vector<tracked_value_t> values;
            values.reserve(4);

            for (i32 value : { 1, 2, 3 }) {
                seed.value = value;
                values.push_back(seed);
            }

            auto* storage = values.begin();
            trace.events.clear();
            values.clear();

            CHECK(trace.values(value_destroyed) == std::vector<i32> { 3, 2, 1 });
            CHECK(values.empty());
            CHECK(values.capacity() == 4);
            CHECK(values.begin() == storage);
            CHECK(values.end() == storage);
            CHECK_FALSE(find(storage)->released);

            values.clear();
            values.push_back(seed);

            CHECK(allocation_count(tests::heap_allocator) == 1);
            CHECK(values.back().value == 3);
        }

        CHECK(trace.live == 0);
        CHECK(live_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "vector destroy releases storage once and permits reuse") {

        dinkumware::vector<u32> values;
        values.assign(3, 7);
        auto* storage = values.begin();

        values.destroy();
        values.destroy();

        CHECK(find(storage)->released);
        CHECK(values.begin() == nullptr);
        CHECK(values.end() == nullptr);
        CHECK(values.size() == 0);
        CHECK(values.capacity() == 0);
        CHECK(free_count(tests::heap_allocator) == 1);

        values.push_back(9);

        CHECK(values.size() == 1);
        CHECK(values.capacity() == 1);
        CHECK(values[0] == 9);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "vector copying creates independent storage sized to the live elements") {

        dinkumware::vector<u32> source;
        source.reserve(12);
        source.push_back(3);
        source.push_back(5);

        dinkumware::vector<u32> copied(source);

        CHECK(vector_values(copied) == std::vector<u32> { 3, 5 });
        CHECK(copied.capacity() == 2);
        CHECK(copied.begin() != source.begin());

        copied[0] = 99;
        source.destroy();

        CHECK(vector_values(copied) == std::vector<u32> { 99, 5 });
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "vector copying an empty reserved vector makes no allocation") {

        dinkumware::vector<u32> source;
        source.reserve(8);
        dinkumware::vector<u32> copied(source);

        CHECK(copied.empty());
        CHECK(copied.capacity() == 0);
        CHECK(copied.begin() == nullptr);
        CHECK(allocation_count(tests::heap_allocator) == 1);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "vector assignment by count reuses capacity and replaces every element") {

        dinkumware::vector<u32> values;
        values.reserve(8);
        values.assign(5, 3);
        auto* storage = values.begin();
        const size_t original_allocation = allocations.size() - 1;

        values.assign(2, 9);

        CHECK(vector_values(values) == std::vector<u32> { 9, 9 });
        CHECK(values.begin() == storage);
        CHECK(values.capacity() == 8);

        values.assign(0, 99);

        CHECK(values.empty());
        CHECK(values.begin() == storage);
        CHECK(values.capacity() == 8);
        CHECK(allocation_count(tests::heap_allocator) == 1);

        values.assign(10, 4);

        CHECK(values.size() == 10);
        CHECK(values.capacity() == 10);

        // assign frees before reallocating, so the heap is allowed to return the same address for the larger request.
        // check the original allocation record rather than whichever allocation currently occupies that address.
        CHECK(allocations[original_allocation].released);
        CHECK(vector_values(values) == std::vector<u32>(10, 4));
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "vector resize preserves the prefix and fills only added elements") {

        dinkumware::vector<u32> values;
        values.reserve(6);
        values.push_back(1);
        values.push_back(2);
        auto* storage = values.begin();

        values.resize(5, 7);

        CHECK(vector_values(values) == std::vector<u32> { 1, 2, 7, 7, 7 });
        CHECK(values.begin() == storage);

        values.resize(1, 99);
        values.resize(1, 88);

        CHECK(vector_values(values) == std::vector<u32> { 1 });
        CHECK(values.capacity() == 6);

        values.resize(7, 8);

        CHECK(values.capacity() == 9);
        CHECK(vector_values(values) == std::vector<u32> { 1, 8, 8, 8, 8, 8, 8 });
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "vector shrinking and popping destroy only the removed suffix") {

        value_trace_t trace;
        {
            tracked_value_t seed(6, trace);
            dinkumware::vector<tracked_value_t> values;
            values.assign(4, seed);

            for (u32 index = 0; index < 4; ++index)
                values[index].value = index + 1;

            trace.events.clear();
            values.resize(2, seed);
            values.pop_back();

            CHECK(trace.values(value_destroyed) == std::vector<i32> { 4, 3, 2 });
            CHECK(trace.count(value_copied) == 0);
            CHECK(values.size() == 1);
            CHECK(values.back().value == 1);
            CHECK(values.capacity() == 4);
        }

        CHECK(trace.live == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "vector single erase shifts by move assignment and returns the following position") {

        value_trace_t trace;
        {
            tracked_value_t seed(0, trace);
            dinkumware::vector<tracked_value_t> values;
            values.assign(4, seed);

            for (u32 index = 0; index < 4; ++index)
                values[index].value = index + 1;

            auto* storage = values.begin();
            trace.events.clear();
            auto* next = values.erase(values.begin() + 1);

            CHECK(next == storage + 1);
            CHECK(next->value == 3);
            CHECK(values.size() == 3);
            CHECK(values[2].value == 4);
            CHECK(trace.values(value_move_assigned) == std::vector<i32> { 3, 4 });
            CHECK(trace.values(value_destroyed) == std::vector<i32> { - 1});

            auto* erased_end = values.erase(values.end() - 1);

            CHECK(erased_end == values.end());
            CHECK(values.begin() == storage);
            CHECK(values.capacity() == 4);
        }

        CHECK(trace.live == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "vector range erase handles empty middle prefix suffix and whole ranges") {

        for (const auto &range : { std::pair<u32, u32> { 0, 0 },
                                                       { 2, 2 },
                                                       { 1, 4 },
                                                       { 0, 2 },
                                                       { 3, 5 },
                                                       { 0, 5 } }) {

            CAPTURE(range.first);
            CAPTURE(range.second);

            dinkumware::vector<u32> values;
            std::vector<u32> expected { 1, 2, 3, 4, 5 };

            for (u32 value : expected)
                values.push_back(value);

            auto* storage = values.begin();
            auto* next    = values.erase(storage + range.first, storage + range.second);
            expected.erase(expected.begin() + range.first, expected.begin() + range.second);

            CHECK(next == storage + range.first);
            CHECK(vector_values(values) == expected);
            CHECK(values.begin() == storage);
        }
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "vector reserve rejects byte size overflow before allocating") {

        dinkumware::vector<u64> values;
        values.push_back(42);
        auto* storage = values.begin();

        CHECK_THROWS_WITH_AS(values.reserve(0x20000000), "vector<T> too long", std::length_error);
        CHECK(values.begin() == storage);
        CHECK(values.size() == 1);
        CHECK(values.back() == 42);
        CHECK(allocation_count(tests::heap_allocator) == 1);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "vector copy construction copies each live value without moving its source") {

        value_trace_t trace;
        {
            tracked_value_t seed(9, trace);
            dinkumware::vector<tracked_value_t> source;
            source.assign(3, seed);
            trace.events.clear();

            {
                dinkumware::vector<tracked_value_t> copied(source);

                CHECK(trace.values(value_copied) == std::vector<i32> { 9, 9, 9 });
                CHECK(trace.count(value_moved) == 0);
                CHECK(trace.live == 7);
                copied[0].value = 44;
                CHECK(source[0].value == 9);
            }

            CHECK(trace.values(value_destroyed) == std::vector<i32> { 9, 9, 44 });
            CHECK(trace.live == 4);
        }

        CHECK(trace.live == 0);
        CHECK(live_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "vector counted assignment destroys old values then copies replacements") {

        value_trace_t trace;
        {
            tracked_value_t seed(1, trace);
            dinkumware::vector<tracked_value_t> values;
            values.assign(2, seed);
            values[1].value = 2;
            auto* storage = values.begin();
            seed.value = 7;
            trace.events.clear();

            values.assign(2, seed);

            CHECK(trace.events == std::vector<value_event_t> { { value_destroyed, 2 },
                                                               { value_destroyed, 1 },
                                                               { value_copied, 7 },
                                                               { value_copied, 7 } });
            CHECK(values.begin() == storage);
            CHECK(trace.count(value_copy_assigned) == 0);
        }

        CHECK(trace.live == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "vector growing resize moves its prefix before copying the fill values") {

        value_trace_t trace;
        {
            tracked_value_t seed(3, trace);
            dinkumware::vector<tracked_value_t> values;
            values.assign(2, seed);
            values[0].value = 1;
            values[1].value = 2;
            trace.events.clear();

            values.resize(4, seed);

            CHECK(trace.values(value_moved) == std::vector<i32> { 1, 2 });
            CHECK(trace.values(value_copied) == std::vector<i32> { 3, 3 });
            CHECK(trace.values(value_destroyed) == std::vector<i32> { -1, -1 });
            CHECK(values.capacity() == 4);
            CHECK(values.size() == 4);
        }

        CHECK(trace.live == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "vector mixed edits preserve the expected sequence") {

        dinkumware::vector<u32> values;
        std::vector<u32>        expected;
        u32                     state = 0x12345678;

        for (u32 step = 0; step < 600; ++step) {
            state = state * 1664525u + 1013904223u;
            const u32 operation = state % 5;

            if (operation == 0 || expected.empty()) {
                values.push_back(step);
                expected.push_back(step);
            } else if (operation == 1) {
                u32 index = (state >> 8) % expected.size();
                values.erase(values.begin() + index);
                expected.erase(expected.begin() + index);
            } else if (operation == 2) {
                u32 size = (state >> 8) % 24;
                values.resize(size, step);
                expected.resize(size, step);
            } else if (operation == 3) {
                values.pop_back();
                expected.pop_back();
            } else {
                u32 count = (state >> 8) % 12;
                values.assign(count, step);
                expected.assign(count, step);
            }

            CAPTURE(step);
            CHECK(vector_values(values) == expected);
            CHECK(values.capacity() >= values.size());
        }
    }
}
