#pragma once

#include "game/reference.hh"
#include "treyarch/mash/types.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class sm_retrofit_param {

    public:
        enum e_shadow_mode : i32 {
            shadow_mode_receive_only,
            shadow_mode_cast_and_receive,
            shadow_mode_cast_only,
            shadow_mode_exempt
        };

        void* self; // the resource handler points this at the parameters themselves

        e_shadow_mode shadow_mode;

        reference
            <treyarch::adjustment_layers_library> adjustment_layers_library;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (sm_retrofit_param,                            0x10);
    ASSERT_OFFSETOF(sm_retrofit_param, shadow_mode,               0x04);
    ASSERT_OFFSETOF(sm_retrofit_param, adjustment_layers_library, 0x08);
} // treyarch
