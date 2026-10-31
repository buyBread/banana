#pragma once

#include "game/wds/entity/interface/generic_interface.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // an entity's effects; ise_variant_interface and red_and_black_ise_interface share its slots
    class ise_interface : public entity_interface {

    public:
        u8 reserved_00c[0x3C];

        virtual void* unk_019(void* unk, i32 unk_2);
        virtual i32   unk_020();
        virtual void  stop_all_effects();
        virtual void* unk_022(void* unk);
        virtual i32   unk_023();
    };

    ASSERT_SIZEOF(ise_interface, 0x48);
} // treyarch
