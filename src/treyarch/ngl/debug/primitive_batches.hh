#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl {
    struct debug_primitive_batch_state {
        u32 batch_counter_0;
        u32 batch_counter_1;
        u32 batch_counter_2;
        u32 allocation_bytes;
        u32 batch_counter_3;
        u32 batch_counter_4;
        u8  allocation_overflowed;
        u8  synchronized_allocation_overflowed;
        u8  pad_01a[0x02];
        u32 peak_allocation_bytes;
        f32 synchronized_peak_utilization;
        u32 synchronized_frame_epoch;
    };

    ASSERT_SIZEOF  (debug_primitive_batch_state,                                      0x28);
    ASSERT_OFFSETOF(debug_primitive_batch_state, batch_counter_0,                     0x00);
    ASSERT_OFFSETOF(debug_primitive_batch_state, batch_counter_1,                     0x04);
    ASSERT_OFFSETOF(debug_primitive_batch_state, batch_counter_2,                     0x08);
    ASSERT_OFFSETOF(debug_primitive_batch_state, allocation_bytes,                    0x0C);
    ASSERT_OFFSETOF(debug_primitive_batch_state, batch_counter_3,                     0x10);
    ASSERT_OFFSETOF(debug_primitive_batch_state, batch_counter_4,                     0x14);
    ASSERT_OFFSETOF(debug_primitive_batch_state, allocation_overflowed,               0x18);
    ASSERT_OFFSETOF(debug_primitive_batch_state, synchronized_allocation_overflowed,  0x19);
    ASSERT_OFFSETOF(debug_primitive_batch_state, peak_allocation_bytes,               0x1C);
    ASSERT_OFFSETOF(debug_primitive_batch_state, synchronized_peak_utilization,       0x20);
    ASSERT_OFFSETOF(debug_primitive_batch_state, synchronized_frame_epoch,            0x24);

    namespace debug {
        void __cdecl reset_primitive_batches();
    } // debug

    namespace references {
        inline util::memory_reference<debug_primitive_batch_state> debug_primitive_batches { 0x01123928 };
    } // references
}} // treyarch::ngl
