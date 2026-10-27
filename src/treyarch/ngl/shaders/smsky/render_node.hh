#pragma once

#include <d3d9.h>

#include "game/light/light_source_data.hh"
#include "treyarch/ngl/fx/mesh_node_data.hh"
#include "treyarch/ngl/list/render_node.hh"
#include "treyarch/ngl/material/material.hh"
#include "treyarch/ngl/mesh/mesh.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"

namespace treyarch { namespace ngl { namespace shaders { namespace smsky {
    struct node : shader_node {
        material* material_data;
        u8        reserved_018[0x08];

        void render() override;

        void render_normal();
    };

    namespace references {
        inline util::memory_reference<void*>                    node_vftable       { 0x00BE5660 };
        inline util::memory_reference<IDirect3DVertexShader9**> sky_vertex_program { 0x010F7D04 };
        inline util::memory_reference<IDirect3DPixelShader9**>  sky_pixel_program  { 0x010F7D08 };
    } // references

    ASSERT_SIZEOF  (node,                0x20);
    ASSERT_OFFSETOF(node, material_data, 0x14);
}}}} // treyarch::ngl::shaders::smsky
