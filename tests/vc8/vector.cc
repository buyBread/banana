#include "vc8/fixtures.hh"

namespace vc8 = treyarch::vc8;
using namespace tests::vc8_support;

TEST_SUITE("vc8") {
    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "vector starts empty without storage") {

        vc8::vector<u32> values;
        const auto      &constant = values;

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
                      "vector reserve allocates the exact capacity") {

        {
            vc8::vector<u64> values;
            values.reserve(7);

            auto* storage = values.begin();

            REQUIRE(allocations.size() == 1);
            CHECK(allocations[0].size == 7 * sizeof(u64));
            CHECK(values.capacity() == 7);
            CHECK(values.size() == 0);
            CHECK(values.end() == storage);

            values.reserve(3);
            values.reserve(7);

            CHECK(values.begin() == storage);
            CHECK(allocations.size() == 1);
        }

        CHECK(live_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "vector reserve copies into the new storage") {

        value_trace_t trace;
        {
            tracked_value_t first(1, trace);
            tracked_value_t second(2, trace);

            vc8::vector<tracked_value_t> values;
            values.reserve(2);
            values.push_back(first);
            values.push_back(second);

            auto* storage = values.begin();
            trace.events.clear();

            values.reserve(5);

            CHECK(trace.events == std::vector<value_event_t> { { value_copied,    1 },
                                                               { value_copied,    2 },
                                                               { value_destroyed, 1 },
                                                               { value_destroyed, 2 } });
            CHECK(find(storage)->released);
            CHECK(values.capacity() == 5);
            CHECK(values.size() == 2);
            CHECK(values[0].value == 1);
            CHECK(values[1].value == 2);
        }

        CHECK(trace.live == 0);
        CHECK(live_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "vector reserve rejects a byte size past u32") {

        vc8::vector<u64> values;
        values.push_back(42);

        auto* storage = values.begin();

        CHECK_THROWS_WITH_AS(values.reserve(0x20000000), "vector<T> too long", std::length_error);
        CHECK(values.begin() == storage);
        CHECK(vector_values(values) == std::vector<u64> { 42 });
        CHECK(allocations.size() == 1);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "vector push_back grows capacity by half") {

        const u32 capacities[] { 1, 2, 3, 4, 6, 6, 9, 9, 9, 13, 13, 13 };

        vc8::vector<u32> values;
        std::vector<u32> expected;

        for (u32 index = 0; index < 12; ++index) {
            CAPTURE(index);

            values.push_back(100 + index);
            expected.push_back(100 + index);

            CHECK(values.capacity() == capacities[index]);
            CHECK(values.back() == 100 + index);
            CHECK(vector_values(values) == expected);
        }

        CHECK(allocations.size() == 7);
        CHECK(live_count(tests::heap_allocator) == 1);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "vector push_back growth moves the old elements") {

        value_trace_t trace;
        {
            tracked_value_t first(1, trace);
            tracked_value_t second(2, trace);

            vc8::vector<tracked_value_t> values;
            values.push_back(first);
            trace.events.clear();

            values.push_back(second);

            CHECK(trace.events == std::vector<value_event_t> { { value_moved,     1 },
                                                               { value_destroyed, -1 },
                                                               { value_copied,    2 } });
            CHECK(values[0].value == 1);
            CHECK(values[1].value == 2);
        }

        CHECK(trace.live == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "vector clear destroys front to back and keeps the storage") {

        value_trace_t trace;
        {
            tracked_value_t seed(0, trace);

            vc8::vector<tracked_value_t> values;
            values.reserve(4);

            for (i32 value : { 1, 2, 3 }) {
                seed.value = value;
                values.push_back(seed);
            }

            auto* storage = values.begin();
            trace.events.clear();

            values.clear();

            CHECK(trace.events == std::vector<value_event_t> { { value_destroyed, 1 },
                                                               { value_destroyed, 2 },
                                                               { value_destroyed, 3 } });
            CHECK(values.empty());
            CHECK(values.capacity() == 4);
            CHECK(values.begin() == storage);
            CHECK(values.end() == storage);
            CHECK(frees.empty());
        }

        CHECK(trace.live == 0);
        CHECK(live_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "vector destroy frees the storage once") {

        vc8::vector<u32> values;
        values.assign(3, 7);

        auto* storage = values.begin();

        values.destroy();
        values.destroy();

        CHECK(find(storage)->released);
        CHECK(free_count(tests::heap_allocator) == 1);
        CHECK(values.begin() == nullptr);
        CHECK(values.end() == nullptr);
        CHECK(values.capacity() == 0);

        values.push_back(9);

        CHECK(vector_values(values) == std::vector<u32> { 9 });
        CHECK(values.capacity() == 1);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "vector copy sizes its storage to the source elements") {

        vc8::vector<u32> source;
        source.reserve(12);
        source.push_back(3);
        source.push_back(5);

        vc8::vector<u32> copied(source);
        copied[0] = 99;

        CHECK(vector_values(copied) == std::vector<u32> { 99, 5 });
        CHECK(vector_values(source) == std::vector<u32> { 3, 5 });
        CHECK(copied.capacity() == 2);

        source.clear();

        vc8::vector<u32> empty(source);

        CHECK(empty.begin() == nullptr);
        CHECK(empty.capacity() == 0);
        CHECK(allocations.size() == 2);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "vector copy copies every element") {

        value_trace_t trace;
        {
            tracked_value_t seed(9, trace);

            vc8::vector<tracked_value_t> source;
            source.assign(3, seed);
            trace.events.clear();

            {
                vc8::vector<tracked_value_t> copied(source);
                copied[0].value = 44;

                CHECK(trace.events == std::vector<value_event_t> { { value_copied, 9 },
                                                                   { value_copied, 9 },
                                                                   { value_copied, 9 } });
                CHECK(source[0].value == 9);
            }

            CHECK(trace.values(value_destroyed) == std::vector<i32> { 44, 9, 9 });
        }

        CHECK(trace.live == 0);
        CHECK(live_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "vector assign reallocates only past its capacity") {

        vc8::vector<u32> values;
        values.reserve(8);
        values.assign(5, 3);

        auto* storage = values.begin();

        values.assign(2, 9);

        CHECK(vector_values(values) == std::vector<u32> { 9, 9 });
        CHECK(values.begin() == storage);

        values.assign(0, 99);

        CHECK(values.empty());
        CHECK(values.begin() == storage);
        CHECK(values.capacity() == 8);
        CHECK(allocations.size() == 1);

        values.assign(10, 4);

        CHECK(vector_values(values) == std::vector<u32>(10, 4));
        CHECK(values.capacity() == 10);
        CHECK(allocations.size() == 2);
        CHECK(allocations[0].released);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "vector assign destroys the old elements before copying") {

        value_trace_t trace;
        {
            tracked_value_t seed(1, trace);

            vc8::vector<tracked_value_t> values;
            values.assign(2, seed);
            values[1].value = 2;

            seed.value = 7;
            trace.events.clear();

            values.assign(2, seed);

            CHECK(trace.events == std::vector<value_event_t> { { value_destroyed, 1 },
                                                               { value_destroyed, 2 },
                                                               { value_copied,    7 },
                                                               { value_copied,    7 } });
        }

        CHECK(trace.live == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "vector resize keeps the prefix and fills the rest") {

        vc8::vector<u32> values;
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

        CHECK(vector_values(values) == std::vector<u32> { 1, 8, 8, 8, 8, 8, 8 });
        CHECK(values.capacity() == 9);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "vector resize moves on growth and destroys the cut tail") {

        value_trace_t trace;
        {
            tracked_value_t seed(0, trace);

            vc8::vector<tracked_value_t> values;
            values.assign(2, seed);
            values[0].value = 1;
            values[1].value = 2;

            seed.value = 3;
            trace.events.clear();

            values.resize(4, seed);

            CHECK(trace.events == std::vector<value_event_t> { { value_moved,     1 },
                                                               { value_moved,     2 },
                                                               { value_destroyed, -1 },
                                                               { value_destroyed, -1 },
                                                               { value_copied,    3 },
                                                               { value_copied,    3 } });
            CHECK(values.capacity() == 4);

            values[3].value = 4;
            trace.events.clear();

            values.resize(1, seed);
            values.pop_back();

            CHECK(trace.events == std::vector<value_event_t> { { value_destroyed, 2 },
                                                               { value_destroyed, 3 },
                                                               { value_destroyed, 4 },
                                                               { value_destroyed, 1 } });
            CHECK(values.empty());
            CHECK(values.capacity() == 4);
        }

        CHECK(trace.live == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "vector erase shifts the tail down by move assignment") {

        value_trace_t trace;
        {
            tracked_value_t seed(0, trace);

            vc8::vector<tracked_value_t> values;
            values.assign(4, seed);

            for (u32 index = 0; index < 4; ++index)
                values[index].value = index + 1;

            auto* storage = values.begin();
            trace.events.clear();

            auto* next = values.erase(storage + 1);

            CHECK(next == storage + 1);
            CHECK(trace.events == std::vector<value_event_t> { { value_move_assigned, 3 },
                                                               { value_move_assigned, 4 },
                                                               { value_destroyed,     -1 } });
            CHECK(values.size() == 3);
            CHECK(values[1].value == 3);
            CHECK(values[2].value == 4);

            auto* last = values.end() - 1;
            next       = values.erase(last);

            CHECK(next == last);
            CHECK(values.end() == last);
            CHECK(values.capacity() == 4);
        }

        CHECK(trace.live == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "vector range erase returns the range start") {

        for (const auto &range : { std::pair<u32, u32> { 0, 0 },
                                                       { 2, 2 },
                                                       { 1, 4 },
                                                       { 0, 2 },
                                                       { 3, 5 },
                                                       { 0, 5 } }) {

            CAPTURE(range);

            vc8::vector<u32> values;
            std::vector<u32> expected { 1, 2, 3, 4, 5 };

            for (u32 value : expected)
                values.push_back(value);

            auto* storage = values.begin();
            auto* next    = values.erase(storage + range.first, storage + range.second);

            expected.erase(expected.begin() + range.first, expected.begin() + range.second);

            CHECK(next == storage + range.first);
            CHECK(values.begin() == storage);
            CHECK(vector_values(values) == expected);
        }
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "vector random edits match std::vector") {

        vc8::vector<u32> values;
        std::vector<u32> expected;
        random_t         random { 0x12345678 };

        for (u32 step = 0; step < 600; ++step) {
            CAPTURE(step);

            u32 roll = random.next();

            if (roll % 5 == 0 || expected.empty()) {
                values.push_back(step);
                expected.push_back(step);
            } else if (roll % 5 == 1) {
                u32 index = (roll >> 8) % expected.size();

                values.erase(values.begin() + index);
                expected.erase(expected.begin() + index);
            } else if (roll % 5 == 2) {
                u32 size = (roll >> 8) % 24;

                values.resize(size, step);
                expected.resize(size, step);
            } else if (roll % 5 == 3) {
                values.pop_back();
                expected.pop_back();
            } else {
                u32 count = (roll >> 8) % 12;

                values.assign(count, step);
                expected.assign(count, step);
            }

            CHECK(vector_values(values) == expected);
            CHECK(values.capacity() >= values.size());
        }
    }
}
