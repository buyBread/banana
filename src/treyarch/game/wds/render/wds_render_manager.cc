#include <cmath>
#include <cstddef>
#include <cstring>

#include "retail.hh"
#include "util/types.hh"
#include "treyarch/game/cutscene/cutscene_player.hh"
#include "treyarch/game/frontend/frontend_manager.hh"
#include "treyarch/game/game.hh"
#include "treyarch/game/light/light_source.hh"
#include "treyarch/game/wds/region.hh"
#include "treyarch/game/wds/terrain.hh"
#include "treyarch/game/wds/world_dynamics_system.hh"
#include "treyarch/game/wds/render/references.hh"
#include "treyarch/game/wds/render/wds_render_manager.hh"
#include "treyarch/game/wds/entity/entity.hh"
#include "treyarch/game/wds/camera/references.hh"
#include "treyarch/ngl/d3d9/framebuffer.hh"
#include "treyarch/ngl/fx/references.hh"
#include "treyarch/ngl/lighting/light_data.hh"
#include "treyarch/ngl/lighting/references.hh"
#include "treyarch/ngl/scene/lifecycle.hh"
#include "treyarch/ngl/scene/matrices.hh"
#include "treyarch/ngl/scene/references.hh"
#include "treyarch/ngl/scene/viewport.hh"
#include "treyarch/ngl/shadow/device_resources.hh"
#include "treyarch/shared/math/types/matrix4x4.hh"
#include "treyarch/shared/math/types/vector4.hh"

using namespace treyarch;

namespace treyarch {
    using render_phase_method = void (__thiscall*)(void* self);
    using activate_method     = void (__thiscall*)(void* self, f32 amount);
    using radius_method       = f32  (__thiscall*)(light_source* self);

    void invoke_render_phase(entity* source) {
        void* target = (void*)retail::sub_402CC0(source->render_object_table, 11);

        auto method = (render_phase_method)(*(void***)target)[90];

        method(target);
    }

    f32 get_light_radius(light_source* source) {
        auto method = (radius_method)source->vtable[142];

        return method(source);
    }

    void submit_point_light(light_source* source) {
        ngl::lighting::point_light_data light;

        light.position   = source->my_abs_po->matrix.w;
        light.position.w = get_light_radius(source);

        light.color   = source->properties->diffuse_color;
        light.color.w = 1.0f;

        char* result = retail::sub_7C88F0((u32*)ngl::lighting::references::light_manager.read(),
                                              (u64*)&light);

        retail::sub_7A77F0((i32)result);
    }

    void submit_spot_light(light_source* source) {
        ngl::lighting::spot_light_data light;

        light.position = source->my_abs_po->matrix.w;
        light.position.w = get_light_radius(source);

        light.direction.x = source->my_abs_po->matrix.z.x;
        light.direction.y = source->my_abs_po->matrix.z.y;
        light.direction.z = source->my_abs_po->matrix.z.z;

        light.angles.x = (f32)std::cos((f64)source->properties->inner_angle * 0.01745329300562541);
        light.angles.y = (f32)std::cos((f64)source->properties->outer_angle * 0.01745329300562541);

        light.color = source->properties->diffuse_color;
        light.color.w = 1.0f;

        char* result = retail::sub_7D6330((u32*)ngl::lighting::references::light_manager.read(), &light);

        retail::sub_7A7870((i32)result);
    }

    void submit_directional_light(light_source* source) {
        vector4 direction_source = source->my_abs_po->matrix.y;
        vector4 direction;
        vector4 color = source->properties->diffuse_color;

        direction_source.w = 0.0f;

        retail::sub_401A20((f32*)&direction, (f32*)&direction_source);
        direction.w = 0.0f;

        retail::sub_9E4E10(0x01000000, (u64*)&direction, (i32)&color);
    }

    void submit_light(render_region_info* entry,
                      light_source*       source,
                      entity*             selected_render_entity,
                      i32                 worker_index) {

        if (!source || !source->properties ||
            source->render_generations[worker_index] == (&references::worker_generations.get())[worker_index]) {

            return;
        }

        switch (source->properties->flavor) {
            case light_flavor_point:
                submit_point_light(source);
                break;

            case light_flavor_spot:
                submit_spot_light(source);

                break;

            case light_flavor_directional:
                if ((region*)retail::sub_612D30((u32*)selected_render_entity) != entry->reg)
                    return;

                submit_directional_light(source);

                break;

            default:
                return;
        }

        source->render_generations[worker_index] =
            (&references::worker_generations.get())[worker_index];
    }

