#include <algorithm>

#include "vc8/fixtures.hh"

namespace vc8 = treyarch::vc8;
using namespace tests::vc8_support;

TEST_SUITE("vc8") {
    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "hash set starts with one bucket and nine bounds") {

        vc8::hash_set<u32> values;

        CHECK(values.empty());
        CHECK(values.begin() == values.end());
        CHECK(values.find(123) == values.end());
        CHECK(allocations.size() == 2);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "hash set insert refuses a value it already holds") {

        vc8::hash_set<u32> values;

        auto first = values.insert(0x00B83C04);
        auto same  = values.insert(0x00B83C04);

        CHECK(first.second);
        CHECK_FALSE(same.second);
        CHECK(same.first == first.first);
        CHECK(first.first->value == 0x00B83C04);
        CHECK(values.size() == 1);
        CHECK(allocations.size() == 3);
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "hash set finds every value across bucket splits") {

        vc8::hash_set<u32> values;
        std::vector<u32>   keys = shuffled_keys(200, 0x5EED);

        for (u32 key : keys)
            CHECK(values.insert(key * 4 + 0x00B80000).second);

        CHECK(values.size() == 200);

        std::vector<u32> found;

        for (auto* position = values.begin(); position != values.end(); position = position->next) {
            CHECK(position->next->previous == position);
            found.push_back(position->value);
        }

        std::sort(found.begin(), found.end());

        for (u32 index = 0; index < found.size(); ++index) {
            CHECK(found[index] == index * 4 + 0x00B80000);
            CHECK(values.find(found[index])->value == found[index]);
        }

        CHECK(values.find(0x00B80002) == values.end());
    }

    TEST_CASE_FIXTURE(tests::c_memory_fixture,
                      "hash set destructor frees the entries sentinel and bounds") {

        {
            vc8::hash_set<u32> values;

            for (u32 key = 0; key < 50; ++key)
                values.insert(key);
        }

        CHECK(live_count(tests::heap_allocator) == 0);
        CHECK(free_count(tests::plain_allocator) == 0);
    }
}
