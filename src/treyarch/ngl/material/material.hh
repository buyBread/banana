#pragma once

#include "treyarch/apk/file.hh"
#include "treyarch/ngl/shaders/shader.hh"
#include "treyarch/base/tl_skip_list.hh"
#include "game/fixed_string.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

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

    struct material_directory_t : tl_skip_list<material, material_name> {};

    material &get_default_material();
    void initialize_default_material(shader* empty_shader);
    void process_material(material* value);
    void initialize_material_directory();

    void apk_material_load_callback(apk::file*       owner,
                                    apk::file_entry* entry,
                                    void**           mapped_sections,
                                    void*            user_data);
    void apk_material_delete_callback(apk::file*       owner,
                                      apk::file_entry* entry,
                                      void**           mapped_sections,
                                      void*            user_data);

    namespace references {
        inline util::memory_reference<material>             default_material      { 0x01116160 };
        inline util::memory_reference<fixed_string>         default_material_name { 0x01116174 };
        inline util::memory_reference<material_directory_t> material_directory    { 0x011161F0 };
    } // references

    ASSERT_SIZEOF  (material,                 0x14);
    ASSERT_OFFSETOF(material, name,           0x00);
    ASSERT_OFFSETOF(material, shader_data,    0x08);
    ASSERT_OFFSETOF(material, binary_version, 0x0C);
    ASSERT_OFFSETOF(material, runtime_data,   0x10);

    ASSERT_SIZEOF(material_directory_t, 0x10);
}} // treyarch::ngl
