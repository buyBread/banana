#pragma once

#include "game/arch_base_vhandle.hh"
#include "game/wds/entity/interface/generic_interface.hh"
#include "treyarch/mash/string_hash.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class switch_obj_interface : public entity_interface {

    public:
        enum e_switch_state : i32 {
            switch_off = 0,
            switch_on  = 1
        };

        enum e_switch_flags : u32 {
            switch_flipped         = 0x01,
            switch_alarm           = 0x02,
            switch_web_target      = 0x04,
            switch_one_time        = 0x08,
            switch_ignore_hero     = 0x10,
            switch_enabled         = 0x20,
            switch_web_target_only = 0x40
        };

        switch_obj_interface* sl_next_element; // all_switch_objs
        switch_obj_interface* sl_prev_element;
        void*                 sl_list_owner;
        string_hash           flick_animation;
        e_switch_state        state;
        arch_base_vhandle     flicker_ent;
        f32                   radius_mod;
        u32                   switch_flags;

        void set_flag(e_switch_flags flag, bool onoff) {
            if (onoff)
                switch_flags |= flag;
            else
                switch_flags &= ~flag;
        }

        void set_state(e_switch_state s) {
            state = s;
        }

        void set_web_targetable(bool onoff) {
            set_flag(switch_web_target, onoff);
        }

        void set_web_target_only(bool onoff) {
            set_flag(switch_web_target_only, onoff);
        }

        void set_ignore_hero(bool b) {
            set_flag(switch_ignore_hero, b);
        }

        void set_alarm(bool onoff) {
            set_flag(switch_alarm, onoff);
        }

        void set_enabled(bool enabled) {
            set_flag(switch_enabled, enabled);
        }

        // sub_601710
        void set_single_use(bool onoff) {
            set_flag(switch_one_time, onoff);
        }

        // sub_601730
        void set_no_use() { // as if flipped for good
            switch_flags |= switch_flipped | switch_one_time;
        }

        // sub_601740
        void set_usable() {
            switch_flags &= ~switch_flipped;
        }
    };

    ASSERT_SIZEOF  (switch_obj_interface,                  0x2C);
    ASSERT_OFFSETOF(switch_obj_interface, sl_next_element, 0x0C);
    ASSERT_OFFSETOF(switch_obj_interface, flick_animation, 0x18);
    ASSERT_OFFSETOF(switch_obj_interface, radius_mod,      0x24);
    ASSERT_OFFSETOF(switch_obj_interface, switch_flags,    0x28);
} // treyarch
