#pragma once

#include "treyarch/amalga/file.hh"
#include "treyarch/shared/container/skip_list.hh"
#include "treyarch/shared/fixed_string.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl {
    struct morph_component {
        u32   value_count;
        u32   type;
        void* data;
        u32   stride;
    };

    struct morph_section {
        u32              vertex_count;
        u32              component_count;
        morph_component* components;
    };

    struct morph_frame {
        u32            flags;
        u32            section_count;
        morph_section* sections;
    };

    struct morph_set {
        fixed_string name;
        u32          frame_count;
        morph_frame* frames;
    };

    struct morph_set_name {
        static string_hash get(const morph_set* value) {
            return value->name.hash;
        }
    };

    struct morph_directory : container::skip_list<morph_set, morph_set_name> {};

    void initialize_morph_directory();

    void __cdecl load_morph(amalga::file*       owner,
                            amalga::file_entry* entry,
                            void**              mapped_sections,
                            void*               user_data);
    void __cdecl remove_morph(amalga::file*       owner,
                              amalga::file_entry* entry,
                              void**              mapped_sections,
                              void*               user_data);

    namespace references {
        inline util::memory_reference<morph_directory> morphs { 0x01118660 };
    } // references

    ASSERT_SIZEOF  (morph_component,              0x10);
    ASSERT_OFFSETOF(morph_component, value_count, 0x00);
    ASSERT_OFFSETOF(morph_component, type,        0x04);
    ASSERT_OFFSETOF(morph_component, data,        0x08);
    ASSERT_OFFSETOF(morph_component, stride,      0x0C);

    ASSERT_SIZEOF  (morph_section,                  0x0C);
    ASSERT_OFFSETOF(morph_section, vertex_count,    0x00);
    ASSERT_OFFSETOF(morph_section, component_count, 0x04);
    ASSERT_OFFSETOF(morph_section, components,      0x08);

    ASSERT_SIZEOF  (morph_frame,                0x0C);
    ASSERT_OFFSETOF(morph_frame, flags,         0x00);
    ASSERT_OFFSETOF(morph_frame, section_count, 0x04);
    ASSERT_OFFSETOF(morph_frame, sections,      0x08);

    ASSERT_SIZEOF  (morph_set,              0x10);
    ASSERT_OFFSETOF(morph_set, name,        0x00);
    ASSERT_OFFSETOF(morph_set, frame_count, 0x08);
    ASSERT_OFFSETOF(morph_set, frames,      0x0C);

    ASSERT_SIZEOF(morph_directory, 0x10);
}} // treyarch::ngl
