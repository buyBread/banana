#include "treyarch/amalga/apkf/file.hh"
#include "treyarch/shared/memory/memory.hh"

using namespace treyarch;

i32 amalga::apkf::file::find_section_index(string_hash section_name) const {
    for (u32 index = 0; index < section_count; ++index) {
        if (sections[index].name == section_name)
            return (i32)index;
    }

    return -1;
}

u32 amalga::apkf::file::get_section_span(file_entry* entry,
                                         i32         section_index,
                                         u32         occurrence) const {

    (void)occurrence;

    file_type* type = file_types;

    if (!type || section_index == -1 || !type->type)
        return 0;

    u32 section_offset = 0;

    do {
        if (type->entry_count) {
            u8* entry_data = (u8*)type->entries;

            for (u32 index = 0; index < type->entry_count; ++index) {
                auto current_entry = (file_entry*)entry_data;
                u8   section_slot  = type->mapping(section_index).section_slot();

                if (section_offset) {
                    if (section_slot != absent_section_slot)
                        return (u32)current_entry->section(section_slot) - section_offset;
                } else if (current_entry == entry) {
                    if (section_slot == absent_section_slot)
                        return 0;

                    section_offset = (u32)current_entry->section(section_slot);
                }

                entry_data += type->entry_stride();
            }
        }

        type = type->next(section_count);
    } while (type->type);

    if (section_offset) {
        const file_section &section = sections[section_index];

        return (u32)section.data + section.size - section_offset;
    }

    return 0;
}

void* amalga::apkf::file::resolve_data_reference(      data_reference reference,
                                                 const u8*            string_base) const {

    if (reference.section_index() == string_section_index)
        return (void*)(string_base + reference.byte_offset());

    return (u8*)sections[reference.section_index()].data + reference.byte_offset();
}

void amalga::apkf::file::apply_fixups(      data_reference* &fixup_data,
                                      const u8*              string_base) {

    while (!fixup_data->is_terminator()) {
        auto target = (data_reference*)resolve_data_reference(*fixup_data++, string_base);

        *(void**)target = resolve_data_reference(*target, string_base);
    }

    ++fixup_data;
}

void amalga::apkf::file::apply_references(      data_reference* reference_data,
                                          const u8*             string_base) {

    if (!reference_data)
        return;

    u32   previous_type  = 0;
    u32   previous_hash  = 0;
    void* previous_value = nullptr;
    bool  have_previous  = false;

    while (!reference_data->is_terminator()) {
        auto reference = (resource_reference*)reference_data;
        auto target    = (void**)resolve_data_reference(reference->target, string_base);

        if ((u32)reference->name.text & 1)
            reference->name.text = (char*)(string_base + ((u32)reference->name.text & ~1u));

        if (have_previous &&
            previous_type == reference->type &&
            previous_hash == reference->name.hash.source_hash_code) {

            *target = previous_value;
        } else {
            resource_resolver resolver = references::resource_resolver.read();

            *target = resolver ? resolver(&reference->name, reference->type) : nullptr;

            previous_type  = reference->type;
            previous_hash  = reference->name.hash.source_hash_code;
            previous_value = *target;
            have_previous  = true;
        }

        reference_data = (data_reference*)(reference + 1);
    }
}

void amalga::apkf::file::invoke_section_load_callbacks() {
    for (u32 index = 0; index < section_count; ++index) {
        file_section     &section = sections[index];
        section_handler*  handler = find_section_handler(section.name);

        if (handler && handler->load)
            handler->load(this, &section, handler->user_data);
    }
}

void amalga::apkf::file::invoke_file_type_load_callbacks() {
    if (!file_types)
        return;

    for (file_type* type = file_types; type->type; type = type->next(section_count)) {
        file_type_handler* handler = find_file_type_handler(type->type, type->version);

        if (!handler || !handler->load)
            continue;

        u8* entry_data = (u8*)type->entries;

        for (u32 entry_index = 0; entry_index < type->entry_count; ++entry_index) {
            auto entry = (file_entry*)entry_data;

            void* mapped_sections[maximum_section_count] {};

            for (u32 section_index = 0; section_index < section_count; ++section_index) {
                u8 section_slot = type->mapping(section_index).section_slot();

                if (section_slot != absent_section_slot)
                    mapped_sections[section_index] = entry->section(section_slot);
            }

            handler->load(this, entry, mapped_sections, handler->user_data);

            entry_data += type->entry_stride();
        }
    }
}

void amalga::apkf::file::invoke_load_callbacks() {
    invoke_section_load_callbacks();
    invoke_file_type_load_callbacks();
}

amalga::apkf::file_type_handler* amalga::apkf::find_file_type_handler(u32 type,
                                                                      u32 version) {

    for (file_type_handler* handler = references::file_type_handlers.read(); handler; handler = handler->next) {
        if (handler->type == type && handler->version == version)
            return handler;
    }

    return nullptr;
}

amalga::apkf::section_handler* amalga::apkf::find_section_handler(string_hash name) {
    for (section_handler* handler = references::section_handlers.read(); handler; handler = handler->next) {
        if (handler->name == name)
            return handler;
    }

    return nullptr;
}

amalga::apkf::file_type_handler* __cdecl amalga::apkf::register_file_type(u32           type,
                                                                          u32           version,
                                                                          file_callback load,
                                                                          file_callback remove,
                                                                          void*         user_data) {

    auto handler = (file_type_handler*)memory::allocate
        (sizeof(file_type_handler), 8, 0);

    handler->type      = type;
    handler->version   = version;
    handler->load      = load;
    handler->remove    = remove;
    handler->user_data = user_data;
    handler->next      = references::file_type_handlers.read();

    references::file_type_handlers.write(handler);

    return handler;
}

amalga::apkf::section_handler* __cdecl amalga::apkf::register_section_type(string_hash      name,
                                                                           section_resolver resolve,
                                                                           section_callback load,
                                                                           section_callback remove,
                                                                           void*            user_data) {

    auto handler = (section_handler*)memory::allocate
        (sizeof(section_handler), 8, 0);

    handler->name      = name;
    handler->resolve   = resolve;
    handler->load      = load;
    handler->remove    = remove;
    handler->user_data = user_data;
    handler->next      = references::section_handlers.read();

    references::section_handlers.write(handler);

    return handler;
}

void amalga::apkf::set_resource_resolver(resource_resolver resolver) {
    references::resource_resolver.write(resolver);
}
