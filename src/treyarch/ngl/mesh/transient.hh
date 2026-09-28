#pragma once

#include "treyarch/ngl/mesh/mesh.hh"

namespace treyarch { namespace ngl {
    mesh* create_transient_mesh(u32 flags, u32 section_count);
    u32   attach_transient_mesh_section(mesh*         value,
                                        mesh_section* section,
                                        material*     section_material,
                                        u32           flags);
}} // treyarch::ngl
