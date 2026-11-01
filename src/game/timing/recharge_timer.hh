#pragma once

#include "game/timing/simple_timer.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // SM3 recharge_timer; time counts down from duration
    struct recharge_timer {
        simple_timer time;
        f32          duration;

        // sub_7E9750
        recharge_timer() : duration(0.0f) {}

        // inlined @ sub_996980
        void reset() {
            time = duration;
        }

        // inlined @ sub_994200
        bool is_ready() const {
            return time.get_time() <= 0.0f;
        }

        // inlined @ sub_994200
        bool frame_advance(f32 t) {
            time -= t;

            return is_ready();
        }

        // sub_986890
        f32 charge_percent() const {
            if (duration > 0.0f)
                return 1.0f - time.get_time() / duration;
            else
                return 1.0f;
        }
    };

    ASSERT_SIZEOF  (recharge_timer,           0x08);
    ASSERT_OFFSETOF(recharge_timer, duration, 0x04);
} // treyarch
