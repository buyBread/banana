#include <bit>

#include "treyarch/ngl/gpu/d3d9/scratch_mesh.hh"
#include "treyarch/ngl/gpu/d3d9/work_buffers.hh"
#include "treyarch/ngl/list/list.hh"

using namespace treyarch;

// sub_9E2970
ngl::mesh_section* ngl::d3d9::create_scratch_section(D3DPRIMITIVETYPE   primitive_type,
                                                     u32                index_count,
                                                     u32                vertex_count,
                                                     vertex_definition* definition) {

    auto* section = (mesh_section*)ngl::list_alloc(0x60, 16);

    section->sphere.set_x(0.0f);
    section->sphere.set_y(0.0f);
    section->sphere.set_z(0.0f);
    section->sphere.set_w(std::bit_cast<f32>(0x749DC5AEu));
    section->unk_010 = 0x749DC5AE;
    section->unk_014 = 0x749DC5AE;
    section->unk_018 = 0x749DC5AE;
    section->unk_01c = 0;
    section->material_data = nullptr;

    scratch_buffers &scratch = references::scratch_buffers.get();

    section->vertex_definition_data = definition;
    section->primitive_type         = primitive_type;
    section->index_count            = index_count;
    section->index_size             = sizeof(u16);
    section->index_buffer           = scratch.index_buffer_0;

    u32 index_offset = scratch.index_write_offset;

    if (index_offset + sizeof(u16) * index_count > scratch.maximum_index_count)
        index_offset = 0;

    scratch.index_write_offset = index_offset + sizeof(u16) * index_count;
    section->index_offset = index_offset;

    section->vertex_count  = vertex_count;
    section->vertex_buffer = scratch.vertex_buffer_0;

    u32 vertex_size   = definition->vertex_size;
    u32 vertex_bytes  = vertex_count * vertex_size;
    u32 vertex_offset = scratch.vertex_write_offset;

    if (vertex_size)
        vertex_offset = vertex_size * ((vertex_offset + vertex_size - 1) / vertex_size);

    if (vertex_offset + vertex_bytes > scratch.vertex_buffer_size)
        vertex_offset = 0;

    scratch.vertex_write_offset = vertex_offset + vertex_bytes;
    
    section->vertex_offset  = vertex_offset;
    section->vertex_size    = vertex_size;
    section->binary_version = -1;

    return section;
}

// sub_9E2A80
u16* ngl::d3d9::lock_section_indices(mesh_section* section) {
    u16* indices;

    section->index_buffer->Lock(section->index_offset,
                                section->index_count * section->index_size,
                                (void**)&indices,
                                0);

    return indices;
}

// sub_9E2AB0
void ngl::d3d9::unlock_section_indices(mesh_section* section) {
    section->index_buffer->Unlock();
}

// sub_9E2AC0
void* ngl::d3d9::lock_section_vertices(mesh_section* section) {
    void* vertices;

    section->vertex_buffer->Lock( section->vertex_offset,
                                  section->vertex_count * section->vertex_size,
                                 &vertices,
                                  0);

    return vertices;
}

// sub_9E2AF0
void ngl::d3d9::unlock_section_vertices(mesh_section* section) {
    section->vertex_buffer->Unlock();
}
