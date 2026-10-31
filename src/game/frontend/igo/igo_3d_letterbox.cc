#include "game/frontend/igo/igo_3d_letterbox.hh"

using namespace treyarch;

// sub_694940
void igo_3d_letterbox::letterbox(f32 time, f32 height) {
    total_time     = time;
    desired_height = height;
    elapsed        = 0.0f;

    if ((f64)height > 0.0001)
        set_shown(true);

    if ((f64)time < 0.0001) {
        if ((f64)desired_height < 0.0001) {
            alpha      = 0.0f;
            cur_height = 0.0f;

            set_shown(false);
        } else {
            alpha      = 1.0f;
            cur_height = desired_height;
        }
    }
}
