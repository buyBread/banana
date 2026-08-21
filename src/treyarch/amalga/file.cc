#include "treyarch/amalga/file.hh"
#include "treyarch/shared/memory/memory.hh"

using namespace treyarch;

i32 amalga::file::find_section_index(string_hash section_name) const {
    for (u32 index = 0; index < section_count; ++index) {
        if (sections[index].name == section_name)
            return (i32)index;
    }

    return -1;
}

u32 amalga::file::get_section_span(file_entry* entry,
                                   i32         section_index,
                                   u32         occurrence) const {

    (void)occurrence;

    u32* file_type = (u32*)file_types;

    if (!file_type || section_index == -1 || !file_type[0])
        return 0;

    u32 section_offset = 0;

    do {
        u32 entry_count = file_type[4];

        if (entry_count) {
            u8* current_entry = (u8*)file_type[3];

            for (u32 index = 0; index < entry_count; ++index) {
                u8 section_slot = *((u8*)&file_type[section_index + 5] + 3);

                if (section_offset) {
                    if (section_slot != 0xFF)
                        return *(u32*)(current_entry + 4 * section_slot + 8) - section_offset;
                } else if (current_entry == (u8*)entry) {
                    if (section_slot == 0xFF)
                        return 0;

                    section_offset = *(u32*)(current_entry + 4 * section_slot + 8);
                }

                current_entry += 4 * file_type[2] + 8;
            }
        }

        file_type += section_count + 5;
    } while (file_type[0]);

    if (section_offset) {
        const file_section &section = sections[section_index];
        
        return (u32)section.data + section.size - section_offset;
    }

    return 0;
}

amalga::file_type_handler* __cdecl amalga::register_file_type(u32           type,
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

amalga::section_handler* __cdecl amalga::register_section_handler(string_hash      name,
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

void amalga::set_resource_resolver(resource_resolver resolver) {
    references::resource_resolver.write(resolver);
}
