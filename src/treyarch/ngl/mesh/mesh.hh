#pragma once

#include <d3d9.h>

#include "treyarch/amalga/file.hh"
#include "treyarch/ngl/d3d9/vertex_definition.hh"
#include "treyarch/shared/container/skip_list.hh"
#include "treyarch/shared/fixed_string.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl {
    struct material;
    struct mesh;

    struct mesh_section {
        f32                          sphere[4];
        u32                          unk_010;
        u32                          unk_014;
        u32                          unk_018;
        u32                          unk_01c;
        material*                    material_data;
        u16*                         bone_indices;
        i32                          bone_count;
        IDirect3DVertexBuffer9*      vertex_buffer;
        i32                          vertex_offset;
        u32                          vertex_count;
        u32                          vertex_size;
        IDirect3DIndexBuffer9*       index_buffer;
        i32                          index_offset;
        u32                          index_count;
        u32                          index_size;
        vertex_definition*           vertex_definition_data;
        i32                          primitive_type;
        i32                          binary_version;
    };

    struct mesh_section_table_entry {
        u32           flags;
        mesh_section* section;
    };

    struct mesh_lod {
        mesh* value;
        f32   range;
    };

    struct mesh {
        fixed_string              name;
        u32                       flags;
        u32                       section_count;
        mesh_section_table_entry* sections;
        void*                     skeleton;
        u32                       lod_count;
        mesh_lod*                 lods;
        u32                       polygon_count;
        f32                       sphere[4];
        u32                       unk_034;
        u32                       unk_038;
        u32                       unk_03c;
        amalga::file*             owner_file;
        u32                       unk_044;
        i32                       last_frame_reference;
        u32                       pad_04c;
        u32                       pad_050;
    };

    struct mesh_name {
        static string_hash get(const mesh* value) {
            return value->name.hash;
        }
    };

    struct mesh_directory : container::skip_list<mesh, mesh_name> {};

    void upload_mesh_section(mesh_section* section);
    void release_mesh_section(mesh_section* section);
    void process_mesh_section(mesh* owner, mesh_section* section);
    void process_mesh(mesh* value);
    void release_mesh(mesh* value);
    
    void initialize_mesh_directory();

    void __cdecl load_mesh(amalga::file*       owner,
                           amalga::file_entry* entry,
                           void**              mapped_sections,
                           void*               user_data);
    void __cdecl remove_mesh(amalga::file*       owner,
                             amalga::file_entry* entry,
                             void**              mapped_sections,
                             void*               user_data);

    namespace references {
        inline util::memory_reference<mesh_directory> meshes { 0x01116190 };
    } // references

    ASSERT_SIZEOF  (mesh_section,                         0x58);
    ASSERT_OFFSETOF(mesh_section, sphere,                 0x00);
    ASSERT_OFFSETOF(mesh_section, unk_010,                0x10);
    ASSERT_OFFSETOF(mesh_section, unk_014,                0x14);
    ASSERT_OFFSETOF(mesh_section, unk_018,                0x18);
    ASSERT_OFFSETOF(mesh_section, unk_01c,                0x1C);
    ASSERT_OFFSETOF(mesh_section, material_data,          0x20);
    ASSERT_OFFSETOF(mesh_section, bone_indices,           0x24);
    ASSERT_OFFSETOF(mesh_section, bone_count,             0x28);
    ASSERT_OFFSETOF(mesh_section, vertex_buffer,          0x2C);
    ASSERT_OFFSETOF(mesh_section, vertex_offset,          0x30);
    ASSERT_OFFSETOF(mesh_section, vertex_count,           0x34);
    ASSERT_OFFSETOF(mesh_section, vertex_size,            0x38);
    ASSERT_OFFSETOF(mesh_section, index_buffer,           0x3C);
    ASSERT_OFFSETOF(mesh_section, index_offset,           0x40);
    ASSERT_OFFSETOF(mesh_section, index_count,            0x44);
    ASSERT_OFFSETOF(mesh_section, index_size,             0x48);
    ASSERT_OFFSETOF(mesh_section, vertex_definition_data, 0x4C);
    ASSERT_OFFSETOF(mesh_section, primitive_type,         0x50);
    ASSERT_OFFSETOF(mesh_section, binary_version,         0x54);

    ASSERT_SIZEOF  (mesh_section_table_entry,          0x08);
    ASSERT_OFFSETOF(mesh_section_table_entry, flags,   0x00);
    ASSERT_OFFSETOF(mesh_section_table_entry, section, 0x04);

    ASSERT_SIZEOF  (mesh_lod,        0x08);
    ASSERT_OFFSETOF(mesh_lod, value, 0x00);
    ASSERT_OFFSETOF(mesh_lod, range, 0x04);

    ASSERT_SIZEOF  (mesh,                       0x54);
    ASSERT_OFFSETOF(mesh, name,                 0x00);
    ASSERT_OFFSETOF(mesh, flags,                0x08);
    ASSERT_OFFSETOF(mesh, section_count,        0x0C);
    ASSERT_OFFSETOF(mesh, sections,             0x10);
    ASSERT_OFFSETOF(mesh, skeleton,             0x14);
    ASSERT_OFFSETOF(mesh, lod_count,            0x18);
    ASSERT_OFFSETOF(mesh, lods,                 0x1C);
    ASSERT_OFFSETOF(mesh, polygon_count,        0x20);
    ASSERT_OFFSETOF(mesh, sphere,               0x24);
    ASSERT_OFFSETOF(mesh, unk_034,              0x34);
    ASSERT_OFFSETOF(mesh, unk_038,              0x38);
    ASSERT_OFFSETOF(mesh, unk_03c,              0x3C);
    ASSERT_OFFSETOF(mesh, owner_file,           0x40);
    ASSERT_OFFSETOF(mesh, unk_044,              0x44);
    ASSERT_OFFSETOF(mesh, last_frame_reference, 0x48);

    ASSERT_SIZEOF(mesh_directory, 0x10);
}} // treyarch::ngl
