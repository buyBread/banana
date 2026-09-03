#pragma once

#include "treyarch/ngl/fx/effect.hh"
#include "treyarch/ngl/fx/material.hh"
#include "treyarch/ngl/list/render_node.hh"
#include "treyarch/ngl/math/types/matrix4x4.hh"
#include "treyarch/ngl/scene/scene.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl {
    struct mesh_section;

namespace fx {
    struct mesh_node_data {
        matrix4x4         local_to_world;
        u8                reserved_040[0x40];
        u8*               mesh_data;
        scene_parameters* parameters;
        u8*               node_info;
        f32               scale;
        u8                reserved_090[0x08];
        u8                point_light_indices[8];
        u8                point_light_count;
    };

    struct render_node {
        ngl::render_node base;
        mesh_node_data*  node_data;
        mesh_section*    section;
        material*        material_data;
        effect*          effect_data;
        i32              technique_index;
        u32              queue_class;
        u32              order_key;
    };

    void render(render_node* value);

    namespace references {
        inline util::memory_reference<void*> render_node_vtable { 0x00DB7B84 };
    } // references

    ASSERT_OFFSETOF(mesh_node_data, local_to_world,      0x00);
    ASSERT_OFFSETOF(mesh_node_data, mesh_data,           0x80);
    ASSERT_OFFSETOF(mesh_node_data, parameters,          0x84);
    ASSERT_OFFSETOF(mesh_node_data, node_info,           0x88);
    ASSERT_OFFSETOF(mesh_node_data, scale,               0x8C);
    ASSERT_OFFSETOF(mesh_node_data, point_light_indices, 0x98);
    ASSERT_OFFSETOF(mesh_node_data, point_light_count,   0xA0);

    ASSERT_SIZEOF  (render_node,                  0x28);
    ASSERT_OFFSETOF(render_node, node_data,       0x0C);
    ASSERT_OFFSETOF(render_node, section,         0x10);
    ASSERT_OFFSETOF(render_node, material_data,   0x14);
    ASSERT_OFFSETOF(render_node, effect_data,     0x18);
    ASSERT_OFFSETOF(render_node, technique_index, 0x1C);
    ASSERT_OFFSETOF(render_node, queue_class,     0x20);
    ASSERT_OFFSETOF(render_node, order_key,       0x24);
} // fx
}} // treyarch::ngl
