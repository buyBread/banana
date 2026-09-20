#pragma once

#include "treyarch/ngl/mesh/mesh.hh"
#include "treyarch/ngl/morph/morph.hh"

namespace treyarch { namespace ngl { namespace morph_geometry {
    u32 get_vertex_element_size(u8 type);

    void read_component_value(u32 value_type, const u8* &source, vector4 &output);

    void decode_base_vertices(      mesh_section*      section,
                              const D3DVERTEXELEMENT9 &element,
                              const u8*                mesh_data,
                                    vector4*           output);

    void encode_morph_vertices(      IDirect3DVertexBuffer9*  buffer,
                                     u32                      buffer_offset,
                                     u32                      vertex_stride,
                                     u32                      vertex_count,
                               const D3DVERTEXELEMENT9       &element,
                               const u8*                      mesh_data,
                               const vector4*                 input);
}}} // treyarch::ngl::morph_geometry
