#pragma once

#include "treyarch/ngl/fx/effect.hh"
#include "treyarch/ngl/fx/material.hh"
#include "treyarch/ngl/fx/mesh_node_data.hh"
#include "treyarch/ngl/list/render_node.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl {
    struct mesh_section;

namespace fx {
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
        inline util::memory_reference<void*> node_vtable { 0x00DB7B84 };
    } // references

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
