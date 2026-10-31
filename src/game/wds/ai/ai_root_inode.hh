#pragma once

#include "game/wds/ai/ai_flag_store.hh"
#include "game/wds/ai/ai_simple_target.hh"
#include "game/wds/ai/info_node.hh"
#include "game/wds/ai/param_block.hh"
#include "game/math/algebra/vector3d.hh"
#include "treyarch/mash/string_hash.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class arch_base;
    class default_values;

    // ai_basic_inode's and ai_fd_inode's base
    class ai_root_inode : public info_node {

    public:
        default_values* generic_actor_defaults; // looked up once
        u32             unk_034;
        param_block*    var_block;              // vars live here, or in my_param_block without one
        param_block*    initial_vars;           // what reset copies into var_block
        u8              reserved_040[0x20];
        ai_flag_store*  flags;

        f32  get_var_float(const string_hash* key, bool* missing);
        void set_var_float(const string_hash* key, f32 value);

        void set_flag(i32 flag, bool value);
        bool get_flag(i32 flag);
        bool is_inside_car();
    };

    // a fight group's
    class ai_fd_inode : public ai_root_inode {

    public:
        enum { target_count = 5 };

        u8                reserved_068[0x30];
        ai_simple_target* targets[target_count];
        u8                reserved_0ac[0x04];

        void                 clear_target(i32 index);
        e_simple_target_type get_target_type(i32 index); // drops a none target first
        ai_simple_target*    get_live_target(i32 index); // same

        void set_target_entity(i32 index, arch_base* target);
        void set_target_point (i32 index, vector3d   point);
    };

    ASSERT_OFFSETOF(ai_root_inode, generic_actor_defaults, 0x30);
    ASSERT_OFFSETOF(ai_root_inode, var_block,              0x38);
    ASSERT_OFFSETOF(ai_root_inode, initial_vars,           0x3C);
    ASSERT_OFFSETOF(ai_root_inode, flags,                  0x60);

    ASSERT_SIZEOF  (ai_fd_inode,          0xB0);
    ASSERT_OFFSETOF(ai_fd_inode, targets, 0x98);
} // treyarch
