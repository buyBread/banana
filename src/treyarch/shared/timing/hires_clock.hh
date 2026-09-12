#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    namespace timing {
        u64 get_cpu_cycle();
        f64 cycles_to_milliseconds(u64 cycles);
    } // timing

    class hires_clock_t {

        u64 last_reset_ticks;

    public:
        hires_clock_t();

        void reset();

        f32 elapsed_and_reset();
        f32 elapsed() const;
    };

    ASSERT_SIZEOF(hires_clock_t, 0x08);
} // treyarch
