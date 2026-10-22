#include <cmath>

#include "treyarch/ngl/fx/lighting_parameters.hh"
#include "treyarch/shared/math/vmath/matrix_rot.hh"

using namespace treyarch;

namespace treyarch { namespace ngl { namespace fx { namespace lighting_transform {
    math::vector4 position(const math::vector4        &value,
                           const math::rot_tran_mat43 &matrix) {

        const math::unit_dir3 &x = matrix.get_x();
        const math::unit_dir3 &y = matrix.get_y();
        const math::unit_dir3 &z = matrix.get_z();
        const math::position3 &w = matrix.get_w();

        return math::vector4(value[0] * x[0] + value[1] * y[0] + value[2] * z[0] + w[0],
                             value[0] * x[1] + value[1] * y[1] + value[2] * z[1] + w[1],
                             value[0] * x[2] + value[1] * y[2] + value[2] * z[2] + w[2],
                             value[3]);
    }

    math::vector4 direction(const math::vector4        &value,
                            const math::rot_tran_mat43 &matrix) {

        const math::unit_dir3 &x = matrix.get_x();
        const math::unit_dir3 &y = matrix.get_y();
        const math::unit_dir3 &z = matrix.get_z();

        return math::vector4(value[0] * x[0] + value[1] * y[0] + value[2] * z[0],
                             value[0] * x[1] + value[1] * y[1] + value[2] * z[1],
                             value[0] * x[2] + value[1] * y[2] + value[2] * z[2],
                             value[3]);
    }

    math::vector4 plane(const math::vector4 &value,
                        const math::mat44   &local_to_world) {

        return math::vector4(local_to_world[0][0] * value[0] + local_to_world[0][1] * value[1] + local_to_world[0][2] * value[2],
                             local_to_world[1][0] * value[0] + local_to_world[1][1] * value[1] + local_to_world[1][2] * value[2],
                             local_to_world[2][0] * value[0] + local_to_world[2][1] * value[1] + local_to_world[2][2] * value[2],
                             local_to_world[3][0] * value[0] + local_to_world[3][1] * value[1] + local_to_world[3][2] * value[2] + value[3]);
    }
}}}} // treyarch::ngl::fx::lighting_transform

void ngl::fx::apply_material_values(      general_lighting_parameters* value,
                                    const math::vector4*               material_values) {

    value->reserved_1d0 = material_values[0];
    value->reserved_1e0 = material_values[1];
    value->reserved_1f0 = material_values[2];
    value->reserved_200 = material_values[3];
}

void ngl::fx::transform_lighting_to_local(      general_lighting_parameters*  value,
                                          const math::mat44                  &local_to_world) {

    math::rot_tran_mat43 world_to_local = math::inv(*(const math::rot_tran_mat43*)&local_to_world);

    value->view_position = lighting_transform::position(value->view_position, world_to_local);

    value->ambient_direction = lighting_transform::direction(value->ambient_direction, world_to_local);
    value->ambient_color     = lighting_transform::plane(value->ambient_color, local_to_world);

    value->post_view_position = lighting_transform::position(value->post_view_position, world_to_local);
    value->post_direction     = lighting_transform::direction(value->post_direction, world_to_local);

    f32 post_length = std::sqrt(value->post_direction[0] * value->post_direction[0] +
                                value->post_direction[1] * value->post_direction[1] +
                                value->post_direction[2] * value->post_direction[2]);

    if (post_length != 0.0f) {
        value->post_direction[0] /= post_length;
        value->post_direction[1] /= post_length;
        value->post_direction[2] /= post_length;
    }

    value->post_plane_0 = lighting_transform::position(value->post_plane_0, world_to_local);
    value->post_plane_1 = lighting_transform::position(value->post_plane_1, world_to_local);

    value->horizon_direction        = lighting_transform::direction(value->horizon_direction, world_to_local);
    value->horizon_axis             = lighting_transform::direction(value->horizon_axis, world_to_local);
    value->horizon_direction_scaled = lighting_transform::direction(value->horizon_direction_scaled, world_to_local);
    value->horizon_projection_u     = lighting_transform::plane(value->horizon_projection_u, local_to_world);
    value->horizon_projection_v     = lighting_transform::plane(value->horizon_projection_v, local_to_world);

    for (i32 index = 0; index < value->light_count; ++index) {
        if (value->light_positions[index].get_w() == 0.0f)
            value->light_positions[index] = lighting_transform::direction(value->light_positions[index], world_to_local);
        else
            value->light_positions[index] = lighting_transform::position(value->light_positions[index], world_to_local);

        value->light_directions[index] = lighting_transform::direction(value->light_directions[index], world_to_local);
    }

    value->projector_matrix = math::mat44(*(const math::mat43*)&local_to_world) *
                              math::mat44(*(const math::mat43*)&value->projector_matrix);
}
