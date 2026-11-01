#pragma once

#include "game/color.hh"
#include "game/frontend/igo/igo_3d_text.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
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

        virtual void set_time_text(f32 seconds, bool hundredths, bool thousandths) = 0; // m:ss into text
        virtual void set_color32(color32 col) = 0;
        virtual void show(bool shown, bool shifted, i32 text_frozen) = 0;
        virtual void set_shown_shifted(bool shown, bool shifted) = 0;

        void set_time_left(f32 left);      // clamped at 0 from below
        void inc_time_left(f32 time_inc);  // clamped at 0 from below

        void set_run_when_hidden    (bool run_hidden);
        void set_display_hundredths (bool display);
        void set_display_thousandths(bool display);

        static void set_z(f32 z); // on the front end's timer widget
    };

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
