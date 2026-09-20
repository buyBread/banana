#pragma once

#include "treyarch/ngl/mesh/mesh.hh"

namespace treyarch { namespace ngl { namespace d3d9 {
    mesh_section* allocate_scratch_mesh_section(D3DPRIMITIVETYPE primitive_type,
                                                u32              index_count,
                                                u32              vertex_count,
                                                vertex_definition* definition);

    u16* lock_scratch_mesh_indices(mesh_section* section);
    void unlock_scratch_mesh_indices(mesh_section* section);

    void* lock_scratch_mesh_vertices(mesh_section* section);
    void unlock_scratch_mesh_vertices(mesh_section* section);
}}} // treyarch::ngl::d3d9
