#pragma once

#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector_basic.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // the level's "rvb_render_mgr"
    class render_lod_mgr {

    public:
        void* self; // the resource handler points this at the manager itself

        mash::vector_basic<string_hash> entity_names; // sorted; only these join the far render list unless forced

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (render_lod_mgr,               0x14);
    ASSERT_OFFSETOF(render_lod_mgr, entity_names, 0x04);
} // treyarch
