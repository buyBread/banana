#include "game/frontend/igo/igo_3d_gta_widget.hh"
#include "game/wds/entity/entity.hh"

using namespace treyarch;

// sub_6A0EE0
void igo_3d_gta_widget::set_color(vector3d color) {
    auto* ring = (entity*)target_ring.resolve();

    if (!ring)
        return;

    color32 render_color;

    render_color.set_red_float  (color.x);
    render_color.set_green_float(color.y);
    render_color.set_blue_float (color.z);
    render_color.a = 0xFF;

    ring->set_render_color(render_color);
}
