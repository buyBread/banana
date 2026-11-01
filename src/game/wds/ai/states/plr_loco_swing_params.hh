#pragma once

#include "game/boolx.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // SM3 swing_params, as far as it's read; retail adds two floats after max_high_speed_sweet_length
    class swing_params {

    public:
        f32   main_angle;
        f32   side_angle;
        f32   entry_speed;
        f32   speed;
        f32   to_anchor_angle;
        f32   to_anchor_length;
        boolx r_hand_is_locked;
        boolx l_hand_is_locked;
        u8    pad_01a[0x02];
        f32   min_web_length_squared;
        f32   max_web_length_squared;
        f32   sweet_spot_incline_angle_cos;
        f32   sweet_spot_incline_angle_sin;
        f32   sweet_spot_distance;
        f32   sweet_cone_angle_cos;
        f32   min_dynamic_sweet_length;
        f32   max_dynamic_sweet_length;
        f32   sweet_spot_overdrive_velocity;
        f32   max_high_speed_sweet_length;
        f32   max_no_anchor_sweet_length;
        f32   unk_48;
        f32   min_dynamic_sweet_angle;
        f32   max_dynamic_sweet_angle;
    };

    ASSERT_OFFSETOF(swing_params, r_hand_is_locked,             0x18);
    ASSERT_OFFSETOF(swing_params, min_web_length_squared,       0x1C);
    ASSERT_OFFSETOF(swing_params, sweet_spot_incline_angle_cos, 0x24);
    ASSERT_OFFSETOF(swing_params, sweet_cone_angle_cos,         0x30);
    ASSERT_OFFSETOF(swing_params, max_high_speed_sweet_length,  0x40);
    ASSERT_OFFSETOF(swing_params, max_no_anchor_sweet_length,   0x44);
    ASSERT_OFFSETOF(swing_params, min_dynamic_sweet_angle,      0x4C);
    ASSERT_OFFSETOF(swing_params, max_dynamic_sweet_angle,      0x50);
} // treyarch
