#pragma once

#include "treyarch/game/wds/ai/param_block.hh"
#include "treyarch/shared/mash/types.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class default_values {

    public:
        void* self; // the resource handler points this at the defaults themselves

        u32         unk_04;
        param_block my_param_block;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (default_values,                 0x28);
    ASSERT_OFFSETOF(default_values, my_param_block, 0x08);
} // treyarch
