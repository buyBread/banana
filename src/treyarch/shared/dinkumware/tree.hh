#pragma once

#include <new>
#include <stdexcept>
#include <utility>

#include "treyarch/shared/memory/heap.hh"
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

        tree_node* next() noexcept { // in-order successor; the end (head) node maps to itself
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

        // sub_A76160
        tree_node* previous() noexcept { // in-order predecessor; the end (head) node maps to the rightmost node
            if (is_nil)
                return right;

            if (!left->is_nil) {
                tree_node* node = left;

                while (!node->right->is_nil)
                    node = node->right;

                return node;
            }

            tree_node* node  = this;
            tree_node* above = parent;

            while (!above->is_nil && node == above->left) {
                node  = above;
                above = above->parent;
            }

            if (!node->is_nil)
                node = above;

            return node;
        }

        // sub_781850
        static tree_node* minimum(tree_node* node) noexcept {
            while (!node->left->is_nil)
                node = node->left;

            return node;
        }

        // sub_428E70
        static tree_node* maximum(tree_node* node) noexcept {
            while (!node->right->is_nil)
                node = node->right;

            return node;
        }
    };

    /* VC8 _Tree. the head is the only nil node and doubles as end():
       head->parent is the root, head->left the leftmost node, head->right the rightmost.
       nodes come from the engine heap. */
    template<typename K, typename T, typename key_of_t, typename compare_t>
    class tree {

    public:
        using node = tree_node<T>;

    protected:
        compare_t compare;
        node*     head_node;
        u32       count;

    public:
        // inlined @ sub_A1BF80
        tree() : head_node(buy_head_node()),
                 count(0) {

            head_node->is_nil = true;
            head_node->parent = head_node;
            head_node->left   = head_node;
            head_node->right  = head_node;
        }

        // inlined @ sub_A20D60
        ~tree() {
            erase(begin(), end());

            memory::heap::free(head_node);

            head_node = nullptr;
            count     = 0;
        }

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

        // sub_A1BB00
        std::pair<node*, bool> insert(const T &value) { // unique insert; an equal key already present is returned with `false`
            node* try_node   = head_node->parent;
            node* where_node = head_node;
            bool  add_left   = true;

            while (!try_node->is_nil) {
                where_node = try_node;
                add_left   = compare(key_of_t()(value), key_of_t()(try_node->value));
                try_node   = add_left ? try_node->left : try_node->right;
            }

            node* where = where_node;

            if (add_left) {
                if (where == begin())
                    return { insert_at(true, where_node, value), true };

                where = where->previous();
            }

            if (compare(key_of_t()(where->value), key_of_t()(value)))
                return { insert_at(add_left, where_node, value), true };

            return { where, false };
        }

        // sub_A1B180
        node* erase(node* where) { // returns the node after the erased one
            if (where->is_nil)
                throw std::out_of_range("invalid map/set<T> iterator");

            node* erased_node = where;
            node* next_node   = where->next();
            node* position    = erased_node;
            node* fix_node;
            node* fix_node_parent;

            if (position->left->is_nil)
                fix_node = position->right;
            else if (position->right->is_nil)
                fix_node = position->left;
            else {
                position = next_node;
                fix_node = position->right;
            }

            if (position == erased_node) {
                fix_node_parent = erased_node->parent;

                if (!fix_node->is_nil)
                    fix_node->parent = fix_node_parent;

                if (head_node->parent == erased_node)
                    head_node->parent = fix_node;
                else if (fix_node_parent->left == erased_node)
                    fix_node_parent->left = fix_node;
                else
                    fix_node_parent->right = fix_node;

                if (head_node->left == erased_node)
                    head_node->left = fix_node->is_nil ? fix_node_parent : node::minimum(fix_node);

                if (head_node->right == erased_node)
                    head_node->right = fix_node->is_nil ? fix_node_parent : node::maximum(fix_node);
            } else {
                erased_node->left->parent = position;
                position->left            = erased_node->left;

                if (position == erased_node->right)
                    fix_node_parent = position;
                else {
                    fix_node_parent = position->parent;

                    if (!fix_node->is_nil)
                        fix_node->parent = fix_node_parent;

                    fix_node_parent->left      = fix_node;
                    position->right            = erased_node->right;
                    erased_node->right->parent = position;
                }

                if (head_node->parent == erased_node)
                    head_node->parent = position;
                else if (erased_node->parent->left == erased_node)
                    erased_node->parent->left = position;
                else
                    erased_node->parent->right = position;

                position->parent = erased_node->parent;

                e_tree_color color = position->color;
                position->color    = erased_node->color;
                erased_node->color = color;
            }

            if (erased_node->color == tree_color_black) {
                for (; fix_node != head_node->parent && fix_node->color == tree_color_black;
                       fix_node = fix_node_parent, fix_node_parent = fix_node->parent) {

                    if (fix_node == fix_node_parent->left) {
                        position = fix_node_parent->right;

                        if (position->color == tree_color_red) {
                            position->color        = tree_color_black;
                            fix_node_parent->color = tree_color_red;
                            rotate_left(fix_node_parent);
                            position = fix_node_parent->right;
                        }

                        if (position->is_nil)
                            fix_node = fix_node_parent;
                        else if (position->left->color == tree_color_black && position->right->color == tree_color_black) {
                            position->color = tree_color_red;
                            fix_node        = fix_node_parent;
                        } else {
                            if (position->right->color == tree_color_black) {
                                position->left->color = tree_color_black;
                                position->color       = tree_color_red;
                                rotate_right(position);
                                position = fix_node_parent->right;
                            }

                            position->color        = fix_node_parent->color;
                            fix_node_parent->color = tree_color_black;
                            position->right->color = tree_color_black;
                            rotate_left(fix_node_parent);

                            break;
                        }
                    } else {
                        position = fix_node_parent->left;

                        if (position->color == tree_color_red) {
                            position->color        = tree_color_black;
                            fix_node_parent->color = tree_color_red;
                            rotate_right(fix_node_parent);
                            position = fix_node_parent->left;
                        }

                        if (position->is_nil)
                            fix_node = fix_node_parent;
                        else if (position->right->color == tree_color_black && position->left->color == tree_color_black) {
                            position->color = tree_color_red;
                            fix_node        = fix_node_parent;
                        } else {
                            if (position->left->color == tree_color_black) {
                                position->right->color = tree_color_black;
                                position->color        = tree_color_red;
                                rotate_left(position);
                                position = fix_node_parent->left;
                            }

                            position->color        = fix_node_parent->color;
                            fix_node_parent->color = tree_color_black;
                            position->left->color  = tree_color_black;
                            rotate_right(fix_node_parent);

                            break;
                        }
                    }
                }

                fix_node->color = tree_color_black;
            }

            erased_node->value.~T();
            memory::heap::free(erased_node);

            if (count > 0)
                --count;

            return next_node;
        }

        // sub_A20CA0
        node* erase(node* first, node* last) { // the whole tree is dropped without rebalancing
            if (first == begin() && last == end()) {
                erase_subtree(head_node->parent);

                head_node->parent = head_node;
                count             = 0;
                head_node->left   = head_node;
                head_node->right  = head_node;

                return begin();
            }

            while (first != last) {
                node* where = first;

                first = first->next();

                erase(where);
            }

            return first;
        }

    private:
        // sub_42A1C0
        static node* buy_head_node() {
            node* result = (node*)memory::heap::allocate(sizeof(node));

            result->left   = nullptr;
            result->parent = nullptr;
            result->right  = nullptr;
            result->color  = tree_color_black;
            result->is_nil = false;

            return result;
        }

        // sub_937860
        void erase_subtree(node* root) {
            for (node* position = root; !position->is_nil; root = position) {
                erase_subtree(position->right);

                position = position->left;

                root->value.~T();
                memory::heap::free(root);
            }
        }

        // sub_79B4D0
        node* buy_node(node* left, node* parent, node* right, const T &value, e_tree_color color) {
            node* result = (node*)memory::heap::allocate(sizeof(node));

            if (result) {
                result->left   = left;
                result->parent = parent;
                result->right  = right;

                new (&result->value) T(value);

                result->color  = color;
                result->is_nil = false;
            }

            return result;
        }

        // sub_A1B4B0
        node* insert_at(bool add_left, node* where_node, const T &value) {
            if (count >= 0x3FFFFFFE)
                throw std::length_error("map/set<T> too long");

            node* new_node = buy_node(head_node, where_node, head_node, value, tree_color_red);

            ++count;

            if (where_node == head_node) {
                head_node->parent = new_node;
                head_node->left   = new_node;
                head_node->right  = new_node;
            } else if (add_left) {
                where_node->left = new_node;

                if (where_node == head_node->left)
                    head_node->left = new_node;
            } else {
                where_node->right = new_node;

                if (where_node == head_node->right)
                    head_node->right = new_node;
            }

            for (node* position = new_node; position->parent->color == tree_color_red; ) {
                node* grandparent = position->parent->parent;

                if (position->parent == grandparent->left) {
                    node* uncle = grandparent->right;

                    if (uncle->color == tree_color_red) {
                        position->parent->color = tree_color_black;
                        uncle->color            = tree_color_black;
                        grandparent->color      = tree_color_red;
                        position                = grandparent;
                    } else {
                        if (position == position->parent->right) {
                            position = position->parent;
                            rotate_left(position);
                        }

                        position->parent->color         = tree_color_black;
                        position->parent->parent->color = tree_color_red;
                        rotate_right(position->parent->parent);
                    }
                } else {
                    node* uncle = grandparent->left;

                    if (uncle->color == tree_color_red) {
                        position->parent->color = tree_color_black;
                        uncle->color            = tree_color_black;
                        grandparent->color      = tree_color_red;
                        position                = grandparent;
                    } else {
                        if (position == position->parent->left) {
                            position = position->parent;
                            rotate_right(position);
                        }

                        position->parent->color         = tree_color_black;
                        position->parent->parent->color = tree_color_red;
                        rotate_left(position->parent->parent);
                    }
                }
            }

            head_node->parent->color = tree_color_black;

            return new_node;
        }

        // sub_428E20
        void rotate_left(node* where_node) noexcept {
            node* position = where_node->right;

            where_node->right = position->left;

            if (!position->left->is_nil)
                position->left->parent = where_node;

            position->parent = where_node->parent;

            if (where_node == head_node->parent)
                head_node->parent = position;
            else if (where_node == where_node->parent->left)
                where_node->parent->left = position;
            else
                where_node->parent->right = position;

            position->left     = where_node;
            where_node->parent = position;
        }

        // sub_781870
        void rotate_right(node* where_node) noexcept {
            node* position = where_node->left;

            where_node->left = position->right;

            if (!position->right->is_nil)
                position->right->parent = where_node;

            position->parent = where_node->parent;

            if (where_node == head_node->parent)
                head_node->parent = position;
            else if (where_node == where_node->parent->right)
                where_node->parent->right = position;
            else
                where_node->parent->left = position;

            position->right    = where_node;
            where_node->parent = position;
        }
    };

    ASSERT_SIZEOF  (tree_node<void*>,         0x14);
    ASSERT_OFFSETOF(tree_node<void*>, value,  0x0C);
    ASSERT_OFFSETOF(tree_node<void*>, color,  0x10);
    ASSERT_OFFSETOF(tree_node<void*>, is_nil, 0x11);
}} // treyarch::dinkumware
