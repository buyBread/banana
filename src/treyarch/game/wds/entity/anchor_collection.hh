#pragma once

#include "treyarch/shared/mash/mash_info.hh"
#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector_basic.hh"
#include "treyarch/shared/math/types/vector3.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    // a slot's anchor points; the handler makes an anchor entity at each
    class anchor_collection {

    public:
        void* self; // the resource handler points this at the collection itself

        mash::vector_basic<vector3> anchor_positions;
        void*                       entity_instances; // the anchor entities, from the entity manager

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (anchor_collection,                   0x18);
    ASSERT_OFFSETOF(anchor_collection, anchor_positions, 0x04);
    ASSERT_OFFSETOF(anchor_collection, entity_instances, 0x14);
} // treyarch
