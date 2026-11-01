#pragma once

#include "game/timing/recharge_timer.hh"
#include "game/wds/ai/ai_tentacle_info.hh"
#include "game/wds/entity/pendulum.hh"
#include "game/arch_base_vhandle.hh"
#include "game/boolx.hh"
#include "game/math/algebra/vector3d.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class actor;
    class ai_tentacle_slack;
    class ai_tentacle_web_curly;
    class player_interface;

    // SM3 eSwingerHand
    enum e_swinger_hand : i32 {
        swinger_none  = -1,
        swinger_right = 0,
        swinger_left  = 1,
        max_swingers  = 2,
        swinger_both  = max_swingers
    };

    // SM3 eSwingerStatus
    enum e_swinger_status : i32 {
        swinger_inactive,
        swinger_animating,
        swinger_launching,
        swinger_attached
    };

    // SM3 swing_anchor_data; retail adds a point before goal_constraint
    struct swing_anchor_data {
        arch_base_vhandle anchor;
        vector3d          pos;
        vector3d          unk_10;
        f32               goal_constraint;
    };

    // SM3 swing_data, one per hand; retail adds unk_54, unk_68 and unk_2d4
    class swing_data {

    public:
        e_swinger_status       status;
        pendulum               attach_pendulum;
        vector3d               unk_54;          // where the web is shot at
        f32                    goal_constraint;
        f32                    base_length;
        f32                    unk_68;          // how long the web has been blocked short of its pivot
        ai_tentacle_info       web;
        ai_tentacle_info       curly;
        u32                    fire_engine_id;  // the swirl strike on web
        ai_tentacle_web_curly* curly_engine;
        ai_tentacle_slack*     unk_2d4;         // on web
        boolx                  for_zip;
        u8                     pad_2d9[0x03];

        void set_my_actor(actor* own); // builds both lines' engines and hides them

        bool frame_advance(player_interface* plr_ifc, f32 delta_t, i32 index); // true when the web should break
        void post_process(player_interface* plr_ifc, f32 delta_t, i32 index);
        void anim_started();
        void fire(f32 speed = 50.0f, f32 accel = 50.0f, f32 curve_rad = 1.0f, bool zip = false);
        void hide();
    };

    // SM3 swing_leftover, a broken web left dangling
    class swing_leftover {

    public:
        pendulum         attach_pendulum;
        ai_tentacle_info web;
        recharge_timer   life_time;
        recharge_timer   fade_time;

        void set_my_actor(actor* own);

        void frame_advance(f32 delta_t); // the web hangs for life_time, then fades out over fade_time
        void post_process(f32 delta_t);
        void copy(const swing_data* data);
        void hide();
    };

    ASSERT_SIZEOF  (swing_anchor_data,                  0x20);
    ASSERT_OFFSETOF(swing_anchor_data, pos,             0x04);
    ASSERT_OFFSETOF(swing_anchor_data, unk_10,          0x10);
    ASSERT_OFFSETOF(swing_anchor_data, goal_constraint, 0x1C);

    ASSERT_SIZEOF  (swing_data,                  0x2DC);
    ASSERT_OFFSETOF(swing_data, attach_pendulum, 0x004);
    ASSERT_OFFSETOF(swing_data, unk_54,          0x054);
    ASSERT_OFFSETOF(swing_data, goal_constraint, 0x060);
    ASSERT_OFFSETOF(swing_data, unk_68,          0x068);
    ASSERT_OFFSETOF(swing_data, web,             0x06C);
    ASSERT_OFFSETOF(swing_data, curly,           0x19C);
    ASSERT_OFFSETOF(swing_data, fire_engine_id,  0x2CC);
    ASSERT_OFFSETOF(swing_data, unk_2d4,         0x2D4);
    ASSERT_OFFSETOF(swing_data, for_zip,         0x2D8);

    ASSERT_SIZEOF  (swing_leftover,            0x190);
    ASSERT_OFFSETOF(swing_leftover, web,       0x050);
    ASSERT_OFFSETOF(swing_leftover, life_time, 0x180);
    ASSERT_OFFSETOF(swing_leftover, fade_time, 0x188);
} // treyarch
