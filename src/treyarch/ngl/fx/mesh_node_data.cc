#include "treyarch/ngl/fx/mesh_node_data.hh"

using namespace treyarch;

matrix4x4 ngl::fx::get_unscaled_local_to_world(const mesh_node_data* value) {
    matrix4x4 result = value->local_to_world;

    if (value->node_info[0] & 2) {
        const vector3 &scales = *(const vector3*)(value->node_info + 0x10);

        for (u32 column = 0; column < 4; ++column) {
            result[0][column] /= scales.x;
            result[1][column] /= scales.y;
            result[2][column] /= scales.z;
        }
    }

    return result;
}

matrix4x4 ngl::fx::get_compressed_to_local(const mesh_node_data* value) {
    const f32* mesh_data = (const f32*)value->mesh_data;
    f32 scale = mesh_data[11] >= 32.0f ? 0.01f : 0.001f;

    return matrix4x4(scale,        0.0f,         0.0f,          0.0f,
                     0.0f,         scale,        0.0f,          0.0f,
                     0.0f,         0.0f,         scale,         0.0f,
                     mesh_data[8], mesh_data[9], mesh_data[10], 1.0f);
}

matrix4x4 ngl::fx::get_compressed_to_uv() {
    return matrix4x4(1.0f / 1024.0f, 0.0f,           0.0f, 0.0f,
                     0.0f,           1.0f / 1024.0f, 0.0f, 0.0f,
                     0.0f,           0.0f,           1.0f, 0.0f,
                     0.0f,           0.0f,           0.0f, 1.0f);
}
