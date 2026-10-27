#include <cmath>

#include "treyarch/ngl/display.hh"
#include "treyarch/ngl/scene/matrices.hh"
#include "treyarch/math/trig.hh"

using namespace treyarch;

// sub_9D3A10
math::mat44 make_perspective(f32 horizontal_scale,
                             f32 vertical_scale,
                             f32 near_plane,
                             f32 far_plane) {

    f32 depth_scale = (f32)((f64)far_plane / ((f64)far_plane - near_plane));

    return math::mat44(math::vector4(horizontal_scale, 0.0f,           0.0f,                     0.0f),
                       math::vector4(0.0f,             vertical_scale, 0.0f,                     0.0f),
                       math::vector4(0.0f,             0.0f,           depth_scale,              1.0f),
                       math::vector4(0.0f,             0.0f,          -near_plane * depth_scale, 0.0f));
}

// sub_9D3B30
math::mat44 make_orthographic(f32 horizontal_scale,
                              f32 vertical_scale,
                              f32 near_plane,
                              f32 far_plane) {

    f32 depth_scale = (f32)(1.0 / ((f64)far_plane - near_plane));

    return math::mat44(math::vector4(horizontal_scale, 0.0f,           0.0f,                     0.0f),
                       math::vector4(0.0f,             vertical_scale, 0.0f,                     0.0f),
                       math::vector4(0.0f,             0.0f,           depth_scale,              0.0f),
                       math::vector4(0.0f,             0.0f,          -near_plane * depth_scale, 1.0f));
}

// sub_9D3C50
math::mat44 make_viewport(f32 left,
                          f32 top,
                          f32 right,
                          f32 bottom) {

    return math::mat44(math::vector4((right - left) * 0.5f,  0.0f,                 0.0f, 0.0f),
                       math::vector4( 0.0f,                 (top - bottom) * 0.5f, 0.0f, 0.0f),
                       math::vector4( 0.0f,                  0.0f,                 1.0f, 0.0f),
                       math::vector4((left + right) * 0.5f, (top + bottom) * 0.5f, 0.0f, 1.0f));
}

// sub_9E9120
// sub_9E9040
math::mat44 make_d3d9_clip_adjustment() {
    return math::mat44(math::vector4(1.0f,  0.0f, 0.0f, 0.0f),
                       math::vector4(0.0f, -1.0f, 0.0f, 0.0f),
                       math::vector4(0.0f,  0.0f, 1.0f, 0.0f),
                       math::vector4(0.0f,  0.0f, 0.0f, 1.0f));
}

// sub_9D4380
f32 determinant(const math::mat44 &value) {
    const math::vector4 &x = value.get_x();
    const math::vector4 &y = value.get_y();
    const math::vector4 &z = value.get_z();
    const math::vector4 &w = value.get_w();

    f64 minor_zw = (f64)z[2] * w[3] - (f64)z[3] * w[2];
    f64 minor_yw = (f64)y[2] * w[3] - (f64)y[3] * w[2];
    f64 minor_yz = (f64)y[2] * z[3] - (f64)y[3] * z[2];
    f64 minor_xz = (f64)x[2] * z[3] - (f64)x[3] * z[2];
    f64 minor_xw = (f64)x[2] * w[3] - (f64)x[3] * w[2];
    f64 minor_xy = (f64)x[2] * y[3] - (f64)x[3] * y[2];

    f32 z_term = ((f32)(w[1] * minor_xy) + ((f32)(x[1] * minor_yw) - (f32)(y[1] * minor_xw))) * z[0];
    f32 x_term = ((f32)(y[1] * minor_zw) - (f32)(z[1] * minor_yw) + (f32)(w[1] * minor_yz)) * x[0];
    f32 y_term = ((f32)(x[1] * minor_zw) - (f32)(z[1] * minor_xw) + (f32)(w[1] * minor_xz)) * y[0];
    f32 w_term = ((f32)(x[1] * minor_yz) - (f32)(y[1] * minor_xz) + (f32)(z[1] * minor_xy)) * w[0];

    return z_term + (x_term - y_term) - w_term;
}

