#pragma once

#include "treyarch/game/wds/ai/wds_ai_manager.hh"
#include "treyarch/game/wds/camera/wds_camera_manager.hh"
#include "treyarch/game/wds/render/wds_render_manager.hh"
#include "treyarch/game/wds/script/wds_script_manager.hh"
#include "treyarch/game/wds/time/wds_time_manager.hh"
#include "treyarch/game/wds/entity/wds_entity_manager.hh"
#include "treyarch/shared/dinkumware/vector.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class entity; // impl
    class animation_controller; // impl?
    class terrain; // impl

    class world_dynamics_system {

    public:
        u8                          reserved_000[0x20];
        f32*                        gv_busy_teaching;
        dinkumware::vector
            <animation_controller*> anim_ctrls;
        u8                          reserved_034[0x08];
        wds_ai_manager              ai_mgr;
        wds_camera_manager          camera_mgr;
        wds_entity_manager          ent_mgr;
        wds_render_manager          render_mgr;
        wds_script_manager          script_mgr;
        wds_time_manager            time_mgr;
        terrain*                    the_terrain;
        u8                          reserved_114[0x0C];
        entity*                     hero_ptr;
        camera_handle               chase_cam_ptr;
        u8                          reserved_128[0x18];

        entity* get_hero_ptr() const {
            return hero_ptr;
        }

        camera_handle get_chase_cam_ptr() const {
            return chase_cam_ptr;
        }
    };

    ASSERT_SIZEOF  (world_dynamics_system,                   0x140);
    ASSERT_OFFSETOF(world_dynamics_system, gv_busy_teaching, 0x020);
    ASSERT_OFFSETOF(world_dynamics_system, anim_ctrls,       0x024);
    ASSERT_OFFSETOF(world_dynamics_system, ai_mgr,           0x03C);
    ASSERT_OFFSETOF(world_dynamics_system, camera_mgr,       0x050);
    ASSERT_OFFSETOF(world_dynamics_system, ent_mgr,          0x088);
    ASSERT_OFFSETOF(world_dynamics_system, render_mgr,       0x0C0);
    ASSERT_OFFSETOF(world_dynamics_system, script_mgr,       0x0C8);
    ASSERT_OFFSETOF(world_dynamics_system, time_mgr,         0x0D4);
    ASSERT_OFFSETOF(world_dynamics_system, the_terrain,      0x110);
    ASSERT_OFFSETOF(world_dynamics_system, hero_ptr,         0x120);
    ASSERT_OFFSETOF(world_dynamics_system, chase_cam_ptr,    0x124);
} // treyarch
