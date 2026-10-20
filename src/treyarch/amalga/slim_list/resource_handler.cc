#include "retail.hh"
#include "treyarch/amalga/resource_directory.hh"
#include "treyarch/amalga/resource_manager.hh"
#include "treyarch/amalga/resource_types.hh"
#include "treyarch/amalga/slim_list/resource_handler.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/resource_key.hh"

using namespace treyarch;

// sub_76DA70
i32 amalga::slim_list_handler::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer*) {
    u32 size = descriptor->payload_size;
    u8* text = descriptor->raw_payload;

    if (behavior == load) {
        string_hash name;
        name.initialize(mash::ALLOCATED, "force_include");

        resource_key key;
        key.set(name, (u32)e_resource_type::slim_list);

        u32 force_include_size;
        u8* force_include = resource_manager::get_resource(key, &force_include_size, nullptr);

        if (force_include != text)
            retail::sub_76D8E0(this, force_include, force_include_size);

        retail::sub_76D8E0(this, text, size);
    }

    return 0;
}
