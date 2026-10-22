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

math::mat44 ngl::fx::get_compressed_to_local(const mesh_node_data* value) {
    const f32* mesh_data = (const f32*)value->mesh_data;
    f32 scale = mesh_data[11] >= 32.0f ? 0.01f : 0.001f;

    return math::mat44(math::vector4(scale,        0.0f,         0.0f,          0.0f),
                       math::vector4(0.0f,         scale,        0.0f,          0.0f),
                       math::vector4(0.0f,         0.0f,         scale,         0.0f),
                       math::vector4(mesh_data[8], mesh_data[9], mesh_data[10], 1.0f));
}

math::mat44 ngl::fx::get_compressed_to_uv() {
    return math::mat44(math::vector4(1.0f / 1024.0f, 0.0f,           0.0f, 0.0f),
                       math::vector4(0.0f,           1.0f / 1024.0f, 0.0f, 0.0f),
                       math::vector4(0.0f,           0.0f,           1.0f, 0.0f),
                       math::vector4(0.0f,           0.0f,           0.0f, 1.0f));
}
