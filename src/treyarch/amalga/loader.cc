#include <cstring>

#include "treyarch/amalga/loader.hh"

using namespace treyarch;

static constexpr u32 apkf_version              = 0x0107;
static constexpr u32 absent_section_slot       = 0xFF;
static constexpr u32 relocation_section_shift  = 26;
static constexpr u32 relocation_offset_mask    = 0x03FFFFFF;
static constexpr u32 relocation_string_section = 0x3F;

static u32 align_offset(u32 value, u32 alignment) {
    if (!alignment)
        return value;

    return (value + alignment - 1) & ~(alignment - 1);
}

static amalga::section_handler* find_section_handler(string_hash name) {
    for (amalga::section_handler* handler = amalga::references::section_handlers.read(); handler; handler = handler->next) {
        if (handler->name == name)
            return handler;
    }

    return nullptr;
}

static amalga::file_type_handler* find_file_type_handler(u32 type, u32 version) {
    for (amalga::file_type_handler* handler = amalga::references::file_type_handlers.read(); handler; handler = handler->next) {
        if (handler->type == type && handler->version == version)
            return handler;
    }

    return nullptr;
}

static void* resolve_relocation(      amalga::file* owner,
                                      u32           encoded,
                                const u8*           string_base) {

    u32 section_index = encoded >> relocation_section_shift;
    u32 byte_offset   = (encoded & relocation_offset_mask) * sizeof(u32);

    if (section_index == relocation_string_section)
        return (void*)(string_base + byte_offset);

    return (u8*)owner->sections[section_index].data + byte_offset;
}

static void apply_pointer_relocations(      amalga::file*  owner,
                                            u32*          &cursor,
                                      const u8*            string_base) {

    while (*cursor != 0xFFFFFFFF) {
        auto target = (void**)resolve_relocation(owner, *cursor++, string_base);

        *target = resolve_relocation(owner, (u32)*target, string_base);
    }

    ++cursor;
}

static void apply_resource_relocations(      amalga::file* owner,
                                             u32*          cursor,
                                       const u8*           string_base) {

    if (!cursor)
        return;

    u32   previous_type = 0;
    u32   previous_hash = 0;
    void* previous_value = nullptr;
    bool  have_previous = false;

    while (*cursor != 0xFFFFFFFF) {
        auto target = (void**)resolve_relocation(owner, *cursor++, string_base);
        u32  type   = *cursor++;
        auto name   = (fixed_string*)cursor;

        if ((u32)name->text & 1)
            name->text = (char*)(string_base + ((u32)name->text & ~1u));

        cursor += sizeof(fixed_string) / sizeof(u32);

        if (have_previous && previous_type == type &&
            previous_hash == name->hash.source_hash_code) {

            *target = previous_value;
            continue;
        }

        amalga::resource_resolver resolver = amalga::references::resource_resolver.read();

        *target = resolver ? resolver(name, type) : nullptr;

        previous_type  = type;
        previous_hash  = name->hash.source_hash_code;
        previous_value = *target;
        have_previous  = true;
    }
}

static void dispatch_section_handlers(amalga::file* owner) {
    for (u32 index = 0; index < owner->section_count; ++index) {
        amalga::file_section     &section = owner->sections[index];
        amalga::section_handler*  handler = find_section_handler(section.name);

        if (handler && handler->load)
            handler->load(owner, &section, handler->user_data);
    }
}

static void dispatch_file_types(amalga::file* owner) {
    if (!owner->file_types)
        return;

    for (amalga::file_type* type = owner->file_types; type->type; type = type->next(owner->section_count)) {
        amalga::file_type_handler* handler = find_file_type_handler(type->type, type->version);

        if (!handler || !handler->load)
            continue;

        auto entry_data   = (u8*)type->entries;
        u32  entry_stride = sizeof(amalga::file_entry) + type->entry_section_count * sizeof(void*);

        for (u32 entry_index = 0; entry_index < type->entry_count; ++entry_index) {
            auto entry = (amalga::file_entry*)entry_data;
            
            void* mapped_sections[63] {};

            for (u32 section_index = 0; section_index < owner->section_count; ++section_index) {
                u32 mapping = type->section_mapping(section_index);
                u32 slot = mapping >> 24;

                if (slot != absent_section_slot)
                    mapped_sections[section_index] = entry->section(slot);
            }

            handler->load(owner, entry, mapped_sections, handler->user_data);
            
            entry_data += entry_stride;
        }
    }
}

