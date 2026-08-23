#include <cmath>

#include "treyarch/ngl/display.hh"
#include "treyarch/ngl/scene/matrices.hh"

using namespace treyarch;

static ngl::matrix4x4 make_perspective(f32 horizontal_scale,
                                       f32 vertical_scale,
                                       f32 near_plane,
                                       f32 far_plane) {

    f32 depth_scale = far_plane / (far_plane - near_plane);

    return ngl::matrix4x4(horizontal_scale, 0.0f,           0.0f,                      0.0f,
                          0.0f,             vertical_scale, 0.0f,                      0.0f,
                          0.0f,             0.0f,           depth_scale,               1.0f,
                          0.0f,             0.0f,          -near_plane * depth_scale,  0.0f);
}

static ngl::matrix4x4 make_orthographic(f32 horizontal_scale,
                                        f32 vertical_scale,
                                        f32 near_plane,
                                        f32 far_plane) {

    f32 depth_scale = 1.0f / (far_plane - near_plane);

    return ngl::matrix4x4(horizontal_scale, 0.0f,           0.0f,                     0.0f,
                          0.0f,             vertical_scale, 0.0f,                     0.0f,
                          0.0f,             0.0f,           depth_scale,              0.0f,
                          0.0f,             0.0f,          -near_plane * depth_scale, 1.0f);
}

static ngl::matrix4x4 make_viewport(f32 left,
                                    f32 top,
                                    f32 right,
                                    f32 bottom) {

    return ngl::matrix4x4((right - left) * 0.5f,  0.0f,                 0.0f, 0.0f,
                           0.0f,                 (top - bottom) * 0.5f, 0.0f, 0.0f,
                           0.0f,                  0.0f,                 1.0f, 0.0f,
                          (left + right) * 0.5f, (top + bottom) * 0.5f, 0.0f, 1.0f);
}

static ngl::matrix4x4 make_device() {
    return ngl::matrix4x4(1.0f,  0.0f, 0.0f, 0.0f,
                          0.0f, -1.0f, 0.0f, 0.0f,
                          0.0f,  0.0f, 1.0f, 0.0f,
                          0.0f,  0.0f, 0.0f, 1.0f);
}

static ngl::matrix4x4 make_ui_to_screen(const ngl::scene* value) {
    f32 half_width;
    f32 half_height;

    if (value->color_target && (value->color_target->flags & 0x200) != 0) {
        half_width  = (f32)ngl::get_screen_width() * 0.5f;
        half_height = (f32)ngl::get_screen_height() * 0.5f;
    } else {
        half_width  = (f32)value->target_width * 0.5f;
        half_height = (f32)value->target_height * 0.5f;
    }

    return ngl::matrix4x4(1.0f / half_width, 0.0f,               0.0f, 0.0f,
                          0.0f,              1.0f / half_height, 0.0f, 0.0f,
                          0.0f,              0.0f,               1.0f, 0.0f,
                         -1.0f,             -1.0f,               0.0f, 1.0f);
}

