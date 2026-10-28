#pragma once

#include "game/wds/ai/param_block.hh"
#include "game/arch_base_vhandle.hh"
#include "game/math/algebra/vector3d.hh"
#include "game/math/references.hh"
#include "vc8/list.hh"
#include "vc8/vector.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class vehicle;

    // SM3's AI_VEHICLE_PURPOSE
    enum e_ai_vehicle_purpose : i32 {
        ai_vehicle_purpose_none,
        ai_vehicle_purpose_stop,
        ai_vehicle_purpose_wander,
        ai_vehicle_purpose_goto,
        ai_vehicle_purpose_chase,
        ai_vehicle_purpose_script_controlled
    };

    class ai_vehicle_inode {

    public:
        vehicle*             my_vehicle;
        u8                   reserved_004[0x14];
        u32                  my_flags;
        u8                   reserved_01c[0x50];
        f32                  my_max_speed;
        u8                   reserved_070[0x10];
        bool                 my_purpose_done;
        u8                   pad_081[0x03];
        e_ai_vehicle_purpose my_purpose;
        arch_base_vhandle    my_purpose_entity;
        vector3d             my_purpose_pos;
        f32                  my_purpose_radius;
        u8                   reserved_09c[0x04];
        param_block          my_param_block;    // what a script reads and writes as the vehicle's ai params
        bool                 doors_open;        // while set, the purpose reads as stop
        u8                   pad_0c1[0x03];
        u8                   reserved_0c4[0x0C];

        // inlined @ sub_8512E0
        void set_max_speed(f32 speed) { my_max_speed = speed; }

        // inlined @ sub_851320
        f32 get_max_speed() const { return my_max_speed; }

        // inlined @ sub_8518F0
        // inlined @ sub_87C190
        // inlined @ sub_87C220
        // inlined @ sub_87C2D0
        // inlined @ sub_87C360
        // inlined @ sub_87C410
        // inlined @ sub_87C4A0
        // inlined @ sub_87C550
        // inlined @ sub_87C5F0
        // inlined @ sub_87C6A0
        // inlined @ sub_87C740
        e_ai_vehicle_purpose get_purpose() const { return doors_open ? ai_vehicle_purpose_stop : my_purpose; }

        // inlined @ sub_851990
        arch_base_vhandle get_purpose_entity() const { return doors_open ? arch_base_vhandle() : my_purpose_entity; }

        // inlined @ sub_851A30
        const vector3d &get_purpose_pos() const { return doors_open ? references::unk_010f7140.get() : my_purpose_pos; }

        // inlined @ sub_851AC0
        f32 get_purpose_radius() const { return doors_open ? 0.0f : my_purpose_radius; }

        // inlined @ sub_851B60
        bool is_purpose_done() const { return doors_open ? false : my_purpose_done; }
    };

    class ai_vehicle_state {

    public:
        enum e_type : i32 {
            type_none,
            type_stop,
            type_follow_lane,
            type_player,
            type_dodge,
            type_chase,
            type_script
        };

        void** vtable;
        u8     reserved_004[0x0C];
        f32    my_steer_angle;
        f32    my_gas_pedal;
        f32    my_brake_pedal;
        u8     reserved_01c[0x04];

        // slot 1
        e_type get_type() {
            return ((e_type (__thiscall*)(ai_vehicle_state*))vtable[1])(this);
        }

        void set_steer_angle(f32 angle); // within -1..1
        void set_gas_pedal  (f32 pedal); // within 0..1, under a tenth is none
        void set_brake_pedal(f32 pedal); // within 0..1, under a tenth is none
    };

    // what a script drives through the set_vehicle_* natives
    class ai_vehicle_state_script : public ai_vehicle_state {

    public:
        bool power_brake;
        bool reverse;
        u8   pad_022[0x02];
    };

    class ai_vehicle_machine {

    public:
        u8                           reserved_000[0x04];
        vc8::list<ai_vehicle_state*> states;           // the last one runs
        u8                           reserved_010[0x0C];

        // inlined @ sub_87C190
        // inlined @ sub_87C220
        // inlined @ sub_87C2D0
        // inlined @ sub_87C360
        // inlined @ sub_87C410
        // inlined @ sub_87C4A0
        // inlined @ sub_87C550
        // inlined @ sub_87C5F0
        // inlined @ sub_87C6A0
        // inlined @ sub_87C740
        ai_vehicle_state* get_state() const { return states.size() ? states.back() : nullptr; }
    };

    class ai_vehicle {

    public:
        ai_vehicle_inode*   m_inode;
        ai_vehicle_machine* m_machine;
        u8                  reserved_008[0x05];
        bool                can_be_stolen;
        bool                sleeps_on_stop;   // a stopped vehicle's physics go to sleep
        u8                  pad_00f;
        u8                  reserved_010[0x04];

        // inlined @ sub_851400
        u32 get_flags() const { return m_inode->my_flags; }

        void set_flags(u32 flags);

        void set_purpose(e_ai_vehicle_purpose purpose);
        void set_purpose(e_ai_vehicle_purpose purpose, arch_base_vhandle pe);
        bool set_purpose(e_ai_vehicle_purpose purpose, vector3d pos, f32 radius); // false when it found no way there
        void set_purpose(e_ai_vehicle_purpose          purpose,
                         const vc8::vector<vector3d>  &positions,
                         const vc8::vector<f32>       &radii);

        void set_sleeps_on_stop(bool sleeps);
    };

    ASSERT_OFFSETOF(ai_vehicle_inode, my_flags,          0x18);
    ASSERT_OFFSETOF(ai_vehicle_inode, my_max_speed,      0x6C);
    ASSERT_OFFSETOF(ai_vehicle_inode, my_purpose_done,   0x80);
    ASSERT_OFFSETOF(ai_vehicle_inode, my_purpose,        0x84);
    ASSERT_OFFSETOF(ai_vehicle_inode, my_purpose_entity, 0x88);
    ASSERT_OFFSETOF(ai_vehicle_inode, my_purpose_pos,    0x8C);
    ASSERT_OFFSETOF(ai_vehicle_inode, my_purpose_radius, 0x98);
    ASSERT_OFFSETOF(ai_vehicle_inode, my_param_block,    0xA0);
    ASSERT_OFFSETOF(ai_vehicle_inode, doors_open,        0xC0);
    ASSERT_SIZEOF  (ai_vehicle_inode,                    0xD0);

    ASSERT_OFFSETOF(ai_vehicle_state, my_steer_angle, 0x10);
    ASSERT_OFFSETOF(ai_vehicle_state, my_gas_pedal,   0x14);
    ASSERT_OFFSETOF(ai_vehicle_state, my_brake_pedal, 0x18);
    ASSERT_SIZEOF  (ai_vehicle_state,                 0x20);

    ASSERT_OFFSETOF(ai_vehicle_state_script, power_brake, 0x20);
    ASSERT_OFFSETOF(ai_vehicle_state_script, reverse,     0x21);
    ASSERT_SIZEOF  (ai_vehicle_state_script,              0x24);

    ASSERT_OFFSETOF(ai_vehicle_machine, states, 0x04);
    ASSERT_SIZEOF  (ai_vehicle_machine,         0x1C);

    ASSERT_OFFSETOF(ai_vehicle, m_machine,      0x04);
    ASSERT_OFFSETOF(ai_vehicle, can_be_stolen,  0x0D);
    ASSERT_OFFSETOF(ai_vehicle, sleeps_on_stop, 0x0E);
    ASSERT_SIZEOF  (ai_vehicle,                 0x14);
} // treyarch
