#pragma once

#include "vc8/list.hh"
#include "vc8/map.hh"
#include "vc8/vector.hh"
#include "game/animation/scene_anim.hh"
#include "game/wds/ai/wds_ai_manager.hh"
#include "game/wds/camera/wds_camera_manager.hh"
#include "game/wds/render/wds_render_manager.hh"
#include "game/wds/script/wds_script_manager.hh"
#include "game/wds/time/wds_time_manager.hh"
#include "game/wds/entity/wds_entity_manager.hh"
#include "treyarch/mash/string.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class entity; // impl
    class animation_controller; // impl?
    class terrain; // impl

    class world_dynamics_system {

    public:
        using scene_anim_map_t    = vc8::map<mash::string,        scene_anim*>;
        using sa_handle_obj_map_t = vc8::map<scene_anim_handle_t, scene_anim*>;

        scene_anim_handle_t         last_snm_handle;
        scene_anim_map_t            scene_anim_map;
        sa_handle_obj_map_t         playing_scene_anims;
        u32                         unk_01c;
        f32*                        gv_busy_teaching;
        vc8::vector
            <animation_controller*> anim_ctrls;
        u32                         unk_034;
        bool                        unk_038;
        bool                        unk_039;
        bool                        saved_hero_enabled; // player interface enabled, carried over to the next hero
        u8                          pad_03b[0x01];
        wds_ai_manager              ai_mgr;
        wds_camera_manager          camera_mgr;
        wds_entity_manager          ent_mgr;
        wds_render_manager          render_mgr;
        wds_script_manager          script_mgr;
        wds_time_manager            time_mgr;
        terrain*                    the_terrain;
        mash::string                unk_114;
        entity*                     hero_ptr;
        camera_handle               chase_cam_ptr;
        vc8::list<void*>            unk_128;
        u32                         unk_134;
        u8                          reserved_138[0x01];
        bool                        unk_139;
        u8                          pad_13a[0x02];
        i32                         unk_13c;

        world_dynamics_system();

        entity* get_hero_ptr() const {
            return hero_ptr;
        }

        camera_handle get_chase_cam_ptr() const {
            return chase_cam_ptr;
        }

        // sub_95C710
        camera_handle get_current_camera() const {
            return chase_cam_ptr ? chase_cam_ptr : camera_mgr.marky_camera;
        }

        void frame_advance(f32 time_inc);
        void misc_entity_updates(f32 time_inc);

        void                load_scene_anim(const mash::string &filename);
        scene_anim_handle_t play_scene_anim(scene_anim* scene_anim_ptr, u16 flags, f32 offset);
        scene_anim_handle_t play_scene_anim(const mash::string &filename, u16 flags, f32 offset);
        void                kill_scene_anim(scene_anim_handle_t handle, bool b_reverse, bool finish);
        void                remove_scene_anim(const mash::string &filename);
        void                set_scene_anim_paused(scene_anim_handle_t handle, bool paused);
        bool                is_scene_anim_finished(scene_anim_handle_t handle);
        f32                 get_scene_anim_time(scene_anim_handle_t handle);
        f32                 scene_anim_time_remaining(scene_anim_handle_t handle);

        void update_scene_anims(f32 t);
        void scene_anim_dies(scene_anim_handle_t handle);
    };

    // node strides the retail constructor (sub_97A860) implies through is_nil
    ASSERT_OFFSETOF(world_dynamics_system::scene_anim_map_t::node,    is_nil, 0x1D);
    ASSERT_OFFSETOF(world_dynamics_system::sa_handle_obj_map_t::node, is_nil, 0x15);

    ASSERT_SIZEOF  (world_dynamics_system,                      0x140);
    ASSERT_OFFSETOF(world_dynamics_system, last_snm_handle,     0x000);
    ASSERT_OFFSETOF(world_dynamics_system, scene_anim_map,      0x004);
    ASSERT_OFFSETOF(world_dynamics_system, playing_scene_anims, 0x010);
    ASSERT_OFFSETOF(world_dynamics_system, gv_busy_teaching,    0x020);
    ASSERT_OFFSETOF(world_dynamics_system, anim_ctrls,          0x024);
    ASSERT_OFFSETOF(world_dynamics_system, ai_mgr,              0x03C);
    ASSERT_OFFSETOF(world_dynamics_system, camera_mgr,          0x050);
    ASSERT_OFFSETOF(world_dynamics_system, ent_mgr,             0x088);
    ASSERT_OFFSETOF(world_dynamics_system, render_mgr,          0x0C0);
    ASSERT_OFFSETOF(world_dynamics_system, script_mgr,          0x0C8);
    ASSERT_OFFSETOF(world_dynamics_system, time_mgr,            0x0D4);
    ASSERT_OFFSETOF(world_dynamics_system, the_terrain,         0x110);
    ASSERT_OFFSETOF(world_dynamics_system, unk_114,             0x114);
    ASSERT_OFFSETOF(world_dynamics_system, hero_ptr,            0x120);
    ASSERT_OFFSETOF(world_dynamics_system, chase_cam_ptr,       0x124);
    ASSERT_OFFSETOF(world_dynamics_system, unk_128,             0x128);
    ASSERT_OFFSETOF(world_dynamics_system, unk_134,             0x134);
    ASSERT_OFFSETOF(world_dynamics_system, unk_139,             0x139);
    ASSERT_OFFSETOF(world_dynamics_system, unk_13c,             0x13C);
} // treyarch
