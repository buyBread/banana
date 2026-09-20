#pragma once

#include "treyarch/ngl/fx/render_node.hh"
#include "treyarch/ngl/list/render_node.hh"
#include "treyarch/ngl/material/material.hh"
#include "treyarch/ngl/mesh/mesh.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace ngl { namespace shaders { namespace puv {
    struct render_node {
        ngl::render_node    base;
        fx::mesh_node_data* node_data;
        mesh_section*       section;
        material*           material_data;
    };

    void render(render_node* value);

    namespace references {
        inline util::memory_reference<void*> node_vtable { 0x00BE5380 };
        inline util::memory_reference<u8>    puv_shader  { 0x00F4AA40 };
    } // references

    ASSERT_SIZEOF  (render_node,                0x18);
    ASSERT_OFFSETOF(render_node, node_data,     0x0C);
    ASSERT_OFFSETOF(render_node, section,       0x10);
    ASSERT_OFFSETOF(render_node, material_data, 0x14);
}}}} // treyarch::ngl::shaders::puv
