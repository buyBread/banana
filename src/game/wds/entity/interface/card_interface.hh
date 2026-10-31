#pragma once

#include "game/wds/entity/interface/generic_interface.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class card_interface : public entity_interface {

    public:
        u32 unk_00c;            // bit 0: the ped work set the ped up past the anim move radius
        u8  reserved_010[0x40];
    };

    ASSERT_SIZEOF  (card_interface,          0x50);
    ASSERT_OFFSETOF(card_interface, unk_00c, 0x0C);
} // treyarch
