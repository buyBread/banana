#include "treyarch/ngl/fx/mesh_node_data.hh"

using namespace treyarch;

// sub_884370
math::rot_tran_mat43 ngl::fx::get_unscaled_local_to_world(const mesh_node_data* value) {
    if (!(value->node_info[0] & 2))
        return *(const math::rot_tran_mat43*)&value->local_to_world;

    const math::position3 &scales = *(const math::position3*)(value->node_info + 0x10);

    f32 reciprocal_x = 1.0f / scales.get_x();
    f32 reciprocal_y = 1.0f / scales.get_y();
    f32 reciprocal_z = 1.0f / scales.get_z();

    math::mat43 result(value->local_to_world.get_x() * reciprocal_x,
                       value->local_to_world.get_y() * reciprocal_y,
                       value->local_to_world.get_z() * reciprocal_z,
                       value->local_to_world.get_w());

    return *(const math::rot_tran_mat43*)&result;
}

// sub_884560
math::rot_tran_mat43 ngl::fx::get_unscaled_world_to_local(const mesh_node_data* value) {
    return math::inv(get_unscaled_local_to_world(value));
}
