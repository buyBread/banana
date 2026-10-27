#pragma once

#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    namespace timing {
        u64 get_cpu_cycle();
        f64 cycles_to_milliseconds(u64 cycles);

        namespace references {
            inline util::memory_reference<f32> performance_counts_per_millisecond { 0x00F51E84 };
        } // references
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
