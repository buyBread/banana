#pragma once

#include <d3d9.h>

#include "treyarch/apk/file.hh"
#include "treyarch/ngl/gpu/d3d9/vertex_definition.hh"
#include "treyarch/ngl/list/render_node.hh"
#include "treyarch/base/tl_skip_list.hh"
#include "game/fixed_string.hh"
#include "treyarch/math/vector.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace ngl {
    struct material;
    struct mesh;

    struct mesh_section {
        math::vector4           sphere;
        u32                     unk_010;
        u32                     unk_014;
        u32                     unk_018;
        u32                     unk_01c;
        material*               material_data;
        u16*                    bone_indices;
        i32                     bone_count;
        IDirect3DVertexBuffer9* vertex_buffer;
        i32                     vertex_offset;
        u32                     vertex_count;
        u32                     vertex_size;
        IDirect3DIndexBuffer9*  index_buffer;
        i32                     index_offset;
        u32                     index_count;
        u32                     index_size;
        vertex_definition*      vertex_definition_data;
        i32                     primitive_type;
        i32                     binary_version;
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
        math::vector4             sphere;
        math::vector4             position_scale;
        apk::file* owner_file;
        u32                       polygon_count;
        i32                       last_frame_reference;
        u32                       pad_04c;
        u32                       pad_050;
    };

    struct mesh_name {
        static string_hash get(const mesh* value) {
            return value->name.hash;
        }
    };

    struct mesh_directory_t : tl_skip_list<mesh, mesh_name> {};

    void ngli_process_section(mesh_section* section);
    void ngli_release_section(mesh_section* section);
    void process_section(mesh* owner, mesh_section* section);
    void process_mesh(mesh* value);
    void unload_mesh(mesh* value);
    
    void initialize_mesh_directory();

    void apk_mesh_load_callback(apk::file*       owner,
                                apk::file_entry* entry,
                                void**           mapped_sections,
                                void*            user_data);
    void apk_mesh_delete_callback(apk::file*       owner,
                                  apk::file_entry* entry,
                                  void**           mapped_sections,
                                  void*            user_data);

    void mesh_init();

    mesh* create_transient_mesh(u32 flags, u32 section_count);
    u32   attach_transient_mesh_section(mesh*         value,
                                        mesh_section* section,
                                        material*     section_material,
                                        u32           flags);

    struct sm_lod_mesh_instance_node : render_node {
        // sub_5B4A70
        void render() override {}

        // sub_8B01A0
        void get_desc(char* desc) override {
            std::strcpy(desc, "sm_lod_mesh_instance_node");
        }
    };

    namespace references {
        inline util::memory_reference<mesh_directory_t> mesh_directory                    { 0x01116190 };
        inline util::memory_reference<void*>            sm_lod_mesh_instance_node_vftable { 0x00BE5348 };
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
    ASSERT_OFFSETOF(mesh, sphere,               0x20);
    ASSERT_OFFSETOF(mesh, position_scale,       0x30);
    ASSERT_OFFSETOF(mesh, owner_file,           0x40);
    ASSERT_OFFSETOF(mesh, polygon_count,        0x44);
    ASSERT_OFFSETOF(mesh, last_frame_reference, 0x48);

    ASSERT_SIZEOF(mesh_directory_t, 0x10);
}} // treyarch::ngl
