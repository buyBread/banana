#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class world_dynamics_system {

    public:
        u8  reserved_000[0x12C];
        u8* rendering_state;

        bool is_rendering_blocked() const {
            return rendering_state[0x0D] || rendering_state[0x8F90];
        }
    };

    ASSERT_OFFSETOF(world_dynamics_system, rendering_state, 0x12C);
} // treyarch