void ngl::calculate_matrices(scene* value) {
    if (!value->derived_matrices_dirty)
        return;

    value->derived_matrices_dirty = 0;

    texture* target = value->color_target ?
        value->color_target : value->depth_target;

    if (target) {
        value->target_width  = target->gpu_texture.width;
        value->target_height = target->gpu_texture.height;
    } else {
        value->target_width  = 1;
        value->target_height = 1;
    }

    f32 inverse_viewport_width = 1.0f / (value->viewport_right - value->viewport_left);
    f32 viewport_x_offset = -(value->viewport_left + value->viewport_right) * inverse_viewport_width;
    f32 viewport_x_scale  = inverse_viewport_width * 2.0f;

    f32 inverse_viewport_height = 1.0f / (value->viewport_bottom - value->viewport_top);
    f32 viewport_y_offset = -(value->viewport_top + value->viewport_bottom) * inverse_viewport_height;
    f32 viewport_y_scale  = inverse_viewport_height * 2.0f;

    value->pixel_scissor_left   = value->scissor_left   * viewport_x_scale + viewport_x_offset;
    value->pixel_scissor_right  = value->scissor_right  * viewport_x_scale + viewport_x_offset;
    value->pixel_scissor_top    = value->scissor_top    * viewport_y_scale + viewport_y_offset;
    value->pixel_scissor_bottom = value->scissor_bottom * viewport_y_scale + viewport_y_offset;

    vector4 view_planes[6];
    f32 viewport_left   = value->viewport_left;
    f32 viewport_top    = value->viewport_top;
    f32 viewport_right  = value->viewport_right;
    f32 viewport_bottom = value->viewport_bottom;

    if (value->projection_type == projection_perspective) {
        f32 tangent            = std::tan(value->field_of_view * 0.008726646502812704f);
        f32 horizontal_tangent = value->aspect_ratio * tangent;

        f32 maximum_x = std::fmax(std::fabs(value->pixel_scissor_left),
                                  std::fabs(value->pixel_scissor_right));
        f32 maximum_y = std::fmax(std::fabs(value->pixel_scissor_top),
                                  std::fabs(value->pixel_scissor_bottom));

        value->viewport_half_width  = maximum_x * horizontal_tangent;
        value->viewport_half_height = maximum_y * tangent;

        f32 viewport_center_x = (viewport_left + viewport_right)  * 0.5f;
        f32 viewport_center_y = (viewport_top  + viewport_bottom) * 0.5f;

        viewport_left   = (viewport_left   - viewport_center_x) * maximum_x + viewport_center_x;
        viewport_right  = (viewport_right  - viewport_center_x) * maximum_x + viewport_center_x;
        viewport_top    = (viewport_top    - viewport_center_y) * maximum_y + viewport_center_y;
        viewport_bottom = (viewport_bottom - viewport_center_y) * maximum_y + viewport_center_y;

        value->projection = make_perspective(1.0f / value->viewport_half_width,
                                             1.0f / value->viewport_half_height,
                                             value->near_plane,
                                             value->far_plane);

        f32 left_slope    = horizontal_tangent * value->pixel_scissor_left;
        f32 right_slope   = horizontal_tangent * value->pixel_scissor_right;
        f32 top_slope     = tangent * value->pixel_scissor_top;
        f32 bottom_slope  = tangent * value->pixel_scissor_bottom;
        f32 left_length   = 1.0f / std::sqrt(left_slope   * left_slope   + 1.0f);
        f32 right_length  = 1.0f / std::sqrt(right_slope  * right_slope  + 1.0f);
        f32 top_length    = 1.0f / std::sqrt(top_slope    * top_slope    + 1.0f);
        f32 bottom_length = 1.0f / std::sqrt(bottom_slope * bottom_slope + 1.0f);

        view_planes[0] = vector4(left_length,
                                 0.0f,
                                 -left_length * left_slope,
                                 0.0f);
        view_planes[1] = vector4(-right_length,
                                 0.0f,
                                 right_length * right_slope,
                                 0.0f);
        view_planes[2] = vector4(0.0f,
                                 -top_length,
                                 -top_length * top_slope,
                                 0.0f);
        view_planes[3] = vector4(0.0f,
                                 bottom_length,
                                 bottom_length * bottom_slope,
                                 0.0f);
    } else {
        value->projection = make_orthographic(1.0f / (value->ortho_width * value->aspect_ratio),
                                              1.0f / value->ortho_height,
                                              value->near_plane,
                                              value->far_plane);

        view_planes[0] = vector4(1.0f, 0.0f, 0.0f,
                                 value->pixel_scissor_left * value->aspect_ratio);
        view_planes[1] = vector4(-1.0f, 0.0f, 0.0f,
                                 -value->pixel_scissor_right * value->aspect_ratio);
        view_planes[2] = vector4(0.0f, -1.0f, 0.0f,
                                 value->pixel_scissor_top);
        view_planes[3] = vector4(0.0f, 1.0f, 0.0f,
                                 -value->pixel_scissor_bottom);
    }

    view_planes[4] = vector4(0.0f, 0.0f, 1.0f, value->near_plane);
    view_planes[5] = vector4(0.0f, 0.0f, -1.0f, -value->far_plane);

    value->view = make_viewport(viewport_left,
                                viewport_top,
                                viewport_right,
                                viewport_bottom);

    value->device = make_device();

    value->view_to_screen    = value->projection * value->view * value->device;
    value->view_to_world     = value->world_to_view.inverse();
    value->world_to_screen   = value->world_to_view * value->view_to_screen;
    value->viewport_to_world = value->world_to_screen.inverse();

    matrix4x4 view_to_viewport = value->projection * value->view;
    value->ui_to_device = view_to_viewport.inverse();
    
    value->derived_matrix_250 = value->ui_to_device * value->view_to_world;
    value->derived_matrix_290 = value->world_to_screen.inverse();
    value->derived_matrix_2d0 = make_ui_to_screen(value) * value->device;

    for (i32 index = 0; index < 6; ++index)
        value->clip_planes[index] = view_planes[index].transform_plane(value->view_to_world);

    value->view_position  = value->view_to_world.w;
    value->view_direction = value->view_to_world.z;
}

void ngl::validate_matrices(scene* value) {
    if (value && value->derived_matrices_dirty)
        calculate_matrices(value);
}
