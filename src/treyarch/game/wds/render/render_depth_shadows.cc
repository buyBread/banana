#include <cmath>

#include "retail.hh"
#include "treyarch/game/geometry/geometry_manager.hh"
#include "treyarch/game/light/light_source_data.hh"
#include "treyarch/game/shadow/shadow.hh"
#include "treyarch/game/wds/entity/entity.hh"
#include "treyarch/game/wds/references.hh"
#include "treyarch/game/wds/region.hh"
#include "treyarch/game/wds/render/depth_shadows.hh"
#include "treyarch/game/wds/render/references.hh"
#include "treyarch/game/wds/render/wds_render_manager.hh"
#include "treyarch/game/wds/camera/references.hh"
#include "treyarch/ngl/scene/defaults.hh"
#include "treyarch/ngl/scene/lifecycle.hh"
#include "treyarch/ngl/scene/matrices.hh"
#include "treyarch/ngl/scene/references.hh"
#include "treyarch/ngl/scene/viewport.hh"
#include "treyarch/shared/container/fixed_vector.hh"

using namespace treyarch;

// sub_968FB0
bool treyarch::find_shadow_light_direction(vector3* direction,
                                           region*  current_region) {

    if (!current_region)
        return false;

    entity* hero     = references::g_world_ptr.read()->get_hero_ptr();
    vector3 position = hero->my_abs_po->get_position();

    container::fixed_vector<region*, 40> regions;

    for (region* hero_region : hero->regions)
        regions.push_back(hero_region);

    light_source_data* light_source =
        (light_source_data*)retail::sub_7D75C0(&regions, (f32*)&position, nullptr);

    sky_data* sky = light_source->sky;

    if (!sky) {
        if (light_source->directional_light_count <= 0)
            return false;

        *direction = -light_source->directional_lights[0].direction;

        return true;
    }

    if (references::shadow_use_sky_direction_050.read() &&
        sky->color_040.get_xyz().length2() > 0.0f       &&
        sky->direction_050.y > 0.0f) {

        *direction = sky->direction_050.get_xyz();
    } else if (references::shadow_use_sky_direction_070.read() &&
               sky->unk_060.get_xyz().length2() > 0.0f) {

        *direction = sky->direction_070.get_xyz();
    } else {
        return false;
    }

    if (std::fabs(direction->y) < 0.25f)
        return false;

    return true;
}

// sub_969A70
void treyarch::build_shadow_frustum_corners(      vector3*  corners,
                                            const vector3  &center,
                                            const vector3  &right,
                                            const vector3  &up,
                                            const vector3  &forward,
                                                  f32       field_of_view,
                                                  f32       aspect_ratio,
                                                  f32       distance) {

    f32 half_width  = std::tan(field_of_view * 0.5f) * distance;
    f32 half_height = half_width * aspect_ratio;

    vector3 right_offset    = right;
            right_offset   *= half_width;
    vector3 up_offset       = up;
            up_offset      *= half_height;
    vector3 forward_offset  = forward;
            forward_offset *= distance;

    corners[0]  = center;
    corners[0] += right_offset;
    corners[0] += up_offset;
    corners[0] += forward_offset;

    corners[1]  = center;
    corners[1] -= right_offset;
    corners[1] += up_offset;
    corners[1] += forward_offset;

    corners[2]  = center;
    corners[2] -= right_offset;
    corners[2] -= up_offset;
    corners[2] += forward_offset;

    corners[3]  = center;
    corners[3] += right_offset;
    corners[3] -= up_offset;
    corners[3] += forward_offset;
}

// sub_771B50
void treyarch::set_shadow_cascades(f32         distance_0,
                                   f32         distance_1,
                                   f32         start_0,
                                   f32         start_1,
                                   ngl::scene* value) {

    shadow::references::distance_0.write(distance_0);
    shadow::references::distance_1.write(distance_1);

    // both cascades get the scene being entered, so after cascade 1 they match
    shadow::references::scene_0.write(value);
    shadow::references::scene_1.write(value);

    ngl::validate_matrices(value);

    f64 range_0 = (f64)distance_0 - (f64)start_0;
    f64 range_1 = (f64)distance_1 - (f64)start_1;

    shadow::references::shadow_distances.write(vector4((f32)(1.0 / range_0),
                                                       (f32)(1.0 / range_1),
                                                       (f32)((f64)start_0 / range_0),
                                                       (f32)((f64)start_1 / range_1)));
}

// sub_95D3F0
void treyarch::enter_shadow_scene(void* context) {
    u32 index = (u32)context;

    ngl::references::in_shadow_scene.write(1);

    // 114.8 is stored one ulp below the nearest float
    set_shadow_cascades(20.0f, 114.799995f, 16.4f, 94.135994f, ngl::references::current_scene.read());

    const matrix4x4 &world_to_screen = *ngl::get_world_to_screen(ngl::references::current_scene.read());

    if (index == 0)
        shadow::references::matrix_0.write(world_to_screen);
    else
        shadow::references::matrix_1.write(world_to_screen);
}

// sub_95D460
void treyarch::leave_shadow_scene(void*) {
    ngl::references::in_shadow_scene.write(0);
}

// sub_96F950
void wds_render_manager::render_depth_shadows() {
    vector3 direction(0.0f, 1.0f, 0.0f);
    vector3 center = references::camera_position.get().get_xyz();
    const f32 span_caps[2] = { 20.0f, 60.0f };

    shadow::device_resource_state &targets    = shadow::references::device_resources.get();
    shadow::target_dimensions     &dimensions = shadow::references::dimensions.get();

    for (u32 index = 0; index < 2; ++index) {
        f32 span_cap = span_caps[index];
        ngl::scene* scene = ngl::list_begin_scene(ngl::scene_parameter_defaults);

        if (index == 0)
            references::shadow_scene_0.write(scene);
        else
            references::shadow_scene_1.write(scene);

        ngl::set_scene_name(index == 0 ? "render_shadows0" : "render_shadows1");
        ngl::set_scene_callback(ngl::scene_callback_mid, enter_shadow_scene, (void*)index);
        ngl::set_scene_callback(ngl::scene_callback_3,   leave_shadow_scene, (void*)index);

        scene->depth_bias_enabled = true;

        if (!find_shadow_light_direction(&direction, references::current_region.read())) {
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

        build_shadow_frustum_corners(points,
                                     center,
                                     camera_right,
                                     camera_up,
                                     camera_forward,
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

        geometry_manager::set_view(eye, center, vertical);

        f32 far_plane = 400.0f + span_cap + references::shadow_far_adjustment.read();

        ngl::set_ortho_parameters(1.0f, 1.0f, 1.0f, far_plane);
        ngl::set_world_to_view_matrix(&geometry_manager::references::world_to_view.get());

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
