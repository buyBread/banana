#include "retail.hh"
#include "treyarch/amalga/ise_library/resource_handler.hh"
#include "treyarch/amalga/mashable_resource_handler.hh"
#include "treyarch/amalga/resource_directory.hh"
#include "treyarch/game/ise/ise_library.hh"
#include "treyarch/shared/mash/string.hh"

using namespace treyarch;

// sub_75E6D0
i32 amalga::ise_library_handler::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer*) {
    if (behavior == load) {
        ise_library* library = mashable_resource_handler<ise_library>::construct(descriptor);

        mash::string unk;

        // add library
        retail::sub_A6FB00((u32*)retail::sub_A6FBE0(), (i32)library, (i32)&unk);

        return 0;
    }

    // remove library
    retail::sub_A6F8F0((u32*)retail::sub_A6FBE0(), (i32)descriptor->calculate_adjusted_payload());

    mashable_resource_handler<ise_library>::destruct(descriptor);

    return 0;
}
