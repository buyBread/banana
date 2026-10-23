#pragma once

#include <set>
#include <type_traits>
#include <utility>
#include <vector>
#include <doctest/doctest.h>

#include "support/memory.hh"
#include "vc8/hash_map.hh"
#include "vc8/list.hh"
#include "vc8/map.hh"
#include "vc8/set.hh"
#include "vc8/tree.hh"
#include "vc8/vector.hh"

namespace tests { namespace vc8_support {
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
        i32                        live = 0;
        std::vector<value_event_t> events;

        u32              count(e_value_event kind) const;
        std::vector<i32> values(e_value_event kind) const;
    };

    // moving leaves -1 behind
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

    struct random_t {
        u32 state;

        u32 next() noexcept {
            state = state * 1664525u + 1013904223u;

            return state;
        }
    };

    std::vector<u32> shuffled_keys(u32 count, u32 seed);

    struct identity_hash_t {
        u32  operator()(u32 key) const noexcept { return key; }
        bool less(u32 left, u32 right) const noexcept { return left < right; }
    };

    struct collision_hash_t : identity_hash_t {
        u32 operator()(u32) const noexcept { return 0; }
    };

    using integer_hash_map = treyarch::vc8::hash_map<u32, u32, identity_hash_t>;

    template<typename T>
    std::vector<T> vector_values(const treyarch::vc8::vector<T> &values) {
        return std::vector<T>(values.begin(), values.end());
    }

    template<typename T>
    std::vector<T> list_values(const treyarch::vc8::list<T> &values) {
        std::vector<T> result;
        auto*          head     = values.head();
        auto*          position = head->next;

        CHECK(head->next->previous == head);
        CHECK(head->previous->next == head);

        for (u32 index = 0; index < values.size(); ++index) {
            REQUIRE(position != head);
            CHECK(position->next->previous == position);
            CHECK(position->previous->next == position);

            result.push_back(position->value);
            position = position->next;
        }

        CHECK(position == head);

        return result;
    }

    // hash_map's private members
    template<typename node_t>
    struct hash_map_layout_t {
        u32                            traits;
        u32                            entries_allocator;
        node_t*                        entries_head;
        u32                            entries_size;
        treyarch::vc8::vector<node_t*> bounds;
        u32                            mask;
        u32                            bucket_count;
    };

    template<typename hash_map_t>
    const auto &hash_layout(const hash_map_t &values) {
        using node_t = std::remove_pointer_t<decltype(values.head())>;

        static_assert(sizeof(hash_map_layout_t<node_t>) == sizeof(hash_map_t));

        return *(const hash_map_layout_t<node_t>*)&values;
    }

    template<typename hash_map_t, typename hash_t = identity_hash_t>
    std::vector<u32> check_hash(const hash_map_t &values, hash_t hash = {}) {
        const auto      &layout   = hash_layout(values);
        auto*            position = values.begin();
        std::vector<u32> result;

        REQUIRE(layout.entries_head == values.head());
        REQUIRE(layout.entries_size == values.size());
        REQUIRE(layout.bucket_count < layout.bounds.size());

        for (u32 bucket = 0; bucket < layout.bucket_count; ++bucket) {
            CAPTURE(bucket);
            CHECK(position == layout.bounds[bucket]);

            for (auto* first = position; position != layout.bounds[bucket + 1]; position = position->next) {
                REQUIRE(position != values.end());
                REQUIRE(result.size() < values.size());
                CHECK(position->next->previous == position);
                CHECK(position->previous->next == position);

                u32 selected = hash(position->value.key) & layout.mask;

                if (selected >= layout.bucket_count)
                    selected -= (layout.mask + 1) / 2;

                CHECK(selected == bucket);

                if (position != first)
                    CHECK(position->previous->value.key < position->value.key);

                CHECK(values.find(position->value.key) == position);
                result.push_back(position->value.key);
            }
        }

        for (u32 bound = layout.bucket_count; bound < layout.bounds.size(); ++bound)
            CHECK(layout.bounds[bound] == values.end());

        CHECK(position == values.end());
        CHECK(result.size() == values.size());
        CHECK(values.head()->next->previous == values.head());
        CHECK(values.head()->previous->next == values.head());

        return result;
    }

    inline u32 tree_key(u32 value) { return value; }

    template<typename V>
    u32 tree_key(const std::pair<const u32, V> &value) { return value.first; }

    template<typename node_t>
    u32 check_tree_branch(node_t* position, node_t* head, std::set<node_t*> &visited) {
        if (position == head)
            return 1;

        REQUIRE(visited.insert(position).second);
        REQUIRE(current_memory_fixture->contains(position));
        CHECK_FALSE(position->is_nil);

        if (position->left != head)
            CHECK(position->left->parent == position);

        if (position->right != head)
            CHECK(position->right->parent == position);

        if (position->color == treyarch::vc8::tree_color_red) {
            CHECK(position->left->color  == treyarch::vc8::tree_color_black);
            CHECK(position->right->color == treyarch::vc8::tree_color_black);
        } else
            CHECK(position->color == treyarch::vc8::tree_color_black);

        u32 left_height  = check_tree_branch(position->left,  head, visited);
        u32 right_height = check_tree_branch(position->right, head, visited);

        CHECK(left_height == right_height);

        return left_height + (position->color == treyarch::vc8::tree_color_black);
    }

    template<typename tree_t>
    void check_tree(const tree_t &values, const std::vector<u32> &expected) {
        using node_t = std::remove_pointer_t<decltype(values.head())>;

        node_t*           head = values.head();
        std::set<node_t*> visited;

        CHECK(head->is_nil);
        CHECK(head->color == treyarch::vc8::tree_color_black);
        CHECK(values.size() == expected.size());
        CHECK(values.empty() == expected.empty());

        if (expected.empty()) {
            CHECK(head->parent == head);
            CHECK(head->left   == head);
            CHECK(head->right  == head);
        } else {
            REQUIRE(head->parent != head);
            CHECK(head->parent->parent == head);
            CHECK(head->parent->color  == treyarch::vc8::tree_color_black);
        }

        check_tree_branch(head->parent, head, visited);

        REQUIRE(visited.size() == expected.size());

        if (!expected.empty()) {
            CHECK(head->left  == node_t::minimum(head->parent));
            CHECK(head->right == node_t::maximum(head->parent));
        }

        std::vector<u32> forward;
        std::vector<u32> backward;

        for (node_t* position = values.begin(); position != head; position = position->next()) {
            REQUIRE(forward.size() < expected.size());
            forward.push_back(tree_key(position->value));
        }

        for (node_t* position = head->previous(); position != head; position = position->previous()) {
            REQUIRE(backward.size() < expected.size());
            backward.insert(backward.begin(), tree_key(position->value));
        }

        CHECK(forward  == expected);
        CHECK(backward == expected);
        CHECK(head->next() == head);
    }
}} // tests::vc8_support

namespace doctest {
    template<>
    struct StringMaker<tests::vc8_support::value_event_t> {
        static String convert(const tests::vc8_support::value_event_t &event) {
            const char* kinds[] { "constructed", "copied", "moved", "copy_assigned", "move_assigned", "destroyed" };

            return String("{") + kinds[event.kind] + ", " + toString(event.value) + "}";
        }
    };
}
