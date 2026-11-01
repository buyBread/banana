#pragma once

#include "game/arch_base_vhandle.hh"
#include "treyarch/mash/types.hh"
#include "game/math/algebra/vector3d.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class entity_base;

    namespace chuck { namespace vm {
        class script_instance;
    }}

    // SM3 player_web_shooters, the hero's link to its SM_web_shooter script; every call does nothing until init finds the function
    class player_web_shooters {

    public:
        u32                         id;                          // only names the script instance; nothing writes it
        arch_base_vhandle           owner;
        chuck::vm::script_instance* web_shooter_script_inst;     // finalize leaves it behind
        i32                         frame_advance_func_id;
        i32                         set_default_web_kill_time_func_id;
        i32                         kill_all_webs_func_id;
        i32                         kill_web_func_id;
        i32                         fire_web_func_id;
        i32                         fire_web_at_ground_func_id;
        i32                         fire_web_at_ceiling_func_id;

        void destruct_mashed_class();
        void finalize(mash::allocation_scope scope);

        void init(entity_base* _owner, const char* script_obj, const char* script_exe);
        void frame_advance(f32 delta_t);
        void kill_all_webs(f32 time = -1.0f);
        void kill_web(u32 id, f32 time = -1.0f);

        u32 fire_web(      bool               right_hand, // the web's id, 0 when the call never ran
                           arch_base_vhandle  target,
                     const vector3d          &target_pos,
                           f32                attach_len,
                           f32                speed,
                           f32                acc,
                           f32                curly_rad,
                           bool               sound);
    };

    ASSERT_SIZEOF  (player_web_shooters,                              0x28);
    ASSERT_OFFSETOF(player_web_shooters, web_shooter_script_inst,     0x08);
    ASSERT_OFFSETOF(player_web_shooters, frame_advance_func_id,       0x0C);
    ASSERT_OFFSETOF(player_web_shooters, fire_web_at_ceiling_func_id, 0x24);
} // treyarch
