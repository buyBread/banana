#include <cmath>

#include "retail.hh"
#include "treyarch/game/wds/region.hh"
#include "treyarch/game/wds/render/references.hh"
#include "treyarch/game/wds/render/wds_render_manager.hh"
#include "treyarch/game/wds/camera/references.hh"
#include "treyarch/ngl/scene/defaults.hh"
#include "treyarch/ngl/scene/lifecycle.hh"
#include "treyarch/ngl/scene/matrices.hh"
#include "treyarch/ngl/scene/viewport.hh"
#include "treyarch/ngl/shadow/device_resources.hh"

using namespace treyarch;

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

        ngl::set_color_target(targets.color_targets[index]);
        ngl::set_depth_target(targets.depth_targets[index]);
        ngl::set_clear_depth(1.0f);

        f32 viewport_right  = (f32)dimensions.widths[index]  - 1.0f;
        f32 viewport_bottom = (f32)dimensions.heights[index] - 1.0f;
        ngl::set_pixel_viewport(0.0f, 0.0f, viewport_right, viewport_bottom);
        ngl::set_aspect_ratio(1.0f);

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
            vector3 offset  = points[point_index];
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

        ngl::set_ortho_parameters(1.0f, 1.0f, 1.0f, far_plane);
        ngl::set_world_to_view_matrix(&references::world_to_view.get());

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