// sub_9D4520
math::mat44 cofactors(const math::mat44 &value, bool transposed) { // transposed into the adjugate when asked to
    const math::vector4 &x = value.get_x();
    const math::vector4 &y = value.get_y();
    const math::vector4 &z = value.get_z();
    const math::vector4 &w = value.get_w();

    // 2x2 minors named by their rows, then columns
    f64 zw_zw = (f64)z[2] * w[3] - (f64)z[3] * w[2];
    f64 yw_zw = (f64)y[2] * w[3] - (f64)y[3] * w[2];
    f64 yz_zw = (f64)y[2] * z[3] - (f64)y[3] * z[2];
    f64 xw_zw = (f64)x[2] * w[3] - (f64)x[3] * w[2];
    f64 xz_zw = (f64)x[2] * z[3] - (f64)x[3] * z[2];
    f64 xy_zw = (f64)x[2] * y[3] - (f64)x[3] * y[2];
    f64 zw_yw = (f64)z[1] * w[3] - (f64)z[3] * w[1];
    f64 yw_yw = (f64)y[1] * w[3] - (f64)y[3] * w[1];
    f64 yz_yw = (f64)y[1] * z[3] - (f64)y[3] * z[1];
    f64 xw_yw = (f64)x[1] * w[3] - (f64)x[3] * w[1];
    f64 xz_yw = (f64)x[1] * z[3] - (f64)x[3] * z[1];
    f64 xy_yw = (f64)x[1] * y[3] - (f64)x[3] * y[1];
    f64 zw_yz = (f64)z[1] * w[2] - (f64)z[2] * w[1];
    f64 yw_yz = (f64)y[1] * w[2] - (f64)y[2] * w[1];
    f64 yz_yz = (f64)y[1] * z[2] - (f64)y[2] * z[1];
    f64 xw_yz = (f64)x[1] * w[2] - (f64)x[2] * w[1];
    f64 xz_yz = (f64)x[1] * z[2] - (f64)x[2] * z[1];
    f64 xy_yz = (f64)x[1] * y[2] - (f64)x[2] * y[1];

    math::mat44 result(math::vector4((f32)(  zw_zw * y[1] - yw_zw * z[1] + yz_zw * w[1]),
                                     (f32)(-(zw_zw * y[0] - yw_zw * z[0] + yz_zw * w[0])),
                                     (f32)(  zw_yw * y[0] - yw_yw * z[0] + yz_yw * w[0]),
                                     (f32)(-(zw_yz * y[0] - yw_yz * z[0] + yz_yz * w[0]))),

                       math::vector4((f32)(-(zw_zw * x[1] - xw_zw * z[1] + xz_zw * w[1])),
                                     (f32)(  zw_zw * x[0] - xw_zw * z[0] + xz_zw * w[0]),
                                     (f32)(-(zw_yw * x[0] - xw_yw * z[0] + xz_yw * w[0])),
                                     (f32)(  zw_yz * x[0] - xw_yz * z[0] + xz_yz * w[0])),

                       math::vector4((f32)(  yw_zw * x[1] - xw_zw * y[1] + xy_zw * w[1]),
                                     (f32)(-(yw_zw * x[0] - xw_zw * y[0] + xy_zw * w[0])),
                                     (f32)(  yw_yw * x[0] - xw_yw * y[0] + xy_yw * w[0]),
                                     (f32)(-(yw_yz * x[0] - xw_yz * y[0] + xy_yz * w[0]))),

                       math::vector4((f32)(-(yz_zw * x[1] - xz_zw * y[1] + xy_zw * z[1])),
                                     (f32)(  yz_zw * x[0] - xz_zw * y[0] + xy_zw * z[0]),
                                     (f32)(-(yz_yw * x[0] - xz_yw * y[0] + xy_yw * z[0])),
                                     (f32)(  yz_yz * x[0] - xz_yz * y[0] + xy_yz * z[0])));

    return transposed ? ~result : result;
}

// sub_9D4DE0
math::mat44 inverse(const math::mat44 &value) {
    f32 value_determinant = determinant(value);

    // retail returns an uninitialized stack matrix here
    if (value_determinant == 0.0f)
        return math::identity_mat33();

    return cofactors(value, true) * (1.0f / value_determinant);
}

// sub_9D5060
math::mat44 make_ui_to_screen(const ngl::scene* value) {
    f32 half_width;
    f32 half_height;

    if (value->color_target && (value->color_target->flags & 0x200) != 0) {
        half_width  = (f32)ngl::get_screen_width()  * 0.5f;
        half_height = (f32)ngl::get_screen_height() * 0.5f;
    } else {
        half_width  = (f32)value->target_width  * 0.5f;
        half_height = (f32)value->target_height * 0.5f;
    }

    return math::mat44(math::vector4( 1.0f / half_width, 0.0f,               0.0f, 0.0f),
                       math::vector4( 0.0f,              1.0f / half_height, 0.0f, 0.0f),
                       math::vector4( 0.0f,              0.0f,               1.0f, 0.0f),
                       math::vector4(-1.0f,             -1.0f,               0.0f, 1.0f));
}

// sub_9D3D90
math::mat44 make_ui_to_device(const ngl::scene* value) {
    const math::mat44 &viewport   = value->view;
    const math::mat44 &projection = value->projection;

    // the viewport and the projection are inverted on their own, not as a product
    math::mat44 viewport_inverse(math::vector4( 1.0f / viewport[0][0],            0.0f,                            0.0f, 0.0f),
                                 math::vector4( 0.0f,                             1.0f / viewport[1][1],           0.0f, 0.0f),
                                 math::vector4( 0.0f,                             0.0f,                            1.0f, 0.0f),
                                 math::vector4(-viewport[3][0] / viewport[0][0], -viewport[3][1] / viewport[1][1], 0.0f, 1.0f));

    math::mat44 projection_inverse;

    if (value->projection_type == ngl::projection_perspective) {
        projection_inverse = math::mat44(math::vector4(1.0f / projection[0][0], 0.0f,                   0.0f, 0.0f),
                                         math::vector4(0.0f,                   1.0f / projection[1][1], 0.0f, 0.0f),
                                         math::vector4(0.0f,                   0.0f,                   0.0f, 1.0f / projection[3][2]),
                                         math::vector4(0.0f,                   0.0f,                   1.0f, -projection[2][2] / projection[3][2]));
    } else {
        projection_inverse = math::mat44(math::vector4(1.0f / projection[0][0], 0.0f,                    0.0f,                                0.0f),
                                         math::vector4(0.0f,                   1.0f / projection[1][1],  0.0f,                                0.0f),
                                         math::vector4(0.0f,                   0.0f,                    1.0f / projection[2][2],             0.0f),
                                         math::vector4(0.0f,                   0.0f,                   -projection[3][2] / projection[2][2], 1.0f));
    }

    return viewport_inverse * projection_inverse;
}

