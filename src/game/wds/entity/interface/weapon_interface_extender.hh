#pragma once

#include "game/wds/entity/interface/generic_interface.hh"
#include "treyarch/mash/types.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // what the gun, melee and thrown weapon interfaces share
    class weapon_interface_extender : public entity_interface {

    public:
        explicit weapon_interface_extender(mash::generating_vtable_version so_do_nothing) : entity_interface(so_do_nothing) {}

        virtual i32 unk_019(void* unk) = 0;
        virtual i32 unk_020() = 0;
        virtual i32 unk_021() = 0;
    };
} // treyarch
