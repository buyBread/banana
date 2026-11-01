#pragma once

#include "game/boolx.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class actor;

    // SM3 swing_params, the hero's swing tune and als swing angles; retail inserts a few blocks
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
        f32   unk_54;
        f32   unk_58;
        f32   collision_break_length;
        f32   collision_stick_length;
        f32   collision_min_time;
        f32   ground_clearance;
        f32   max_dot_for_velocity_preservation;
        f32   min_reel_in_rate;
        f32   max_reel_in_rate;
        f32   unk_78;                            // get_reel_in_rate's max_time
        f32   unk_7c;                            // get_reel_in_rate's max_range
        f32   unk_80;
        f32   unk_84;                            // get_blacksuit_swing_params's swing_velocity_mod
        f32   unk_88;                            // get_blacksuit_swing_params's to_anchor_force
        f32   boost_cooldown_time;
        f32   min_boost_angle;
        f32   max_boost_angle;
        f32   boost_force;
        f32   unk_9c;                            // get_swing_boost_force's base
        f32   unk_a0;                            // get_swing_boost_force's rate
        f32   unk_a4;                            // get_fake_anchor_params's max_time
        f32   unk_a8;                            // get_fake_anchor_params's pull_rate
        f32   drag_coefficient_base;
        f32   drag_coefficient_rate;
        f32   drag_coefficient_min_speed_base;
        f32   drag_coefficient_min_speed_rate;
        f32   jump_boost_base;
        f32   jump_boost_rate;
        f32   swing_level_min;
        f32   swing_level_max;
        f32   swing_level_cur;
        f32   drag_coefficient_horz_scale;
        f32   drag_coefficient_up_scale;
        f32   drag_coefficient_down_scale;
        f32   unk_dc;                            // the running mean of the webs' lengths
        f32   unk_e0;                            // the running mean of their distance from unk_dc
        i32   unk_e4;                            // how many webs went into both
        f32   unk_e8;
        boolx avoid_ground;
        u8    pad_0ed[0x03];

        void frame_advance(actor* my_actor, f32 delta_t);
        void update_swing_angles(actor* my_actor);
        void set_sweet_spot_incline_angle_degrees(f32 angle);
        void set_sweet_cone_angle_degrees(f32 angle);
        void set_max_dynamic_sweet_angle(f32 angle);
        void set_swing_ground_clearance(f32 len);
        void set_min_reel_in_rate(f32 rate);
        void set_max_reel_in_rate(f32 rate);
        void set_unk_78(f32 value);
        void set_unk_7c(f32 value);
        void set_unk_80(f32 value);
    };

    ASSERT_SIZEOF  (swing_params,                                    0xF0);
    ASSERT_OFFSETOF(swing_params, r_hand_is_locked,                  0x18);
    ASSERT_OFFSETOF(swing_params, min_web_length_squared,            0x1C);
    ASSERT_OFFSETOF(swing_params, sweet_spot_incline_angle_cos,      0x24);
    ASSERT_OFFSETOF(swing_params, sweet_cone_angle_cos,              0x30);
    ASSERT_OFFSETOF(swing_params, max_high_speed_sweet_length,       0x40);
    ASSERT_OFFSETOF(swing_params, max_no_anchor_sweet_length,        0x44);
    ASSERT_OFFSETOF(swing_params, min_dynamic_sweet_angle,           0x4C);
    ASSERT_OFFSETOF(swing_params, max_dynamic_sweet_angle,           0x50);
    ASSERT_OFFSETOF(swing_params, collision_break_length,            0x5C);
    ASSERT_OFFSETOF(swing_params, collision_min_time,                0x64);
    ASSERT_OFFSETOF(swing_params, ground_clearance,                  0x68);
    ASSERT_OFFSETOF(swing_params, max_dot_for_velocity_preservation, 0x6C);
    ASSERT_OFFSETOF(swing_params, min_reel_in_rate,                  0x70);
    ASSERT_OFFSETOF(swing_params, unk_80,                            0x80);
    ASSERT_OFFSETOF(swing_params, unk_84,                            0x84);
    ASSERT_OFFSETOF(swing_params, boost_cooldown_time,               0x8C);
    ASSERT_OFFSETOF(swing_params, boost_force,                       0x98);
    ASSERT_OFFSETOF(swing_params, unk_9c,                            0x9C);
    ASSERT_OFFSETOF(swing_params, unk_a4,                            0xA4);
    ASSERT_OFFSETOF(swing_params, drag_coefficient_base,             0xAC);
    ASSERT_OFFSETOF(swing_params, jump_boost_base,                   0xBC);
    ASSERT_OFFSETOF(swing_params, swing_level_min,                   0xC4);
    ASSERT_OFFSETOF(swing_params, swing_level_cur,                   0xCC);
    ASSERT_OFFSETOF(swing_params, drag_coefficient_horz_scale,       0xD0);
    ASSERT_OFFSETOF(swing_params, unk_dc,                            0xDC);
    ASSERT_OFFSETOF(swing_params, unk_e4,                            0xE4);
    ASSERT_OFFSETOF(swing_params, unk_e8,                            0xE8);
    ASSERT_OFFSETOF(swing_params, avoid_ground,                      0xEC);
} // treyarch
