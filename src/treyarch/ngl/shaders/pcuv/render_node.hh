#pragma once

#include "treyarch/ngl/fx/mesh_node_data.hh"
#include "treyarch/ngl/list/render_node.hh"
#include "treyarch/ngl/material/material.hh"
#include "treyarch/ngl/mesh/mesh.hh"
#include "treyarch/ngl/texture/texture.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"

namespace treyarch { namespace ngl { namespace shaders { namespace pcuv {
    struct pcuv_material {
        ngl::material base;
        texture*      texture_data;
        u32           blend_mode_low;
        u32           blend_mode_high;
        u32           map_flags;
    };

    struct node : shader_node {
        pcuv_material* material;

        void render() override;
        void get_sort_info(sort_info &info) override;

        void render_normal();
    };

    namespace references {
        inline util::memory_reference<void*> node_vftable { 0x00BE5274 };
        inline util::memory_reference<u8>    pcuv_shader { 0x00F4AA3F };
    } // references

    ASSERT_SIZEOF  (pcuv_material,                  0x24);
    ASSERT_OFFSETOF(pcuv_material, texture_data,    0x14);
    ASSERT_OFFSETOF(pcuv_material, blend_mode_low,  0x18);
    ASSERT_OFFSETOF(pcuv_material, blend_mode_high, 0x1C);
    ASSERT_OFFSETOF(pcuv_material, map_flags,       0x20);

    ASSERT_SIZEOF  (node,           0x18);
    ASSERT_OFFSETOF(node, material, 0x14);
}}}} // treyarch::ngl::shaders::pcuv
