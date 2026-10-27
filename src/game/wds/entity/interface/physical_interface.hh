#pragma once

#include "game/wds/entity/interface/generic_interface.hh"
#include "game/math/algebra.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    enum e_physical_interface_flags : u32 {
        physical_interface_flag_biped_running = 0x00080000
    };

    class physical_interface : public actor_interface {

    public:
        u32      flags;
        u8       reserved_010[0x40];
        vector3d last_collision_normal;

        // inlined
        bool is_flagged(e_physical_interface_flags flag) const {
            return (flags & flag) != 0;
        }

        void stop_biped_physics(bool on_destroyed = false);
        void cancel_all_velocity(const vector3d &dir, bool both_dir = true);
    };

    ASSERT_OFFSETOF(physical_interface, flags,                 0x0C);
    ASSERT_OFFSETOF(physical_interface, last_collision_normal, 0x50);
} // treyarch
