#pragma once

#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // value runs from `from` to `to` while remaining counts down over the last `duration` seconds
    struct igo_generic_decay_timer {
        void* vtable;
        bool  active;
        u8    pad_05[0x03];
        f32   value;
        f32   from;
        f32   to;
        f32   remaining;
        f32   duration;

        // inlined @ sub_6B13F0
        // inlined @ sub_6A7000
        // inlined @ sub_6DE490
        // inlined @ sub_6DE8F0
        // inlined @ sub_6DEBC0
        void start(f32 start_value, f32 end_value, f32 length, f32 delay) { // value holds at start_value for delay
            f32 total = (f32)((f64)(f32)((f64)length + (f64)delay) - 0.0);

            from      = start_value;
            to        = end_value;
            duration  = length;
            value     = start_value;
            active    = true;
            remaining = total;

            if (total <= 0.0f) {
                remaining = 0.0f;
                value     = end_value;
                active    = false;
            }

            f32 left = remaining;

            if (left > length)
                left = length;

            if (length != 0.0f)
                value = (f32)((f64)(f32)((1.0 - (f64)left / (f64)length) * ((f64)end_value - (f64)start_value)) + (f64)start_value);
        }
    };

    ASSERT_SIZEOF  (igo_generic_decay_timer,            0x1C);
    ASSERT_OFFSETOF(igo_generic_decay_timer, active,    0x04);
    ASSERT_OFFSETOF(igo_generic_decay_timer, value,     0x08);
    ASSERT_OFFSETOF(igo_generic_decay_timer, from,      0x0C);
    ASSERT_OFFSETOF(igo_generic_decay_timer, to,        0x10);
    ASSERT_OFFSETOF(igo_generic_decay_timer, remaining, 0x14);
    ASSERT_OFFSETOF(igo_generic_decay_timer, duration,  0x18);
} // treyarch
