#pragma once

#include "game/wds/entity/interface/generic_interface.hh"
#include "game/wds/entity/interface/physical_interface.hh"
#include "game/arch_base_vhandle.hh"
#include "game/bounded_variable.hh"
#include "game/variance_variable.hh"
#include "treyarch/mash/string_hash.hh"
#include "game/math/algebra/vector3d.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class combo_move_data;

    // SM3's eDamageInterfaceFlags, as far as WoS's are read
    enum e_damage_interface_flags : u32 {
        damage_ifc_flag_immune_to_death_from_damage = 0x00000100,
        damage_ifc_flag_stunned                     = 0x00000800,
        damage_ifc_flag_dealt_damage_this_frame     = 0x02000000,
        damage_ifc_flag_die_on_fall_damage          = 0x80000000
    };

    // WoS's second flag word
    enum e_damage_interface_ext_flags : u32 {
        damage_ifc_ext_flag_life_bar_hidden                    = 0x00000001,
        damage_ifc_ext_flag_use_distance_based_invulnerability = 0x00000008,
        damage_ifc_ext_flag_webcast_enabled                    = 0x00000010,
        damage_ifc_ext_flag_has_webcast                        = 0x00000020,
        damage_ifc_ext_flag_disable_push_away_force_on_damage  = 0x00000040,
        damage_ifc_ext_flag_disable_pain_sounds                = 0x00000080
    };

    struct damage_info {
        u8                reserved_00[0x6C];
        arch_base_vhandle attacker;
        arch_base_vhandle attacker_weapon;
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
        f32 recover_remaining;
    };

    // a negative level, style or chance is unset
    struct damage_webcast {
        string_hash key;
        f32         default_level;               // answered while level is unset
        f32         level;
        u8          sync_enabled;
        u8          pad_0d[0x03];
        i32         default_style;               // answered while style is unset
        i32         style;
        f32         counter_scale;
        f32         counter_chance;
        f32         counter_chance_override;
        f32         counter_chance_default;
        f32         counter_chance_not_standing;
        f32         counter_chance_standing;
        f32         counter_chance_web_subdued;
        f32         counter_chance_adv_standing;
        f32         counter_chance_wounded;
        f32         counter_chance_combat;

        // inlined @ sub_60A4A0
        damage_webcast() : default_level(0.0f),
                           level(-1.0f),
                           sync_enabled(1),
                           default_style(0),
                           style(-1),
                           counter_scale(-1.0f),
                           counter_chance(-1.0f),
                           counter_chance_override(-1.0f),
                           counter_chance_default(0.0f),
                           counter_chance_not_standing(-1.0f),
                           counter_chance_standing(-1.0f),
                           counter_chance_web_subdued(-1.0f),
                           counter_chance_adv_standing(-1.0f),
                           counter_chance_wounded(-1.0f),
                           counter_chance_combat(-1.0f) {}
    };

    // a variance_variable<f32> and two floats
    struct hit_react_scale {
        variance_variable<f32> scale;
        f32                    unk_08;
        f32                    unk_0c;
    };

    struct hit_react_record {
        i32             hit_react_level;
        f32             unk_04;
        hit_react_scale scales[3];
        u8              unk_38;

        hit_react_record(i32 hit_react_level, f32 unk_04);

        // inlined @ sub_848380
        // inlined @ sub_848A10
        // inlined @ sub_848E20
        // inlined @ sub_849440
        // inlined @ sub_8498D0
        hit_react_record(i32 hit_react_level, f32 unk_04, f32 scale_0, f32 scale_1, f32 scale_2) {
            this->hit_react_level = hit_react_level;
            this->unk_04          = unk_04;

            scales[0] = { { scale_0, 0.0f }, 0.0f, 0.0f };
            scales[1] = { { scale_1, 0.0f }, 0.0f, 0.0f };
            scales[2] = { { scale_2, 0.0f }, 0.0f, 0.0f };

            unk_38 = 0;
        }
    };

    class damage_interface : public actor_interface {

    public:
        damage_info           dmg_info;
        u8                    reserved_080[0x04];      // the last damage record
        void*                 life_bar;
        u8                    reserved_088[0x14];      // the life bar offset and scale
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
        u8                    reserved_108[0x0C];
        damage_webcast*       webcast;
        f32                   damage_scales[5];        // by damage type
        u8                    reserved_12c[0x08];
        f32                   force_attack_multiplier;
        f32                   force_react_multiplier;

        // inlined
        i32 get_max_hit_points() const {
            return hit_points.get_max_val();
        }

        void set_max_hit_points(i32 max);
        void set_hit_points(i32 value, bool use_signals = false, entity* attacker = nullptr, entity* attacker_weapon = nullptr);

        void set_armor_points(i32 value);
        void set_max_armor_points(i32 max);

        bool is_stunned() const;

        bool is_life_bar_hidden() const;
        void set_life_bar_hidden(bool hidden);

        void cancel_web_subdue();

        void allocate_webcast();
        void set_webcast_level(f32 level);
        void set_webcast_style(i32 style);
        void set_webcast_sync_enabled(bool enabled);
        void set_webcast_counter_chance(f32 chance, bool is_override);
        void set_webcast_counter_scale(f32 scale);

        void set_damage_scale(i32 type, f32 scale);
        void set_damage_scale(f32 scale);
        f32  get_damage_scale(i32 type) const;

        entity* get_damage_attacker() const;

        i32 apply_damage(      entity*           attacker,
                               entity*           attacker_weapon,
                               i32               damage,
                               f32               web_damage,
                               i32               experience_kind,
                               i32               type,
                         const hit_react_record* react,
                         const hit_react_record* second_react,
                         const vector3d*         loc,
                         const vector3d*         dir,
                               i32               dmg_flags,
                         const string_hash*      wounded_category,
                         const vector3d*         override_dir,
                               e_phys_bones      bone_idx,
                         const combo_move_data*  combo_move,
                               u8*               applied,
                               bool              force_react);

        i32 apply_damage_with_combo_web_damage(      entity*           attacker,
                                                     entity*           attacker_weapon,
                                                     i32               damage,
                                                     i32               experience_kind,
                                                     i32               type,
                                               const hit_react_record* react,
                                               const hit_react_record* second_react,
                                               const vector3d*         loc,
                                               const vector3d*         dir,
                                                     i32               dmg_flags,
                                               const string_hash*      wounded_category,
                                               const vector3d*         override_dir,
                                                     e_phys_bones      bone_idx,
                                               const combo_move_data*  combo_move,
                                                     u8*               applied,
                                                     bool              force_react);
    };

    ASSERT_SIZEOF  (damage_info, 0x74);

    ASSERT_OFFSETOF(damage_stun, recover_remaining, 0x0C);

    ASSERT_OFFSETOF(damage_web_subdue, web_hit_points,    0x08);
    ASSERT_OFFSETOF(damage_web_subdue, recover_duration,  0x14);
    ASSERT_OFFSETOF(damage_web_subdue, recover_remaining, 0x18);

    ASSERT_SIZEOF  (damage_webcast,                        0x40);
    ASSERT_OFFSETOF(damage_webcast, style,                 0x14);
    ASSERT_OFFSETOF(damage_webcast, counter_chance_combat, 0x3C);

    ASSERT_SIZEOF  (hit_react_record,         0x3C);
    ASSERT_OFFSETOF(hit_react_record, unk_38, 0x38);

    ASSERT_OFFSETOF(damage_interface, dmg_info,                0x00C);
    ASSERT_OFFSETOF(damage_interface, life_bar,                0x084);
    ASSERT_OFFSETOF(damage_interface, flags,                   0x09C);
    ASSERT_OFFSETOF(damage_interface, damage_dealt,            0x0A8);
    ASSERT_OFFSETOF(damage_interface, hit_points,              0x0B4);
    ASSERT_OFFSETOF(damage_interface, armor_points,            0x0CC);
    ASSERT_OFFSETOF(damage_interface, damage_lock_threshold,   0x0DC);
    ASSERT_OFFSETOF(damage_interface, stun,                    0x100);
    ASSERT_OFFSETOF(damage_interface, web_subdue,              0x104);
    ASSERT_OFFSETOF(damage_interface, webcast,                 0x114);
    ASSERT_OFFSETOF(damage_interface, damage_scales,           0x118);
    ASSERT_OFFSETOF(damage_interface, force_attack_multiplier, 0x134);
} // treyarch
