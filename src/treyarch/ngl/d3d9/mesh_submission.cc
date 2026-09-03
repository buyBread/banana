#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/mesh_submission.hh"

using namespace treyarch;

u32 ngl::d3d9::get_primitive_count(D3DPRIMITIVETYPE primitive_type,
                                   u32 element_count) {

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

static void bind_mesh_section_with_offset(ngl::mesh_section* value,
                                          i32                vertex_offset) {

    ngl::d3d9::binding_cache &bindings = ngl::d3d9::references::bindings.get();

    IDirect3DVertexDeclaration9* declaration = value->vertex_definition_data->declaration;

    u32 stream_key = (u32)&value->vertex_buffer + vertex_offset;

    if ((u32)bindings.stream_source      == stream_key &&
             bindings.vertex_declaration == declaration) {

        return;
    }

    IDirect3DDevice9* device = ngl::d3d9::references::device.get();

    if (bindings.vertex_declaration != declaration) {
        bindings.vertex_declaration = declaration;

        device->SetVertexDeclaration(declaration);
    }

    device->SetStreamSource(0,
                            value->vertex_buffer,
                            vertex_offset,
                            value->vertex_definition_data->vertex_size);

    bindings.stream_source = (IDirect3DVertexBuffer9*)stream_key;
}

void ngl::d3d9::bind_mesh_section(mesh_section* value) {
    bind_mesh_section_with_offset(value, value->vertex_offset);
}

void ngl::d3d9::draw_mesh_section(mesh_section* value) {
    IDirect3DDevice9* device = references::device.get();

    D3DPRIMITIVETYPE primitive_type = (D3DPRIMITIVETYPE)value->primitive_type;

    bind_mesh_section(value);

    if (!value->index_count) {
        device->DrawPrimitive(primitive_type,
                              0,
                              get_primitive_count(primitive_type, value->vertex_count));

        return;
    }

    binding_cache &bindings = references::bindings.get();

    if ((u32)bindings.indices != (u32)&value->index_buffer) {
        bindings.indices = (IDirect3DIndexBuffer9*)&value->index_buffer;
        
        device->SetIndices(value->index_buffer);
    }

    u32 index_size = value->index_size == 2 ? 2 : 4;

    device->DrawIndexedPrimitive(primitive_type,
                                 0,
                                 0,
                                 value->vertex_count,
                                 value->index_offset / index_size,
                                 get_primitive_count(primitive_type, value->index_count));
}

void ngl::d3d9::draw_mesh_section_individual(mesh_section* value) {
    if (value->index_count) {
        draw_mesh_section(value);
        
        return;
    }

    bind_mesh_section_with_offset(value, 0);

    D3DPRIMITIVETYPE primitive_type = (D3DPRIMITIVETYPE)value->primitive_type;

    references::device.get()
        ->DrawPrimitive(primitive_type,
                        0,
                        get_primitive_count(primitive_type, value->vertex_count));
}

void ngl::d3d9::draw_mesh_section_runs(mesh_section* value, const i32* runs) {
    bind_mesh_section_with_offset(value, 0);

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
