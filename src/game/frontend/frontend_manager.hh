#pragma once

#include "game/frontend/igo/igo_generic_decay_timer.hh"
#include "game/frontend/ui_frontend.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/ngl/quad/quad.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class frontend_manager {

    public:
        u8                      reserved_000[0x54];
        ui_frontend*            igo;
        f32                     pause_menu_timer;
        u8                      start_on;
        u8                      fonts_loaded;
        u8                      reserved_05e[0x02];
        f32                     default_screen_width;
        f32                     aspect_ratio;
        u8                      aspect_ratio_test_mode;
        u8                      reserved_069[0x03];
        ngl::quad*              cutscene_quad;
        igo_generic_decay_timer cutscene_timer;
        u8                      draw_cutscene_quad;

        void draw_igo();

        void refresh_igo_for_input_device(); // nothing before the fonts are loaded

        void set_cutscene_texture(const mash::string* name); // an empty name hides the quad
        void display_cutscene(f32 duration, f32 delay);
        void hide_cutscene();
        void send_hide_cutscene_quad_signal(); // hides the quad once the running timer is past its delay
    };

    namespace references {
        inline util::memory_reference<frontend_manager> frontend { 0x0102CFA8 };
    }

    ASSERT_OFFSETOF(frontend_manager, igo,                    0x54);
    ASSERT_OFFSETOF(frontend_manager, pause_menu_timer,       0x58);
    ASSERT_OFFSETOF(frontend_manager, start_on,               0x5C);
    ASSERT_OFFSETOF(frontend_manager, fonts_loaded,           0x5D);
    ASSERT_OFFSETOF(frontend_manager, default_screen_width,   0x60);
    ASSERT_OFFSETOF(frontend_manager, aspect_ratio,           0x64);
    ASSERT_OFFSETOF(frontend_manager, aspect_ratio_test_mode, 0x68);
    ASSERT_OFFSETOF(frontend_manager, cutscene_quad,          0x6C);
    ASSERT_OFFSETOF(frontend_manager, cutscene_timer,         0x70);
    ASSERT_OFFSETOF(frontend_manager, draw_cutscene_quad,     0x8C);
} // treyarch
