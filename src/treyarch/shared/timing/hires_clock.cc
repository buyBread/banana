#include <windows.h>

#include "treyarch/shared/timing/hires_clock.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace timing {
    namespace references {
        util::memory_reference<f32> performance_counts_per_millisecond { 0x00F51E84 };
    } // references

    u64 get_cpu_cycle() {
        LARGE_INTEGER counter;
        QueryPerformanceCounter(&counter);

        return (u64)counter.QuadPart;
    }

    f64 cycles_to_milliseconds(u64 cycles) {
        return (f64)cycles / (f64)references::performance_counts_per_millisecond.read();
    }
}} // treyarch::timing

using namespace treyarch;

hires_clock_t::hires_clock_t() {
    reset();
}

void hires_clock_t::reset() {
    last_reset_ticks = timing::get_cpu_cycle();
}

f32 hires_clock_t::elapsed_and_reset() {
    u64 elapsed_ticks = timing::get_cpu_cycle() - last_reset_ticks;
    f32 elapsed_time = (f32)
        (timing::cycles_to_milliseconds(elapsed_ticks) / 1000.0);

    last_reset_ticks = timing::get_cpu_cycle();

    return elapsed_time;
}

f32 hires_clock_t::elapsed() const {
    u64 elapsed_ticks = timing::get_cpu_cycle() - last_reset_ticks;

    return (f32)(timing::cycles_to_milliseconds(elapsed_ticks) / 1000.0);
}
