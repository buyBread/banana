#pragma once

#include "util/types.hh"

namespace treyarch { namespace ngl {
    u16 get_screen_width();
    u16 get_screen_height();

    bool is_display_widescreen();
    
    f32 get_vblank_milliseconds();
}} // treyarch::ngl
