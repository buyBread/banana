#pragma once

#include "game/wds/ai/fight_group.hh"
#include "game/singleton.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
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
