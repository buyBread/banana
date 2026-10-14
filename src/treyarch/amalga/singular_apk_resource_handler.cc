#include "treyarch/amalga/merged_apk/file.hh"
#include "treyarch/amalga/merged_apk/loader.hh"
#include "treyarch/amalga/singular_apk_resource_handler.hh"

using namespace treyarch;

// sub_72E270
i32 amalga::singular_apk_resource_handler::handle_resource(e_behavior           behavior,
                                                           resource_descriptor* descriptor,
                                                           limited_timer*) {

    if (behavior == unload) {
        merged_apk::file* owner = (merged_apk::file*)((u8*)descriptor->raw_payload + sizeof(merged_apk::file_header));

        if (!owner->can_release())
            return 1;

        owner->unload();

        return 0;
    }

    merged_apk::file* owner = merged_apk::relocate_file_in_place(descriptor->raw_payload,
                                                                 descriptor->extension->resource_references,
                                                                 descriptor->extension->string_base);

    owner->apply_references(descriptor->extension->resource_references, descriptor->extension->string_base);
    owner->invoke_load_callbacks();

    return 0;
}
