#include "game/frontend/igo/igo_3d_objective_text_widget.hh"

using namespace treyarch;

// sub_6E7AC0
void igo_3d_objective_text_widget::clear(bool fade) {
    message_text* message = shown_message;

    if (message) {
        if (!fade) {
            message->destroy();

            shown_message = nullptr;

            text->set_text_no_localize("");
        } else if (message->end_time < 0.0f || message->fade_out_start < 0.0f) {
            message->fade_out_start = message->time;
            message->end_time       = (f32)((f64)message->time + 0.5);
        } else {
            bool fading = message->time > message->fade_out_start && message->end_time > message->time;

            if (!fading && message->fade_out_start > message->time)
                message->time = message->fade_out_start;
        }
    }

    queue.clear();
}
