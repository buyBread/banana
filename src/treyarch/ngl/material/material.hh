#pragma once

#include "treyarch/amalga/file.hh"
#include "treyarch/ngl/shaders/shader.hh"
#include "treyarch/shared/container/skip_list.hh"
#include "treyarch/shared/fixed_string.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl {
    struct material {
        fixed_string name;
        shader*      shader_data;
        i32          binary_version;
        void*        runtime_data;
    };

    struct material_name {
        static string_hash get(const material* value) {
            return value->name.hash;
        }
    };

    struct material_directory : container::skip_list<material, material_name> {};

    material &get_default_material();
    void process_material(material* value);
    void initialize_material_directory();

    void __cdecl load_material(amalga::file*       owner,
                               amalga::file_entry* entry,
                               void**              mapped_sections,
                               void*               user_data);
    void __cdecl remove_material(amalga::file*       owner,
                                 amalga::file_entry* entry,
                                 void**              mapped_sections,
                                 void*               user_data);

    namespace references {
        inline util::memory_reference<material>           default_material { 0x01116160 };
        inline util::memory_reference<material_directory> materials        { 0x011161F0 };
    } // references

    ASSERT_SIZEOF  (material,                 0x14);
    ASSERT_OFFSETOF(material, name,           0x00);
    ASSERT_OFFSETOF(material, shader_data,    0x08);
    ASSERT_OFFSETOF(material, binary_version, 0x0C);
    ASSERT_OFFSETOF(material, runtime_data,   0x10);

    ASSERT_SIZEOF(material_directory, 0x10);
}} // treyarch::ngl
