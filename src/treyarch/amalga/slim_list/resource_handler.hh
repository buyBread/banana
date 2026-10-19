#pragma once

#include "treyarch/amalga/resource_handler.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace amalga {
    // parses every .slim file a list names, the "force_include" list's first
    struct slim_list_handler : resource_handler_base {
        i32 handle_resource(e_behavior           behavior,
                            resource_descriptor* descriptor,
                            limited_timer*       time_limit) override;
    };

    ASSERT_SIZEOF(slim_list_handler, 0x14);
}} // treyarch::amalga
