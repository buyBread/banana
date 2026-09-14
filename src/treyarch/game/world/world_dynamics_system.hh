#pragma once

#include "treyarch/game/camera/wds_camera_manager.hh"
#include "treyarch/game/render/wds_render_manager.hh"
#include "treyarch/game/time/wds_time_manager.hh"
#include "treyarch/game/world/wds_entity_manager.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class entity;
    class terrain;

    class world_dynamics_system {

    public:
        u8                  reserved_000[0x50]; // wds_ai_manager, wds_anim_manager? todo: check path_graph size
        wds_camera_manager  camera_mgr;
        wds_entity_manager  ent_mgr;
        wds_render_manager  render_mgr;
        u8                  reserved_0c8[0x0C]; // wds_script_manager? did it get removed?
        wds_time_manager    time_mgr;
        terrain*            the_terrain;
        u8                  reserved_114[0x0C];
        entity*             hero_ptr;
        camera_handle       chase_cam_ptr;
        u8                  reserved_128[0x18];

        entity* get_hero_ptr() const {
            return hero_ptr;
        }

        camera_handle get_chase_cam_ptr() const {
            return chase_cam_ptr;
        }
    };

    ASSERT_SIZEOF  (world_dynamics_system,                0x140);
    ASSERT_OFFSETOF(world_dynamics_system, camera_mgr,    0x050);
    ASSERT_OFFSETOF(world_dynamics_system, ent_mgr,       0x088);
    ASSERT_OFFSETOF(world_dynamics_system, render_mgr,    0x0C0);
    ASSERT_OFFSETOF(world_dynamics_system, time_mgr,      0x0D4);
    ASSERT_OFFSETOF(world_dynamics_system, the_terrain,   0x110);
    ASSERT_OFFSETOF(world_dynamics_system, hero_ptr,      0x120);
    ASSERT_OFFSETOF(world_dynamics_system, chase_cam_ptr, 0x124);
} // treyarch
