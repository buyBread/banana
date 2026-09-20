#include <bit>

#include "treyarch/ngl/d3d9/scratch_mesh.hh"
#include "treyarch/ngl/d3d9/work_buffers.hh"
#include "treyarch/ngl/list/arena.hh"

using namespace treyarch;

ngl::mesh_section* ngl::d3d9::allocate_scratch_mesh_section(D3DPRIMITIVETYPE   primitive_type,
                                                            u32                index_count,
                                                            u32                vertex_count,
                                                            vertex_definition* definition) {

    auto* section = (mesh_section*)ngl::list::allocate(0x60, 16);

    section->sphere.x = 0.0f;
    section->sphere.y = 0.0f;
    section->sphere.z = 0.0f;
    section->sphere.w = std::bit_cast<f32>(0x749DC5AEu);
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

u16* ngl::d3d9::lock_scratch_mesh_indices(mesh_section* section) {
    u16* indices;

    section->index_buffer->Lock(section->index_offset,
                                section->index_count * section->index_size,
                                (void**)&indices,
                                0);

    return indices;
}

void ngl::d3d9::unlock_scratch_mesh_indices(mesh_section* section) {
    section->index_buffer->Unlock();
}

void* ngl::d3d9::lock_scratch_mesh_vertices(mesh_section* section) {
    void* vertices;

    section->vertex_buffer->Lock( section->vertex_offset,
                                  section->vertex_count * section->vertex_size,
                                 &vertices,
                                  0);

    return vertices;
}

void ngl::d3d9::unlock_scratch_mesh_vertices(mesh_section* section) {
    section->vertex_buffer->Unlock();
}
