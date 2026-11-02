#pragma once

#include "vc8/vector.hh"
#include "game/audio/sound_response.hh"
#include "game/frontend/igo/igo_3d_scrapbook_station_widget.hh"
#include "game/frontend/igo/igo_3d_scrapbook_sub_tab_pages.hh"
#include "game/frontend/igo/igo_3d_tab_page_widget.hh"
#include "game/frontend/igo/igo_3d_text.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "game/frontend/igo/igo_generic_decay_timer.hh"
#include "game/frontend/igo/igo_generic_input_listener.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/virtual_base.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    namespace references {
        inline util::memory_reference<string_hash> scrapbook_entered { 0x0102C668 };
        inline util::memory_reference<string_hash> scrapbook_exited  { 0x0102C418 };
    } // references

    class igo_3d_scrapbook : public igo_generic_input_listener {

    public:
        vc8::vector
            <igo_3d_scrapbook_station_widget*> stations;                 // the mission log, skills and options stations
        igo_3d_scrapbook_tab_pages*            tab_pages;
        igo_3d_scrapbook_sub_tab_pages*        sub_tab_pages;
        igo_3d_widget*                         main_panel;               // "ui_shell_main_panel", never deleted
        igo_3d_widget*                         pause_bar;                // "ui_shell_pause_bar"
        igo_3d_text*                           pause_bar_text;           // "ui_shell_pause_bar_text"
        u8                                     active;
        u8                                     reserved_031;
        bool                                   game_was_paused;          // activate pauses the game unless it was
        bool                                   loading_screen_was_shown; // activate hides the loading screen, unk_028 brings it back
        i32                                    current_page;             // -1 for none
        i32                                    unk_038;
        u8                                     reserved_03c[0x0C];
        igo_generic_decay_timer                unk_048;
        igo_generic_decay_timer                unk_064;
        igo_generic_decay_timer                unk_080;
        bool                                   unk_09c;
        bool                                   unk_09d;
        bool                                   unk_09e;
        u8                                     pad_09f;
        sound_response                         unk_0a0;
        bool                                   sliding;                  // the main panel slides from slide_from to slide_to
        u8                                     pad_0b1[0x03];
        f32                                    slide_elapsed;
        f32                                    slide_to;
        f32                                    slide_from;
        f32                                    main_panel_x;             // the main panel's x at construction
        f32                                    unk_0c4;

        explicit igo_3d_scrapbook(mash::generating_vtable_version so_do_nothing) : igo_generic_input_listener(so_do_nothing) {}

        ~igo_3d_scrapbook() override;

        // writes the source vptrs over a scrapbook the retail constructor built, its tab pages included
        void stamp_source_vtable() {
            *(mash::vtable_value_t*)this = mash::generate_vtable<igo_3d_scrapbook>();

            tab_pages->stamp_source_vtable();
            sub_tab_pages->stamp_source_vtable();

            ((igo_3d_scrapbook_skills*)stations[1])->tab_pages->stamp_source_vtable();
        }

        void unk_001(i32 unk) override;
        void unk_002(i32 unk) override;
        void unk_003(i32 unk) override;
        void unk_004(i32 unk) override;
        void unk_005(i32 unk) override;
        void unk_006(i32 unk) override;
        void unk_007(i32 unk) override;
        void unk_008(i32 unk) override;
        void unk_009(i32 unk) override;
        void unk_010(i32 unk) override;
        void unk_011(i32 unk) override;
        void unk_012(i32 unk) override;
        void unk_013(i32 unk) override;
        void unk_014(i32 unk) override;
        void unk_015(i32 unk) override;
        void unk_016(i32 unk) override;

        virtual void unk_024();
        virtual void unk_025(f32 unk);
        virtual void draw_pages();
        virtual void activate(bool unk);
        virtual void unk_028();
        virtual u8   is_active();
        virtual bool unk_030();
        virtual bool can_open();

        igo_3d_scrapbook_station_widget* current_station(); // null when current_page is out of range

        bool is_background_fading(); // the dialog background is fading in or out

        void set_page(i32 request, bool animate, i32 unlock); // moves to the first unlocked page the request reaches
        i32  first_unlocked_page(i32 request, i32 unlock);
        void clear_page_unlocks();
        bool is_any_page_locked();
    };

    ASSERT_SIZEOF  (igo_3d_scrapbook,                  0xC8);
    ASSERT_OFFSETOF(igo_3d_scrapbook, stations,        0x0C);
    ASSERT_OFFSETOF(igo_3d_scrapbook, tab_pages,       0x1C);
    ASSERT_OFFSETOF(igo_3d_scrapbook, pause_bar_text,  0x2C);
    ASSERT_OFFSETOF(igo_3d_scrapbook, active,          0x30);
    ASSERT_OFFSETOF(igo_3d_scrapbook, game_was_paused, 0x32);
    ASSERT_OFFSETOF(igo_3d_scrapbook, current_page,    0x34);
    ASSERT_OFFSETOF(igo_3d_scrapbook, unk_048,         0x48);
    ASSERT_OFFSETOF(igo_3d_scrapbook, unk_064,         0x64);
    ASSERT_OFFSETOF(igo_3d_scrapbook, unk_080,         0x80);
    ASSERT_OFFSETOF(igo_3d_scrapbook, unk_09c,         0x9C);
    ASSERT_OFFSETOF(igo_3d_scrapbook, unk_0a0,         0xA0);
    ASSERT_OFFSETOF(igo_3d_scrapbook, sliding,         0xB0);
    ASSERT_OFFSETOF(igo_3d_scrapbook, slide_elapsed,   0xB4);
    ASSERT_OFFSETOF(igo_3d_scrapbook, slide_to,        0xB8);
    ASSERT_OFFSETOF(igo_3d_scrapbook, slide_from,      0xBC);
    ASSERT_OFFSETOF(igo_3d_scrapbook, main_panel_x,    0xC0);
    ASSERT_OFFSETOF(igo_3d_scrapbook, unk_0c4,         0xC4);
} // treyarch
