#include "treyarch/amalga/resource_handler.hh"
#include "treyarch/amalga/apkf/loader.hh"
#include "treyarch/ngl/ngl.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace amalga { namespace references {
    util::memory_reference<void*> resource_context { 0x0102FE74 };
}}} // treyarch::amalga::references

using namespace treyarch;

bool amalga::resource_handler::advance(i32 operation, resource_budget* budget) {
    if (state == 2)
        return false;

    resource_directory* directory = pack_slot->directory;

    if (state == 0) {
        state             = 1;
        descriptor_cursor = directory->type_starts[(u32)type];

        if (operation == 1)
            directory->type_state[type] = 0;

        switch(type) {
            case e_resource_type::merged_apk:
                ((merged_apk_resource_handler*)this)->begin_merged_apk(operation);

                break;

            default:
                begin(operation);

                break;
        }

        if (budget && budget->clock.elapsed() >= budget->deadline)
            return true;
    }

    directory = pack_slot->directory;

    i32 end = directory->type_starts[type] + directory->type_counts[type];

    if (descriptor_cursor >= end) {
        state = 2;

        return false;
    }

    while (true) {
        resource_descriptor* descriptor = pack_slot->directory->descriptors[descriptor_cursor];

        i32 result;

        switch(type) {
            case e_resource_type::merged_apk:
                result = ((merged_apk_resource_handler*)this)->progress_merged_apk(operation, descriptor);

                break;

            default:
                result = progress(operation, descriptor, budget);

                break;
        }

        if (result == 1)
            return true;

        if (result == 0)
            ++descriptor_cursor;

        if (budget && budget->clock.elapsed() >= budget->deadline)
            return true;

        if (descriptor_cursor >= end) {
            state = 2;

            return false;
        }
    }
}

void amalga::merged_apk_resource_handler::begin_merged_apk(i32 operation) {
    resource_directory* directory = pack_slot->directory;

    if (!directory->type_counts[e_resource_type::merged_apk])
        return;

    resource_descriptor* descriptor = directory->descriptors
        [directory->type_starts[e_resource_type::merged_apk]];

    if (operation)
        return;

    references::resource_context.write(pack_slot->reference_context);

    apkf::data_reference* &resource_references = descriptor->extension->resource_references;
    u8*                   &string_base         = descriptor->extension->string_base;
    apkf::file* owner = apkf::relocate_file_in_place
        (descriptor->raw_payload, resource_references, string_base);

    directory->merged_apk_file = owner;
    owner->apply_references(resource_references, string_base);

    references::resource_context.write(nullptr);

    owner->invoke_section_load_callbacks();

    entry_count  = owner->count_file_type_entries();
    entry_cursor = 0;
}

i32 amalga::merged_apk_resource_handler::progress_merged_apk(i32                  operation,
                                                             resource_descriptor* descriptor) {

    apkf::file* owner = (apkf::file*)((u8*)descriptor->raw_payload + sizeof(apkf::file_header));

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
