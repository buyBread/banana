#pragma once

#include "game/wds/entity/interface/generic_interface.hh"
#include "game/bounded_variable.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // SM3's eDamageInterfaceFlags, as far as WoS's are read
    enum e_damage_interface_flags : u32 {
        damage_ifc_flag_immune_to_death_from_damage = 0x00000100,
        damage_ifc_flag_dealt_damage_this_frame     = 0x02000000,
        damage_ifc_flag_die_on_fall_damage          = 0x80000000
    };

    // WoS's second flag word
    enum e_damage_interface_ext_flags : u32 {
        damage_ifc_ext_flag_use_distance_based_invulnerability = 0x00000008,
        damage_ifc_ext_flag_disable_push_away_force_on_damage  = 0x00000040
    };

    struct damage_stun {
        i32 stun_hp;
        i32 stun_restore_hp;
        f32 recover_duration;
        f32 recover_remaining;
    };

    struct damage_web_subdue {
        u32 unk_00;
        f32 max_web_hit_points;
        f32 web_hit_points;     // subdued at 0 or below
        u8  reserved_0c[0x08];
        f32 recover_duration;
    };

    class damage_interface : public actor_interface {

    public:
        u8                    reserved_00c[0x90];      // dmg_info, the last damage record, the life bar
        u32                   flags;
        u32                   ext_flags;
        u32                   unk_0a4;
        i32                   damage_dealt;
        u8                    reserved_0ac[0x08];
        bounded_variable<i32> hit_points;
        bounded_variable<i32> overkill_hit_points;
        bounded_variable<i32> armor_points;
        f32                   damage_mod;
        i32                   damage_lock_threshold;   // -1 for none
        u8                    reserved_0e0[0x20];
        damage_stun*          stun;
        damage_web_subdue*    web_subdue;
        u8                    reserved_108[0x2C];
        f32                   force_attack_multiplier;
        f32                   force_react_multiplier;

        // inlined
        i32 get_max_hit_points() const {
            return hit_points.get_max_val();
        }

        void set_hit_points(i32 value, bool use_signals = false, entity* attacker = nullptr, entity* attacker_weapon = nullptr);
    };

    ASSERT_OFFSETOF(damage_stun, recover_remaining, 0x0C);

    ASSERT_OFFSETOF(damage_web_subdue, web_hit_points,   0x08);
    ASSERT_OFFSETOF(damage_web_subdue, recover_duration, 0x14);

    ASSERT_OFFSETOF(damage_interface, flags,                   0x09C);
    ASSERT_OFFSETOF(damage_interface, damage_dealt,            0x0A8);
    ASSERT_OFFSETOF(damage_interface, hit_points,              0x0B4);
    ASSERT_OFFSETOF(damage_interface, armor_points,            0x0CC);
    ASSERT_OFFSETOF(damage_interface, damage_lock_threshold,   0x0DC);
    ASSERT_OFFSETOF(damage_interface, stun,                    0x100);
    ASSERT_OFFSETOF(damage_interface, web_subdue,              0x104);
    ASSERT_OFFSETOF(damage_interface, force_attack_multiplier, 0x134);
} // treyarch
