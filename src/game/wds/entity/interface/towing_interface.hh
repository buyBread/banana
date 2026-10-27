#pragma once

#include "game/wds/entity/interface/generic_interface.hh"
#include "game/arch_base_vhandle.hh"
#include "game/math/algebra/vector3d.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // paired up by peds_setup_towing: the tower's side holds the towee and the tow point, the towee's the tower and the hitch point
    class towing_interface : public entity_interface {

    public:
        u8                reserved_00c[0x04];
        arch_base_vhandle tower;
        arch_base_vhandle towee;
        vector3d          tow_point;   // in the tower's space
        vector3d          hitch_point; // in the towee's space
    };

    ASSERT_SIZEOF  (towing_interface,              0x30);
    ASSERT_OFFSETOF(towing_interface, tower,       0x10);
    ASSERT_OFFSETOF(towing_interface, towee,       0x14);
    ASSERT_OFFSETOF(towing_interface, tow_point,   0x18);
    ASSERT_OFFSETOF(towing_interface, hitch_point, 0x24);
} // treyarch
