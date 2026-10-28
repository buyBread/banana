#pragma once

#include "game/wds/entity/interface/generic_interface.hh"
#include "game/event/generic_event_callback.hh"
#include "game/timing/simple_timer.hh"
#include "game/arch_base_vhandle.hh"
#include "game/math/algebra.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class entity;

    // named after the natives that read and write them
    enum e_manip_obj_flags : u32 {
        manip_obj_flag_throwable          = 0x00000010,
        manip_obj_flag_allow_pickup       = 0x00000400,
        manip_obj_flag_allow_putdown      = 0x00000800,
        manip_obj_flag_allow_auto_pickup  = 0x00001000,
        manip_obj_flag_allow_auto_putdown = 0x00002000
    };

    class manip_obj_interface : public actor_interface {

    public:
        manip_obj_interface*   sl_next_element;
        manip_obj_interface*   sl_prev_element;
        void*                  sl_list_owner;
        u32                    unk_018;
        i32                    damage;
        i32                    damage_type;
        i32                    unk_024;
        arch_base_vhandle      thrower;
        simple_timer           ignore_thrower_collision_timer;
        generic_event_callback manip_obj_callback;
        generic_event_callback standard_manip_obj_callback;
        arch_base_vhandle      target;
        arch_base_vhandle      dodged_target;
        vector3d               target_pos;
        f32                    homing_time;
        simple_timer           homing_timer;
        simple_timer           auto_activate_timer;
        f32                    throw_velocity;
        u8                     reserved_064[0x0C];
        u32                    flags;
        i32                    collision_frame;
        vector3d               collision_frame_normal;
        u8                     reserved_084[0x84];

        void set_flag(e_manip_obj_flags flag, bool onoff) {
            if (onoff)
                flags |= flag;
            else
                flags &= ~flag;
        }

        // sub_6016D0
        bool get_allow_pickup() const {
            return (flags >> 10) & 1;
        }

        // sub_6016E0
        bool get_allow_putdown() const {
            return (flags >> 11) & 1;
        }

        // sub_6016B0
        bool get_allow_auto_pickup() const {
            return (flags >> 12) & 1;
        }

        // sub_6016C0
        bool get_allow_auto_putdown() const {
            return (flags >> 13) & 1;
        }

        // sub_824BA0
        void set_allow_auto_pickup(bool allow) { // also allows or forbids pickup itself
            if (allow) {
                flags |= manip_obj_flag_allow_pickup;
                flags |= manip_obj_flag_allow_auto_pickup;
            } else {
                flags &= ~manip_obj_flag_allow_pickup;
                flags &= ~manip_obj_flag_allow_auto_pickup;
            }
        }

        // sub_824BE0
        void set_allow_auto_putdown(bool allow) { // also allows or forbids putdown itself
            if (allow) {
                flags |= manip_obj_flag_allow_putdown;
                flags |= manip_obj_flag_allow_auto_putdown;
            } else {
                flags &= ~manip_obj_flag_allow_putdown;
                flags &= ~manip_obj_flag_allow_auto_putdown;
            }
        }

        void throw_it           (entity* thrower, const vector3d &target_pos, entity* target, bool use_rotation, const vector3d &rotation);
        void throw_it_velocity  (entity* thrower, const vector3d &force, const vector3d &rotation, bool unk);
        void throw_it_with_speed(entity* thrower, f32 speed, entity* target, const vector3d &rotation, f32 gravity_scale, f32 max_reach);

        void disarm();

        arch_base_vhandle update_for_grab(entity* grabber);
    };

    ASSERT_SIZEOF  (manip_obj_interface,                         0x108);
    ASSERT_OFFSETOF(manip_obj_interface, sl_next_element,        0x0C);
    ASSERT_OFFSETOF(manip_obj_interface, damage,                 0x1C);
    ASSERT_OFFSETOF(manip_obj_interface, thrower,                0x28);
    ASSERT_OFFSETOF(manip_obj_interface, manip_obj_callback,     0x30);
    ASSERT_OFFSETOF(manip_obj_interface, target,                 0x40);
    ASSERT_OFFSETOF(manip_obj_interface, target_pos,             0x48);
    ASSERT_OFFSETOF(manip_obj_interface, throw_velocity,         0x60);
    ASSERT_OFFSETOF(manip_obj_interface, flags,                  0x70);
    ASSERT_OFFSETOF(manip_obj_interface, collision_frame_normal, 0x78);
} // treyarch
