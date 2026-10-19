#pragma once

#include "treyarch/shared/dinkumware/vector.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/mash/mash_info.hh"
#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector_basic.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class environment;

    // each region's environments for its cosmetic meshes, by index
    class cosmetic_environments_set {

    public:
        void* self; // the resource handler points this at the set itself

        mash::vector_basic
            <string_hash>             environment_names; // type-30 environments
        dinkumware::vector
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
