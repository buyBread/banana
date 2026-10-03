#pragma once

#include "treyarch/shared/timing/hires_clock.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

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
    };

    struct timed_progress : progress {
        limited_timer* time_limit;
    };

    ASSERT_SIZEOF  (limited_timer,        0x10);
    ASSERT_OFFSETOF(limited_timer, limit, 0x08);

    ASSERT_SIZEOF  (timed_progress,             0x08);
    ASSERT_OFFSETOF(timed_progress, time_limit, 0x04);
} // treyarch
