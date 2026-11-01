#pragma once

#include "game/timing/simple_timer.hh"
#include "game/color.hh"
#include "game/math/algebra/vector2d.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_text;

    // SM3 player_combo_counter; WoS adds three ints that only player_interface's display is allocated with
    class player_combo_counter {

    public:
        igo_3d_text* combo_point_text;
        igo_3d_text* combo_string_text;
        color32      base_color;
        i32          color_offset[4];
        f32          color_duration;
        simple_timer color_timer;
        vector2d     base_scale;
        vector2d     scale_mod;
        f32          scale_duration;
        simple_timer scale_timer;
        vector2d     position;
        vector2d     text_offset;
        i32          unk_4c;         // player_interface slots 290 / 293
        i32          unk_50;         // player_interface slots 289 / 294
        i32          combo_counter;

        // inlined @ sub_996680
        void flash_color(const color32 &col, f32 duration) {
            color_offset[0] = (i32)col.r - (i32)base_color.r;
            color_offset[1] = (i32)col.g - (i32)base_color.g;
            color_offset[2] = (i32)col.b - (i32)base_color.b;
            color_offset[3] = (i32)col.a - (i32)base_color.a;
            color_duration  = duration;
            color_timer     = duration;
        }

        // inlined @ sub_996680
        void scale_text(const vector2d &scale, f32 duration) {
            scale_mod.x    = (f32)((f64)scale.x - base_scale.x);
            scale_mod.y    = (f32)((f64)scale.y - base_scale.y);
            scale_duration = duration;
            scale_timer    = duration;
        }
    };

    ASSERT_SIZEOF  (player_combo_counter,                0x58);
    ASSERT_OFFSETOF(player_combo_counter, base_color,    0x08);
    ASSERT_OFFSETOF(player_combo_counter, color_timer,   0x20);
    ASSERT_OFFSETOF(player_combo_counter, base_scale,    0x24);
    ASSERT_OFFSETOF(player_combo_counter, scale_timer,   0x38);
    ASSERT_OFFSETOF(player_combo_counter, text_offset,   0x44);
    ASSERT_OFFSETOF(player_combo_counter, unk_4c,        0x4C);
    ASSERT_OFFSETOF(player_combo_counter, combo_counter, 0x54);
} // treyarch
