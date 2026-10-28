#pragma once

#include "game/wds/entity/conglomerate.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/vector_basic.hh"
#include "game/math/algebra/vector3d.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class ai_vehicle;
    class rigid_body;

    class vehicle : public conglomerate {

    public:
        u8                          reserved_198[0x2C];
        bool                        physics_enabled;
        u8                          pad_1c5[0x03];
        u8                          reserved_1c8[0x04];
        rigid_body*                 my_rb;
        u8                          reserved_1d0[0x60];
        f32                         my_mass;
        u8                          reserved_234[0x08];
        f32                         self_damage;
        f32                         unk_240;
        f32                         unk_244;
        f32                         unk_248;
        f32                         other_damage;
        f32                         unk_250;
        f32                         unk_254;
        f32                         unk_258;
        vector3d                    my_dimensions;
        u8                          reserved_268[0x28];
        bool                        throwable;      // answered before the manip obj interface's flag
        bool                        always_upright;
        u8                          pad_292[0x02];
        ai_vehicle*                 my_ai;
        mash::vector_basic<entity*> my_undamaged_members;
        mash::vector_basic<entity*> my_damaged_members;
        u8                          reserved_2b8[0x28];
        i32                         condition;

        void set_model     (string_hash name);
        void set_mass      (f32 mass);
        void enable_physics(bool enable);
        void set_condition (i32 condition);

        void set_self_damage (f32 damage, f32 unk_a, f32 unk_b, f32 unk_c);
        void set_other_damage(f32 damage, f32 unk_a, f32 unk_b, f32 unk_c);

        void damage_member     (i32 member_idx); // only while the vehicle is visible
        void damage_member     (string_hash member_id);
        void damage_all_members();
        void repair_member     (i32 member_idx);
        void repair_member     (string_hash member_id);
        void repair_all_members();
    };

    ASSERT_OFFSETOF(vehicle, physics_enabled,      0x1C4);
    ASSERT_OFFSETOF(vehicle, my_rb,                0x1CC);
    ASSERT_OFFSETOF(vehicle, my_mass,              0x230);
    ASSERT_OFFSETOF(vehicle, self_damage,          0x23C);
    ASSERT_OFFSETOF(vehicle, other_damage,         0x24C);
    ASSERT_OFFSETOF(vehicle, my_dimensions,        0x25C);
    ASSERT_OFFSETOF(vehicle, throwable,            0x290);
    ASSERT_OFFSETOF(vehicle, always_upright,       0x291);
    ASSERT_OFFSETOF(vehicle, my_ai,                0x294);
    ASSERT_OFFSETOF(vehicle, my_undamaged_members, 0x298);
    ASSERT_OFFSETOF(vehicle, my_damaged_members,   0x2A8);
    ASSERT_OFFSETOF(vehicle, condition,            0x2E0);
} // treyarch
