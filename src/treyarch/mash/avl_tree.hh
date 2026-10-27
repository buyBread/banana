#pragma once

#include <algorithm>

#include "treyarch/mash/container_base.hh"
#include "treyarch/mash/mash_info.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mm/heap.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

// https://en.wikipedia.org/wiki/AVL_tree

namespace treyarch { namespace mash {
    // SM3's mAvlNode
    template<typename T>
    class avl_node {

    public:
        T*        element;
        avl_node* left;   // smaller elements
        avl_node* right;  // larger elements
        avl_node* parent; // root
        i8        height;
        u8        pad_011[0x03];

        void construct_mashed_class() {}

        void destruct_mashed_class() {
            if (element) {
                element->destruct_mashed_class();
                element = nullptr;
            }
        }

        void unmash(mash_info_struct* mash_info, void*, buffer_type buffer) {
            u8 class_mashed;
            mash_info->read_from_buffer(SHARED_BUFFER, class_mashed);

            if (class_mashed == member_class_mashed)
                mash_info->unmash_class(element, this, buffer);
            else
                element = nullptr;
        }
    };

    // SM3's mAvlTree; ordered by T's `operator<`.
    // an element equal to one already in it isn't added.
    template<typename T>
    class avl_tree : public container_base {

    public:
        using node_type = avl_node<T>;

        node_type* root_node;
        bool       destruct_contents;
        u8         pad_00d[0x03];

        struct iterator {
            node_type* node;

            // sub_A73870
            void iterate() {
                if (node->right) {
                    node = node->right;

                    while (node->left)
                        node = node->left;

                    return;
                }

                while (node->parent && node == node->parent->right)
                    node = node->parent;

                node = node->parent;
            }
        };

        ~avl_tree() {
            finalize(ALLOCATED);
        }

        // sub_805450
        iterator begin() const {
            node_type* node = root_node;

            if (node) {
                while (node->left)
                    node = node->left;
            }

            return { node };
        }

        void construct_mashed_class() {
            container_base::construct_mashed_class();

            initialize(FROM_MASH);
        }

        // sub_808510
        // sub_A6F4B0
        void initialize(allocation_scope scope) {
            if (scope == ALLOCATED) {
                root_node         = nullptr;
                size              = 0;
                destruct_contents = true;

                return;
            }

            if (!root_node)
                return;

            for (iterator it = begin(); it.node; it.iterate())
                mash_info_struct::construct_class(it.node->element);
        }

        void destruct_mashed_class() {
            clear();
            container_base::destruct_mashed_class();
        }

        void unmash(mash_info_struct* mash_info,
                    void*             containing_class_ptr,
                    buffer_type       buffer) {

            container_base::unmash(mash_info, containing_class_ptr, buffer);
            custom_unmash(mash_info, containing_class_ptr, buffer);
        }

        // sub_808E20
        // sub_A6F520
        void custom_unmash(mash_info_struct* mash_info,
                           void*,
                           buffer_type       buffer) {

            u32 real_size = size;

            root_node = nullptr;
            size      = 0;

            for (u32 index = 0; index < real_size; ++index) {
                node_type* node = nullptr;

                mash_info->unmash_class(node, this, buffer);

                add_helper(node, root_node, nullptr);
            }

            container_base::custom_unmash_base(mash_info, buffer);
        }

        void clear() {
            dump(root_node);
        }

    private:
        void finalize(allocation_scope) {
            clear();
        }

        // sub_805480
        static i32 compare(const node_type* left_node, const node_type* right_node) {
            if (*right_node->element < *left_node->element)
                return 1;

            if (*left_node->element < *right_node->element)
                return -1;

            return 0;
        }

        static i8 node_height(const node_type* node) {
            return node ? node->height : -1;
        }

        // sub_A6EED0
        static void calc_height(node_type* node) {
            node->height = (i8)(1 + std::max(node_height(node->left), node_height(node->right)));
        }
    
        // rotate left / right as in promote child left / right;
        // these are treyarch's names, not mine.
        // it's not like i understand the math either way, algorithms are annoying.

        // sub_A73950
        static void single_rotate_left(node_type* &k2) {
            node_type* k1            = k2->left;
            node_type* parent_of_top = k2->parent;

            k2->left = k1->right;

            if (k2->left)
                k2->left->parent = k2;

            k1->right = k2;

            if (k1->right)
                k1->right->parent = k1;

            k2->height = (i8)(std::max(node_height(k2->left), node_height(k2->right)) + 1);
            k1->height = (i8)(std::max(node_height(k1->left), k2->height) + 1);

            k2         = k1;
            k2->parent = parent_of_top;
        }

        // sub_806EF0
        static void double_rotate_left(node_type* &k3) {
            single_rotate_right(k3->left);
            single_rotate_left(k3);
        }

        // sub_805C20
        static void single_rotate_right(node_type* &k2) {
            node_type* k1            = k2->right;
            node_type* parent_of_top = k2->parent;

            k2->right = k1->left;

            if (k1->left)
                k1->left->parent = k2;

            k1->left = k2;

            if (k2)
                k2->parent = k1;

            k2->height = (i8)(std::max(node_height(k2->left), node_height(k2->right)) + 1);
            k1->height = (i8)(std::max(node_height(k1->right), k2->height) + 1);

            k2         = k1;
            k2->parent = parent_of_top;
        }

        // sub_A6EEB0
        static void double_rotate_right(node_type* &k3) {
            single_rotate_left(k3->right);
            single_rotate_right(k3);
        }

        // sub_807170
        // sub_A6F0D0
        i32 add_helper(node_type* node, node_type* &top, node_type* parent) {
            if (!top) {
                node->parent = parent;
                top          = node;

                ++size;

                return 1;
            }

            i32 comp = compare(node, top);

            if (comp < 0) {
                comp = add_helper(node, top->left, top);

                if (node_height(top->left) - node_height(top->right) == 2) {
                    if (compare(node, top->left) < 0)
                        single_rotate_left(top);
                    else
                        double_rotate_left(top);
                } else
                    calc_height(top);
            } else if (comp > 0) {
                comp = add_helper(node, top->right, top);

                if (node_height(top->right) - node_height(top->left) == 2) {
                    if (compare(node, top->right) > 0)
                        single_rotate_right(top);
                    else
                        double_rotate_right(top);
                } else
                    calc_height(top);
            }

            return comp;
        }

        // sub_8078B0
        // sub_A6F250
        void destroy_element(node_type* &node) {
            if (destruct_contents) {
                T* element = node->element;

                if (is_pointer_in_mash_image(element))
                    element->destruct_mashed_class();
                else if (element) {
                    element->~T();
                    memory::heap::free(element);
                }
            }

            node->element = nullptr;

            if (is_pointer_in_mash_image(node))
                node->destruct_mashed_class();
            else if (node)
                memory::heap::free(node);

            node = nullptr;
        }

        // sub_807BC0
        // sub_A6F3B0
        void dump(node_type* &node) {
            if (!node)
                return;

            dump(node->left);
            dump(node->right);
            destroy_element(node);

            --size;
        }
    };

    ASSERT_SIZEOF  (avl_node<void>,         0x14);
    ASSERT_OFFSETOF(avl_node<void>, height, 0x10);

    ASSERT_SIZEOF  (avl_tree<void>,                    0x10);
    ASSERT_OFFSETOF(avl_tree<void>, root_node,         0x08);
    ASSERT_OFFSETOF(avl_tree<void>, destruct_contents, 0x0C);
}} // treyarch::mash
