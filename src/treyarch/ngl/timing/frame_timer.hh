#pragma once

#include <windows.h>
#include <mmsystem.h>

#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl { namespace timing {
    struct frame_tick_state {
        u32 tick_count;
        u32 performance_counter_low;
        i32 performance_counter_high;
    };

    void restart_frame_timer();
    void CALLBACK frame_timer_callback(UINT      timer_id,
                                       UINT      message,
                                       DWORD_PTR user_data,
                                       DWORD_PTR first,
                                       DWORD_PTR second);

    namespace references {
        inline ::util::memory_reference<MMRESULT>         timer_id   { 0x01117150 };
        inline ::util::memory_reference<frame_tick_state> tick_state { 0x011184FC };
    } // references

    ASSERT_SIZEOF  (frame_tick_state,                          0x0C);
    ASSERT_OFFSETOF(frame_tick_state, tick_count,               0x00);
    ASSERT_OFFSETOF(frame_tick_state, performance_counter_low,  0x04);
    ASSERT_OFFSETOF(frame_tick_state, performance_counter_high, 0x08);
}}} // treyarch::ngl::timing
