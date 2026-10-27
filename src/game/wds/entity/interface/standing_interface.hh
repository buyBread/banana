#pragma once

#include "game/wds/entity/interface/generic_interface.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class standing_interface : public generic_interface {

    public:
        u8  reserved_008[0x34];
        f32 unk_03c; // the als's elevation
        u8  reserved_040[0x04];
        u32 unk_044; // the cut scene player clears bit 0 while it has the hero
    };

    ASSERT_OFFSETOF(standing_interface, unk_03c, 0x3C);
    ASSERT_OFFSETOF(standing_interface, unk_044, 0x44);
} // treyarch
