#include "treyarch/game/frontend/igo/igo_3d_ped_warning_widget.hh"
#include "treyarch/ngl/quad/quad.hh"

using namespace treyarch;

// sub_6A1490
void igo_3d_ped_warning_widget::draw() {
    if (!visible)
        return;

    u32 opacity;

    if (alpha <= 0.0f)
        opacity = 0;
    else if (alpha >= 1.0f)
        opacity = 255;
    else
        opacity = (u32)(i32)((f64)alpha * 255.0);

    ngl::set_quad_color(quad, (opacity << 24) | 0x00FFFFFF);
    ngl::list_add_quad(quad);
}
