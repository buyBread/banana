#pragma once

#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // a float pulled toward a target
    struct float_spring {
        f32  value;
        f32  target;
        f32  velocity;
        f32  stiffness;
        f32  damping;
        f32  mute_rate;
        bool reset;          // the next update jumps straight to the target
        bool stop_at_target; // an update that would overshoot lands on the target
        u8   pad_01a[0x02];

        f32 update(f32 dt, f32 new_target); // the new value
    };

    ASSERT_SIZEOF  (float_spring,                 0x1C);
    ASSERT_OFFSETOF(float_spring, stiffness,      0x0C);
    ASSERT_OFFSETOF(float_spring, mute_rate,      0x14);
    ASSERT_OFFSETOF(float_spring, reset,          0x18);
    ASSERT_OFFSETOF(float_spring, stop_at_target, 0x19);
} // treyarch
