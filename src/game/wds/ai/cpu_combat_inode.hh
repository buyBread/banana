#pragma once

#include "treyarch/chuck/vm/vm_dynamic_array_manager.hh"
#include "game/wds/ai/cpu_combat_pie.hh"
#include "game/wds/ai/info_node.hh"
#include "game/arch_base_vhandle.hh"
#include "game/mutex.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class  ai_core;
    class  combo_move_data;
    class  entity;
    class  entity_base;
    class  weapon_interface_extender;
    struct cpu_combat_movement;
    struct cpu_combat_queued_move;

    // combat_state's move_transition sets the move bits, hit_target and the dodge and block slots the result bits
    enum e_cpu_combat_inode_flags : u32 {
        cpu_combat_inode_flag_in_dodge_move          = 0x00000008,
        cpu_combat_inode_flag_in_counter_attack_move = 0x00000010,
        cpu_combat_inode_flag_attack_missed          = 0x00000020,
        cpu_combat_inode_flag_attack_dodged          = 0x00000040,
        cpu_combat_inode_flag_attack_connected       = 0x00000080,
        cpu_combat_inode_flag_attack_blocked         = 0x00000100,
        cpu_combat_inode_flag_in_block_move          = 0x00000200
    };

    class cpu_combat_inode : public info_node {

    public:
        u32                                   flags;                    // e_cpu_combat_inode_flags
        u8                                    reserved_034[0x28];
        arch_base_vhandle                     combat_move_target;       // the move's first target, taken by move_transition
        cpu_combat_pie_slice                  pie_slice;
        chuck::vm::script_dynamic_array<f32>* script_nums;              // the combat script's num[]
        cpu_combat_movement*                  movement;                 // made the first time update_movement runs
        u8                                    reserved_094[0x10];
        arch_base_vhandle                     script_instance_handle;
        u8                                    reserved_0a8[0x30];
        i32                                   update_movement_function; // -1 when the script has none
        u8                                    reserved_0dc[0x1C];
        engine_recursive_lock                 script_lock;              // held around the script calls

        void set_script_chain_size(i32 chain_size);
        void set_script_strength_test_value(f32 value);
        void set_script_strength_test_status(f32 status);
        void set_attack_blocked(bool blocked);

        // slot 24; an attacker's hit landed on the inode's actor, the script hears of it
        void unk_024(      entity_base*               attacker,
                     const combo_move_data*           combo_move,
                           weapon_interface_extender* weapon,
                           entity*                    weapon_entity,
                           i32                        usage_type,
                           i32                        damage) {

            ((void (__thiscall*)(cpu_combat_inode*,
                                 entity_base*,
                                 const combo_move_data*,
                                 weapon_interface_extender*,
                                 entity*,
                                 i32,
                                 i32))(*(void***)this)[24])(this,
                                                            attacker,
                                                            combo_move,
                                                            weapon,
                                                            weapon_entity,
                                                            usage_type,
                                                            damage);
        }

        // slot 25; what add_incoming_attack calls on the target's inode
        bool unk_025(entity_base* attacker, const combo_move_data* combo_move, i32 chain_position) {
            return ((bool (__thiscall*)(cpu_combat_inode*, entity_base*, const combo_move_data*, i32))(*(void***)this)[25])(this, attacker, combo_move, chain_position);
        }

        // slot 28; under the script lock, the combat script hears of the move with the targets and buddies
        bool unk_028(cpu_combat_queued_move*                          move,
                     chuck::vm::script_dynamic_array<arch_base_vhandle>* targets,
                     chuck::vm::script_dynamic_array<arch_base_vhandle>* buddies) {

            return ((bool (__thiscall*)(cpu_combat_inode*,
                                        cpu_combat_queued_move*,
                                        chuck::vm::script_dynamic_array<arch_base_vhandle>*,
                                        chuck::vm::script_dynamic_array<arch_base_vhandle>*))(*(void***)this)[28])(this, move, targets, buddies);
        }

        // slot 31; a flag the script raises, read under the script lock
        bool unk_031(f32 delta_t) {
            return ((bool (__thiscall*)(cpu_combat_inode*, f32))(*(void***)this)[31])(this, delta_t);
        }

        // slot 37; another one
        bool unk_037(f32 delta_t) {
            return ((bool (__thiscall*)(cpu_combat_inode*, f32))(*(void***)this)[37])(this, delta_t);
        }

        static string_hash my_type_key;
    };

    // the target's cpu combat inode hears of the attack
    void add_incoming_attack(entity_base* target, entity_base* attacker, const combo_move_data* combo_move, i32 chain_position);

    // what the combat script's update_movement steers its actor through
    struct cpu_combat_movement {
        ai_core*          core;
        cpu_combat_inode* combat;
    };

    ASSERT_SIZEOF  (cpu_combat_inode,                           0x108);
    ASSERT_OFFSETOF(cpu_combat_inode, flags,                    0x030);
    ASSERT_OFFSETOF(cpu_combat_inode, combat_move_target,       0x05C);
    ASSERT_OFFSETOF(cpu_combat_inode, pie_slice,                0x060);
    ASSERT_OFFSETOF(cpu_combat_inode, script_nums,              0x08C);
    ASSERT_OFFSETOF(cpu_combat_inode, movement,                 0x090);
    ASSERT_OFFSETOF(cpu_combat_inode, script_instance_handle,   0x0A4);
    ASSERT_OFFSETOF(cpu_combat_inode, update_movement_function, 0x0D8);
    ASSERT_OFFSETOF(cpu_combat_inode, script_lock,              0x0F8);

    ASSERT_SIZEOF  (cpu_combat_movement,         0x08);
    ASSERT_OFFSETOF(cpu_combat_movement, combat, 0x04);
} // treyarch
