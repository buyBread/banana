#pragma once

#include "game/wds/ai/info_node.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class ai_core;

    class cpu_combat_inode : public info_node {

    public:
        u8 reserved_030[0xD8];
    };

    // what the combat script's update_movement steers its actor through
    struct cpu_combat_movement {
        ai_core*          core;
        cpu_combat_inode* combat;
    };

    ASSERT_SIZEOF(cpu_combat_inode, 0x108);

    ASSERT_SIZEOF  (cpu_combat_movement,         0x08);
    ASSERT_OFFSETOF(cpu_combat_movement, combat, 0x04);
} // treyarch
