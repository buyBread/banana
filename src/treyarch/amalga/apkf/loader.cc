#include "treyarch/amalga/apkf/loader.hh"

using namespace treyarch;

bool amalga::apkf::file::relocate_in_place(data_reference* &resource_references,
                                           u8*             &string_base) {

    flags |= file_loaded_in_place;

    if (flags & file_big_endian)
        return false;

    if (!section_count) {
        sections   = nullptr;
        file_types = nullptr;

        return true;
    }

    sections   = (file_section*)((u8*)&sections + (u32)sections);
    file_types = (file_type*)((u8*)this + sizeof(file));

    for (u32 index = 0; index < section_count; ++index) {
        file_section     &section = sections[index];
        section_handler*  handler = find_section_handler(section.name);

        if ((section.flags & section_external) && handler && handler->resolve) {
            void* data = handler->resolve(this, &section, handler->user_data);

            if (data) {
                section.data = data;

                continue;
            }
        }

        section.data = (u8*)&section.data + (u32)section.data;
    }

    u32 section_offsets[maximum_section_count] {};

    for (file_type* type = file_types; type->type; type = type->next(section_count)) {
        type->entries = (file_entry*)((u8*)&type->entries + (u32)type->entries);

        u8* entry_data = (u8*)type->entries;

        for (u32 entry_index = 0; entry_index < type->entry_count; ++entry_index) {
            auto entry = (file_entry*)entry_data;

            if (entry->name.text)
                entry->name.text = (char*)((u8*)entry + (u32)entry->name.text);

            for (u32 section_index = 0; section_index < section_count; ++section_index) {
                const section_mapping &mapping = type->mapping(section_index);
                u8                     slot    = mapping.section_slot();

                if (slot == absent_section_slot)
                    continue;

                u32 serialized_span = (u32)entry->section(slot);

                section_offsets[slot]  = mapping.align(section_offsets[slot]);
                entry->section(slot)   = (u8*)sections[section_index].data + section_offsets[slot];
                section_offsets[slot] += serialized_span;
            }

            entry_data += type->entry_stride();
            string_base = entry_data;
        }
    }

    u8* fixup_bytes = (u8*)sections + section_count * sizeof(file_section);

    for (u32 index = 0; index < section_count; ++index) {
        file_section &section = sections[index];

        if (!(section.flags & section_external))
            fixup_bytes = (u8*)section.data + section.size;
    }

    auto fixup_data = (data_reference*)(((u32)fixup_bytes + sizeof(u32) - 1) & ~(sizeof(u32) - 1));

    apply_fixups(fixup_data, string_base);

    resource_references = fixup_data;

    return true;
}

amalga::apkf::file* amalga::apkf::load_file_in_place(void* image) {
    auto header = (file_header*)image;

    if (header->identifier != file_identifier)
        return nullptr;

    if ((u16)header->version != file_version)
        return nullptr;

    auto owner = (file*)(header + 1);

    data_reference* resource_references = nullptr;
    u8*             string_base         = nullptr;

    if (!owner->relocate_in_place(resource_references, string_base))
        return nullptr;

    owner->apply_references(resource_references, string_base);
    owner->invoke_load_callbacks();

    return owner;
}
