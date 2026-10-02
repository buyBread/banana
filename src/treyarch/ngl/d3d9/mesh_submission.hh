#pragma once

#include "treyarch/ngl/mesh/mesh.hh"

namespace treyarch { namespace ngl { namespace d3d9 {
    u32 get_primitive_count(D3DPRIMITIVETYPE primitive_type, u32 element_count);

    void draw_primitive(      D3DPRIMITIVETYPE         primitive_type,
                              u32                      vertex_count,
                              u32                      vertex_offset,
                              IDirect3DVertexBuffer9** vertex_buffer,
                        const vertex_definition*       definition);

    void draw_indexed_primitive(      D3DPRIMITIVETYPE         primitive_type,
                                      u32                      index_count,
                                      u32                      index_offset,
                                      IDirect3DIndexBuffer9**  index_buffer,
                                      D3DFORMAT                index_format,
                                      u32                      vertex_count,
                                      u32                      vertex_offset,
                                      IDirect3DVertexBuffer9** vertex_buffer,
                                const vertex_definition*       definition);

    void draw_mesh_section(mesh_section* value);
    void draw_mesh_section_runs(mesh_section* value, const i32* runs);
}}} // treyarch::ngl::d3d9
