#pragma once

#include "treyarch/game/frontend/igo/igo_3d_widget.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class igo_3d_loading_screen : public igo_3d_widget {
    public:
        u8  reserved_150[0x04];
        f32 fade;
        u8  reserved_158[0x10];
        u32 color;
    };

    ASSERT_OFFSETOF(igo_3d_loading_screen, fade,  0x154);
    ASSERT_OFFSETOF(igo_3d_loading_screen, color, 0x168);
} // treyarch
