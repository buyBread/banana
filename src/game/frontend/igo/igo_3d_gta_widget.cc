#include "game/frontend/igo/igo_3d_gta_widget.hh"
#include "game/wds/entity/entity.hh"

using namespace treyarch;

// sub_6A0EE0
void igo_3d_gta_widget::set_color(vector3d color) {
    auto* ring = (entity*)target_ring.resolve();

    if (!ring)
        return;

    color32 render_color;

    render_color.r = 0.0f > color.x ? 0 : color.x > 1.0f ? 0xFF : (u8)(i32)((f64)color.x * 255.0);
    render_color.g = 0.0f > color.y ? 0 : color.y > 1.0f ? 0xFF : (u8)(i32)((f64)color.y * 255.0);
    render_color.b = 0.0f > color.z ? 0 : color.z > 1.0f ? 0xFF : (u8)(i32)((f64)color.z * 255.0);
    render_color.a = 0xFF;

    ring->set_render_color(render_color);
}
