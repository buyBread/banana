#pragma once

#include "vc8/vector.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/mash_info.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector_basic.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class environment;

    // each region's environments for its cosmetic meshes, by index
    class cosmetic_environments_set {

    public:
        void* self; // the resource handler points this at the set itself

        mash::vector_basic
            <string_hash>             environment_names; // type-30 environments
        vc8::vector
            <treyarch::environment*>* environments;      // built by activate, one per name

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (cosmetic_environments_set,                    0x18);
    ASSERT_OFFSETOF(cosmetic_environments_set, environment_names, 0x04);
    ASSERT_OFFSETOF(cosmetic_environments_set, environments,      0x14);
} // treyarch
