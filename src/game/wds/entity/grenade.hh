#pragma once

#include "game/wds/entity/actor.hh"
#include "game/script/script_access.hh"
#include "game/arch_base_vhandle.hh"
#include "game/math/algebra/vector3d.hh"
#include "vc8/vector.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class grenade;
    class thrown_weapon_interface;

    namespace references {
        // every grenade out of its cache, under the grenade and thrown_weapon_interface lock
        inline util::memory_reference<vc8::vector<grenade*>> unk_01003320 { 0x01003320 };
    } // references

    class grenade : public actor {

    public:
        u8                       reserved_0e8[0x08];
        bool                     unk_0f0;             // its cache hands it out again only while it's set
        u8                       reserved_0f1;
        bool                     unk_0f2;
        bool                     unk_0f3;
        bool                     unk_0f4;
        bool                     unk_0f5;             // cleared when the launch takes a given position
        u8                       reserved_0f6;
        bool                     unk_0f7;
        u8                       reserved_0f8[0x10];
        thrown_weapon_interface* thrown_weapon_owner;
        f32                      fuse_timer;
        f32                      arm_timer;
        u8                       reserved_114[0x08];
        f32                      unk_11c;
        u8                       reserved_120[0x28];
        u32                      unk_148;             // its RADIO_DETONATE callback on the thrower
        u8                       reserved_14c[0x04];
        script_object_instance   script_obj_instance;
        arch_base_vhandle        visual_entity;       // the "grenade_visual" entity
        u8                       reserved_18c[0x08];
        vector3d                 unk_194;             // where it was launched from
        vector3d                 spin_axis;
        f32                      spin_speed;
        u8                       reserved_1b0[0x08];
        u32                      unk_1b8;             // the sticky grenade's looping effect
        u32                      unk_1bc;             // the looping effect

        // sub_61D6A0
        // inlined @ sub_66CE80
        entity* get_visual_entity() const {
            return (entity*)visual_entity.resolve();
        }

        // slot 160
        void prepare_for_launch(thrown_weapon_interface* own);

        void change_list_status(bool unk);

        // slot 166
        void redirect(arch_base_vhandle target, entity* redirector) {
            ((void (__thiscall*)(grenade*, arch_base_vhandle, entity*))(*(void***)this)[166])(this, target, redirector);
        }

        // slot 167
        void redirect(const vector3d &dir, entity* redirector) {
            ((void (__thiscall*)(grenade*, const vector3d*, entity*))(*(void***)this)[167])(this, &dir, redirector);
        }

        // slot 168
        void disable_guidance() {
            ((void (__thiscall*)(grenade*))(*(void***)this)[168])(this);
        }
    };

    ASSERT_SIZEOF  (grenade,                      0x1C0);
    ASSERT_OFFSETOF(grenade, unk_0f0,             0x0F0);
    ASSERT_OFFSETOF(grenade, unk_0f7,             0x0F7);
    ASSERT_OFFSETOF(grenade, thrown_weapon_owner, 0x108);
    ASSERT_OFFSETOF(grenade, unk_11c,             0x11C);
    ASSERT_OFFSETOF(grenade, unk_148,             0x148);
    ASSERT_OFFSETOF(grenade, script_obj_instance, 0x150);
    ASSERT_OFFSETOF(grenade, visual_entity,       0x188);
    ASSERT_OFFSETOF(grenade, unk_194,             0x194);
    ASSERT_OFFSETOF(grenade, spin_axis,           0x1A0);
    ASSERT_OFFSETOF(grenade, spin_speed,          0x1AC);
    ASSERT_OFFSETOF(grenade, unk_1b8,             0x1B8);
} // treyarch