    void submit_region_renderables(render_data* frame_data) {
        for (u32 index = 0; index < frame_data->regions.size; ++index) {
            region* reg = frame_data->regions.entries[index].reg;

            for (u32* renderable = reg->renderables_begin; renderable != reg->renderables_end; ++renderable)
                retail::sub_798590(*renderable, 0, (i32)reg);
        }
    }
} // treyarch

world_dynamics_system* wds_render_manager::get_world() {
    return (world_dynamics_system*)((u8*)this - offsetof(world_dynamics_system, render_mgr));
}

bool wds_render_manager::is_special_case_level() {
    cutscene_player* player = treyarch::references::cutscene_player.read();

    const char* level_name = (const char*)retail::sub_97E390((u32*)references::game_state.read());

    return level_name && !player->is_playing() &&
           (!std::strncmp(level_name, "act4_h", 6) || !std::strcmp(level_name, "Act3_Vulture_Hive"));
}

void wds_render_manager::publish_scene_parameter(ngl::scene* value) {
    u32 parameter_id    = ngl::fx::references::parameter_id_scene_light_source.read();
    u32 parameter_value = retail::sub_7D72F0();

    u64* valid_parameters = (u64*)value->parameters;

    *valid_parameters |= (u64)1 << parameter_id;

    value->parameters->values[parameter_id] = parameter_value;
}

f32 wds_render_manager::calculate_scene_near_plane(world_dynamics_system* world) {
    cutscene_player* player = treyarch::references::cutscene_player.read();
    f32 result = 0.25f;

    if (!player->is_playing()) {
        vector4 difference;
        vector4 position = world->hero_ptr->my_abs_po->matrix.w;

        position.w = 0.0f;

        retail::sub_583020((f32*)&difference,
                           (f32*)&position,
                           (f32*)&references::near_plane_reference.get());

        f64 squared_length = (f64)difference.x * (f64)difference.x +
                             (f64)difference.y * (f64)difference.y +
                             (f64)difference.z * (f64)difference.z +
                                 (f64)difference.w * (f64)difference.w;
        f32 adjusted_length = (f32)std::sqrt((f64)(f32)squared_length) - 2.0f;

        if (adjusted_length < 0.3f)
            adjusted_length = 0.3f;
        else if (adjusted_length > 0.5f)
            adjusted_length = 0.5f;

        result = adjusted_length;
    } else {
        u8* active_cutscene = *(u8**)player;

        if (active_cutscene)
            result = *(f32*)(active_cutscene + 0x124);
    }

    return result;
}

void wds_render_manager::configure_game_scene() {
    ngl::scene_callback_function null_callback =
        (ngl::scene_callback_function)retail::sub_5B4A70;

    ngl::set_scene_callback(ngl::scene_callback_mid,  null_callback);
    ngl::set_scene_callback(ngl::scene_callback_post, null_callback);
    ngl::set_scene_callback(ngl::scene_callback_3,    null_callback);

    ngl::set_scene_option_group_0(false, true, false);
    ngl::set_scene_option_group_1(false, true, ngl::d3d9::references::particle_depth_texture_requested.read());

    retail::sub_75E7D0();

    ngl::set_scene_callback(ngl::scene_callback_4,
                            (ngl::scene_callback_function)retail::sub_95CEF0);
}

void wds_render_manager::render_z_pre_pass(ngl::scene* game_scene, f32 near_plane) {
    ngl::scene* z_pre_pass = ngl::list_begin_scene(ngl::scene_parameter_parent);

    references::z_pre_pass_scene.write(z_pre_pass);

    ngl::set_scene_name("Z pre pass");
    ngl::set_clear_flags(0);
    ngl::set_z_test_enable(true);
    ngl::set_z_write_enable(true);

    retail::sub_9D54C0(0);

    z_pre_pass->depth_bias_enabled     = true;
    z_pre_pass->near_plane             = near_plane - 0.19f;
    z_pre_pass->derived_matrices_dirty = true;

    ngl::validate_matrices(z_pre_pass);

    ngl::list_end_scene();

    if (references::z_pre_pass_enabled.read()) {
        ngl::scene* previous = ngl::list_select_scene(z_pre_pass);

        retail::sub_972EF0(1);

        ngl::list_select_scene(previous);
    }

    ngl::list_select_scene(game_scene);

    game_scene->near_plane             = near_plane;
    game_scene->derived_matrices_dirty = true;

    ngl::validate_matrices(game_scene);
}

