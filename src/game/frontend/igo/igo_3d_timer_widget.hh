#pragma once

#include "game/color.hh"
#include "game/frontend/igo/igo_3d_text.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_timer_widget;

    struct igo_3d_timer_widget_vtable : igo_3d_widget_vtable {
        void (__thiscall* set_time_text)(igo_3d_timer_widget* self, f32 seconds, bool hundredths, bool thousandths); // m:ss into text
        void (__thiscall* set_color32)(igo_3d_timer_widget* self, color32 col);
        void (__thiscall* show)(igo_3d_timer_widget* self, bool shown, bool shifted, i32 text_frozen);
        void (__thiscall* set_shown_shifted)(igo_3d_timer_widget* self, bool shown, bool shifted);
    };

    class igo_3d_timer_widget : public igo_3d_widget {

    public:
        f32          time_left;
        f32          total_time;
        f32          color_timer;
        f32          time_modifier;
        color32      col_from;
        color32      col_to;
        bool         count_down;
        bool         running;
        bool         timer_too_high;
        bool         is_shifted;
        bool         run_when_hidden;
        bool         display_hundredths;
        bool         display_thousandths;
        u8           pad_16f;
        i32          text_frozen;         // while set the text isn't refreshed
        igo_3d_text* text;
        u8           reserved_178[0x08];

        igo_3d_timer_widget_vtable* get_vtable() const {
            return (igo_3d_timer_widget_vtable*)vtable;
        }

        void show(bool shown, bool shifted, i32 frozen) {
            get_vtable()->show(this, shown, shifted, frozen);
        }

        void set_shown_shifted(bool shown, bool shifted) {
            get_vtable()->set_shown_shifted(this, shown, shifted);
        }

        void set_time_left(f32 left);      // clamped at 0 from below
        void inc_time_left(f32 time_inc);  // clamped at 0 from below

        void set_run_when_hidden    (bool run_hidden);
        void set_display_hundredths (bool display);
        void set_display_thousandths(bool display);

        static void set_z(f32 z); // on the front end's timer widget
    };

    ASSERT_SIZEOF  (igo_3d_timer_widget_vtable,                    58 * 4);
    ASSERT_OFFSETOF(igo_3d_timer_widget_vtable, set_time_text,     0x0D8);
    ASSERT_OFFSETOF(igo_3d_timer_widget_vtable, set_color32,       0x0DC);
    ASSERT_OFFSETOF(igo_3d_timer_widget_vtable, show,              0x0E0);
    ASSERT_OFFSETOF(igo_3d_timer_widget_vtable, set_shown_shifted, 0x0E4);

    ASSERT_SIZEOF  (igo_3d_timer_widget,                      0x180);
    ASSERT_OFFSETOF(igo_3d_timer_widget, time_left,           0x150);
    ASSERT_OFFSETOF(igo_3d_timer_widget, time_modifier,       0x15C);
    ASSERT_OFFSETOF(igo_3d_timer_widget, col_from,            0x160);
    ASSERT_OFFSETOF(igo_3d_timer_widget, count_down,          0x168);
    ASSERT_OFFSETOF(igo_3d_timer_widget, run_when_hidden,     0x16C);
    ASSERT_OFFSETOF(igo_3d_timer_widget, display_thousandths, 0x16E);
    ASSERT_OFFSETOF(igo_3d_timer_widget, text_frozen,         0x170);
    ASSERT_OFFSETOF(igo_3d_timer_widget, text,                0x174);
} // treyarch
