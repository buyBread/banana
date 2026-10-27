#pragma once

#include "treyarch/ngl/gpu/d3d9/geometry_stream.hh"
#include "treyarch/ngl/list/render_node.hh"
#include "treyarch/ngl/mesh/mesh.hh"
#include "treyarch/ngl/morph/morph.hh"

namespace treyarch { namespace ngl {
    namespace morph_geometry {
        struct binding {
            morph_set* set;
            u32*       frame_indices;
            f32*       weights;
            u32        frame_count;
        };

        ASSERT_SIZEOF  (binding,                0x10);
        ASSERT_OFFSETOF(binding, set,           0x00);
        ASSERT_OFFSETOF(binding, frame_indices, 0x04);
        ASSERT_OFFSETOF(binding, weights,       0x08);
        ASSERT_OFFSETOF(binding, frame_count,   0x0C);
    } // morph_geometry

    struct morph_geometry_shader_node : geometry_shader_node {
        u32                             reserved_00c;
        shader_node*                    material_node;
        mesh_section*                   section;
        mesh_section*                   original_section;
        u32                             section_index;
        d3d9::geometry_stream::segment* stream_segment;

        void render() override;
        void get_desc(char* desc) override;
        void get_sort_info(sort_info &info) override;
    };

    namespace references {
        inline util::memory_reference<void*> morph_geometry_shader_node_vftable { 0x00DB8A60 };
    } // references

    ASSERT_SIZEOF  (morph_geometry_shader_node,                   0x24);
    ASSERT_OFFSETOF(morph_geometry_shader_node, material_node,    0x10);
    ASSERT_OFFSETOF(morph_geometry_shader_node, section,          0x14);
    ASSERT_OFFSETOF(morph_geometry_shader_node, original_section, 0x18);
    ASSERT_OFFSETOF(morph_geometry_shader_node, section_index,    0x1C);
    ASSERT_OFFSETOF(morph_geometry_shader_node, stream_segment,   0x20);
}} // treyarch::ngl