void wds_render_manager::submit_world(world_dynamics_system* world) {
    using namespace references;

    if (auxiliary_producer_enabled.read() &&
        ngl::d3d9::references::particle_depth_texture_requested.read()) {

        retail::sub_A37670((i32)ngl::d3d9::references::framebuffers.get().linear_depth_buffer);
        retail::sub_A37680(auxiliary_callback_enabled.read() ?
            (void*)retail::sub_95D470 : nullptr);

        if (auxiliary_render_enabled.read())
            retail::sub_A378B0();
    }

    invoke_render_phase(world->hero_ptr);

    i32 worker_index = retail::sub_6020A0(0);
    entity* selected_render_entity = world->hero_ptr;

    if (world->camera_mgr.marky_camera_enabled &&
        world->camera_mgr.marky_camera) {

        selected_render_entity = world->camera_mgr.marky_camera;
    }

    retail::sub_7A5130();

    treyarch::render_data* frame_data = references::render_data.read();

    for (u32 index = 0; index < frame_data->regions.size; ++index) {
        render_region_info* entry = &frame_data->regions.entries[index];
        region*             reg   = entry->reg;

        if (!reg)
            continue;

        for (light_source** source = reg->lights_begin; source != reg->lights_end; ++source)
            submit_light(entry, *source, selected_render_entity, worker_index);
    }

    submit_region_renderables(frame_data);
    retail::sub_602210(worker_index, 0);
}

void wds_render_manager::submit_blocked_world(world_dynamics_system* world) {
    invoke_render_phase(world->hero_ptr);

    for (far_away_render_list_entry* entry = references::far_away_render_list.read(); entry; entry = entry->next) {
        void* target = (void*)retail::sub_605300((i32*)&entry->vhandle);

        if (target) {
            auto method = (activate_method)(*(void***)target)[108];

            method(target, 1.0f);
        }
    }
}

void wds_render_manager::release_render_lock() {
    u32 depth = references::render_lock_depth.read() - 1;

    references::render_lock_depth.write(depth);

    if (!depth) {
        references::render_lock_owner.write(0);
        references::render_lock_thread.write(0);
    }
}

void wds_render_manager::render() {
    world_dynamics_system* world = get_world();

    retail::sub_96A350();

    cutscene_player* player = treyarch::references::cutscene_player.read();
    bool cutscene_is_playing = player->is_playing();
    u8* active_cutscene = cutscene_is_playing ? *(u8**)player : nullptr;

    references::render_distance.write
        (cutscene_is_playing && active_cutscene && !(*(u32*)(active_cutscene + 0x28) & 0x8000) ?
            2500.0f : 250.0f);

    references::frame_state_1117194.write(0);

    retail::sub_96E5B0((u32*)references::render_data.read());

    ensure_resources();
    request_environment_texture();

    references::level_render_special_case.write(0);

    if (is_special_case_level())
        references::level_render_special_case.write(1);

    ngl::scene* game_scene = ngl::references::current_scene.read();

    game_scene->world_to_view          = references::world_to_view.get();
    game_scene->derived_matrices_dirty = true;

    ngl::calculate_matrices(game_scene);
    publish_scene_parameter(game_scene);

    retail::sub_9694A0((u64*)references::published_scene.read(), (i32)game_scene);

    if (!references::current_region.read()) {
        ngl::set_scene_callback(ngl::scene_callback_4,
                                (ngl::scene_callback_function)retail::sub_95CEF0);

        return;
    }

    retail::sub_401930((i32)&references::render_lock_owner.get());
    references::render_sequence.write(references::render_sequence.read() + 1);

    retail::sub_95D7D0((u32*)references::render_data.read(),
                        world->the_terrain->generation + 1);

    configure_game_scene();

    f32 near_plane = calculate_scene_near_plane(world);

    render_z_pre_pass(game_scene, near_plane);

    retail::sub_975C50();

    igo_3d_zoom_map* zoom_map = references::frontend.get().igo->zoom_map;

    if (!zoom_map || !zoom_map->blocks_world_rendering())
        submit_world(world);
    else
        submit_blocked_world(world);

    release_render_lock();
}

