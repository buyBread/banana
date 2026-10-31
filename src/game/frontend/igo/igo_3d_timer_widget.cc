#include "game/frontend/frontend_manager.hh"
#include "game/frontend/igo/igo_3d_timer_widget.hh"
#include "game/frontend/ui_frontend.hh"

using namespace treyarch;

// sub_6918B0
void igo_3d_timer_widget::set_time_left(f32 left) {
    time_left = left;

    if (0.0f > left)
        time_left = 0.0f;

    total_time = time_left;
}

// sub_6918E0
void igo_3d_timer_widget::inc_time_left(f32 time_inc) {
    f32 left = (f32)((f64)time_left + time_inc);

    time_left = left;

    if (0.0f > left)
        time_left = 0.0f;

    total_time = time_left;
}

// sub_691930
void igo_3d_timer_widget::set_run_when_hidden(bool run_hidden) {
    run_when_hidden = run_hidden;
}

// sub_4514A0
void igo_3d_timer_widget::set_display_hundredths(bool display) {
    display_hundredths = display;
}

// sub_4514C0
void igo_3d_timer_widget::set_display_thousandths(bool display) {
    display_thousandths = display;
}

// sub_691950
void igo_3d_timer_widget::set_z(f32 z) {
    igo_3d_timer_widget* timer = references::frontend.get().igo->timer_widget;

    if (timer)
        timer->set_position_z(z);
}
