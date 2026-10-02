#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/mesh_submission.hh"

using namespace treyarch;

// sub_72BD20
u32 ngl::d3d9::get_primitive_count(D3DPRIMITIVETYPE primitive_type,
                                   u32              element_count) {

    switch (primitive_type) {
        case D3DPT_POINTLIST:
            return element_count;
        case D3DPT_LINELIST:
            return element_count >> 1;
        case D3DPT_LINESTRIP:
            return element_count - 1;
        case D3DPT_TRIANGLELIST:
            return element_count / 3;
        case D3DPT_TRIANGLESTRIP:
        case D3DPT_TRIANGLEFAN:
            return element_count - 2;
        default:
            return 0;
    }
}

// sub_72C2E0
void ngl::d3d9::draw_primitive(      D3DPRIMITIVETYPE         primitive_type,
                                     u32                      vertex_count,
                                     u32                      vertex_offset,
                                     IDirect3DVertexBuffer9** vertex_buffer,
                               const vertex_definition*       definition) {

    set_vertex_buffer(vertex_buffer, definition, vertex_offset, 0);

    references::device.get()->DrawPrimitive(primitive_type,
                                            0,
                                            get_primitive_count(primitive_type, vertex_count));
}

// sub_7AD120
void ngl::d3d9::draw_indexed_primitive(      D3DPRIMITIVETYPE         primitive_type,
                                             u32                      index_count,
                                             u32                      index_offset,
                                             IDirect3DIndexBuffer9**  index_buffer,
                                             D3DFORMAT                index_format,
                                             u32                      vertex_count,
                                             u32                      vertex_offset,
                                             IDirect3DVertexBuffer9** vertex_buffer,
                                       const vertex_definition*       definition) {

    set_vertex_buffer(vertex_buffer, definition, vertex_offset, 0);

    binding_cache &bindings = references::bindings.get();

    // like the vertex streams, the cache holds where the buffer pointer lives
    if ((u32)bindings.indices != (u32)index_buffer) {
        bindings.indices = (IDirect3DIndexBuffer9*)index_buffer;

        references::device.get()->SetIndices(*index_buffer);
    }

    u32 index_size = index_format != D3DFMT_INDEX16 ? 4 : 2;

    references::device.get()->DrawIndexedPrimitive(primitive_type,
                                                   0,
                                                   0,
                                                   vertex_count,
                                                   index_offset / index_size,
                                                   get_primitive_count(primitive_type, index_count));
}

// sub_9E5EA0
void ngl::d3d9::draw_mesh_section(mesh_section* value) {
    D3DPRIMITIVETYPE primitive_type = (D3DPRIMITIVETYPE)value->primitive_type;

    if (value->index_count) {
        draw_indexed_primitive(primitive_type,
                               value->index_count,
                               value->index_offset,
                               &value->index_buffer,
                               value->index_size != 2 ? D3DFMT_INDEX32 : D3DFMT_INDEX16,
                               value->vertex_count,
                               value->vertex_offset,
                               &value->vertex_buffer,
                               value->vertex_definition_data);

        return;
    }

    // the non-indexed branch is draw_primitive inlined, with no vertex offset
    set_vertex_buffer(&value->vertex_buffer, value->vertex_definition_data, 0, 0);

    references::device.get()->DrawPrimitive(primitive_type,
                                            0,
                                            get_primitive_count(primitive_type, value->vertex_count));
}

void ngl::d3d9::draw_mesh_section_runs(mesh_section* value, const i32* runs) {
    set_vertex_buffer(&value->vertex_buffer, value->vertex_definition_data, 0, 0);

    IDirect3DDevice9* device = references::device.get();
    
    D3DPRIMITIVETYPE primitive_type = (D3DPRIMITIVETYPE)value->primitive_type;

    i32 start_vertex = runs[0];

    while (start_vertex >= 0) {
        u32 vertex_count = (u32)runs[1];

        device->DrawPrimitive(primitive_type,
                              start_vertex,
                              get_primitive_count(primitive_type, vertex_count));

        start_vertex = runs[2];

        runs += 2;
    }
}
