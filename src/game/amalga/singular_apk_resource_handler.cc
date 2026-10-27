#include "treyarch/apk/file.hh"
#include "treyarch/apk/loader.hh"
#include "game/amalga/singular_apk_resource_handler.hh"
#include "treyarch/ngl/ngl.hh"

using namespace treyarch;

// sub_72E270
i32 amalga::singular_apk_resource_handler::handle_resource(e_behavior           behavior,
                                                           resource_descriptor* descriptor,
                                                           limited_timer*) {

    if (behavior == unload) {
        apk::file* owner = (apk::file*)((u8*)descriptor->raw_payload + sizeof(apk::file_header));

        if (!ngl::can_release_file(owner))
            return 1;

        apk::delete_file(owner);

        return 0;
    }

    apk::file* owner = apk::relocate_file_in_place(descriptor->raw_payload,
                                                   descriptor->extension->resource_references,
                                                   descriptor->extension->string_base);

    owner->apply_references(descriptor->extension->resource_references, descriptor->extension->string_base);
    owner->invoke_load_callbacks();

    return 0;
}
