#include "treyarch/amalga/merged_apk/loader.hh"
#include "treyarch/amalga/merged_apk/resource_handler.hh"
#include "treyarch/amalga/resource_manager.hh"
#include "treyarch/ngl/ngl.hh"

using namespace treyarch;

// sub_73A5E0
void amalga::merged_apk_resource_handler::begin_merged_apk(i32 operation) {
    resource_directory* directory = pack_slot->pack_directory;

    if (!directory->type_counts[(u32)e_resource_type::merged_apk])
        return;

    resource_descriptor* descriptor = directory->descriptors
        [directory->type_starts[(u32)e_resource_type::merged_apk]];

    if (operation)
        return;

    resource_manager::references::resource_context.write(pack_slot->header_mem_addr[buffer_location_vram]);

    merged_apk::data_reference* &resource_references = descriptor->extension->resource_references;
    u8*                         &string_base         = descriptor->extension->string_base;
    merged_apk::file* owner = merged_apk::relocate_file_in_place
        (descriptor->raw_payload, resource_references, string_base);

    directory->merged_apk_file = owner;
    owner->apply_references(resource_references, string_base);

    resource_manager::references::resource_context.write(nullptr);

    owner->invoke_section_load_callbacks();

    entry_count  = owner->count_file_type_entries();
    entry_cursor = 0;
}

// sub_73A670
i32 amalga::merged_apk_resource_handler::progress_merged_apk(i32                  operation,
                                                             resource_descriptor* descriptor) {

    merged_apk::file* owner = (merged_apk::file*)((u8*)descriptor->raw_payload + sizeof(merged_apk::file_header));

    if (operation) {
        if (owner->last_frame_reference + 1 >= (i32)ngl::references::frame_epoch.read())
            return 1;

        owner->unload();

        return 0;
    }

    u32 remaining = entry_count - entry_cursor;
    u32 count = remaining < 128 ? remaining : 128;

    owner->invoke_file_type_load_callbacks(entry_cursor, count);
    entry_cursor += count;

    return entry_cursor != entry_count ? 1 : 0;
}
