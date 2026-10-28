#pragma once

#include "game/wds/ai/param_block.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class ai_vehicle_inode {

    public:
        u8          reserved_000[0xA0];
        param_block my_param_block; // what a script reads and writes as the vehicle's ai params
    };

    class ai_vehicle {

    public:
        ai_vehicle_inode* my_inode;
        u8                reserved_004[0x10];
    };

    ASSERT_OFFSETOF(ai_vehicle_inode, my_param_block, 0xA0);

    ASSERT_SIZEOF(ai_vehicle, 0x14);
} // treyarch
