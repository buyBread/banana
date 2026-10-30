#pragma once

#include "game/singleton.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // created through sub_93D700 by region_spawn_manager's constructor (sub_951A90)
    class region_pack_manager : public singleton<region_pack_manager, 0x010FB18C> {

    public:
        u8   reserved_004[0x84];
        u32  unk_088;
        u8   reserved_08c[0x08];
        u32  unk_094;
        u8   reserved_098[0x1A];
        u8   unk_0b2;
        bool quest_giver_locked_in_mission;
        u8   reserved_0b4[0x04];

        void despawn_and_lock_quest_giver_in_mission(bool lock);
    };

    ASSERT_SIZEOF  (region_pack_manager,                                0xB8);
    ASSERT_OFFSETOF(region_pack_manager, unk_088,                       0x88);
    ASSERT_OFFSETOF(region_pack_manager, unk_094,                       0x94);
    ASSERT_OFFSETOF(region_pack_manager, unk_0b2,                       0xB2);
    ASSERT_OFFSETOF(region_pack_manager, quest_giver_locked_in_mission, 0xB3);
} // treyarch
