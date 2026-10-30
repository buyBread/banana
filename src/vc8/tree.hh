#pragma once

#include <new>
#include <stdexcept>
#include <utility>

#include "treyarch/mm/mm.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace vc8 {
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

        // sub_7EB870
        // sub_9220A0
        // sub_95D780
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

        // sub_620600
        // sub_7EB7F0
        // sub_910B40
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

        // sub_588520
        // sub_781850
        // sub_7EB7D0
        // sub_910AC0
        static tree_node* minimum(tree_node* node) noexcept {
            while (!node->left->is_nil)
                node = node->left;

            return node;
        }

        // sub_428E70
        // sub_6970B0
        // sub_7EB7B0
        // sub_910B20
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

        // sub_800C90
        // sub_800D20
        tree(const tree &right) : compare(right.compare),
                                  head_node(buy_head_node()),
                                  count(0) {

            head_node->is_nil = true;
            head_node->parent = head_node;
            head_node->left   = head_node;
            head_node->right  = head_node;

            copy(right);
        }

        // inlined @ sub_A20D60
        ~tree() {
            erase(begin(), end());

            mm::free(head_node);

            head_node = nullptr;
            count     = 0;
        }

        // sub_804DF0
        tree &operator=(const tree &right) {
            if (this != &right) {
                erase(begin(), end());

                compare = right.compare;

                copy(right);
            }

            return *this;
        }

        node* head()  const noexcept { return head_node; }
        node* begin() const noexcept { return head_node->left; }
        node* end()   const noexcept { return head_node; }

        u32  size()     const noexcept { return count; }
        u32  max_size() const noexcept { return 0xFFFFFFFF / sizeof(T); }
        bool empty()    const noexcept { return count == 0; }

        // inlined @ sub_93D010
        void clear() {
            erase(begin(), end());
        }

        // sub_802590
        // sub_921FD0
        node* lower_bound(const K &key) const { // first node whose key is not less than `key`
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

        node* upper_bound(const K &key) const { // first node whose key is greater than `key`
            node* result   = head_node;
            node* position = head_node->parent;

            while (!position->is_nil) {
                if (compare(key, key_of_t()(position->value))) {
                    result   = position;
                    position = position->left;
                } else
                    position = position->right;
            }

            return result;
        }

        // sub_7F9760
        // sub_930810
        std::pair<node*, node*> equal_range(const K &key) const {
            return std::pair<node*, node*>(lower_bound(key), upper_bound(key));
        }

        // sub_6D56D0
        // sub_7F9470
        // sub_7F94E0
        // sub_802CD0
        node* find(const K &key) {
            node* position = lower_bound(key);

            if (position == head_node || compare(key, key_of_t()(position->value)))
                return head_node;

            return position;
        }

        // sub_63CE50
        // sub_802D70
        // inlined @ sub_977170
        // inlined @ sub_979BB0
        node* find(const K &key) const {
            node* position = lower_bound(key);

            if (position == head_node || compare(key, key_of_t()(position->value)))
                return head_node;

            return position;
        }

        // sub_759870
        // sub_759930
        // sub_7FB1B0
        // sub_7FB250
        // sub_802AF0
        // sub_975FD0
        // sub_976090
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

        // sub_7618D0
        // sub_761A40
        // sub_9782F0
        // sub_9784A0
        node* insert(node* where, const T &value) { // unique insert next to `where` when it fits there, else a plain insert
            if (count == 0)
                return insert_at(true, head_node, value);

            if (where == begin()) {
                if (compare(key_of_t()(value), key_of_t()(where->value)))
                    return insert_at(true, where, value);
            } else if (where == end()) {
                if (compare(key_of_t()(head_node->right->value), key_of_t()(value)))
                    return insert_at(false, head_node->right, value);
            } else {
                if (compare(key_of_t()(value), key_of_t()(where->value))) {
                    node* before = where->previous();

                    if (compare(key_of_t()(before->value), key_of_t()(value))) {
                        if (before->right->is_nil)
                            return insert_at(false, before, value);

                        return insert_at(true, where, value);
                    }
                }

                if (compare(key_of_t()(where->value), key_of_t()(value))) {
                    node* after = where->next();

                    if (after == end() || compare(key_of_t()(value), key_of_t()(after->value))) {
                        if (where->right->is_nil)
                            return insert_at(false, where, value);

                        return insert_at(true, after, value);
                    }
                }
            }

            return insert(value).first;
        }

        // sub_6D57A0
        // sub_6D5A60
        // sub_7FB330
        // sub_7FB5F0
        // sub_803AB0
        // sub_8030D0
        // sub_973A40
        // sub_975D10
        // sub_A1B180
        // sub_A20950
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
            mm::free(erased_node);

            if (count > 0)
                --count;

            return next_node;
        }

        // sub_7FF640
        // sub_7FF730
        // sub_8041A0
        // sub_8042E0
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

        // sub_800B00
        // sub_800B60
        u32 erase(const K &key) { // returns how many nodes went
            std::pair<node*, node*> where = equal_range(key);

            u32 erased = 0;
            distance(where.first, where.second, erased);

            erase(where.first, where.second);

            return erased;
        }

    private:
        // sub_7F8BA0
        // sub_7F8C00
        static void distance(node* first, node* last, u32 &result) noexcept {
            for (; first != last; first = first->next())
                ++result;
        }

        // sub_7FFA70
        // sub_7FFAF0
        // sub_804260
        void copy(const tree &right) {
            head_node->parent = copy(right.head_node->parent, head_node);
            count             = right.count;

            if (!head_node->parent->is_nil) {
                head_node->left  = node::minimum(head_node->parent);
                head_node->right = node::maximum(head_node->parent);
            } else {
                head_node->left  = head_node;
                head_node->right = head_node;
            }
        }

        // sub_7FE700
        // sub_7FE7B0
        // sub_803D70
        node* copy(node* root, node* where_node) { // copies the subtree under `root`, hanging it off `where_node`
            node* new_root = head_node;

            if (!root->is_nil) {
                node* new_node = buy_node(head_node, where_node, head_node, root->value, root->color);

                if (new_root->is_nil)
                    new_root = new_node;

                new_node->left  = copy(root->left,  new_node);
                new_node->right = copy(root->right, new_node);
            }

            return new_root;
        }

        // sub_42A1C0
        // sub_9285E0
        // sub_9288A0
        // sub_928C40
        static node* buy_head_node() {
            node* result = (node*)mm::allocate(sizeof(node));

            result->left   = nullptr;
            result->parent = nullptr;
            result->right  = nullptr;
            result->color  = tree_color_black;
            result->is_nil = false;

            return result;
        }

        // sub_79B580
        // sub_7FB8B0
        // sub_803390
        // sub_803A70
        // sub_937860
        void erase_subtree(node* root) {
            for (node* position = root; !position->is_nil; root = position) {
                erase_subtree(position->right);

                position = position->left;

                root->value.~T();
                mm::free(root);
            }
        }

        // sub_58F9C0
        // sub_79B4D0
        // sub_7F85B0
        // sub_802690
        // sub_9716F0
        node* buy_node(node* left, node* parent, node* right, const T &value, e_tree_color color) {
            node* result = (node*)mm::allocate(sizeof(node));

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

        // sub_7549C0
        // sub_754BC0
        // sub_7F9560
        // sub_7F97C0
        // sub_8028C0
        // sub_973D00
        // sub_973FC0
        // sub_A1B4B0
        node* insert_at(bool add_left, node* where_node, const T &value) {
            if (max_size() - 1 <= count)
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
        // sub_6B0CF0
        // sub_7F2AF0
        // sub_9220F0
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

        // sub_743F20
        // sub_781870
        // sub_7F2A90
        // sub_7F2B40
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
}} // treyarch::vc8
