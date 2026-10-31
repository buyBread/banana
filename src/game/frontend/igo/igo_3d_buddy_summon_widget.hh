#pragma once

#include "game/frontend/igo/igo_3d_widget.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class simple_buddy_summon_token;

    class igo_3d_buddy_summon_widget : public igo_3d_widget {

    public:
        u8                         reserved_150[0x10];
        simple_buddy_summon_token* tokens[4];
        u8                         reserved_170[0x10];
        bool                       summon_power_mode;
        u8                         reserved_181[0x1F];
    };

    ASSERT_SIZEOF  (igo_3d_buddy_summon_widget,                    0x1A0);
    ASSERT_OFFSETOF(igo_3d_buddy_summon_widget, tokens,            0x160);
    ASSERT_OFFSETOF(igo_3d_buddy_summon_widget, summon_power_mode, 0x180);
} // treyarch
