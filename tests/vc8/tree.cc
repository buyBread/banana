#include <algorithm>
#include <set>

#include "vc8/fixtures.hh"

namespace vc8 = treyarch::vc8;
using namespace tests::vc8_support;

TEST_SUITE("vc8") {
    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "tree starts with a black head linked to itself") {

        {
            vc8::set<u32> values;

            check_tree(values, {});

            CHECK(values.lower_bound(0) == values.end());
            CHECK(values.find(0) == values.end());
            CHECK(values.head()->previous() == values.head());

            REQUIRE(allocations.size() == 1);
            CHECK(allocations[0].size == sizeof(vc8::tree_node<u32>));
        }

        CHECK(live_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "tree insert rotates every unbalanced shape") {

        for (const auto &order : { std::vector<u32> { 3, 2, 1 },
                                   std::vector<u32> { 1, 2, 3 },
                                   std::vector<u32> { 3, 1, 2 },
                                   std::vector<u32> { 1, 3, 2 } }) {

            CAPTURE(order);

            vc8::set<u32>    values;
            std::vector<u32> expected;

            for (u32 key : order) {
                values.insert(key);
                expected.insert(std::upper_bound(expected.begin(), expected.end(), key), key);

                check_tree(values, expected);
            }

            CHECK(values.head()->parent->value == 2);
            CHECK(values.head()->left->value == 1);
            CHECK(values.head()->right->value == 3);
        }
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "tree insert stays balanced for sorted reversed and shuffled keys") {

        for (u32 order = 0; order < 3; ++order) {
            CAPTURE(order);

            auto keys = shuffled_keys(96, 0x87654321);

            if (order == 0)
                std::sort(keys.begin(), keys.end());
            else if (order == 1)
                std::sort(keys.rbegin(), keys.rend());

            vc8::set<u32>    values;
            std::vector<u32> expected;

            for (u32 key : keys) {
                CAPTURE(key);

                auto inserted = values.insert(key);
                expected.insert(std::upper_bound(expected.begin(), expected.end(), key), key);

                CHECK(inserted.second);
                CHECK(inserted.first->value == key);
                check_tree(values, expected);
            }
        }
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "tree insert returns an existing key without allocating") {

        vc8::set<u32> values;

        auto original = values.insert(7);
        auto repeated = values.insert(7);

        CHECK(original.second);
        CHECK_FALSE(repeated.second);
        CHECK(repeated.first == original.first);
        CHECK(allocations.size() == 2);
        check_tree(values, { 7 });
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "tree hinted insert uses a fitting hint and falls back otherwise") {

        vc8::set<u32> values;

        CHECK(values.insert(values.end(), 30)->value == 30);

        values.insert(10);
        values.insert(50);

        auto* thirty = values.find(30);
        auto* fifty  = values.find(50);

        CHECK(values.insert(values.begin(), 5)->value == 5);
        CHECK(values.insert(values.end(), 60)->value == 60);
        CHECK(values.insert(thirty, 25)->value == 25);
        CHECK(values.insert(thirty, 35)->value == 35);
        CHECK(values.insert(values.begin(), 40)->value == 40);
        CHECK(values.insert(values.end(), 20)->value == 20);
        CHECK(values.insert(thirty, 30) == thirty);
        CHECK(values.insert(values.begin(), 50) == fifty);

        check_tree(values, { 5, 10, 20, 25, 30, 35, 40, 50, 60 });
        CHECK(allocations.size() == 10);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "tree hinted insert stays balanced for any hint") {

        vc8::set<u32> values;
        std::set<u32> expected;
        random_t      random { 0x5EED1234 };

        for (u32 key : shuffled_keys(160, 7)) {
            CAPTURE(key);

            u32   roll = random.next();
            auto* hint = values.lower_bound(key);

            if (roll % 4 == 1)
                hint = hint->previous();
            else if (roll % 4 == 2)
                hint = values.begin();
            else if (roll % 4 == 3)
                hint = values.end();

            CHECK(values.insert(hint, key)->value == key);
            expected.insert(key);

            check_tree(values, std::vector<u32>(expected.begin(), expected.end()));
        }

        for (u32 key : shuffled_keys(160, 8)) {
            CAPTURE(key);

            auto* existing = values.find(key);

            CHECK(values.insert(values.begin(), key) == existing);
            CHECK(values.insert(values.end(), key) == existing);
        }

        CHECK(values.size() == 160);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "tree lower_bound and find compare keys unsigned") {

        vc8::set<u32> values;

        for (u32 key : { 9u, 0xFFFFFFFFu, 0u, 0x80000000u, 4u })
            values.insert(key);

        const auto &constant = values;

        check_tree(constant, { 0, 4, 9, 0x80000000, 0xFFFFFFFF });

        for (const auto &query : { std::pair<u32, u32> { 0,          0          },
                                                       { 1,          4          },
                                                       { 4,          4          },
                                                       { 5,          9          },
                                                       { 9,          9          },
                                                       { 10,         0x80000000 },
                                                       { 0x80000000, 0x80000000 },
                                                       { 0x80000001, 0xFFFFFFFF },
                                                       { 0xFFFFFFFF, 0xFFFFFFFF } }) {

            CAPTURE(query);

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
                      "tree node walks agree with the traversal order") {

        using node = vc8::tree_node<u32>;

        vc8::set<u32> values;

        for (u32 key : { 4u, 2u, 6u, 1u, 3u, 5u, 7u })
            values.insert(key);

        check_tree(values, { 1, 2, 3, 4, 5, 6, 7 });

        CHECK(node::minimum(values.head()->parent) == values.find(1));
        CHECK(node::maximum(values.head()->parent) == values.find(7));
        CHECK(node::minimum(values.find(6)) == values.find(5));
        CHECK(node::maximum(values.find(2)) == values.find(3));
        CHECK(values.find(3)->next() == values.find(4));
        CHECK(values.find(5)->previous() == values.find(4));
        CHECK(values.find(7)->next() == values.end());
        CHECK(values.find(1)->previous() == values.end());
        CHECK(values.end()->previous() == values.find(7));
        CHECK(values.end()->next() == values.end());
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "tree erase leaves the other nodes in place") {

        for (const auto &keys : { std::vector<u32> { 4 },
                                  std::vector<u32> { 4, 2 },
                                  std::vector<u32> { 4, 6 },
                                  std::vector<u32> { 4, 2, 6, 1, 3, 5, 7 },
                                  std::vector<u32> { 8, 3, 12, 1, 6, 10, 14, 4, 7, 9, 11, 13, 15 } }) {

            for (u32 removed : keys) {
                CAPTURE(keys);
                CAPTURE(removed);

                vc8::set<u32>                     values;
                std::vector<vc8::tree_node<u32>*> nodes;

                for (u32 key : keys)
                    nodes.push_back(values.insert(key).first);

                std::vector<u32> expected = keys;

                std::sort(expected.begin(), expected.end());
                expected.erase(std::find(expected.begin(), expected.end(), removed));

                auto* erased = values.find(removed);
                auto* next   = erased->next();

                CHECK(values.erase(erased) == next);
                CHECK(find(erased)->released);
                check_tree(values, expected);

                for (auto* node : nodes) {
                    if (node != erased)
                        CHECK(values.find(node->value) == node);
                }
            }
        }
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "tree erase stays balanced for every order of six keys") {

        std::vector<u32> order { 0, 1, 2, 3, 4, 5 };

        do {
            CAPTURE(order);

            vc8::set<u32>    values;
            std::vector<u32> expected { 0, 1, 2, 3, 4, 5 };

            for (u32 key : { 3u, 1u, 5u, 0u, 2u, 4u })
                values.insert(key);

            for (u32 key : order) {
                values.erase(values.find(key));
                expected.erase(std::find(expected.begin(), expected.end(), key));

                check_tree(values, expected);
            }
        } while (std::next_permutation(order.begin(), order.end()));

        CHECK(live_count(tests::heap_allocator) == 0);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "tree erase rejects the head") {

        vc8::set<u32> values;

        CHECK_THROWS_WITH_AS(values.erase(values.end()), "invalid map/set<T> iterator", std::out_of_range);
        check_tree(values, {});

        values.insert(4);

        CHECK_THROWS_WITH_AS(values.erase(values.end()), "invalid map/set<T> iterator", std::out_of_range);
        check_tree(values, { 4 });
        CHECK(frees.empty());
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "tree range erase stops at its last node") {

        vc8::set<u32> values;

        for (u32 key = 0; key < 10; ++key)
            values.insert(key);

        auto* three = values.find(3);
        auto* seven = values.find(7);
        auto* two   = values.find(2);

        CHECK(values.erase(three, three) == three);
        check_tree(values, { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });

        CHECK(values.erase(three, seven) == seven);
        check_tree(values, { 0, 1, 2, 7, 8, 9 });

        CHECK(values.erase(values.find(8), values.end()) == values.end());
        check_tree(values, { 0, 1, 2, 7 });

        CHECK(values.erase(values.begin(), two) == two);
        check_tree(values, { 2, 7 });
        CHECK(values.find(7) == seven);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "tree whole range erase and clear keep the head") {

        vc8::set<u32> values;
        auto*         head = values.head();

        for (u32 key : shuffled_keys(80, 3))
            values.insert(key);

        CHECK(values.erase(values.begin(), values.end()) == head);
        CHECK(values.head() == head);
        CHECK(live_count(tests::heap_allocator) == 1);
        check_tree(values, {});

        values.insert(19);
        values.insert(4);
        values.clear();
        values.clear();

        CHECK(values.head() == head);
        CHECK(live_count(tests::heap_allocator) == 1);
        check_tree(values, {});

        values.insert(19);

        check_tree(values, { 19 });
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "tree random edits match std::set") {

        vc8::set<u32> values;
        std::set<u32> expected;
        random_t      random { 0xABCDEF01 };

        for (u32 step = 0; step < 700; ++step) {
            CAPTURE(step);

            u32 roll = random.next();
            u32 key  = (roll >> 9) % 90;

            if (roll & 3) {
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
