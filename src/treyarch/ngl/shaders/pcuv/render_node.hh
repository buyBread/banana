#pragma once

#include "treyarch/ngl/fx/render_node.hh"
#include "treyarch/ngl/list/render_node.hh"
#include "treyarch/ngl/material/material.hh"
#include "treyarch/ngl/mesh/mesh.hh"
#include "treyarch/ngl/texture/texture.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace ngl { namespace shaders { namespace pcuv {
    struct pcuv_material {
        ngl::material base;
        texture*      texture_data;
        u32           blend_mode_low;
        u32           blend_mode_high;
        u32           map_flags;
    };

    struct render_node {
        ngl::render_node    base;
        fx::mesh_node_data* node_data;
        mesh_section*       section;
        pcuv_material*      material_data;
    };

    void render(render_node* value);

    namespace references {
        inline util::memory_reference<void*> node_vtable { 0x00BE5274 };
        inline util::memory_reference<u8>    pcuv_shader { 0x00F4AA3F };
    } // references

    ASSERT_SIZEOF  (pcuv_material,                  0x24);
    ASSERT_OFFSETOF(pcuv_material, texture_data,    0x14);
    ASSERT_OFFSETOF(pcuv_material, blend_mode_low,  0x18);
    ASSERT_OFFSETOF(pcuv_material, blend_mode_high, 0x1C);
    ASSERT_OFFSETOF(pcuv_material, map_flags,       0x20);

    ASSERT_SIZEOF  (render_node,                0x18);
    ASSERT_OFFSETOF(render_node, node_data,     0x0C);
    ASSERT_OFFSETOF(render_node, section,       0x10);
    ASSERT_OFFSETOF(render_node, material_data, 0x14);
}}}} // treyarch::ngl::shaders::pcuv
