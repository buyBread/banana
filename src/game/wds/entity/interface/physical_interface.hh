#pragma once

#include "game/wds/entity/interface/generic_interface.hh"
#include "game/container/multi_vector.hh"
#include "game/timing/simple_timer.hh"
#include "game/arch_base_vhandle.hh"
#include "game/math/algebra.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class biped_system;
    class guidance_system;
    class pendulum;
    class rigid_body_prop_head;
    struct physical_interface_driver;

    // SM3's _PHYS_*, as far as WoS's are read
    enum e_physical_interface_flags : u32 {
        physical_interface_flag_enabled           = 0x00000001,
        physical_interface_flag_suspended         = 0x00000002,
        physical_interface_flag_gravity           = 0x00000004,
        physical_interface_flag_immobile          = 0x00000100,
        physical_interface_flag_non_biped_physics = 0x00040000,
        physical_interface_flag_biped_running     = 0x00080000,
        physical_interface_flag_bouncy            = 0x10000000,
        physical_interface_flag_sticky            = 0x20000000
    };

    class physical_interface : public actor_interface {

    public:
        u32                        flags;
        f32                        mass;
        vector3d                   velocity;
        vector3d                   unk_020;
        vector3d                   velocity_increment;
        vector3d                   current_gravity_vector;
        vector3d                   angular_velocity;
        vector3d                   last_collision_normal;
        vector3d                   unk_05c;
        u32                        collision_flags;
        f32                        slide_factor;
        f32                        bounce_factor;
        f32                        sticky_offset;
        f32                        biped_physics_max_delta_t;
        simple_timer               gravity_delay_timer;
        vector3d                   bounce_pos;
        vector3d                   bounce_norm;
        arch_base_vhandle          bounce_ent;
        f32                        unk_09c;
        f32                        unk_0a0;
        f32                        blur_speed_min;
        f32                        blur_speed_max;
        arch_base_vhandle          collision_ent;
        f32                        gravity_multiplier;
        f32                        combat_gravity_multiplier;
        simple_timer               combat_gravity_restore_timer;
        guidance_system*           guide_sys;
        physical_interface_driver* current_driver;
        container::multi_vector
            <pendulum*>*           pendulums;
        f32                        drag_coefficient;
        f32                        drag_coefficient_min_speed;
        f32                        drag_coefficient_horz_scale;
        f32                        drag_coefficient_up_scale;
        f32                        drag_coefficient_down_scale;
        f32                        low_vel_time;
        rigid_body_prop_head*      m_rbph;
        biped_system*              m_bp_info;

        // inlined
        void set_flag(u32 flag, bool val = true) {
            if (val)
                flags |= flag;
            else
                flags &= ~flag;
        }

        // inlined
        bool is_flagged(e_physical_interface_flags flag) const {
            return (flags & flag) != 0;
        }

        // inlined
        bool is_enabled() const {
            return is_flagged(physical_interface_flag_enabled);
        }

        void suspend(bool val = true) {
            set_flag(physical_interface_flag_suspended, val);
        }

        // inlined
        bool is_suspended() const {
            return is_flagged(physical_interface_flag_suspended);
        }

        // inlined
        bool is_gravity() const {
            return is_flagged(physical_interface_flag_gravity);
        }

        // inlined
        void set_bouncy(bool c) {
            set_flag(physical_interface_flag_bouncy, c);
        }

        // inlined
        void set_sticky(bool c) {
            set_flag(physical_interface_flag_sticky, c);
        }

        // inlined
        void set_immobile(bool i) {
            set_flag(physical_interface_flag_immobile, i);
        }

        // inlined
        void set_allow_biped_physics(bool i) {
            set_flag(physical_interface_flag_non_biped_physics, !i);
        }

        // inlined
        void set_mass(f32 m) {
            mass = m;
        }

        // inlined
        void set_slide_factor(f32 v) {
            slide_factor = v;
        }

        // inlined
        void set_bounce_factor(f32 m) {
            bounce_factor = m;
        }

        // inlined
        f32 get_gravity_multiplier() const {
            return gravity_multiplier;
        }

        // inlined
        void set_gravity_multiplier(f32 g) {
            gravity_multiplier = g;
        }

        // inlined
        void set_angular_velocity(const vector3d &v) {
            angular_velocity = v;
        }

        void enable(bool val = true);
        void set_gravity(bool val = true); // a change also restarts the gravity delay

        void     set_velocity(const vector3d &v);
        vector3d get_velocity() const;

        void add_velocity_increment       (const vector3d &v, bool allow_limbo = false);
        void add_velocity_increment_at_pos(const vector3d &v, const vector3d &pos, f32 torque_mult, bool allow_limbo = false);

        void cancel_all_velocity(const vector3d &dir, bool both_dir = true);

        void          stop_biped_physics(bool on_destroyed = false);
        biped_system* get_biped_system() const; // null unless biped physics runs
        void          set_biped_physics_is_weapon(bool is_weapon, f32 unk_1, f32 unk_2);

        void start_prop_physics(const vector3d &force,
                                f32             random,
                                bool            lightweight       = false,
                                bool            kill_when_stable  = false,
                                bool            traverse_children = true,
                                bool            in_collision      = false);

        void stop_prop_physics(bool on_destroyed = false);
        bool is_prop_physics_running() const;
        bool is_prop_physics_stable(bool full_stable = false) const;

        void set_auto_prop_physics(bool i);
        void set_physics_ballistic_target(const vector3d &pos);

        pendulum* get_static_pendulum(u32 p_enum) const;
        void      set_static_pendulum(u32 p_enum, pendulum* _pendulum); // null clears the slot

        i32       add_dynamic_pendulum(pendulum* _pendulum); // answers its index
        pendulum* get_dynamic_pendulum(u32 index) const;
        pendulum* remove_dynamic_pendulum(i32 index);

        i32 get_num_active_pendulums() const;
    };

    ASSERT_OFFSETOF(physical_interface, flags,                 0x0C);
    ASSERT_OFFSETOF(physical_interface, angular_velocity,      0x44);
    ASSERT_OFFSETOF(physical_interface, last_collision_normal, 0x50);
    ASSERT_OFFSETOF(physical_interface, slide_factor,          0x6C);
    ASSERT_OFFSETOF(physical_interface, gravity_delay_timer,   0x7C);
    ASSERT_OFFSETOF(physical_interface, bounce_ent,            0x98);
    ASSERT_OFFSETOF(physical_interface, collision_ent,         0xAC);
    ASSERT_OFFSETOF(physical_interface, gravity_multiplier,    0xB0);
    ASSERT_OFFSETOF(physical_interface, current_driver,        0xC0);
    ASSERT_OFFSETOF(physical_interface, pendulums,             0xC4);
    ASSERT_OFFSETOF(physical_interface, low_vel_time,          0xDC);
    ASSERT_OFFSETOF(physical_interface, m_rbph,                0xE0);
    ASSERT_OFFSETOF(physical_interface, m_bp_info,             0xE4);
} // treyarch
