#pragma once

#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector_basic.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    struct adjustment_layers {
        f32 unk_00[12];
    };

    class adjustment_layers_library {

    public:
        void* self; // the resource handler points this at the library itself
        u32   unk_04[3];

        mash::vector_basic
            <adjustment_layers> layers;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (adjustment_layers, 0x30);

    ASSERT_SIZEOF  (adjustment_layers_library,         0x20);
    ASSERT_OFFSETOF(adjustment_layers_library, layers, 0x10);
} // treyarch
