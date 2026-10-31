#pragma once

#include "game/arch_base_vhandle.hh"
#include "game/boolx.hh"
#include "game/math/algebra/vector3d.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class physical_interface;

    // what steers a physical interface's flight; only rockets have one
    class guidance_system {

    public:
        // SM3's eGuidanceSysType
        enum e_guidance_sys_type : i32 {
            guidance_generic,
            guidance_rocket,
            guidance_unknown
        };

        void**              vtable;
        physical_interface* owner;
        f32                 launch_force;
        vector3d            launch_dir;
        u32                 flags;
        boolx               dynamic;
        u8                  pad_01d[0x03];

        // inlined
        void set_flag(u32 flag, bool val = true) {
            if (val)
                flags |= flag;
            else
                flags &= ~flag;
        }

        // slot 3
        void launch(const vector3d &dir, f32 force) {
            ((void (__thiscall*)(guidance_system*, const vector3d*, f32))vtable[3])(this, &dir, force);
        }
    };

    // SM3's _ROCKET_*
    enum e_rocket_guidance_flags : u32 {
        rocket_guidance_flag_guided          = 0x00000001,
        rocket_guidance_flag_point_guided    = 0x00000002,
        rocket_guidance_flag_sin_wave_guided = 0x00000004
    };

    class rocket_guidance_sys : public guidance_system {

    public:
        vector3d          target_pos;
        arch_base_vhandle target;
        f32               launch_force;
        f32               wobble_timer;
        f32               guidance_delay;
        f32               accel_delay;
        f32               guided_accuracy;
        f32               turn_factor;
        f32               accel_factor;
        f32               full_wobble_timer;
        f32               full_guidance_delay;
        f32               full_accel_delay;
        f32               wobble_timer_var;
        f32               guidance_delay_var;
        f32               accel_delay_var;
        f32               thrust_timer;
        u8                reserved_068[0x2C];

        // inlined
        void set_guided(bool val = true) {
            set_flag(rocket_guidance_flag_guided, val);
        }

        // inlined
        void set_point_guided(bool val = true) {
            set_flag(rocket_guidance_flag_point_guided, val);
        }

        void set_target(const arch_base_vhandle &ent);

        // inlined
        void set_target_pos(const vector3d &pos) {
            target_pos = pos;
        }

        // inlined
        void set_guided_accuracy(f32 f) {
            guided_accuracy = f;
        }

        // inlined
        void set_turn_factor(f32 f) {
            turn_factor = f;
        }

        // inlined
        void set_accel_factor(f32 f) {
            accel_factor = f;
        }

        // inlined
        void set_wobble_timer(f32 t) {
            full_wobble_timer = t;
        }

        // inlined
        void set_guidance_delay(f32 t) {
            full_guidance_delay = t;
        }

        // inlined
        void set_accel_delay(f32 t) {
            full_accel_delay = t;
        }

        // inlined
        void set_wobble_timer_var(f32 v) {
            wobble_timer_var = v;
        }

        // inlined
        void set_guidance_delay_var(f32 v) {
            guidance_delay_var = v;
        }

        // inlined
        void set_accel_delay_var(f32 v) {
            accel_delay_var = v;
        }

        // inlined
        void set_thrust_timer(f32 t) {
            thrust_timer = t;
        }
    };

    ASSERT_SIZEOF  (guidance_system,               0x20);
    ASSERT_OFFSETOF(guidance_system, flags,        0x18);

    ASSERT_SIZEOF  (rocket_guidance_sys,                  0x94);
    ASSERT_OFFSETOF(rocket_guidance_sys, target_pos,      0x20);
    ASSERT_OFFSETOF(rocket_guidance_sys, target,          0x2C);
    ASSERT_OFFSETOF(rocket_guidance_sys, guidance_delay,  0x38);
    ASSERT_OFFSETOF(rocket_guidance_sys, guided_accuracy, 0x40);
    ASSERT_OFFSETOF(rocket_guidance_sys, thrust_timer,    0x64);
} // treyarch