static amalga::file* relocate_file(void*  image,
                                   u32*  &resource_relocations,
                                   u8*   &string_base) {

    auto bytes = (u8*)image;

    if (std::memcmp(bytes, "APKF", 4) != 0)
        return nullptr;

    if ((*(u32*)(bytes + 4) & 0xFFFF) != apkf_version)
        return nullptr;

    auto owner = (amalga::file*)(bytes + 8);

    owner->flags |= 1;

    if (owner->flags & 4)
        return nullptr;

    if (!owner->section_count) {
        owner->sections   = nullptr;
        owner->file_types = nullptr;

        return owner;
    }

    owner->sections   = (amalga::file_section*)((u8*)&owner->sections + (u32)owner->sections);
    owner->file_types = (amalga::file_type*)((u8*)owner + sizeof(amalga::file));

    for (u32 index = 0; index < owner->section_count; ++index) {
        amalga::file_section     &section = owner->sections[index];
        amalga::section_handler*  handler = find_section_handler(section.name);

        if ((section.flags & 2) && handler && handler->resolve) {
            void* data = handler->resolve(owner, &section, handler->user_data);

            if (data) {
                section.data = data;

                continue;
            }
        }

        section.data = (u8*)&section.data + (u32)section.data;
    }

    u32 section_offsets[63] {};

    for (amalga::file_type* type = owner->file_types; type->type; type = type->next(owner->section_count)) {

        type->entries = (amalga::file_entry*)((u8*)&type->entries +
            (u32)type->entries);

        u8* entry_data   = (u8*)type->entries;
        u32 entry_stride = sizeof(amalga::file_entry) + type->entry_section_count * sizeof(void*);

        for (u32 entry_index = 0; entry_index < type->entry_count; ++entry_index) {
            auto entry = (amalga::file_entry*)entry_data;

            if (entry->name.text)
                entry->name.text = (char*)((u8*)entry + (u32)entry->name.text);

            for (u32 section_index = 0;
                 section_index < owner->section_count;
                 ++section_index) {

                u32 mapping   = type->section_mapping(section_index);
                u32 slot      = mapping >> 24;
                u32 alignment = mapping & 0x00FFFFFF;

                if (slot == absent_section_slot)
                    continue;

                u32 serialized_span = (u32)entry->section(slot);

                section_offsets[slot]  = align_offset(section_offsets[slot], alignment);
                entry->section(slot)   = (u8*)owner->sections[section_index].data + section_offsets[slot];
                section_offsets[slot] += serialized_span;
            }

            entry_data += entry_stride;
            
            string_base = entry_data;
        }
    }

    u8* relocation_data = (u8*)owner->sections +
        owner->section_count * sizeof(amalga::file_section);

    for (u32 index = 0; index < owner->section_count; ++index) {
        amalga::file_section &section = owner->sections[index];

        if (!(section.flags & 2))
            relocation_data = (u8*)section.data + section.size;
    }

    auto relocation_cursor = (u32*)align_offset((u32)relocation_data, sizeof(u32));

    apply_pointer_relocations(owner, relocation_cursor, string_base);
    
    resource_relocations = relocation_cursor;

    return owner;
}

amalga::file* amalga::load_in_place(void* image) {
    u32* resource_relocations = nullptr;
    u8*  string_base          = nullptr;

    file* owner = relocate_file(image, resource_relocations, string_base);

    if (!owner)
        return nullptr;

    apply_resource_relocations(owner, resource_relocations, string_base);
    dispatch_section_handlers(owner);
    dispatch_file_types(owner);

    return owner;
}
