#pragma once

#include "game/wds/entity/interface/generic_interface.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // an entity's effects; ise_variant_interface and red_and_black_ise_interface share its slots
    class ise_interface : public entity_interface {

    public:
        u8 reserved_00c[0x3C];

        // slot 21
        void stop_all_effects() {
            ((void (__thiscall*)(ise_interface*))((void**)vtable)[21])(this);
        }
    };

    ASSERT_SIZEOF(ise_interface, 0x48);
} // treyarch
