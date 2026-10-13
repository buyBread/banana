#pragma once

#include "treyarch/game/wds/entity/interface/generic_interface.hh"
#include "treyarch/shared/bounded_variable.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class damage_interface : public actor_interface {

    public:
        u8                    reserved_00c[0xA8];
        bounded_variable<i32> hit_points;

        // inlined
        i32 get_max_hit_points() const {
            return hit_points.get_max_val();
        }

        void set_hit_points(i32 value, bool use_signals = false, entity* attacker = nullptr, entity* attacker_weapon = nullptr);
    };

    ASSERT_OFFSETOF(damage_interface, hit_points, 0xB4);
} // treyarch
