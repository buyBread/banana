#pragma once

#include "game/input/input_device.hh"
#include "game/delta_variable.hh"
#include "treyarch/mash/types.hh"
#include "game/timing/simple_timer.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

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
            gbflag_pressed            = 0x0001,
            gbflag_triggered          = 0x0002,
            gbflag_released           = 0x0004,
            gbflag_wait_for_release   = 0x0008,
            gbflag_disabled           = 0x0010,
            gbflag_external_primary   = 0x0020,
            gbflag_external_modifier  = 0x0040,
            gbflag_invert_axis        = 0x0080,
            gbflag_allow_neg_state    = 0x0100,
            gbflag_positive_tap       = 0x0200,
            gbflag_negative_tap       = 0x0400,
            gbflag_suspended          = 0x0800,
            gbflag_press_invalidated  = 0x1000,
            gbflag_missed_press       = 0x2000,
            gbflag_disallow_pos_state = 0x8000, // the update keeps both states at or below 0
            gbflag_non_responsive     = gbflag_disabled | gbflag_suspended
        };

        inline static constexpr f32 default_timeout = 0.3f;

        e_trigger_type      trigger_type;
        device_id_t         device_id;
        i32                 control;
        game_button*        primary;
        game_button*        modifier;
        delta_variable<f32> state;
        delta_variable<f32> unk_1c; // a second state, set alongside state
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

        game_button();
        explicit game_button(i32 control);

        // sub_687320
        ~game_button() {
            finalize(mash::ALLOCATED);
        }

        void initialize(mash::allocation_scope scope);
        void finalize(mash::allocation_scope scope);

        void set_flag(u32 flag, bool set) {
            if (set)
                flags |= flag;
            else
                flags &= ~flag;
        }

        bool is_flagged(u32 flag) const {
            return (flags & flag) != 0;
        }

        void enable(bool tof) {
            set_flag(gbflag_disabled, !tof);
        }

        bool is_enabled() const {
            return !is_flagged(gbflag_disabled);
        }

        // sub_736050
        // inlined @ sub_809BB0
        void set_device_id(device_id_t id) {
            device_id = id;

            if (primary && !is_flagged(gbflag_external_primary))
                primary->set_device_id(id);

            if (modifier && !is_flagged(gbflag_external_modifier))
                modifier->set_device_id(id);

            clear();
        }

        void set_control(i32 ctrl);

        void clear() {
            state.force(0.0f);

            pressed_time        = 0.0f;
            last_pressed_time   = 0.0f;
            tap_counter         = 0;
            tap_timeout         = default_timeout;
            time_since_last_tap = 0.0f;
            trigger_delay_timer = trigger_delay;

            set_flag(gbflag_positive_tap, false);
            set_flag(gbflag_negative_tap, false);
        }

        // inlined @ sub_72C9C0
        // inlined @ sub_72C9F0
        // inlined @ sub_737300
        bool suppressed(bool ignore_disabled) const {
            return !ignore_disabled && is_flagged(gbflag_non_responsive | gbflag_press_invalidated);
        }

        // sub_72C9C0
        bool pressed(bool ignore_disabled = false) const {
            return !suppressed(ignore_disabled) && is_flagged(gbflag_pressed);
        }

        // sub_72C9F0
        f32 get_pressed_time(bool ignore_disabled = false) const {
            return suppressed(ignore_disabled) ? 0.0f : pressed_time;
        }

        // sub_737300
        f32 normalized_state(bool ignore_disabled = false) const { // the state rescaled past the dead zone either way
            const f32 value = suppressed(ignore_disabled) ? 0.0f : state.current;

            if (!(dead_zone > 0.0001))
                return value;

            if (value > dead_zone)
                return (value - dead_zone) / (1.0f - dead_zone);

            if (-0.0f - dead_zone > value)
                return (dead_zone + value) / (1.0f - dead_zone);

            return 0.0f;
        }

        bool triggered(bool ignore_disabled = false) const {
            if (!ignore_disabled && is_flagged(gbflag_non_responsive))
                return false;

            return is_flagged(gbflag_triggered);
        }

        void update(f32 t, bool unk_1, bool unk_2);
        void update_state(f32 t, f32 state, f32 unk_state); // pushes both values, then runs the press and tap logic over t
    };

    ASSERT_SIZEOF  (game_button,                      0x4C);
    ASSERT_OFFSETOF(game_button, state,               0x14);
    ASSERT_OFFSETOF(game_button, pressed_time,        0x24);
    ASSERT_OFFSETOF(game_button, trigger_delay_timer, 0x34);
    ASSERT_OFFSETOF(game_button, min_release_state,   0x44);
    ASSERT_OFFSETOF(game_button, tap_counter,         0x48);
    ASSERT_OFFSETOF(game_button, flags,               0x4A);
} // treyarch
