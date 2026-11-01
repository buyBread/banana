#pragma once

#include "game/wds/entity/interface/weapon_interface_extender.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class gun_weapon_interface : public weapon_interface_extender {

    public:
        u8  reserved_010[0x10];
        i32 unk_020; // the hero's check_for_block takes it as the block value
    };

    ASSERT_OFFSETOF(gun_weapon_interface, unk_020, 0x20);
} // treyarch
