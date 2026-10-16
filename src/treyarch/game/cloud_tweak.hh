#pragma once

#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    struct cloud_tweak_layer_sample {
        f32 u;
        f32 v;
        f32 mul;
        f32 add;
        f32 min;
        f32 max;
        f32 angle;
        f32 scale;

        void construct_mashed_class() {}
        void destruct_mashed_class() {}
        void unmash(mash::mash_info_struct*, void*, mash::buffer_type) {}
    };

    struct cloud_tweak_layer {
        mash::vector
            <cloud_tweak_layer_sample> sample;

        // inlined @ sub_7DEDD0
        void construct_mashed_class() {
            sample.construct_mashed_class();
        }

        // inlined @ sub_7DD520
        void destruct_mashed_class() {
            sample.destruct_mashed_class();
        }

        // inlined @ sub_7DE0B0
        void unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
            sample.unmash(mash_info, this, buffer);
        }
    };

    class cloud_tweak {

    public:
        void* self; // the resource handler points this at the tweak itself

        mash::vector
            <cloud_tweak_layer> layer;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (cloud_tweak_layer_sample, 0x20);

    ASSERT_SIZEOF  (cloud_tweak_layer, 0x14);

    ASSERT_SIZEOF  (cloud_tweak,        0x18);
    ASSERT_OFFSETOF(cloud_tweak, layer, 0x04);
} // treyarch
