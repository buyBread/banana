#pragma once

#include "treyarch/chuck/vm/script_executable.hh"
#include "util/types.hh"

namespace treyarch { namespace chuck { namespace vm {
    struct script_executable_entry {
        script_executable* executable;
    };

    // must engine lock for traversal
    struct script_executable_tree_node {
        script_executable_tree_node* left;
        script_executable_tree_node* parent;
        script_executable_tree_node* right;
            
        script_executable_entry* entry;

        u8 color;
        u8 is_nil;
        u8 pad[2];

        script_executable* get() const {
            return entry->executable;
        }

        script_executable_tree_node* successor() {
            if (is_nil)
                return this;

            if (!right->is_nil) {
                script_executable_tree_node* node = right;

                while (!node->left->is_nil)
                    node = node->left;

                return node;
            }

            script_executable_tree_node* node = this;
            script_executable_tree_node* next = parent;

            while (!next->is_nil && node == next->right) {
                node = next;
                next = next->parent;
            }

            return next;
        }
    };

    struct script_executable_tree {
        u32                          unk_00; // actual idk? lost notes?
        script_executable_tree_node* head;
        u32                          count;

        script_executable_tree_node* begin() const {
            return head->left;
        }

        script_executable_tree_node* end() const {
            return head;
        }
    };
}}} // treyarch::chuck::vm