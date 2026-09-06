#include <windows.h>

#include "treyarch/ngl/ngl.hh"
#include "treyarch/ngl/d3d9/display.hh"
#include "treyarch/ngl/display.hh"

using namespace treyarch;

u32 get_window_dimensions() {
    static u32 dimensions = (640 << 16) | 480;

    static bool got_rect = false;

    if (!got_rect) {
        RECT rect {};

        if (GetClientRect(ngl::references::render_window.get(), &rect)) {
            dimensions  = u32(u16(rect.right  - rect.left)) << 16;
            dimensions |=     u16(rect.bottom - rect.top);
            
            got_rect = true;
        }
    }

    return dimensions;
}

u16 ngl::get_screen_width() {
    return (u16)(get_window_dimensions() >> 16 & 0xFFFF);
}

u16 ngl::get_screen_height() {
    return (u16)(get_window_dimensions() & 0xFFFF);
}

f32 ngl::get_vblank_milliseconds() {
    return d3d9::references::selected_display_mode.get().pal ?
        20.0f : 16.666666f;
}
