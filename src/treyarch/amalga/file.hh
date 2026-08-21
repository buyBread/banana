#pragma once

#include "treyarch/shared/fixed_string.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace amalga {
    struct file;

    struct file_entry {
        fixed_string name;

        void* &section(u32 slot) {
            return *(void**)((u8*)this + sizeof(file_entry) + slot * sizeof(void*));
        }

        const void* section(u32 slot) const {
            return *(void* const*)((const u8*)this + sizeof(file_entry) + slot * sizeof(void*));
        }
    };

    struct file_type {
        u32         type;
        u32         version;
        u32         entry_section_count;
        file_entry* entries;
        u32         entry_count;

        u32 &section_mapping(u32 section_index) {
            return *((u32*)this + 5 + section_index);
        }

        const u32 &section_mapping(u32 section_index) const {
            return *((const u32*)this + 5 + section_index);
        }

        file_type* next(u32 file_section_count) {
            return (file_type*)((u8*)this + sizeof(file_type) + file_section_count * sizeof(u32));
        }

        const file_type* next(u32 file_section_count) const {
            return (const file_type*)((const u8*)this + sizeof(file_type) + file_section_count * sizeof(u32));
        }
    };

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
        file_type*    file_types;

        i32 find_section_index(string_hash name) const;
        u32 get_section_span(file_entry* entry, i32 section_index, u32 occurrence) const;
    };

    using file_callback = void (__cdecl*)(file*       owner,
                                          file_entry* entry,
                                          void**      mapped_sections,
                                          void*       user_data);

    using section_resolver = void* (__cdecl*)(file*         owner,
                                              file_section* section,
                                              void*         user_data);

    using section_callback = void (__cdecl*)(file*         owner,
                                             file_section* section,
                                             void*         user_data);

    using resource_resolver = void* (__cdecl*)(fixed_string* name, u32 type);

    struct file_type_handler {
        u32                type;
        u32                version;
        file_callback      load;
        file_callback      remove;
        void*              user_data;
        file_type_handler* next;
    };

    struct section_handler {
        string_hash      name;
        section_resolver resolve;
        section_callback load;
        section_callback remove;
        void*            user_data;
        section_handler* next;
    };

    file_type_handler* __cdecl register_file_type(u32           type,
                                                  u32           version,
                                                  file_callback load,
                                                  file_callback remove,
                                                  void*         user_data = nullptr);

    section_handler* __cdecl register_section_handler(string_hash       name,
                                                       section_resolver resolve,
                                                       section_callback load,
                                                       section_callback remove,
                                                       void*            user_data = nullptr);

    void set_resource_resolver(resource_resolver resolver);

    namespace references {
        inline util::memory_reference<file_type_handler*> file_type_handlers { 0x01126B70 };
        inline util::memory_reference<section_handler*>   section_handlers   { 0x01126B74 };
        inline util::memory_reference<resource_resolver>  resource_resolver  { 0x01126B78 };
    } // references

    ASSERT_SIZEOF  (file_entry,       0x08);
    ASSERT_OFFSETOF(file_entry, name, 0x00);

    ASSERT_SIZEOF  (file_type,                     0x14);
    ASSERT_OFFSETOF(file_type, type,                0x00);
    ASSERT_OFFSETOF(file_type, version,             0x04);
    ASSERT_OFFSETOF(file_type, entry_section_count, 0x08);
    ASSERT_OFFSETOF(file_type, entries,             0x0C);
    ASSERT_OFFSETOF(file_type, entry_count,         0x10);

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

    ASSERT_SIZEOF  (section_handler,            0x18);
    ASSERT_OFFSETOF(section_handler, name,      0x00);
    ASSERT_OFFSETOF(section_handler, resolve,   0x04);
    ASSERT_OFFSETOF(section_handler, load,      0x08);
    ASSERT_OFFSETOF(section_handler, remove,    0x0C);
    ASSERT_OFFSETOF(section_handler, user_data, 0x10);
    ASSERT_OFFSETOF(section_handler, next,      0x14);
}} // treyarch::amalga
