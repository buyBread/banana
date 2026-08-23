#include <windows.h>

#include "banana/imgui/imgui.hh"
#include "treyarch/ngl/d3d9/display.hh"
#include "treyarch/ngl/display.hh"

using namespace treyarch;

i32 get_window_dimensions() {
    using namespace banana;

    i16 width  = 640;
    i16 height = 480;

    RECT rect;

    /*
        scuffed!
        we should own this on the bootstrap side instead of relying on banana's imgui
    */
    if (imgui::store::handle_window) {
        GetClientRect(imgui::store::handle_window, &rect);

        width  = i16(rect.right  - rect.left);
        height = i16(rect.bottom - rect.top);
    }

    return (i32)(width << 16 | height);
}

i16 ngl::get_screen_width() {
    return (i16)(get_window_dimensions() >> 16 & 0xFFFF);
}

i16 ngl::get_screen_height() {
    return (i16)(get_window_dimensions() & 0xFFFF);
}

f32 ngl::get_vblank_milliseconds() {
    return d3d9::references::selected_display_mode.get().pal ?
        20.0f : 16.666666f;
}
