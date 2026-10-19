#pragma once

#include "treyarch/amalga/resource_handler.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace amalga {
    // a box_trigger_info unmashed like the generic handler, plus the trigger entity it describes
    struct box_trigger_resource_handler : resource_handler_base {
        i32 handle_resource(e_behavior           behavior,
                            resource_descriptor* descriptor,
                            limited_timer*       time_limit) override;
    };

    ASSERT_SIZEOF(box_trigger_resource_handler, 0x14);
}} // treyarch::amalga
