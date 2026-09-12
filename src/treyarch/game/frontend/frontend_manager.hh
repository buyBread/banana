#pragma once

#include "treyarch/game/frontend/ui_frontend.hh"
#include "treyarch/ngl/quad/quad.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class frontend_manager {

    public:
        u8            reserved_000[0x54];
        ui_frontend*  igo;
        f32           pause_menu_timer;
        u8            start_on;
        u8            fonts_loaded;
        u8            reserved_05e[0x02];
        f32           default_screen_width;
        f32           aspect_ratio;
        u8            aspect_ratio_test_mode;
        u8            reserved_069[0x03];
        ngl::quad*    cutscene_quad;
        u8            reserved_070[0x1C];
        u8            draw_cutscene_quad;

        void draw_igo();
    };

    ASSERT_OFFSETOF(frontend_manager, igo,                    0x54);
    ASSERT_OFFSETOF(frontend_manager, pause_menu_timer,       0x58);
    ASSERT_OFFSETOF(frontend_manager, start_on,               0x5C);
    ASSERT_OFFSETOF(frontend_manager, fonts_loaded,           0x5D);
    ASSERT_OFFSETOF(frontend_manager, default_screen_width,   0x60);
    ASSERT_OFFSETOF(frontend_manager, aspect_ratio,           0x64);
    ASSERT_OFFSETOF(frontend_manager, aspect_ratio_test_mode, 0x68);
    ASSERT_OFFSETOF(frontend_manager, cutscene_quad,          0x6C);
    ASSERT_OFFSETOF(frontend_manager, draw_cutscene_quad,     0x8C);
} // treyarch
