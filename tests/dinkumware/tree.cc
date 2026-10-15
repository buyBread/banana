#include "dinkumware/fixtures.hh"

namespace dinkumware = treyarch::dinkumware;
using namespace tests::container_support;

// map and set share a red-black tree; rotations change links while keeping each surviving value in its original node.
// the head is the only nil node and doubles as end; it also remembers the root and the smallest and largest nodes.
TEST_SUITE("dinkumware") {
    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "tree starts with a black sentinel linked to itself") {

        {
            dinkumware::set<u32> values;
            check_tree(values, {});

            CHECK(values.lower_bound(0) == values.end());
            CHECK(values.find(0) == values.end());
            CHECK(values.head()->previous() == values.head());
            REQUIRE(allocations.size() == 1);
            CHECK(allocations[0].size == sizeof(dinkumware::tree_node<u32>));
        }

        CHECK(live_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "tree handles left right and both double insertion rotations") {

        for (const auto &order : { std::vector<u32> { 3, 2, 1 },
                                   std::vector<u32> { 1, 2, 3 },
                                   std::vector<u32> { 3, 1, 2 },
                                   std::vector<u32> { 1, 3, 2 } }) {

            dinkumware::set<u32> values;
            std::vector<u32>     expected;

            for (u32 key : order) {
                values.insert(key);
                expected.push_back(key);
                std::sort(expected.begin(), expected.end());
                check_tree(values, expected);
            }

            CHECK(values.head()->parent->value == 2);
            CHECK(values.head()->left->value == 1);
            CHECK(values.head()->right->value == 3);
        }
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "tree insertion preserves balance for sorted reversed and shuffled keys") {

        for (u32 order = 0; order < 3; ++order) {
            auto keys = shuffled_keys(96, 0x87654321);

            if (order != 2) {
                std::sort(keys.begin(), keys.end());
                if (order == 1)
                    std::reverse(keys.begin(), keys.end());
            }

            dinkumware::set<u32> values;
            std::vector<u32>     expected;

            for (u32 key : keys) {
                CAPTURE(order);
                CAPTURE(key);

                auto inserted = values.insert(key);
                expected.push_back(key);
                std::sort(expected.begin(), expected.end());

                CHECK(inserted.second);
                CHECK(inserted.first->value == key);
                check_tree(values, expected);
            }
        }
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "tree duplicate insertion returns the existing node without allocation") {

        dinkumware::set<u32> values;
        auto                 original = values.insert(7);
        auto                 repeated = values.insert(7);

        CHECK(original.second);
        CHECK_FALSE(repeated.second);
        CHECK(repeated.first == original.first);
        CHECK(allocation_count(tests::heap_allocator) == 2);
        check_tree(values, {7});
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "tree lower bound finds exact keys gaps and unsigned extremes") {

        dinkumware::set<u32> values;
        for (u32 key : {9u, 0xFFFFFFFFu, 0u, 0x80000000u, 4u})
            values.insert(key);

        const auto &constant = values;
        check_tree(constant, {0, 4, 9, 0x80000000, 0xFFFFFFFF});

        for (const auto &query : { std::pair<u32, u32> {0,          0},
                                                       {1,          4},
                                                       {4,          4},
                                                       {5,          9},
                                                       {9,          9},
                                                       {10,         0x80000000},
                                                       {0x80000000, 0x80000000},
                                                       {0x80000001, 0xFFFFFFFF},
                                                       {0xFFFFFFFF, 0xFFFFFFFF} }) {

            auto* result = constant.lower_bound(query.first);
            REQUIRE(result != constant.end());
            CHECK(result->value == query.second);
        }

        CHECK(values.find(1) == values.end());
        CHECK(values.find(0x7FFFFFFF) == values.end());
        values.erase(values.find(0xFFFFFFFF));

        CHECK(values.lower_bound(0x80000001) == values.end());
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "tree successor predecessor and extrema agree with ordered traversal") {

        dinkumware::set<u32> values;
        for (u32 key : { 4u, 2u, 6u, 1u, 3u, 5u, 7u })
            values.insert(key);

        using node = dinkumware::tree_node<u32>;

        CHECK(node::minimum(values.head()->parent) == values.find(1));
        CHECK(node::maximum(values.head()->parent) == values.find(7));
        CHECK(node::minimum(values.find(2)) == values.find(1));
        CHECK(node::maximum(values.find(6)) == values.find(7));
        CHECK(values.find(3)->next() == values.find(4));
        CHECK(values.find(5)->previous() == values.find(4));
        CHECK(values.find(7)->next() == values.end());
        CHECK(values.end()->previous() == values.find(7));
        check_tree(values, { 1, 2, 3, 4, 5, 6, 7 });
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "tree erases leaves one-child nodes and two-child nodes without relocating survivors") {

        for (const auto &keys : { std::vector<u32> { 4 },
                                  std::vector<u32> { 4, 2 },
                                  std::vector<u32> { 4, 6 },
                                  std::vector<u32> { 4, 2, 6, 1, 3, 5, 7 },
                                  std::vector<u32> { 8, 3, 12, 1, 6, 10, 14, 4, 7, 9, 11, 13, 15 } }) {

            for (u32 removed : keys) {
                CAPTURE(removed);

                dinkumware::set<u32> values;

                std::vector
                    <std::pair
                        <u32, decltype(values.head())>> nodes;

                std::vector<u32> expected;

                for (u32 key : keys) {
                    auto inserted = values.insert(key);
                    nodes.push_back({key, inserted.first});
                    if (key != removed)
                        expected.push_back(key);
                }

                std::sort(expected.begin(), expected.end());
                auto* erased = values.find(removed);
                auto* next   = erased->next();

                CHECK(values.erase(erased) == next);
                CHECK(find(erased)->released);
                CHECK(values.find(removed) == values.end());
                check_tree(values, expected);

                for (const auto &saved : nodes) {
                    if (saved.first != removed)
                        CHECK(values.find(saved.first) == saved.second);
                }
            }
        }
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "tree deletion remains balanced through every erase order of six keys") {

        std::vector<u32> order{ 0, 1, 2, 3, 4, 5 };
        u32 permutation = 0;

        // replay all 720 deletion orders; the same initial tree reaches different sibling and replacement cases.
        do {
            CAPTURE(permutation);

            dinkumware::set<u32> values;
            std::vector<u32> expected { 0, 1, 2, 3, 4, 5 };

            for (u32 key : { 3u, 1u, 5u, 0u, 2u, 4u })
                values.insert(key);

            for (u32 key : order) {
                CAPTURE(key);
                values.erase(values.find(key));
                expected.erase(std::find(expected.begin(), expected.end(), key));
                check_tree(values, expected);
            }

            ++permutation;
        } while (std::next_permutation(order.begin(), order.end()));

        CHECK(live_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "tree erase rejects the end marker without changing the tree") {

        dinkumware::set<u32> values;

        CHECK_THROWS_WITH_AS(values.erase(values.end()), "invalid map/set<T> iterator", std::out_of_range);
        check_tree(values, {});

        values.insert(4);

        CHECK_THROWS_AS(values.erase(values.end()), std::out_of_range);
        check_tree(values, { 4 });
        CHECK(frees.empty());
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "tree range erase preserves its excluded end and supports empty ranges") {

        dinkumware::set<u32> values;
        for (u32 key = 0; key < 10; ++key)
            values.insert(key);

        auto* first = values.find(3);
        auto* last  = values.find(7);

        CHECK(values.erase(first, first) == first);
        CHECK(values.erase(first, last) == last);
        check_tree(values, { 0, 1, 2, 7, 8, 9 });
        CHECK(values.find(7) == last);

        CHECK(values.erase(values.find(8), values.end()) == values.end());
        check_tree(values, { 0, 1, 2, 7 });

        CHECK(values.erase(values.begin(), values.find(2)) == values.find(2));
        check_tree(values, { 2, 7 });
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "tree whole range erase retains the sentinel and supports reinsertion") {

        dinkumware::set<u32> values;
        auto*                head   = values.head();

        for (u32 key : shuffled_keys(80, 3))
            values.insert(key);

        CHECK(values.erase(values.begin(), values.end()) == head);
        CHECK(values.head() == head);
        CHECK(live_count(tests::heap_allocator) == 1);
        check_tree(values, {});

        CHECK(values.erase(values.begin(), values.end()) == head);
        values.insert(19);
        check_tree(values, { 19 });
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "tree mixed edits retain lookup ordering and balance") {

        dinkumware::set<u32> values;
        std::set<u32>        expected;
        u32                  state = 0xABCDEF01;

        for (u32 step = 0; step < 700; ++step) {
            state = state * 1664525u + 1013904223u;
            const u32 key = (state >> 9) % 90;

            if ((state & 3) != 0) {
                auto actual = values.insert(key);
                auto wanted = expected.insert(key);

                CHECK(actual.second == wanted.second);
                CHECK(actual.first->value == key);
            } else {
                auto* node = values.find(key);

                if (node != values.end())
                    values.erase(node);

                expected.erase(key);
            }

            CAPTURE(step);
            check_tree(values, std::vector<u32>(expected.begin(), expected.end()));

            auto  wanted = expected.lower_bound(key);
            auto* actual = values.lower_bound(key);

            if (wanted == expected.end())
                CHECK(actual == values.end());
            else {
                REQUIRE(actual != values.end());
                CHECK(actual->value == *wanted);
            }
        }
    }
}
