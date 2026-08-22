#pragma once

#include "treyarch/ngl/d3d9/vertex_definition.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace ngl { namespace d3d9 {
    void initialize_internal_vertex_formats();

    namespace references {
        inline util::memory_reference
            <vertex_definition> quad_position_uv_format        { 0x0111AB00 };
        inline util::memory_reference
            <vertex_definition> quad_position_color_uv4_format { 0x0111AB0C };
        inline util::memory_reference
            <vertex_definition> quad_position_color_format     { 0x0111ABAC };
        inline util::memory_reference
            <vertex_definition> quad_position_color_uv_format  { 0x0111ABB8 };
    } // references
}}} // treyarch::ngl::d3d9
