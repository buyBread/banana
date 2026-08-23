#include "treyarch/ngl/debug/primitive_batches.hh"
#include "treyarch/ngl/ngl.hh"

using namespace treyarch;

void __cdecl ngl::debug::reset_primitive_batches() {
    constexpr f64 allocation_budget_reciprocal = 0.0000019073486328125;

    ngl::debug_primitive_batch_state &state = references::debug_primitive_batches.get();

    state.batch_counter_0 = 0;
    state.batch_counter_1 = 0;
    state.batch_counter_2 = 0;
    state.batch_counter_3 = 0;
    state.batch_counter_4 = 0;

    u32 peak_allocation_bytes = state.peak_allocation_bytes;

    if (state.allocation_bytes > peak_allocation_bytes) {
        peak_allocation_bytes       = state.allocation_bytes;
        state.peak_allocation_bytes = peak_allocation_bytes;
    }

    const u32 frame_epoch = references::frame_epoch.read();

    if (state.synchronized_frame_epoch != frame_epoch) {
        state.synchronized_frame_epoch           = frame_epoch;
        state.synchronized_allocation_overflowed = state.allocation_overflowed;
        state.allocation_overflowed              = 0;
        state.peak_allocation_bytes              = 0;
        state.synchronized_peak_utilization      = (f32)((f64)peak_allocation_bytes * allocation_budget_reciprocal);
    }

    state.allocation_bytes = 0;
}
