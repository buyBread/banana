#pragma once

#include <d3d9.h>

#include "treyarch/ngl/fx/render_node.hh"
#include "treyarch/ngl/list/render_node.hh"
#include "treyarch/ngl/material/material.hh"
#include "treyarch/ngl/mesh/mesh.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace ngl { namespace shaders { namespace smsky {
    struct sky_data {
        u8      reserved_000[0xD8];
        void*   resource_0d8;
        void*   resource_0dc;
        vector4 color_0e0;
        u8      reserved_0f0[0x20];
        vector4 color_110;
    };

    struct light_source_data {
        u8        reserved_000[0x2D0];
        vector4   color_2d0;
        u8        reserved_2e0[0x4C];
        sky_data* sky;
    };

    struct render_node {
        ngl::render_node    base;
        fx::mesh_node_data* node_data;
        mesh_section*       section;
        material*           material_data;
        u8                  reserved_018[0x08];
    };

    void render(render_node* value);

    namespace references {
        inline util::memory_reference<void*>                    node_vtable        { 0x00BE5660 };
        inline util::memory_reference<IDirect3DVertexShader9**> sky_vertex_program { 0x010F7D04 };
        inline util::memory_reference<IDirect3DPixelShader9**>  sky_pixel_program  { 0x010F7D08 };
    } // references

    ASSERT_OFFSETOF(sky_data, resource_0d8, 0x0D8);
    ASSERT_OFFSETOF(sky_data, resource_0dc, 0x0DC);
    ASSERT_OFFSETOF(sky_data, color_0e0,    0x0E0);
    ASSERT_OFFSETOF(sky_data, color_110,    0x110);

    ASSERT_OFFSETOF(light_source_data, color_2d0, 0x2D0);
    ASSERT_OFFSETOF(light_source_data, sky,       0x32C);

    ASSERT_SIZEOF  (render_node,                0x20);
    ASSERT_OFFSETOF(render_node, node_data,     0x0C);
    ASSERT_OFFSETOF(render_node, section,       0x10);
    ASSERT_OFFSETOF(render_node, material_data, 0x14);
}}}} // treyarch::ngl::shaders::smsky
