#pragma once

#include "game/wds/entity/interface/weapon_interface_extender.hh"
#include "game/arch_base_vhandle.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/types.hh"
#include "game/math/algebra/vector3d.hh"
#include "game/mutex.hh"
#include "vc8/vector.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class grenade;
    class thrown_weapon_interface;

    namespace references {
        inline util::memory_reference<string_hash> radio_detonate { 0x0102CC68 };

        // "launch(vector3d,entity)", the grenade script's function spawn_grenade calls
        inline util::memory_reference<string_hash> unk_010039b8 { 0x010039B8 };
    } // references

    // a pool of grenades for thrown weapon interfaces
    class grenade_cache {

    public:
        engine_recursive_lock           lock;
        string_hash                     unk_010;
        vc8::vector<arch_base_vhandle>  grenades;
        i32                             ref_count;
        u32                             unk_028;
        u32                             unk_02c;

        grenade* get_grenade(thrown_weapon_interface* thrown); // a free grenade, else a new one; prepared for launch either way
    };

    class thrown_weapon_interface : public weapon_interface_extender {

    public:
        i32                      unk_010;                // the hero's check_for_block takes it as the block value
        u8                       reserved_014[0x1C];
        f32                      fuse_time;
        f32                      fuse_time_var;
        f32                      arm_time;
        f32                      bounce_factor;
        f32                      slide_factor;
        u8                       reserved_044[0xA4];
        u8                       unk_0e8[0x50];          // the throw's cached_special_effect
        u8                       reserved_138[0xBC];
        vector3d                 unk_1f4;                // the throw effect's offset
        vector3d                 spin_axis;
        f32                      spin_speed;
        f32                      spin_speed_var;
        u8                       reserved_214[0x08];
        string_hash              unk_21c;                // the grenade's looping effect
        string_hash              unk_220;                // a sticky grenade's looping effect
        string_hash              unk_224;                // spawned on the thrower at each throw
        u8                       reserved_228[0x0C];
        f32                      unk_234;                // the chance a rocket is guided
        f32                      guided_accuracy;
        f32                      turn_factor;
        f32                      accel_factor;
        f32                      wobble_timer;
        f32                      guidance_delay;
        f32                      accel_delay;
        f32                      wobble_timer_var;
        f32                      guidance_delay_var;
        f32                      accel_delay_var;
        f32                      thrust_timer;
        f32                      sticky_offset;
        u8                       reserved_264[0x04];
        f32                      unk_268;                // the grenade visual's fade distance
        grenade_cache*           grenade_pool;
        u8                       reserved_270[0x50];
        u8                       script_exec[0x0C];      // a mash::string, as is script_obj; the retail destructor tears both down
        u8                       script_obj[0x0C];
        u8                       reserved_2d8;
        bool                     unk_2d9;                // the grenades aren't detectable
        bool                     unk_2da;                // the grenades spin about a random axis
        u8                       reserved_2db;
        vc8::vector<grenade*>*   live_grenades;

        explicit thrown_weapon_interface(mash::generating_vtable_version so_do_nothing) : weapon_interface_extender(so_do_nothing) {}

        ~thrown_weapon_interface() override;

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        e_entity_ifc get_ifc_id() const override;

        void set_owner(entity* owner) override;

        i32 unk_019(void* unk) override;
        i32 unk_020() override;
        i32 unk_021() override;

        virtual i32  unk_022();
        virtual i32  unk_023();
        virtual i32  unk_024();
        virtual i32  unk_025();
        virtual i32  unk_026(i32 unk, i32 unk_2, i32 unk_3);
        virtual i32  unk_027(i32 unk);
        virtual bool spawn_grenade(      arch_base_vhandle  target,
                                   const vector3d          &target_pos,
                                   const vector3d          &dir,
                                         f32                force,
                                         bool               use_pos,
                                   const vector3d          &pos);

        static string_hash my_type_key;
    };

    ASSERT_SIZEOF  (grenade_cache,            0x30);
    ASSERT_OFFSETOF(grenade_cache, unk_010,   0x10);
    ASSERT_OFFSETOF(grenade_cache, grenades,  0x14);
    ASSERT_OFFSETOF(grenade_cache, ref_count, 0x24);

    ASSERT_SIZEOF  (thrown_weapon_interface,                  0x2E0);
    ASSERT_OFFSETOF(thrown_weapon_interface, fuse_time,       0x030);
    ASSERT_OFFSETOF(thrown_weapon_interface, slide_factor,    0x040);
    ASSERT_OFFSETOF(thrown_weapon_interface, unk_0e8,         0x0E8);
    ASSERT_OFFSETOF(thrown_weapon_interface, unk_1f4,         0x1F4);
    ASSERT_OFFSETOF(thrown_weapon_interface, spin_axis,       0x200);
    ASSERT_OFFSETOF(thrown_weapon_interface, unk_21c,         0x21C);
    ASSERT_OFFSETOF(thrown_weapon_interface, unk_234,         0x234);
    ASSERT_OFFSETOF(thrown_weapon_interface, thrust_timer,    0x25C);
    ASSERT_OFFSETOF(thrown_weapon_interface, unk_268,         0x268);
    ASSERT_OFFSETOF(thrown_weapon_interface, grenade_pool,    0x26C);
    ASSERT_OFFSETOF(thrown_weapon_interface, script_exec,     0x2C0);
    ASSERT_OFFSETOF(thrown_weapon_interface, script_obj,      0x2CC);
    ASSERT_OFFSETOF(thrown_weapon_interface, live_grenades,   0x2DC);
} // treyarch
