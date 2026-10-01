#pragma once

#include "treyarch/game/wds/entity/interface/generic_interface.hh"
#include "treyarch/shared/dinkumware/vector.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    class time_interface;

    // SM3 eTimeMode
    enum e_time_mode : i32 {
        time_mode_world,
        time_mode_absolute,
        time_mode_relative
    };

    // SM3 eCombatDilateMode
    enum e_combat_dilate_mode : i32 {
        combat_dilate_mode_disabled,
        combat_dilate_mode_active,
        combat_dilate_mode_delay,
        combat_dilate_mode_smooth_in,
        combat_dilate_mode_smooth_out
    };

    struct time_interface_vtable : generic_interface_vtable {
        void (__thiscall* frame_advance)(time_interface* self, f32 t); // sub_5FD3E0
    };

    class time_interface : public entity_interface {

    public:
        f32                  time_dilation;
        f32                  attack_dilation;
        f32                  combat_dilation;
        u8                   reserved_018[0x1C];
        e_time_mode          time_mode;
        e_combat_dilate_mode combat_dilate_mode;
        boolx                ignore_reflex_dilation;

        time_interface_vtable* get_vtable() const {
            return (time_interface_vtable*)vtable;
        }

        bool is_combat_dilated() const {
            return combat_dilate_mode != combat_dilate_mode_disabled && combat_dilate_mode != combat_dilate_mode_delay;
        }

        f32 calc_time_dilation();

        void frame_advance(f32 t) {
            get_vtable()->frame_advance(this, t);
        }

        static void frame_advance_all_time_interfaces(f32 t);
    };

    namespace references {
        inline util::memory_reference<dinkumware::vector<time_interface*>*> all_time_interfaces { 0x00FFEABC };
    } // references

    ASSERT_OFFSETOF(time_interface, time_dilation,          0x0C);
    ASSERT_OFFSETOF(time_interface, attack_dilation,        0x10);
    ASSERT_OFFSETOF(time_interface, combat_dilation,        0x14);
    ASSERT_OFFSETOF(time_interface, time_mode,              0x34);
    ASSERT_OFFSETOF(time_interface, combat_dilate_mode,     0x38);
    ASSERT_OFFSETOF(time_interface, ignore_reflex_dilation, 0x3C);
} // treyarch
