#pragma once

#include "game/fixed_string.hh"
#include "game/four_cc.hh"
#include "treyarch/mash/string_hash.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace apk {
    constexpr u32 file_identifier          = four_cc('A', 'P', 'K', 'F');
    constexpr u16 file_version             = 0x0107;
    constexpr u32 maximum_section_count    = 63;
    constexpr u8  absent_section_slot      = 0xFF;
    constexpr u32 string_section_index     = 0x3F;
    constexpr u32 section_index_shift      = 26;
    constexpr u32 section_word_offset_mask = 0x03FFFFFF;

    // APK_FILE_*
    enum e_file_flags : u32 {
        file_loaded_in_place = 0x01,
        file_from_file_buf   = 0x02,
        file_big_endian      = 0x04
    };

    enum e_section_flags : u32 {
        section_external = 0x02
    };

    struct file_header {
        u32 tag;
        u32 version;
    };

    struct data_reference {
        u32 value;

        bool is_terminator() const {
            return value == U32_MAX;
        }

        u32 section_index() const {
            return value >> section_index_shift;
        }

        u32 byte_offset() const {
            return (value & section_word_offset_mask) * sizeof(u32);
        }
    };

    struct section_mapping {
        u32 value;

        u8 section_slot() const {
            return (u8)(value >> 24);
        }

        u32 alignment() const {
            return value & 0x00FFFFFF;
        }

        u32 align(u32 offset) const {
            u32 required = alignment();

            return required ?
                (offset + required - 1) & ~(required - 1) : offset;
        }
    };

    struct file;

    struct file_entry {
        fixed_string name;

        void* &section(u32 slot) {
            return *(void**)((u8*)this + sizeof(file_entry) + slot * sizeof(void*));
        }

        const void* section(u32 slot) const {
            return *(void* const*)((const u8*)this + sizeof(file_entry) + slot * sizeof(void*));
        }

        void* get_data(const file* owner, u32 type, u32 section_index, u32 occurrence) const;
    };

    struct file_type_entry {
        u32         type;
        u32         version;
        u32         n_sections;
        file_entry* first_entry;
        u32         n_entries;

        section_mapping &mapping(u32 section_index) {
            return *(section_mapping*)((u8*)this + sizeof(file_type_entry) + section_index * sizeof(section_mapping));
        }

        const section_mapping &mapping(u32 section_index) const {
            return *(const section_mapping*)((const u8*)this + sizeof(file_type_entry) + section_index * sizeof(section_mapping));
        }

        u32 entry_stride() const {
            return sizeof(file_entry) + n_sections * sizeof(void*);
        }

        file_type_entry* next(u32 file_section_count) {
            return (file_type_entry*)((u8*)this +
                sizeof(file_type_entry) + file_section_count * sizeof(section_mapping));
        }

        const file_type_entry* next(u32 file_section_count) const {
            return (const file_type_entry*)((const u8*)this + sizeof(file_type_entry) + file_section_count * sizeof(section_mapping));
        }
    };

    struct file_section {
        string_hash name;
        u32         flags;
        u32         alignment;
        u32         mem_flags;
        u32         size;
        void*       data;
    };

    struct resource_reference {
        data_reference target;
        u32            type;
        fixed_string   name;
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

    using resource_callback = void* (__cdecl*)(fixed_string* name, u32 type);

    struct file_callback_list_entry {
        u32                       type;
        u32                       version;
        file_callback             load;
        file_callback             remove;
        void*                     user_data;
        file_callback_list_entry* next;
    };

    struct section_callback_list_entry {
        string_hash                  name;
        section_resolver             resolve;
        section_callback             load;
        section_callback             remove;
        void*                        user_data;
        section_callback_list_entry* next;
    };

    struct file {
        u32              flags;
        i32              last_frame_ref;
        u32              n_sections;
        file_section*    sections;
        file_type_entry* file_types;

        i32 get_section_index(string_hash name) const;
        file_entry* get_file(string_hash name, u32 type) const;
        u32 get_data_size(file_entry* entry, i32 section_index, u32 occurrence) const;

        bool relocate_in_place(data_reference* &resource_references,
                               u8*             &string_base);

        void* resolve_data_reference(      data_reference reference,
                                     const u8*            string_base) const;

        void apply_fixups(      data_reference* &fixup_data,
                          const u8*              string_base);

        void apply_references(      data_reference* reference_data,
                              const u8*             string_base);

        u32 get_num_files() const;
        void invoke_section_load_callbacks();
        void invoke_file_type_load_callbacks(u32 start, u32 count);
        void invoke_file_type_load_callbacks();
        void invoke_load_callbacks();
        void invoke_delete_callbacks();
    };

    void delete_file(file* value);

    file_callback_list_entry* find_file_callback(u32 type, u32 version);
    section_callback_list_entry* find_section_callback(string_hash name);

    file_callback_list_entry* register_file_type(u32           type,
                                                 u32           version,
                                                 file_callback load,
                                                 file_callback remove,
                                                 void*         user_data = nullptr);

    section_callback_list_entry* register_section_type(string_hash      name,
                                                       section_resolver resolve,
                                                       section_callback load,
                                                       section_callback remove,
                                                       void*            user_data = nullptr);

    void set_resource_callback(resource_callback callback);
    void* resolve_resource(fixed_string* name, u32 type);

    namespace references {
        inline util::memory_reference<file_callback_list_entry*>    file_callback_list        { 0x01126B70 };
        inline util::memory_reference<section_callback_list_entry*> section_callback_list     { 0x01126B74 };
        inline util::memory_reference<resource_callback>            resource_locator_callback { 0x01126B78 };
    } // references

    ASSERT_SIZEOF  (file_header,          0x08);
    ASSERT_OFFSETOF(file_header, tag,     0x00);
    ASSERT_OFFSETOF(file_header, version, 0x04);

    ASSERT_SIZEOF(data_reference,  0x04);
    ASSERT_SIZEOF(section_mapping, 0x04);

    ASSERT_SIZEOF  (file_entry,       0x08);
    ASSERT_OFFSETOF(file_entry, name, 0x00);

    ASSERT_SIZEOF  (file_type_entry,              0x14);
    ASSERT_OFFSETOF(file_type_entry, type,        0x00);
    ASSERT_OFFSETOF(file_type_entry, version,     0x04);
    ASSERT_OFFSETOF(file_type_entry, n_sections,  0x08);
    ASSERT_OFFSETOF(file_type_entry, first_entry, 0x0C);
    ASSERT_OFFSETOF(file_type_entry, n_entries,   0x10);

    ASSERT_SIZEOF  (file_section,            0x18);
    ASSERT_OFFSETOF(file_section, name,      0x00);
    ASSERT_OFFSETOF(file_section, flags,     0x04);
    ASSERT_OFFSETOF(file_section, alignment, 0x08);
    ASSERT_OFFSETOF(file_section, mem_flags, 0x0C);
    ASSERT_OFFSETOF(file_section, size,      0x10);
    ASSERT_OFFSETOF(file_section, data,      0x14);

    ASSERT_SIZEOF  (resource_reference,         0x10);
    ASSERT_OFFSETOF(resource_reference, target, 0x00);
    ASSERT_OFFSETOF(resource_reference, type,   0x04);
    ASSERT_OFFSETOF(resource_reference, name,   0x08);

    ASSERT_SIZEOF  (file,                 0x14);
    ASSERT_OFFSETOF(file, flags,          0x00);
    ASSERT_OFFSETOF(file, last_frame_ref, 0x04);
    ASSERT_OFFSETOF(file, n_sections,     0x08);
    ASSERT_OFFSETOF(file, sections,       0x0C);
    ASSERT_OFFSETOF(file, file_types,     0x10);

    ASSERT_SIZEOF  (file_callback_list_entry,            0x18);
    ASSERT_OFFSETOF(file_callback_list_entry, type,      0x00);
    ASSERT_OFFSETOF(file_callback_list_entry, version,   0x04);
    ASSERT_OFFSETOF(file_callback_list_entry, load,      0x08);
    ASSERT_OFFSETOF(file_callback_list_entry, remove,    0x0C);
    ASSERT_OFFSETOF(file_callback_list_entry, user_data, 0x10);
    ASSERT_OFFSETOF(file_callback_list_entry, next,      0x14);

    ASSERT_SIZEOF  (section_callback_list_entry,            0x18);
    ASSERT_OFFSETOF(section_callback_list_entry, name,      0x00);
    ASSERT_OFFSETOF(section_callback_list_entry, resolve,   0x04);
    ASSERT_OFFSETOF(section_callback_list_entry, load,      0x08);
    ASSERT_OFFSETOF(section_callback_list_entry, remove,    0x0C);
    ASSERT_OFFSETOF(section_callback_list_entry, user_data, 0x10);
    ASSERT_OFFSETOF(section_callback_list_entry, next,      0x14);
}} // treyarch::apk
