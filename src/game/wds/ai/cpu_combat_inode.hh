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
    struct cpu_combat_movement;

    class cpu_combat_inode : public info_node {

    public:
        u8                                    reserved_030[0x30];
        cpu_combat_pie_slice                  pie_slice;
        chuck::vm::script_dynamic_array<f32>* script_nums;              // the combat script's num[]
        cpu_combat_movement*                  movement;                 // made the first time update_movement runs
        u8                                    reserved_094[0x10];
        arch_base_vhandle                     script_instance_handle;
        u8                                    reserved_0a8[0x30];
        i32                                   update_movement_function; // -1 when the script has none
        u8                                    reserved_0dc[0x1C];
        engine_recursive_lock                 script_lock;              // held around the script calls
    };

    // what the combat script's update_movement steers its actor through
    struct cpu_combat_movement {
        ai_core*          core;
        cpu_combat_inode* combat;
    };

    ASSERT_SIZEOF  (cpu_combat_inode,                           0x108);
    ASSERT_OFFSETOF(cpu_combat_inode, pie_slice,                0x060);
    ASSERT_OFFSETOF(cpu_combat_inode, script_nums,              0x08C);
    ASSERT_OFFSETOF(cpu_combat_inode, movement,                 0x090);
    ASSERT_OFFSETOF(cpu_combat_inode, script_instance_handle,   0x0A4);
    ASSERT_OFFSETOF(cpu_combat_inode, update_movement_function, 0x0D8);
    ASSERT_OFFSETOF(cpu_combat_inode, script_lock,              0x0F8);

    ASSERT_SIZEOF  (cpu_combat_movement,         0x08);
    ASSERT_OFFSETOF(cpu_combat_movement, combat, 0x04);
} // treyarch