void wds_render_manager::render_depth_shadows() {
    vector3 direction(0.0f, 1.0f, 0.0f);
    vector3 center = references::camera_position.get().get_xyz();
    const f32 span_caps[2] = { 20.0f, 60.0f };

    ngl::shadow::device_resource_state &targets =
        ngl::shadow::references::device_resources.get();
    ngl::shadow::target_dimensions &dimensions =
        ngl::shadow::references::dimensions.get();

    for (u32 index = 0; index < 2; ++index) {
        f32 span_cap = span_caps[index];
        ngl::scene* scene = ngl::list_begin_scene(ngl::scene_parameter_defaults);

        if (index == 0)
            references::shadow_scene_0.write(scene);
        else
            references::shadow_scene_1.write(scene);

        ngl::set_scene_name(index == 0 ? "render_shadows0" : "render_shadows1");
        ngl::set_scene_callback(ngl::scene_callback_mid,
                                (ngl::scene_callback_function)retail::sub_95D3F0,
                                (void*)index);
        ngl::set_scene_callback(ngl::scene_callback_3,
                                (ngl::scene_callback_function)retail::sub_95D460,
                                (void*)index);

        scene->depth_bias_enabled = true;

        region* current_region = references::current_region.read();
        vector3* direction_output = &direction;
        u8 fit_valid;

        __asm {
            mov esi, direction_output
            push current_region
            call retail::sub_968FB0
            add esp, 4
            mov fit_valid, al
        }

        if (!fit_valid) {
            ngl::list_end_scene();

            continue;
        }

        ngl::set_clear_flags(7);
        ngl::set_clear_color(1.0f, 1.0f, 1.0f, 1.0f);

        retail::sub_9D54C0((i32)targets.color_targets[index]);
        retail::sub_9D3810((i32)targets.depth_targets[index]);
        retail::sub_9D38C0(1.0f); // set clear depth

        f32 viewport_right = (f32)dimensions.widths[index] - 1.0f;
        f32 viewport_bottom = (f32)dimensions.heights[index] - 1.0f;

        retail::sub_9D7AA0(0.0f, 0.0f, viewport_right, viewport_bottom);
        retail::sub_9D7C10(1.0f); // set aspect ratio

        vector3 points[4];
        vector3 camera_right   = references::camera_right.get().get_xyz();
        vector3 camera_up      = references::camera_up.get().get_xyz();
        vector3 camera_forward = references::camera_forward.get().get_xyz();

        // build points
        retail::sub_969A70((i32)points,
                           (f32*)&center,
                           (f32*)&camera_right,
                           (f32*)&camera_up,
                           (f32*)&camera_forward,
                           references::camera_fov_radians.read(),
                           references::camera_aspect_ratio.read(),
                           span_cap);

        vector3 horizontal(direction.z, 0.0f, -direction.x);
        f32 horizontal_length_squared = horizontal.length2();

        if (horizontal_length_squared <= 0.0f)
            horizontal = references::degenerate_axis_fallback.get().get_xyz();
        else
            horizontal *= 1.0f / (f32)std::sqrt((f64)horizontal_length_squared);

        vector3 vertical(direction.y * horizontal.z - direction.z * horizontal.y,
                         direction.z * horizontal.x - direction.x * horizontal.z,
                         direction.x * horizontal.y - direction.y * horizontal.x);

        f32 horizontal_min = 0.0f;
        f32 horizontal_max = 0.0f;
        f32 vertical_min   = 0.0f;
        f32 vertical_max   = 0.0f;

        for (u32 point_index = 0; point_index < 4; ++point_index) {
            vector3 offset = points[point_index];
            offset -= center;
            f32 horizontal_projection = offset.x * horizontal.x +
                                        offset.y * horizontal.y +
                                        offset.z * horizontal.z;
            f32 vertical_projection = offset.x * vertical.x +
                                      offset.y * vertical.y +
                                      offset.z * vertical.z;

            if (horizontal_projection < horizontal_min)
                horizontal_min = horizontal_projection;
            if (horizontal_projection > horizontal_max)
                horizontal_max = horizontal_projection;
            if (vertical_projection < vertical_min)
                vertical_min = vertical_projection;
            if (vertical_projection > vertical_max)
                vertical_max = vertical_projection;
        }

        f32 horizontal_span = horizontal_max - horizontal_min;
        f32 vertical_span   = vertical_max - vertical_min;

        if (horizontal_span > span_cap)
            horizontal_span = span_cap;
        if (vertical_span > span_cap)
            vertical_span = span_cap;

        ngl::set_viewport(-1.0f / horizontal_span,
                          -1.0f / vertical_span,
                           1.0f / horizontal_span,
                           1.0f / vertical_span);

        vector3 eye = direction;
        eye *= 400.0f;
        eye += center;

        retail::sub_96AF70((f32*)&eye, (f32*)&center, (f32*)&vertical);

        f32 far_plane = 400.0f + span_cap + references::shadow_far_adjustment.read();

        retail::sub_9D7CA0(1.0f, 1.0f, 1.0f, far_plane); // set ortho parameters
        retail::sub_9D7D00((u64*)&references::world_to_view.get());

        ngl::set_z_write_enable(true);
        ngl::set_z_test_enable(true);
        ngl::validate_matrices(scene);
        ngl::list_end_scene();

        if (index == 0) {
            vector3 center_shift = camera_forward;
            center_shift *= 75.0f;
            center += center_shift;
        }
    }
}
