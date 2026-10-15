#pragma once

#include <algorithm>
#include <cstring>
#include <functional>
#include <set>
#include <type_traits>
#include <utility>
#include <vector>
#include <doctest/doctest.h>

#include "support/memory.hh"
#include "treyarch/shared/dinkumware/hash_map.hh"
#include "treyarch/shared/dinkumware/map.hh"
#include "treyarch/shared/dinkumware/set.hh"

namespace tests { namespace container_support {
    enum e_value_event {
        value_constructed,
        value_copied,
        value_moved,
        value_copy_assigned,
        value_move_assigned,
        value_destroyed
    };

    struct value_event_t {
        e_value_event kind;
        i32           value;

        bool operator==(const value_event_t&) const = default;
    };

    struct value_trace_t {
        i32                 live = 0;
        std::vector
            <value_event_t> events;

        u32 count(e_value_event kind) const;

        std::vector<i32> values(e_value_event kind) const;
    };

    // an integer alone cannot show whether storage growth copied, moved or destroyed its elements.
    // this value records those operations; moving also leaves -1 behind so the two objects stay distinguishable.
    struct tracked_value_t {
        i32            value;
        value_trace_t* trace;

        tracked_value_t(i32 value, value_trace_t &trace);
        tracked_value_t(const tracked_value_t &other);
        tracked_value_t(tracked_value_t &&other) noexcept;
       ~tracked_value_t();

        tracked_value_t &operator=(const tracked_value_t &other);
        tracked_value_t &operator=(tracked_value_t &&other) noexcept;
    };

    struct identity_hash_t {
        u32 operator()(u32 key) const noexcept { return key; }
        bool less(u32 left, u32 right) const noexcept { return left < right; }
    };

    struct collision_hash_t : identity_hash_t {
        u32 operator()(u32) const noexcept { return 0; }
    };

    using integer_hash_map = treyarch::dinkumware::hash_map<u32, u32, identity_hash_t>;

    std::vector<u32> shuffled_keys(u32 count, u32 seed);

    template<typename T>
    std::vector<T> vector_values(const treyarch::dinkumware::vector<T> &values) {
        std::vector<T> result;

        for (u32 index = 0; index < values.size(); ++index)
            result.push_back(values[index]);

        return result;
    }

    template<typename T>
    std::vector<T> list_values(const treyarch::dinkumware::list<T> &values) {
        std::vector<T> result;
        auto*          position = values.begin();

        // a broken circular list must fail a bounded check rather than make the test loop forever.
        for (u32 index = 0; index < values.size(); ++index) {
            REQUIRE(position != values.end());
            CHECK(position->next->previous == position);
            CHECK(position->previous->next == position);
            result.push_back(position->value);
            position = position->next;
        }

        CHECK(position == values.end());
        CHECK(values.head()->next->previous == values.head());
        CHECK(values.head()->previous->next == values.head());

        return result;
    }

    // bucket boundaries and counts are part of the 32-bit layout shared with the game.
    // take a snapshot of those fields without adding test accessors or replacing any container operations.
    template<typename Node>
    struct hash_state_t {
        Node** bounds;
        u32    bound_count;
        u32    bound_capacity;
        u32    mask;
        u32    bucket_count;
    };

    template<typename HashMap>
    auto hash_state(const HashMap &values) {
        using node = std::remove_pointer_t<decltype(values.head())>;

        node** first;
        node** last;
        node** end;
        u32    mask;
        u32    count;

        const u8* bytes = (const u8*)&values;
        std::memcpy(&first, bytes + 20, sizeof(first));
        std::memcpy(&last,  bytes + 24, sizeof(last));
        std::memcpy(&end,   bytes + 28, sizeof(end));
        std::memcpy(&mask,  bytes + 32, sizeof(mask));
        std::memcpy(&count, bytes + 36, sizeof(count));

        return hash_state_t<node>{first, (u32)(last - first), (u32)(end - first), mask, count};
    }

