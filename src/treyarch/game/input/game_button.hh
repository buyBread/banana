#pragma once

#include "treyarch/game/input/input_device.hh"
#include "treyarch/shared/delta_variable.hh"
#include "treyarch/shared/timing/simple_timer.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class game_button {

    public:
        enum e_trigger_type : i32 {
            trigger_and,
            trigger_or,
            trigger_xor,
            trigger_nand,
            trigger_sequential,
            trigger_andnot,
            trigger_add,
            trigger_subtract,
            trigger_double
        };

        enum e_flags : u16 {
            gbflag_pressed           = 0x0001,
            gbflag_triggered         = 0x0002,
            gbflag_released          = 0x0004,
            gbflag_wait_for_release  = 0x0008,
            gbflag_disabled          = 0x0010,
            gbflag_external_primary  = 0x0020,
            gbflag_external_modifier = 0x0040,
            gbflag_invert_axis       = 0x0080,
            gbflag_allow_neg_state   = 0x0100,
            gbflag_positive_tap      = 0x0200,
            gbflag_negative_tap      = 0x0400,
            gbflag_suspended         = 0x0800,
            gbflag_press_invalidated = 0x1000,
            gbflag_missed_press      = 0x2000,
            gbflag_non_responsive    = gbflag_disabled | gbflag_suspended
        };

        e_trigger_type      trigger_type;
        device_id_t         device_id;
        i32                 control;
        game_button*        primary;
        game_button*        modifier;
        delta_variable<f32> state;
        u32                 unk_1c;
        u32                 unk_20;
        f32                 pressed_time;
        f32                 last_pressed_time;
        f32                 time_since_last_tap;
        f32                 tap_timeout;
        simple_timer        trigger_delay_timer;
        f32                 trigger_delay;
        f32                 dead_zone;
        f32                 min_trigger_state;
        f32                 min_release_state;
        i16                 tap_counter;
        u16                 flags;

        explicit game_button(i32 control);

        bool is_flagged(u32 flag) const {
            return (flags & flag) != 0;
        }

        bool triggered(bool ignore_disabled = false) const {
            if (!ignore_disabled && is_flagged(gbflag_non_responsive))
                return false;

            return is_flagged(gbflag_triggered);
        }

        void update(f32 t, bool unk_1, bool unk_2);
    };

    ASSERT_SIZEOF  (game_button,                      0x4C);
    ASSERT_OFFSETOF(game_button, state,               0x14);
    ASSERT_OFFSETOF(game_button, pressed_time,        0x24);
    ASSERT_OFFSETOF(game_button, trigger_delay_timer, 0x34);
    ASSERT_OFFSETOF(game_button, min_release_state,   0x44);
    ASSERT_OFFSETOF(game_button, tap_counter,         0x48);
    ASSERT_OFFSETOF(game_button, flags,               0x4A);
} // treyarch
