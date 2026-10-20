#pragma once

#include "treyarch/amalga/mashable_resource_handler.hh"
#include "treyarch/amalga/resource_handler.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

// handlers that still forward to retail; each one moves out as its type gets owned (yay!)
namespace treyarch {
    namespace als {
        class animation_logic_system_shared;
    } // als

    namespace amalga {
        // todo: after NAL and ALS work
        template<> i32 mashable_resource_handler<als::animation_logic_system_shared>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
    } // amalga
} // treyarch
