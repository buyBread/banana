#pragma once

#include "treyarch/shared/mash/string.hh"
#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector.hh"
#include "treyarch/shared/mash/vector_basic.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    struct all_buildings_unk_18 {
        u8 unk_00[4];
    };

    class all_buildings {

    public:
        void* self; // the resource handler points this at the buildings themselves

        mash::vector
            <mash::string>         unk_04;
        mash::vector_basic
            <all_buildings_unk_18> unk_18;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (all_buildings_unk_18, 0x04);

    ASSERT_SIZEOF  (all_buildings,         0x28);
    ASSERT_OFFSETOF(all_buildings, unk_04, 0x04);
    ASSERT_OFFSETOF(all_buildings, unk_18, 0x18);
} // treyarch
