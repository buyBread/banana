#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace dinkumware {
    enum e_tree_color : u8 {
        tree_color_red   = 0,
        tree_color_black = 1
    };

    template<typename T>
    struct tree_node {
        tree_node*   left;
        tree_node*   parent;
        tree_node*   right;
        T            value;
        e_tree_color color;
        bool         is_nil;

        // in-order successor; the end (head) node maps to itself
        tree_node* next() noexcept {
            if (is_nil)
                return this;

            if (!right->is_nil) {
                tree_node* node = right;

                while (!node->left->is_nil)
                    node = node->left;

                return node;
            }

            tree_node* node  = this;
            tree_node* above = parent;

            while (!above->is_nil && node == above->right) {
                node  = above;
                above = above->parent;
            }

            return above;
        }
    };

    /* VC8 _Tree. the head is the only nil node and doubles as end():
       head->parent is the root, head->left the leftmost node, head->right the rightmost.
       only trees the engine built are read through this; insertion and rebalancing are not implemented. */
    template<typename K, typename T, typename key_of_t, typename compare_t>
    class tree {

    public:
        using node = tree_node<T>;

    protected:
        compare_t compare;
        node*     head_node;
        u32       count;

    public:
        tree() = delete;
        tree(const tree&) = delete;
        tree &operator=(const tree&) = delete;

        node* head()  const noexcept { return head_node; }
        node* begin() const noexcept { return head_node->left; }
        node* end()   const noexcept { return head_node; }

        u32  size()  const noexcept { return count; }
        bool empty() const noexcept { return count == 0; }

        // first node whose key is not less than `key`
        node* lower_bound(const K &key) const {
            node* result   = head_node;
            node* position = head_node->parent;

            while (!position->is_nil) {
                if (compare(key_of_t()(position->value), key))
                    position = position->right;
                else {
                    result   = position;
                    position = position->left;
                }
            }

            return result;
        }

        node* find(const K &key) const {
            node* position = lower_bound(key);

            if (position == head_node || compare(key, key_of_t()(position->value)))
                return head_node;

            return position;
        }
    };

    ASSERT_SIZEOF  (tree_node<void*>,         0x14);
    ASSERT_OFFSETOF(tree_node<void*>, value,  0x0C);
    ASSERT_OFFSETOF(tree_node<void*>, color,  0x10);
    ASSERT_OFFSETOF(tree_node<void*>, is_nil, 0x11);
}} // treyarch::dinkumware
