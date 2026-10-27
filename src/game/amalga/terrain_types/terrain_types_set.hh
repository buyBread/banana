#pragma once

#include "treyarch/mash/mash_info.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector.hh"
#include "banana/util/macros/sanity_assert.hh"

namespace treyarch {
    struct terrain_types_set {
        void*        self; // the resource handler points this at the set itself
        mash::vector
            <mash::string> terrain_types;

        // sub_75F020
        void construct_mashed_class() {
            self = nullptr;

            terrain_types.construct_mashed_class();
        }

        // inlined @ sub_759CF0
        void destruct_mashed_class() {
            terrain_types.destruct_mashed_class();
        }

        // inlined @ sub_767700
        void unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
            terrain_types.unmash(mash_info, this, buffer);
        }
    };

    ASSERT_SIZEOF  (terrain_types_set,                0x18);
    ASSERT_OFFSETOF(terrain_types_set, terrain_types, 0x04);
} // treyarch
