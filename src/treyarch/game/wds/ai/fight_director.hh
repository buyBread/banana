#pragma once

#include "treyarch/game/arch_base.hh"
#include "treyarch/shared/singleton.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    namespace slim {
        class slim_fight_director_controller;
    } // slim

    enum e_fight_group_flags : u32 {
        fight_group_flag_active = 0x00000001,
        fight_group_flag_paused = 0x00000002
    };

    class fight_group : public arch_base {

    public:
        slim::slim_fight_director_controller* controller;
        u32                                   flags;
        u8                                    reserved_010[0x24];
    };

    ASSERT_SIZEOF  (fight_group,             0x34);
    ASSERT_OFFSETOF(fight_group, controller, 0x08);
    ASSERT_OFFSETOF(fight_group, flags,      0x0C);

    class fight_director : public singleton<fight_director, 0x00FC5790> {

    public:
        enum { fight_group_count = 50 };

        fight_group fight_groups[fight_group_count];
        u32         unk_a2c;
        u32         unk_a30;
        u32         unk_a34;
    };

    ASSERT_SIZEOF  (fight_director,               0xA38);
    ASSERT_OFFSETOF(fight_director, fight_groups, 0x004);
    ASSERT_OFFSETOF(fight_director, unk_a2c,      0xA2C);
} // treyarch
