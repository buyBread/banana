#pragma once

#include "treyarch/ngl/fx/mesh_node_data.hh"
#include "treyarch/ngl/list/render_node.hh"
#include "treyarch/ngl/material/material.hh"
#include "treyarch/ngl/mesh/mesh.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"

namespace treyarch { namespace ngl { namespace shaders { namespace puv {
    struct node : shader_node {
        material* material_data;

        void render() override;
        void get_sort_info(sort_info &info) override;
    };

    namespace references {
        inline util::memory_reference<void*> node_vftable { 0x00BE5380 };
        inline util::memory_reference<u8>    puv_shader   { 0x00F4AA40 };
    } // references

    ASSERT_SIZEOF  (node,                0x18);
    ASSERT_OFFSETOF(node, material_data, 0x14);
}}}} // treyarch::ngl::shaders::puv
