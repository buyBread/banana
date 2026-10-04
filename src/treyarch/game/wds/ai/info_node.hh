#pragma once

#include "treyarch/game/wds/ai/param_block.hh"
#include "treyarch/shared/mash/virtual_base.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class ai_core;

    // baked into each node by its .BAI; only the indices shipped data uses
    enum e_info_node_type : u32 {
        info_node_type_ai_action_processor     = 0,
        info_node_type_aim_util                = 1,
        info_node_type_aim_mech                = 2,
        info_node_type_cpu_combat              = 3,
        info_node_type_ai_item_holder          = 4,
        info_node_type_wall_sprint             = 6,
        info_node_type_ai_car                  = 7,
        info_node_type_ai_wander               = 8,
        info_node_type_std_puppet              = 9,
        info_node_type_als                     = 10,
        info_node_type_avoidance               = 11,
        info_node_type_biped_layer             = 12,
        info_node_type_unmoving_layer          = 13,
        info_node_type_quad_path               = 14,
        info_node_type_std_carry               = 15,
        info_node_type_ai_civilian             = 17,
        info_node_type_ai_cover                = 18,
        info_node_type_crawl_layer             = 19,
        info_node_type_std_fear                = 20,
        info_node_type_flight_layer            = 21,
        info_node_type_jump_layer              = 22,
        info_node_type_slave                   = 27,
        info_node_type_combat_target           = 30,
        info_node_type_voice_box               = 31,
        info_node_type_ai_weapon               = 32,
        info_node_type_ai_basic                = 36,
        info_node_type_swift_surface_layer     = 37,
        info_node_type_manip_watch             = 38,
        info_node_type_swift_stationary_layer  = 39,
        info_node_type_swift_flight_layer      = 40,
        info_node_type_teleporting_layer       = 42
    };

    class info_node : public mash::mash_virtual_base {

    public:
        u32              unk_004;
        param_block      my_param_block;
        e_info_node_type my_type;
        ai_core*         my_ai_core;

        virtual bool unk_024(u32);
        virtual bool unk_028();
        virtual bool does_need_advance(bool post_pass) const;
        virtual bool unk_030(bool post_pass) const; // asked of a type's first node: advance every node of this type
        virtual void frame_advance(f32 delta_t);
        virtual void post_frame_advance(f32 delta_t);
        virtual bool unk_03c(u32);
        virtual void activate(ai_core* the_ai);
        virtual void deactivate();
        virtual void reset();
        virtual void enter_limbo();
        virtual void exit_limbo();
    };

    ASSERT_SIZEOF  (info_node,                 0x30);
    ASSERT_OFFSETOF(info_node, unk_004,        0x04);
    ASSERT_OFFSETOF(info_node, my_param_block, 0x08);
    ASSERT_OFFSETOF(info_node, my_type,        0x28);
    ASSERT_OFFSETOF(info_node, my_ai_core,     0x2C);
} // treyarch
