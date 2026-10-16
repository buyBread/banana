#pragma once

#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector.hh"
#include "treyarch/shared/mash/virtual_base.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    // the base of every tweak; mashed ones get their retail vtable from the type registry
    class sm_decal_tweak : public mash::mash_virtual_base {};

    class sm_decal_material {

    public:
        void* self; // the resource handler points this at the material itself
        
        mash::vector
            <sm_decal_tweak> tweaks;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (sm_decal_tweak, 0x04);

    ASSERT_SIZEOF  (sm_decal_material,         0x18);
    ASSERT_OFFSETOF(sm_decal_material, tweaks, 0x04);
} // treyarch
