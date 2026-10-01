#pragma once

#include "treyarch/shared/hash/string_hash.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace chuck { namespace vm {
    class script_executable;

    // one per loaded (filename, key_prefix); loading it again only bumps ref_cnt (sub_A1BC60)
    class script_executable_entry {

    public:
        script_executable* exec;
        u32                ref_cnt;
        void*              user_data;  // the loader's resource context; handed to every notification
        string_hash        filename;
        string_hash        key_prefix;
    };

    struct script_executable_entry_set_node {
        script_executable_entry_set_node* left;
        script_executable_entry_set_node* parent;
        script_executable_entry_set_node* right;
        script_executable_entry*          value;
        u8                                color;
        u8                                is_nil;
        u8                                pad[2];

        // the in-order walk retail inlines (e.g. sub_A1AE50); hold the manager's exec_set_lock
        script_executable_entry_set_node* next() {
            if (is_nil)
                return this;

            if (!right->is_nil) {
                script_executable_entry_set_node* node = right;

                while (!node->left->is_nil)
                    node = node->left;

                return node;
            }

            script_executable_entry_set_node* node   = this;
            script_executable_entry_set_node* parent = this->parent;

            while (!parent->is_nil && node == parent->right) {
                node   = parent;
                parent = parent->parent;
            }

            return parent;
        }
    };

    // dinkumware set<script_executable_entry*> ordered by (filename, key_prefix); find sub_A1B440, insert sub_A1BB00
    struct script_executable_entry_set {
        u32                               unk_00;
        script_executable_entry_set_node* head;
        u32                               size;

        script_executable_entry_set_node* begin() const {
            return head->left;
        }

        script_executable_entry_set_node* end() const {
            return head;
        }
    };

    ASSERT_SIZEOF  (script_executable_entry,             0x14);
    ASSERT_OFFSETOF(script_executable_entry, exec,       0x00);
    ASSERT_OFFSETOF(script_executable_entry, ref_cnt,    0x04);
    ASSERT_OFFSETOF(script_executable_entry, user_data,  0x08);
    ASSERT_OFFSETOF(script_executable_entry, filename,   0x0C);
    ASSERT_OFFSETOF(script_executable_entry, key_prefix, 0x10);

    ASSERT_SIZEOF  (script_executable_entry_set_node,         0x14);
    ASSERT_OFFSETOF(script_executable_entry_set_node, value,  0x0C);
    ASSERT_OFFSETOF(script_executable_entry_set_node, is_nil, 0x11);

    ASSERT_SIZEOF  (script_executable_entry_set,       0x0C);
    ASSERT_OFFSETOF(script_executable_entry_set, head, 0x04);
    ASSERT_OFFSETOF(script_executable_entry_set, size, 0x08);
}}} // treyarch::chuck::vm
