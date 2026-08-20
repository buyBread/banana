#include <cstring>

#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/material/material.hh"
#include "treyarch/ngl/mesh/mesh.hh"
#include "treyarch/ngl/ngl.hh"
#include "treyarch/ngl/shaders/shader.hh"

using namespace treyarch;

void ngl::upload_mesh_section(mesh_section* section) {
    IDirect3DDevice9* device = ngl::d3d9::references::device.get();

    const void*     source_indices = section->index_buffer;
          u32       index_bytes    = section->index_count * section->index_size;
          D3DFORMAT index_format   = section->index_size == 2 ? D3DFMT_INDEX16 : D3DFMT_INDEX32;

    device->CreateIndexBuffer(index_bytes,
                              0,
                              index_format,
                              D3DPOOL_MANAGED,
                              &section->index_buffer,
                              nullptr);

    void* destination = nullptr;
    
    section->index_buffer->Lock(0, index_bytes, &destination, 0);
    std::memcpy(destination,
                source_indices,
                index_bytes);
    section->index_buffer->Unlock();

    const void* source_vertices = section->vertex_buffer;
          u32   vertex_bytes    = section->vertex_definition_data->vertex_size * section->vertex_count;

    device->CreateVertexBuffer(vertex_bytes,
                               0,
                               0,
                               D3DPOOL_MANAGED,
                               &section->vertex_buffer,
                               nullptr);

    section->vertex_buffer->Lock(0, vertex_bytes, &destination, 0);
    std::memcpy(destination,
                source_vertices,
                vertex_bytes);
    section->vertex_buffer->Unlock();

    device->CreateVertexDeclaration( section->vertex_definition_data->elements,
                                    &section->vertex_definition_data->declaration);
}

void ngl::release_mesh_section(mesh_section* section) {
    section->vertex_buffer->Release();
    section->index_buffer->Release();
}

void ngl::process_mesh_section(mesh* owner, mesh_section* section) {
    if (!section->material_data)
        section->material_data = &get_default_material();

    material* section_material = section->material_data;
    shader*   section_shader   = section_material->shader_data;

    if (section_shader && !section_shader->check_vertex_definition_version(section))
        section_material->shader_data = &get_default_shader();

    upload_mesh_section(section);

    section_shader = section_material->shader_data;
    section_shader->bind_section(section, owner);
}

void ngl::process_mesh(mesh* value) {
    constexpr u32 mesh_processed = 0x00010000;
    constexpr u32 section_owner  = 0x00000001;

    if (value->flags & mesh_processed)
        return;

    value->flags |= mesh_processed;

    for (u32 index = 0; index < value->section_count; ++index) {
        mesh_section_table_entry &entry = value->sections[index];
        
        entry.flags = section_owner;

        if (entry.section)
            process_mesh_section(value, entry.section);
    }
}

void ngl::release_mesh(mesh* value) {
    constexpr u32 mesh_processed = 0x00010000;
    constexpr u32 section_owner  = 0x00000001;

    if (!(value->flags & mesh_processed))
        return;

    for (u32 index = 0; index < value->section_count; ++index) {
        mesh_section_table_entry& entry = value->sections[index];

        if ((entry.flags & section_owner) && entry.section)
            release_mesh_section(entry.section);
    }

    value->flags &= ~mesh_processed;
}

void ngl::initialize_mesh_directory() {
    references::meshes.get().initialize();
}

void __cdecl ngl::load_mesh(amalga::file*       owner,
                            amalga::file_entry* entry,
                            void**              mapped_sections,
                            void*               user_data) {

    (void)user_data;

    constexpr u32 section_owner = 0x00000001;

    i32 image_section = owner->find_section_index(string_hash(amalga::four_cc('I', 'M', 'G')));
    
    auto value = (mesh*)mapped_sections[image_section];

    value->owner_file = owner;
    value->last_frame_reference = -1;

    process_mesh(value);

    i32 vrml_section = owner->find_section_index(string_hash(amalga::four_cc('V', 'R', 'M', 'L')));

    if (vrml_section != -1 && owner->get_section_span(entry, vrml_section, 0)) {
        for (u32 index = 0; index < value->section_count; ++index)
            value->sections[index].flags &= ~section_owner;
    }

    if (!references::resource_callback.read())
        references::meshes.get().insert(value);
}

void __cdecl ngl::remove_mesh(amalga::file*       owner,
                              amalga::file_entry* entry,
                              void**              mapped_sections,
                              void*               user_data) {

    (void)entry;
    (void)user_data;

    i32 image_section = owner->find_section_index(string_hash(amalga::four_cc('I', 'M', 'G')));
    
    mesh* value = (mesh*)mapped_sections[image_section];

    if (!references::resource_callback.read())
        references::meshes.get().erase(value);

    if (value->last_frame_reference + 1 >= (i32)references::frame_epoch.read())
        d3d9::wait_for_rendering();

    release_mesh(value);
}
