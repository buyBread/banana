#pragma once

#include "game/amalga/resource_handler.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace amalga {
    // an ise_library unmashed like the generic handler, handed to the ise manager while it's loaded
    struct ise_library_handler : resource_handler_base {
        i32 handle_resource(e_behavior           behavior,
                            resource_descriptor* descriptor,
                            limited_timer*       time_limit) override;
    };

    ASSERT_SIZEOF(ise_library_handler, 0x14);
}} // treyarch::amalga
