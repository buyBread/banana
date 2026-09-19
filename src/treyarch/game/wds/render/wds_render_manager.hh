#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class world_dynamics_system;
    class entity;
    class light_source;
    struct render_data;
    struct render_region_info;

    namespace ngl {
        struct scene;
    } // ngl

    class wds_render_manager {

    public:
        u8 reserved_000[0x08];

        void render();
        void render_depth_shadows();

    private:
        world_dynamics_system* get_world();

        void ensure_resources();
        void request_environment_texture();
        bool is_special_case_level();

        void publish_scene_parameter(ngl::scene* scene);
        void configure_game_scene();
        f32  calculate_scene_near_plane(world_dynamics_system* world);
        void render_z_pre_pass(ngl::scene* game_scene, f32 near_plane);
        void submit_world(world_dynamics_system* world);
        void submit_blocked_world(world_dynamics_system* world);
        void submit_light(render_region_info* entry,
                          light_source* source,
                          entity* selected_render_entity,
                          i32 worker_index);
        void submit_region_renderables(render_data* frame_data);
    };

    ASSERT_SIZEOF(wds_render_manager, 0x08);
} // treyarch
