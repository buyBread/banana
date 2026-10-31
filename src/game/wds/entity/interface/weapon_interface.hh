#pragma once

#include "game/wds/entity/interface/generic_interface.hh"
#include "game/arch_base_vhandle.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // what every weapon entity carries; the gun, melee and thrown interfaces reach it through weapon_interface_extender
    class weapon_interface : public entity_interface {

    public:
        u8                reserved_00c[0xA8];
        arch_base_vhandle holder; // the entity the weapon's own entity hangs off
        u32               flags;
        u8                reserved_0bc[0x08];
    };

    ASSERT_SIZEOF  (weapon_interface,         0xC4);
    ASSERT_OFFSETOF(weapon_interface, holder, 0xB4);
    ASSERT_OFFSETOF(weapon_interface, flags,  0xB8);
} // treyarch
