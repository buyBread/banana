#pragma once

#include "treyarch/chuck/vm/so_data_block.hh"
#include "treyarch/shared/dinkumware/map.hh"
#include "treyarch/shared/mash/string.hh"
#include "treyarch/shared/mash/vector.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace chuck { namespace vm {
    // heap-owned; teardown (sub_A20D60) erases and frees the map, then this
    struct script_var_debug_info {
        dinkumware::map<mash::string, i32>* var_to_offset;
    };

    struct script_var_address_entry {
        u32 name_hash;
        u8* address; // serialized as an offset into script_var_block, fixed up by sub_A20810
    };

    enum e_script_var_container_flags : u32 {
        script_var_container_flag_from_mash = 0x01, // teardown leaves image-embedded storage alone
        script_var_container_flag_game      = 0x02, // the game container (resource type 10), otherwise shared (11)
        script_var_container_flag_unk_04    = 0x04  // set after caller data is copied into the game block
    };

    class script_var_container {

    public:
        u32                                    unk_00;                // written by resource-root construction, read by nothing known
        so_data_block                          script_var_block;
        mash::vector<script_var_address_entry> script_var_to_address; // sorted by name_hash; searched by sub_A20780
        script_var_debug_info*                 debug_info;
        e_script_var_container_flags           flags;
    };

    ASSERT_SIZEOF  (script_var_debug_info,    0x04);
    ASSERT_SIZEOF  (script_var_address_entry, 0x08);

    ASSERT_SIZEOF  (script_var_container,                        0x30);
    ASSERT_OFFSETOF(script_var_container, script_var_block,      0x04);
    ASSERT_OFFSETOF(script_var_container, script_var_to_address, 0x14);
    ASSERT_OFFSETOF(script_var_container, debug_info,            0x28);
    ASSERT_OFFSETOF(script_var_container, flags,                 0x2C);
}}} // treyarch::chuck::vm
