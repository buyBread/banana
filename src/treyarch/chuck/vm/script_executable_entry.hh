#pragma once

#include "treyarch/shared/dinkumware/set.hh"
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

    // inlined into retail's lower bound (sub_A1A670) and find (sub_A1B440)
    struct script_executable_entry_less {
        bool operator()(const script_executable_entry* a, const script_executable_entry* b) const noexcept {
            if (a->filename == b->filename)
                return a->key_prefix < b->key_prefix;

            return a->filename < b->filename;
        }
    };

    // the manager's exec_set;
    // walk it under exec_set_lock
    using script_executable_entry_set_t = dinkumware::set<script_executable_entry*, script_executable_entry_less>;

    ASSERT_SIZEOF  (script_executable_entry,             0x14);
    ASSERT_OFFSETOF(script_executable_entry, exec,       0x00);
    ASSERT_OFFSETOF(script_executable_entry, ref_cnt,    0x04);
    ASSERT_OFFSETOF(script_executable_entry, user_data,  0x08);
    ASSERT_OFFSETOF(script_executable_entry, filename,   0x0C);
    ASSERT_OFFSETOF(script_executable_entry, key_prefix, 0x10);

    ASSERT_SIZEOF(script_executable_entry_set_t, 0x0C);
}}} // treyarch::chuck::vm
