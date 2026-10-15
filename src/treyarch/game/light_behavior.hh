#pragma once

#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/virtual_base.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    // the base of every flicker; mashed ones get their retail vtable from the type registry
    class flicker : public mash::mash_virtual_base {};

    class light_behavior {

    public:
        void*              self; // the resource handler points this at the behavior itself
        treyarch::flicker* flicker;
        u32                unk_08;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (flicker, 0x04);

    ASSERT_SIZEOF  (light_behavior,          0x0C);
    ASSERT_OFFSETOF(light_behavior, flicker, 0x04);
} // treyarch
