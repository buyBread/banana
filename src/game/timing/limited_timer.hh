#pragma once

#include "game/timing/hires_clock.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class limited_timer {

    public:
        hires_clock_t timer;
        f32           limit;

                 limited_timer()          : limit(0.0f)  {}
        explicit limited_timer(f32 limit) : limit(limit) {}

        void reset() {
            timer.reset();
        }

        bool out_of_time() const {
            return timer.elapsed() >= limit;
        }
    };

    enum e_progress_state : i32 {
        progress_start,
        progress_active,
        progress_done
    };

    struct progress {
        e_progress_state state;

        progress() : state(progress_start) {}
    };

    struct timed_progress : progress {
        limited_timer* time_limit;

        timed_progress() : time_limit(nullptr) {}
    };

    ASSERT_SIZEOF  (limited_timer,        0x10);
    ASSERT_OFFSETOF(limited_timer, limit, 0x08);

    ASSERT_SIZEOF  (timed_progress,             0x08);
    ASSERT_OFFSETOF(timed_progress, time_limit, 0x04);
} // treyarch
