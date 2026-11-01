#pragma once

#include "vc8/vector.hh"
#include "game/audio/sound_response.hh"
#include "game/frontend/igo/igo_3d_text.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "game/frontend/igo/igo_generic_decay_timer.hh"
#include "game/frontend/igo/igo_generic_input_listener.hh"
#include "treyarch/mash/types.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_scrapbook : public igo_generic_input_listener {

    public:
        vc8::vector
            <igo_3d_widget*>    stations;       // the mission log, skills and options stations
        igo_3d_widget*          tab_pages;
        igo_3d_widget*          sub_tab_pages;
        igo_3d_widget*          main_panel;     // "ui_shell_main_panel", never deleted
        igo_3d_widget*          pause_bar;      // "ui_shell_pause_bar"
        igo_3d_text*            pause_bar_text; // "ui_shell_pause_bar_text"
        u8                      active;
        u8                      reserved_031[0x17];
        igo_generic_decay_timer unk_048;
        igo_generic_decay_timer unk_064;
        igo_generic_decay_timer unk_080;
        u8                      reserved_09c[0x04];
        sound_response          unk_0a0;
        u8                      reserved_0b0[0x18];

        explicit igo_3d_scrapbook(mash::generating_vtable_version so_do_nothing) : igo_generic_input_listener(so_do_nothing) {}

        ~igo_3d_scrapbook() override;

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
        virtual bool unk_031();
    };

    ASSERT_SIZEOF  (igo_3d_scrapbook,                 0xC8);
    ASSERT_OFFSETOF(igo_3d_scrapbook, stations,       0x0C);
    ASSERT_OFFSETOF(igo_3d_scrapbook, tab_pages,      0x1C);
    ASSERT_OFFSETOF(igo_3d_scrapbook, pause_bar_text, 0x2C);
    ASSERT_OFFSETOF(igo_3d_scrapbook, active,         0x30);
    ASSERT_OFFSETOF(igo_3d_scrapbook, unk_048,        0x48);
    ASSERT_OFFSETOF(igo_3d_scrapbook, unk_080,        0x80);
    ASSERT_OFFSETOF(igo_3d_scrapbook, unk_0a0,        0xA0);
} // treyarch
