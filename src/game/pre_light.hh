#pragma once

#include "game/reference.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector_basic.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    struct pre_light_entry {
        f32 values[8];
    };

    class pre_light {

    public:
        void* self; // the resource handler points this at the pre-light itself

        reference<ngl::texture> horizon_map;
        mash::vector_basic
            <pre_light_entry>   entries;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (pre_light_entry, 0x20);

    ASSERT_SIZEOF  (pre_light,              0x20);
    ASSERT_OFFSETOF(pre_light, horizon_map, 0x04);
    ASSERT_OFFSETOF(pre_light, entries,     0x10);
} // treyarch
