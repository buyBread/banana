#pragma once

#include "treyarch/ngl/mesh/mesh.hh"

namespace treyarch { namespace ngl { namespace d3d9 {
    mesh_section* create_scratch_section(D3DPRIMITIVETYPE primitive_type,
                                         u32              index_count,
                                         u32              vertex_count,
                                         vertex_definition* definition);

    u16* lock_section_indices(mesh_section* section);
    void unlock_section_indices(mesh_section* section);

    void* lock_section_vertices(mesh_section* section);
    void unlock_section_vertices(mesh_section* section);
}}} // treyarch::ngl::d3d9
