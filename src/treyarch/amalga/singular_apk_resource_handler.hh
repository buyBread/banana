#pragma once

#include "treyarch/amalga/resource_handler.hh"

namespace treyarch { namespace amalga {
    // each resource is its own apk file
    struct singular_apk_resource_handler : resource_handler_base {
        i32 handle_resource(e_behavior           behavior,
                            resource_descriptor* descriptor,
                            limited_timer*       time_limit) override;
    };
}} // treyarch::amalga
