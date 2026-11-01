#pragma once

#include "game/color.hh"
#include "game/frontend/igo/igo_3d_text.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/virtual_base.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_loading_screen : public igo_3d_widget {

    public:
        f32          unk_150;        // zeroed by blackscreen_on when the screen was hidden; the update cycles it through 4 seconds
        f32          fade;           // the blackscreen's alpha
        bool         no_hero_freeze; // the update leaves the hero alone while it's set
        bool         fading_in;
        u8           pad_15a[0x02];
        f32          fade_duration;
        f32          fade_elapsed;
        bool         muted;          // blackscreen_on posted "ui_loading_mute"
        bool         held;           // blackscreen_off leaves the screen up unless forced or a mission is running
        bool         unk_166;
        u8           pad_167;
        color32      color;
        u8           reserved_16c[0x04];
        igo_3d_text* message_text;
        f32          opaque_time;
        f32          opaque_timeout; // the update takes the black down once opaque_time passes it
        u8           reserved_17c[0x04];
        f32          unk_180;        // the update keeps it at unk_150 * 4.71
        u8           reserved_184[0x08];
        i32          hero_freezes;   // the update's freeze_hero calls, paid back by blackscreen_off
        bool         defer_set_shown;
        bool         deferred_shown;
        bool         hint_picked;
        u8           reserved_193[0x0D];

        explicit igo_3d_loading_screen(mash::generating_vtable_version so_do_nothing) : igo_3d_widget(so_do_nothing) {}

        ~igo_3d_loading_screen() override;

        // writes the source vptr over a screen the retail constructor built
        void stamp_source_vtable() {
            *(mash::vtable_value_t*)this = mash::generate_vtable<igo_3d_loading_screen>();
        }

        void init() override;
        void update(f32 time_inc) override;
        void render() override;
        void set_shown(bool shown) override;
        void spawn_entity() override;
        i32  unk_044(i32 unk_0, i32 unk_1, void* unk_2, i32 unk_3, i32 unk_4, i32 unk_5) override;

        void blackscreen_on (f32 time, bool unk, bool hold, bool silent, bool no_freeze, f32 timeout);
        void blackscreen_off(f32 time, bool force); // nothing while held, unless forced or a mission is running
    };

    ASSERT_SIZEOF  (igo_3d_loading_screen,                  0x1A0);
    ASSERT_OFFSETOF(igo_3d_loading_screen, fade,            0x154);
    ASSERT_OFFSETOF(igo_3d_loading_screen, no_hero_freeze,  0x158);
    ASSERT_OFFSETOF(igo_3d_loading_screen, fade_duration,   0x15C);
    ASSERT_OFFSETOF(igo_3d_loading_screen, muted,           0x164);
    ASSERT_OFFSETOF(igo_3d_loading_screen, color,           0x168);
    ASSERT_OFFSETOF(igo_3d_loading_screen, message_text,    0x170);
    ASSERT_OFFSETOF(igo_3d_loading_screen, opaque_time,     0x174);
    ASSERT_OFFSETOF(igo_3d_loading_screen, unk_180,         0x180);
    ASSERT_OFFSETOF(igo_3d_loading_screen, hero_freezes,    0x18C);
    ASSERT_OFFSETOF(igo_3d_loading_screen, defer_set_shown, 0x190);
    ASSERT_OFFSETOF(igo_3d_loading_screen, hint_picked,     0x192);
} // treyarch
