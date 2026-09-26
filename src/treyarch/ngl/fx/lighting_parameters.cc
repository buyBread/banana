#include <cmath>

#include "treyarch/ngl/fx/lighting_parameters.hh"

using namespace treyarch;

namespace treyarch { namespace ngl { namespace fx { namespace lighting_transform {
    vector4 position(const vector4   &value,
                     const matrix4x4 &matrix) {

        return vector4(value.x * matrix[0][0] + value.y * matrix[1][0] + value.z * matrix[2][0] + matrix[3][0],
                       value.x * matrix[0][1] + value.y * matrix[1][1] + value.z * matrix[2][1] + matrix[3][1],
                       value.x * matrix[0][2] + value.y * matrix[1][2] + value.z * matrix[2][2] + matrix[3][2],
                       value.w);
    }

    vector4 direction(const vector4   &value,
                      const matrix4x4 &matrix) {

        return vector4(value.x * matrix[0][0] + value.y * matrix[1][0] + value.z * matrix[2][0],
                       value.x * matrix[0][1] + value.y * matrix[1][1] + value.z * matrix[2][1],
                       value.x * matrix[0][2] + value.y * matrix[1][2] + value.z * matrix[2][2],
                       value.w);
    }

    vector4 plane(const vector4   &value,
                  const matrix4x4 &local_to_world) {

        return vector4(local_to_world[0][0] * value.x + local_to_world[0][1] * value.y + local_to_world[0][2] * value.z,
                       local_to_world[1][0] * value.x + local_to_world[1][1] * value.y + local_to_world[1][2] * value.z,
                       local_to_world[2][0] * value.x + local_to_world[2][1] * value.y + local_to_world[2][2] * value.z,
                       local_to_world[3][0] * value.x + local_to_world[3][1] * value.y + local_to_world[3][2] * value.z + value.w);
    }
}}}} // treyarch::ngl::fx::lighting_transform

void ngl::fx::apply_material_values(      general_lighting_parameters* value,
                                    const vector4*                     material_values) {

    value->reserved_1d0 = material_values[0];
    value->reserved_1e0 = material_values[1];
    value->reserved_1f0 = material_values[2];
    value->reserved_200 = material_values[3];
}

void ngl::fx::transform_lighting_to_local(      general_lighting_parameters* value,
                                          const matrix4x4&                   local_to_world) {

    matrix4x4 world_to_local = local_to_world.inverse_orthonormal();

    value->view_position = lighting_transform::position(value->view_position, world_to_local);

    value->ambient_direction = lighting_transform::direction(value->ambient_direction, world_to_local);
    value->ambient_color     = lighting_transform::plane(value->ambient_color, local_to_world);

    value->post_view_position = lighting_transform::position(value->post_view_position, world_to_local);
    value->post_direction     = lighting_transform::direction(value->post_direction, world_to_local);

    f32 post_length = std::sqrt(value->post_direction.x * value->post_direction.x +
                                value->post_direction.y * value->post_direction.y +
                                value->post_direction.z * value->post_direction.z);

    if (post_length != 0.0f) {
        value->post_direction.x /= post_length;
        value->post_direction.y /= post_length;
        value->post_direction.z /= post_length;
    }

    value->post_plane_0 = lighting_transform::position(value->post_plane_0, world_to_local);
    value->post_plane_1 = lighting_transform::position(value->post_plane_1, world_to_local);

    value->horizon_direction        = lighting_transform::direction(value->horizon_direction, world_to_local);
    value->horizon_axis             = lighting_transform::direction(value->horizon_axis, world_to_local);
    value->horizon_direction_scaled = lighting_transform::direction(value->horizon_direction_scaled, world_to_local);
    value->horizon_projection_u     = lighting_transform::plane(value->horizon_projection_u, local_to_world);
    value->horizon_projection_v     = lighting_transform::plane(value->horizon_projection_v, local_to_world);

    for (i32 index = 0; index < value->light_count; ++index) {
        if (value->light_positions[index].w == 0.0f)
            value->light_positions[index] = lighting_transform::direction(value->light_positions[index], world_to_local);
        else
            value->light_positions[index] = lighting_transform::position(value->light_positions[index], world_to_local);

        value->light_directions[index] = lighting_transform::direction(value->light_directions[index], world_to_local);
    }

    value->projector_matrix = local_to_world.affine() * value->projector_matrix.affine();
}
