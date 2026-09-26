#pragma once

#include "treyarch/ngl/scene/scene.hh"
#include "treyarch/shared/math/types/matrix4x4.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl { namespace fx {
    struct mesh_node_data {
        matrix4x4         local_to_world;
        u8                reserved_040[0x40];
        u8*               mesh_data;
        scene_parameters* parameters;
        u8*               node_info;
        f32               scale;
        u8                reserved_090[0x04];
        u32               render_flags;
        u8                point_light_indices[8];
        u8                point_light_count;
    };

    matrix4x4 get_unscaled_local_to_world(const mesh_node_data* value);
    matrix4x4 get_compressed_to_local(const mesh_node_data* value);
    matrix4x4 get_compressed_to_uv();

    ASSERT_OFFSETOF(mesh_node_data, local_to_world,      0x00);
    ASSERT_OFFSETOF(mesh_node_data, mesh_data,           0x80);
    ASSERT_OFFSETOF(mesh_node_data, parameters,          0x84);
    ASSERT_OFFSETOF(mesh_node_data, node_info,           0x88);
    ASSERT_OFFSETOF(mesh_node_data, scale,               0x8C);
    ASSERT_OFFSETOF(mesh_node_data, render_flags,        0x94);
    ASSERT_OFFSETOF(mesh_node_data, point_light_indices, 0x98);
    ASSERT_OFFSETOF(mesh_node_data, point_light_count,   0xA0);
}}} // treyarch::ngl::fx
