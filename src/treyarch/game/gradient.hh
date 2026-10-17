#pragma once

#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector_basic.hh"
#include "treyarch/shared/math/types/color.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    struct gradient {
        mash::vector_basic<color> data;

        // inlined @ sub_7C4BE0
        // inlined @ sub_7C4E20
        // inlined @ sub_7D1150
        // inlined @ sub_7D1A70
        // inlined @ sub_7DEC30
        void construct_mashed_class() {
            data.construct_mashed_class();
        }

        // inlined @ sub_7C3100
        // inlined @ sub_7C31E0
        // inlined @ sub_7C3240
        // inlined @ sub_7C4C40
        // inlined @ sub_7C4F10
        // inlined @ sub_7D12F0
        // inlined @ sub_7D1B30
        // inlined @ sub_7DCD40
        void destruct_mashed_class() {
            data.destruct_mashed_class();
        }

        // inlined @ sub_7DADB0
        // inlined @ sub_7DAE40
        // inlined @ sub_7DAF00
        // inlined @ sub_7DB000
        // inlined @ sub_7DB960
        // inlined @ sub_7DBD50
        // inlined @ sub_7DE190
        void unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
            data.unmash(mash_info, this, buffer);
        }
    };

    ASSERT_SIZEOF(gradient, 0x10);
} // treyarch
