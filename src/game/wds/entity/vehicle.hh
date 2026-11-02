#pragma once

#include "game/event/generic_event_callback.hh"
#include "game/wds/entity/conglomerate.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector_basic.hh"
#include "game/math/algebra/vector3d.hh"
#include "game/math/po.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class ai_vehicle;
    class rb_vehicle_model;
    class rigid_body;
    class rigid_body_constraint_wheel;
    class rigid_body_prop_head;

    // finalize(FROM_MASH) posts "stop_vehicle" to the sound interface of the entity it holds
    struct vehicle_sound {
        u8 reserved_00[0x48];

        // inlined @ sub_681E20
        ~vehicle_sound() {
            finalize(mash::ALLOCATED);
        }

        void finalize(mash::allocation_scope scope);

        // sub_902950
        void unmash(mash::mash_info_struct*, void*, mash::buffer_type) {}
    };

    class vehicle : public conglomerate {

    public:
        u8                                 reserved_198[0x0C];
        mash::vector_basic<entity*>        my_wheels;
        mash::vector_basic<po>             my_initial_wheel_rel_pos;
        bool                               physics_enabled;
        u8                                 pad_1c5[0x03];
        rigid_body_prop_head*              my_prop;
        rigid_body*                        my_rb;
        mash::vector_basic
            <rigid_body_constraint_wheel*> my_rbc_wheels;
        rb_vehicle_model*                  my_rb_model;
        vehicle_sound                      sound;
        u8                                 reserved_22c[0x04];
        f32                                my_mass;
        u8                                 reserved_234[0x08];
        f32                                self_damage;
        f32                                unk_240;
        f32                                unk_244;
        f32                                unk_248;
        f32                                other_damage;
        f32                                unk_250;
        f32                                unk_254;
        f32                                unk_258;
        vector3d                           my_dimensions;
        u8                                 reserved_268[0x28];
        bool                               throwable;      // answered before the manip obj interface's flag
        bool                               always_upright;
        u8                                 pad_292[0x02];
        ai_vehicle*                        my_ai;
        mash::vector_basic<entity*>        my_undamaged_members;
        mash::vector_basic<entity*>        my_damaged_members;
        mash::vector_basic<f32>            my_member_hps;
        generic_event_callback             unk_2c8;
        generic_event_callback             unk_2d0;
        generic_event_callback             unk_2d8;
        i32                                condition;
        u8                                 unk_2e4[0x04];
        u8                                 reserved_2e8[0x2C];

        explicit vehicle(mash::generating_vtable_version so_do_nothing) : conglomerate(so_do_nothing),
                                                                          unk_2c8(so_do_nothing),
                                                                          unk_2d0(so_do_nothing),
                                                                          unk_2d8(so_do_nothing) {}

        ~vehicle() override;

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool is_a_vehicle() const override;
        bool is_a_placeholder_vehicle() const override;
        void frame_advance(f32 t) override;

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

        static string_hash my_type_key;
    };

    ASSERT_SIZEOF(vehicle_sound, 0x48);

    ASSERT_SIZEOF  (vehicle,                           0x314);
    ASSERT_OFFSETOF(vehicle, my_wheels,                0x1A4);
    ASSERT_OFFSETOF(vehicle, my_initial_wheel_rel_pos, 0x1B4);
    ASSERT_OFFSETOF(vehicle, physics_enabled,          0x1C4);
    ASSERT_OFFSETOF(vehicle, my_prop,                  0x1C8);
    ASSERT_OFFSETOF(vehicle, my_rb,                    0x1CC);
    ASSERT_OFFSETOF(vehicle, my_rbc_wheels,            0x1D0);
    ASSERT_OFFSETOF(vehicle, my_rb_model,              0x1E0);
    ASSERT_OFFSETOF(vehicle, sound,                    0x1E4);
    ASSERT_OFFSETOF(vehicle, my_mass,                  0x230);
    ASSERT_OFFSETOF(vehicle, self_damage,              0x23C);
    ASSERT_OFFSETOF(vehicle, other_damage,             0x24C);
    ASSERT_OFFSETOF(vehicle, my_dimensions,            0x25C);
    ASSERT_OFFSETOF(vehicle, throwable,                0x290);
    ASSERT_OFFSETOF(vehicle, always_upright,           0x291);
    ASSERT_OFFSETOF(vehicle, my_ai,                    0x294);
    ASSERT_OFFSETOF(vehicle, my_undamaged_members,     0x298);
    ASSERT_OFFSETOF(vehicle, my_damaged_members,       0x2A8);
    ASSERT_OFFSETOF(vehicle, my_member_hps,            0x2B8);
    ASSERT_OFFSETOF(vehicle, unk_2c8,                  0x2C8);
    ASSERT_OFFSETOF(vehicle, condition,                0x2E0);
    ASSERT_OFFSETOF(vehicle, unk_2e4,                  0x2E4);
} // treyarch
