#include "treyarch/ngl/d3d9/display.hh"
#include "treyarch/ngl/display.hh"

using namespace treyarch;

/*
    basically, implementing UIFrontEnd::DrawStartup was producing garbage projection.
    the culript was our crappy getter, that has to be downgraded back to this fixed nonsense.
    why they hardcoded this resolution is beyond me, nor do i really care, so revert it is.
*/

// sub_9E1F90
bool ngl::is_display_widescreen() {
    return d3d9::references::selected_display_mode.get().widescreen;
}

// sub_9E1FC0
u16 ngl::get_screen_width() {
    return 640;
}

// sub_580500
u16 ngl::get_screen_height() {
    return 480;
}

// sub_9E1FA0
f32 ngl::get_vblank_milliseconds() {
    return d3d9::references::selected_display_mode.get().pal ?
        20.0f : 16.666666f;
}
