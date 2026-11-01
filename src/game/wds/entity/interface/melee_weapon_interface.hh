#pragma once

#include "game/wds/entity/interface/weapon_interface_extender.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class melee_weapon_interface : public weapon_interface_extender {

    public:
        f32 unk_010; // the hero's check_for_block blocks it at no strength
    };

    ASSERT_OFFSETOF(melee_weapon_interface, unk_010, 0x10);
} // treyarch
