#pragma once

#include "treyarch/chuck/vm/so_data_block.hh"
#include "treyarch/shared/dinkumware/map.hh"
#include "treyarch/shared/mash/string.hh"
#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace chuck { namespace vm {
    // heap-owned; destroy erases and frees the map, then this
    struct script_var_debug_info {
        dinkumware::map<mash::string, i32>* var_to_offset;
    };

    struct script_var_address_entry {
        u32 name_hash;
        u8* address; // serialized as an offset into script_var_block (game containers store -1 - offset), fixed up by initialize

        void construct_mashed_class() {}
        void destruct_mashed_class() {}
        void unmash(mash::mash_info_struct*, void*, mash::buffer_type) {}
    };

    enum e_script_var_container_flags : u32 {
        script_var_container_flag_from_mash = 0x01, // teardown leaves image-embedded storage alone
        script_var_container_flag_game      = 0x02, // the game container (resource type 10), otherwise shared (11)
        script_var_container_flag_unk_04    = 0x04  // set after caller data is copied into the game block
    };

    class script_var_container {

    public:
        void*                                  self;                  // the resource handler points this at the container itself
        so_data_block                          script_var_block;
        mash::vector<script_var_address_entry> script_var_to_address; // sorted by name_hash
        script_var_debug_info*                 debug_info;
        e_script_var_container_flags           flags;

        // sub_A1BC00
        ~script_var_container() {
            finalize(mash::ALLOCATED);
        }

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);

        // null when the name isn't in this container
        u8* get_address(const char* name) const;

    private:
        void initialize(mash::allocation_scope scope);
        void finalize(mash::allocation_scope scope);
        void destroy();
    };

    ASSERT_SIZEOF  (script_var_debug_info,    0x04);
    ASSERT_SIZEOF  (script_var_address_entry, 0x08);

    ASSERT_SIZEOF  (script_var_container,                        0x30);
    ASSERT_OFFSETOF(script_var_container, self,                  0x00);
    ASSERT_OFFSETOF(script_var_container, script_var_block,      0x04);
    ASSERT_OFFSETOF(script_var_container, script_var_to_address, 0x14);
    ASSERT_OFFSETOF(script_var_container, debug_info,            0x28);
    ASSERT_OFFSETOF(script_var_container, flags,                 0x2C);
}}} // treyarch::chuck::vm
