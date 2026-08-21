#include <windows.h>

#include "treyarch/ngl/timing/frame_timer.hh"

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
    LARGE_INTEGER     counter;

    ++state.tick_count;
    QueryPerformanceCounter(&counter);

    state.performance_counter_low  = counter.LowPart;
    state.performance_counter_high = counter.HighPart;
}

void ngl::timing::restart_frame_timer() {
    MMRESULT timer_id = references::timer_id.read();

    if (timer_id)
        timeKillEvent(timer_id);

    references::timer_id.write
        (timeSetEvent(17, 0, frame_timer_callback, 0, TIME_PERIODIC));
}
