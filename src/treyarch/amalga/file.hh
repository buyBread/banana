#pragma once

#include "treyarch/shared/hash/string_hash.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace amalga {
    struct file_entry;

    struct file_section {
        string_hash name;
        u32         flags;
        u32         alignment;
        u32         memory_flags;
        u32         size;
        void*       data;
    };

    struct file {
        u32           flags;
        i32           last_frame_reference;
        u32           section_count;
        file_section* sections;
        void*         file_types;

        i32 find_section_index(string_hash name) const;
        u32 get_section_span(file_entry* entry, i32 section_index, u32 occurrence) const;
    };

    using file_callback = void (__cdecl*)(file*       owner,
                                          file_entry* entry,
                                          void**      mapped_sections,
                                          void*       user_data);

    struct file_type_handler {
        u32                type;
        u32                version;
        file_callback      load;
        file_callback      remove;
        void*              user_data;
        file_type_handler* next;
    };

    constexpr u32 four_cc(char a,
                          char b,
                          char c,
                          char d = '\0') {

        return (u32)(u8)a
             | ((u32)(u8)b <<  8)
             | ((u32)(u8)c << 16)
             | ((u32)(u8)d << 24);
    }

    file_type_handler* __cdecl register_file_type(u32           type,
                                                  u32           version,
                                                  file_callback load,
                                                  file_callback remove,
                                                  void*         user_data = nullptr);

    namespace references {
        inline util::memory_reference<file_type_handler*> file_type_handlers { 0x01126B70 };
    } // references

    ASSERT_SIZEOF  (file_section,               0x18);
    ASSERT_OFFSETOF(file_section, name,         0x00);
    ASSERT_OFFSETOF(file_section, flags,        0x04);
    ASSERT_OFFSETOF(file_section, alignment,    0x08);
    ASSERT_OFFSETOF(file_section, memory_flags, 0x0C);
    ASSERT_OFFSETOF(file_section, size,         0x10);
    ASSERT_OFFSETOF(file_section, data,         0x14);

    ASSERT_SIZEOF  (file,                       0x14);
    ASSERT_OFFSETOF(file, flags,                0x00);
    ASSERT_OFFSETOF(file, last_frame_reference, 0x04);
    ASSERT_OFFSETOF(file, section_count,        0x08);
    ASSERT_OFFSETOF(file, sections,             0x0C);
    ASSERT_OFFSETOF(file, file_types,           0x10);

    ASSERT_SIZEOF  (file_type_handler,            0x18);
    ASSERT_OFFSETOF(file_type_handler, type,      0x00);
    ASSERT_OFFSETOF(file_type_handler, version,   0x04);
    ASSERT_OFFSETOF(file_type_handler, load,      0x08);
    ASSERT_OFFSETOF(file_type_handler, remove,    0x0C);
    ASSERT_OFFSETOF(file_type_handler, user_data, 0x10);
    ASSERT_OFFSETOF(file_type_handler, next,      0x14);
}} // treyarch::amalga
