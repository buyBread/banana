#include <windows.h>

#include "treyarch/ngl/timing/frame_timer.hh"
#include "treyarch/shared/timing/hires_clock.hh"

using namespace treyarch;

void CALLBACK ngl::timing::frame_timer_callback(UINT      timer_id,
                                                UINT      message,
                                                DWORD_PTR user_data,
                                                DWORD_PTR first,
                                                DWORD_PTR second) {

    (void)timer_id;
    (void)message;
    (void)user_data;
    (void)first;
    (void)second;

    frame_tick_state &state = references::tick_state.get();
    u64               counter;

    ++state.tick_count;
    counter = treyarch::timing::get_cpu_cycle();

    state.performance_counter_low  = (u32)counter;
    state.performance_counter_high = (i32)(counter >> 32);
}

void ngl::timing::restart_frame_timer() {
    MMRESULT timer_id = references::timer_id.read();

    if (timer_id)
        timeKillEvent(timer_id);

    references::timer_id.write
        (timeSetEvent(17, 0, frame_timer_callback, 0, TIME_PERIODIC));
}
