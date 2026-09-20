#pragma once

#include "treyarch/ngl/d3d9/geometry_stream.hh"
#include "treyarch/ngl/list/render_node.hh"
#include "treyarch/ngl/mesh/mesh.hh"
#include "treyarch/ngl/morph/morph.hh"

namespace treyarch { namespace ngl { namespace morph_geometry {
    struct binding {
        morph_set* set;
        u32*       frame_indices;
        f32*       weights;
        u32        frame_count;
    };

    struct render_node {
        ngl::render_node                base;
        u32                             reserved_00c;
        ngl::render_node*               material_node;
        mesh_section*                   section;
        mesh_section*                   original_section;
        u32                             section_index;
        d3d9::geometry_stream::segment* stream_segment;
    };

    void render(render_node* value);

    namespace references {
        inline util::memory_reference<void*> node_vtable { 0x00DB8A60 };
    } // references

    ASSERT_SIZEOF  (binding,                0x10);
    ASSERT_OFFSETOF(binding, set,           0x00);
    ASSERT_OFFSETOF(binding, frame_indices, 0x04);
    ASSERT_OFFSETOF(binding, weights,       0x08);
    ASSERT_OFFSETOF(binding, frame_count,   0x0C);

    ASSERT_SIZEOF  (render_node,                   0x24);
    ASSERT_OFFSETOF(render_node, material_node,    0x10);
    ASSERT_OFFSETOF(render_node, section,          0x14);
    ASSERT_OFFSETOF(render_node, original_section, 0x18);
    ASSERT_OFFSETOF(render_node, section_index,    0x1C);
    ASSERT_OFFSETOF(render_node, stream_segment,   0x20);
}}} // treyarch::ngl::morph_geometry