// inlined @ sub_9D5510
void ngl::apply_matrix_plane(      math::vector4 &out,
                             const math::mat44   &matrix,
                             const math::vector4 &plane) {

    math::vector4 transformed((f32)((f64)plane[0] * matrix[0][0] + (f64)plane[1] * matrix[1][0] + (f64)plane[2] * matrix[2][0]),
                              (f32)((f64)plane[0] * matrix[0][1] + (f64)plane[1] * matrix[1][1] + (f64)plane[2] * matrix[2][1]),
                              (f32)((f64)plane[0] * matrix[0][2] + (f64)plane[1] * matrix[1][2] + (f64)plane[2] * matrix[2][2]),
                              0.0f);

    transformed.set_w((f32)((f64)matrix[3][0] * transformed[0] +
                            (f64)matrix[3][1] * transformed[1] +
                            (f64)matrix[3][2] * transformed[2] + plane[3]));

    out = transformed;
}

// sub_9D5510
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

    math::vector4 view_planes[6];
    f32 viewport_left   = value->viewport_left;
    f32 viewport_top    = value->viewport_top;
    f32 viewport_right  = value->viewport_right;
    f32 viewport_bottom = value->viewport_bottom;

    if (value->projection_type == projection_perspective) {
        f32 tangent            = math::tan((f32)((f64)value->field_of_view * 0.008726646502812704));
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

        view_planes[0] = math::vector4(left_length,
                                       0.0f,
                                       -left_length * left_slope,
                                       0.0f);
        view_planes[1] = math::vector4(-right_length,
                                       0.0f,
                                       right_length * right_slope,
                                       0.0f);
        view_planes[2] = math::vector4(0.0f,
                                       -top_length,
                                       -top_length * top_slope,
                                       0.0f);
        view_planes[3] = math::vector4(0.0f,
                                       bottom_length,
                                       bottom_length * bottom_slope,
                                       0.0f);
    } else {
        value->projection = make_orthographic(1.0f / (value->ortho_width * value->aspect_ratio),
                                              1.0f / value->ortho_height,
                                              value->near_plane,
                                              value->far_plane);

        view_planes[0] = math::vector4(1.0f, 0.0f, 0.0f,
                                       value->pixel_scissor_left * value->aspect_ratio);
        view_planes[1] = math::vector4(-1.0f, 0.0f, 0.0f,
                                       -value->pixel_scissor_right * value->aspect_ratio);
        view_planes[2] = math::vector4(0.0f, -1.0f, 0.0f,
                                       value->pixel_scissor_top);
        view_planes[3] = math::vector4(0.0f, 1.0f, 0.0f,
                                       -value->pixel_scissor_bottom);
    }

    view_planes[4] = math::vector4(0.0f, 0.0f, 1.0f, value->near_plane);
    view_planes[5] = math::vector4(0.0f, 0.0f, -1.0f, -value->far_plane);

    value->view = make_viewport(viewport_left,
                                viewport_top,
                                viewport_right,
                                viewport_bottom);

    value->device = make_d3d9_clip_adjustment();

    value->view_to_screen    = value->projection * value->view * value->device;
    value->view_to_world     = math::inv(value->world_to_view);
    value->world_to_screen   = value->world_to_view * value->view_to_screen;
    value->viewport_to_world = math::inv(value->world_to_screen);
    value->ui_to_device      = make_ui_to_device(value);

    value->derived_matrix_250 = value->ui_to_device * value->view_to_world;
    value->derived_matrix_290 = inverse(value->world_to_screen);
    value->derived_matrix_2d0 = make_ui_to_screen(value) * value->device;

    for (i32 index = 0; index < 6; ++index)
        apply_matrix_plane(value->clip_planes[index], value->view_to_world, view_planes[index]);

    value->view_position  = value->view_to_world.get_w();
    value->view_direction = value->view_to_world.get_z();
}

// sub_9D7980
void ngl::validate_matrices(scene* value) {
    if (value && value->derived_matrices_dirty)
        calculate_matrices(value);
}

// sub_9D7F20
math::mat44* ngl::get_world_to_screen(scene* value) {
    if (value && value->derived_matrices_dirty)
        calculate_matrices(value);

    return &value->world_to_screen;
}