    template<typename HashMap, typename Hash = identity_hash_t>
    std::vector<u32> check_hash(const HashMap &values, Hash hash = {}) {
        const auto       state    = hash_state(values);
        auto*            position = values.begin();
        u32              visited  = 0;
        std::vector<u32> result;

        REQUIRE(state.bucket_count < state.bound_count);
        CHECK(state.bounds[0] == values.begin());

        // adjacent boundaries describe one bucket's half-open range in the shared linked list.
        // test that range against the key's hash independently of the container's find method.
        for (u32 bucket = 0; bucket < state.bucket_count; ++bucket) {
            CHECK(position == state.bounds[bucket]);
            bool has_previous = false;
            u32  previous     = 0;

            while (position != state.bounds[bucket + 1]) {
                REQUIRE(position != values.end());
                REQUIRE(visited++ < values.size());
                CHECK(position->next->previous == position);
                CHECK(position->previous->next == position);

                u32 selected = hash(position->value.key) & state.mask;
                if (selected >= state.bucket_count)
                    selected -= (state.mask + 1) / 2;

                CHECK(selected == bucket);
                if (has_previous)
                    CHECK(previous < position->value.key);

                previous = position->value.key;
                has_previous = true;
                result.push_back(previous);
                CHECK(values.find(previous) == position);
                position = position->next;
            }
        }

        CHECK(position == values.end());
        CHECK(visited == values.size());
        CHECK(values.head()->next->previous == values.head());
        CHECK(values.head()->previous->next == values.head());

        return result;
    }

    inline u32 tree_key(u32 value) { return value; }

    template<typename V>
    u32 tree_key(const std::pair<const u32, V> &value) { return value.first; }

    template<typename Node>
    u32 check_tree_branch(Node* position, Node* head, std::set<Node*> &visited, u32 size) {
        if (position == head)
            return 1;

        REQUIRE(visited.size() < size);
        REQUIRE(visited.insert(position).second);
        REQUIRE(current_memory_fixture->contains(position));
        CHECK_FALSE(position->is_nil);
        REQUIRE(position->left != nullptr);
        REQUIRE(position->right != nullptr);
        CHECK((position->color == treyarch::dinkumware::tree_color_red ||
               position->color == treyarch::dinkumware::tree_color_black));

        if (position->left != head)
            CHECK(position->left->parent == position);
        if (position->right != head)
            CHECK(position->right->parent == position);

        // red nodes cannot have red children; every path down to the sentinel has the same black-node count.
        // these checks catch balancing errors even when a simple lookup still happens to succeed.
        if (position->color == treyarch::dinkumware::tree_color_red) {
            CHECK(position->left->color == treyarch::dinkumware::tree_color_black);
            CHECK(position->right->color == treyarch::dinkumware::tree_color_black);
        }

        u32 left_height  = check_tree_branch(position->left, head, visited, size);
        u32 right_height = check_tree_branch(position->right, head, visited, size);

        CHECK(left_height == right_height);

        return left_height + (position->color == treyarch::dinkumware::tree_color_black);
    }

    template<typename Tree>
    void check_tree(const Tree &values, const std::vector<u32> &expected) {
        auto*                    head    = values.head();
        std::set<decltype(head)> visited;

        CHECK(head->is_nil);
        CHECK(head->color == treyarch::dinkumware::tree_color_black);
        CHECK(values.size() == expected.size());
        CHECK(values.empty() == expected.empty());

        if (expected.empty()) {
            CHECK(head->parent == head);
            CHECK(head->left == head);
            CHECK(head->right == head);
        } else {
            REQUIRE(head->parent != head);
            CHECK(head->parent->parent == head);
            CHECK(head->parent->color == treyarch::dinkumware::tree_color_black);
        }

        check_tree_branch(head->parent, head, visited, values.size());
        CHECK(visited.size() == expected.size());

        auto* position = values.begin();
        for (u32 key : expected) {
            REQUIRE(position != values.end());
            CHECK(tree_key(position->value) == key);
            position = position->next();
        }
        CHECK(position == values.end());

        position = values.end();
        for (auto key = expected.rbegin(); key != expected.rend(); ++key) {
            position = position->previous();
            REQUIRE(position != values.end());
            CHECK(tree_key(position->value) == *key);
        }
        CHECK(position == values.begin());
        CHECK(head->next() == head);
    }
}}
