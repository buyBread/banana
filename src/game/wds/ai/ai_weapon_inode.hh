#pragma once

#include "game/wds/ai/info_node.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/vector.hh"
#include "game/arch_base_vhandle.hh"
#include "game/math/algebra/vector3d.hh"
#include "game/mutex.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class entity;

    // SM3's eAIWeaponState
    enum e_ai_weapon_state : i32 {
        ai_weapon_holstered,
        ai_weapon_drawn,
        ai_weapon_drawing,
        ai_weapon_holstering,
        ai_weapon_reloading,
        ai_weapon_cocking,
        ai_weapon_invalid,
        ai_weapon_disconnect,
        ai_num_weapon_states
    };

    enum e_weapon_fire_kind : u32 {
        weapon_fire_kind_gun     = 0,
        weapon_fire_kind_grenade = 2,
        weapon_fire_kind_rocket  = 3
    };

    bool fire_weapon_entity(entity*            shooter,
                            arch_base_vhandle  weapon,
                            e_weapon_fire_kind kind,
                            bool               if_empty,
                            arch_base_vhandle  target,
                            vector3d           position);

    class ai_weapon {

    public:
        arch_base_vhandle weapon_entity;
        u8                reserved_004[0x08];
        e_ai_weapon_state state;
        u8                reserved_010[0x10];
        string_hash       name;
        u8                reserved_024[0x58];
        bool              enabled;

        // sub_4E7A20
        // inlined @ sub_4E7C20
        // inlined @ sub_4E7D50
        // inlined @ sub_4ECBE0
        bool has_weapon_entity() const {
            return weapon_entity.value && weapon_entity.resolve();
        }

        e_ai_weapon_state get_state() const;

        static bool fire(entity*            shooter,
                         ai_weapon*         weapon,
                         e_weapon_fire_kind kind,
                         bool               if_empty,
                         arch_base_vhandle  target,
                         vector3d           position);
    };

    class ai_action_processor_inode : public info_node {

    public:
        u8 reserved_030[0x18];
    };

    // one hand's weapons
    struct ai_weapon_set {
        mash::vector<ai_weapon> lists[4];
    };

    class ai_weapon_inode : public ai_action_processor_inode {

    public:
        engine_recursive_lock weapon_lock;
        u8                    reserved_058[0x1C];
        ai_weapon*            drawn_weapon[2];   // per hand
        u8                    reserved_07c[0x5C];
        bool                  weapon_sys_busy;
        u8                    pad_0d9[0x03];
        ai_weapon_set**       weapon_sets;       // one per hand
        u8                    reserved_0e0[0x08];

        e_ai_weapon_state get_hand_state  (i32 hand); // holstered without a live weapon in the hand
        ai_weapon*        get_drawn_weapon(i32 hand); // null without a live weapon in the hand

        bool       is_weapon_available(const string_hash &name);
        ai_weapon* find_weapon        (const string_hash &name);
        void       enable_weapon      (const string_hash &name, bool enable);
        void       enable_all_weapons (bool enable);
        void       draw_weapon        (const string_hash &name, const string_hash &other_name, bool immediate);
        void       holster_weapon     (const string_hash &name, const string_hash &other_name, bool immediate);
    };

    ASSERT_OFFSETOF(ai_weapon, weapon_entity, 0x00);
    ASSERT_OFFSETOF(ai_weapon, state,         0x0C);
    ASSERT_OFFSETOF(ai_weapon, name,          0x20);
    ASSERT_OFFSETOF(ai_weapon, enabled,       0x7C);

    ASSERT_SIZEOF(ai_action_processor_inode, 0x48);
    ASSERT_SIZEOF(ai_weapon_set,             0x50);

    ASSERT_SIZEOF  (ai_weapon_inode,                  0xE8);
    ASSERT_OFFSETOF(ai_weapon_inode, weapon_lock,     0x48);
    ASSERT_OFFSETOF(ai_weapon_inode, drawn_weapon,    0x74);
    ASSERT_OFFSETOF(ai_weapon_inode, weapon_sys_busy, 0xD8);
    ASSERT_OFFSETOF(ai_weapon_inode, weapon_sets,     0xDC);
} // treyarch
