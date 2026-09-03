#pragma once

#include "treyarch/ngl/mesh/mesh.hh"

namespace treyarch { namespace ngl { namespace d3d9 {
    u32 get_primitive_count(D3DPRIMITIVETYPE primitive_type, u32 element_count);
    void bind_mesh_section(mesh_section* value);
    void draw_mesh_section(mesh_section* value);
    void draw_mesh_section_individual(mesh_section* value);
    void draw_mesh_section_runs(mesh_section* value, const i32* runs);
}}} // treyarch::ngl::d3d9
