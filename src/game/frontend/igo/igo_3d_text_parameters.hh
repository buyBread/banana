#pragma once

#include "treyarch/mash/string.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector.hh"
#include "treyarch/math/matrix.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_text_parameters {

    public:
        struct parameter_set {
            mash::string        unk_00;
            math::dir3::packed  unk_0c;
            f32                 unk_18;
            f32                 unk_1c;
            f32                 unk_20;
            u32                 unk_24[40];
            mash::string        unk_c4;
            math::mat43::packed unk_d0;
            u32                 unk_100;

            void construct_mashed_class();

            // inlined @ sub_6CC410
            void destruct_mashed_class() {
                unk_00.destruct_mashed_class();
                unk_c4.destruct_mashed_class();
            }

            // inlined @ sub_6E6AE0
            void unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
                unk_00.unmash(mash_info, this, buffer);
                unk_c4.unmash(mash_info, this, buffer);
            }
        };

        void* self; // the resource handler points this at the parameters themselves

        mash::vector
            <parameter_set> parameters;

        i32 english;
        i32 french;
        i32 italian;
        i32 german;
        i32 spanish;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (igo_3d_text_parameters::parameter_set,          0x104);
    ASSERT_OFFSETOF(igo_3d_text_parameters::parameter_set, unk_0c,  0x00C);
    ASSERT_OFFSETOF(igo_3d_text_parameters::parameter_set, unk_24,  0x024);
    ASSERT_OFFSETOF(igo_3d_text_parameters::parameter_set, unk_c4,  0x0C4);
    ASSERT_OFFSETOF(igo_3d_text_parameters::parameter_set, unk_d0,  0x0D0);
    ASSERT_OFFSETOF(igo_3d_text_parameters::parameter_set, unk_100, 0x100);

    ASSERT_SIZEOF  (igo_3d_text_parameters,             0x2C);
    ASSERT_OFFSETOF(igo_3d_text_parameters, parameters, 0x04);
    ASSERT_OFFSETOF(igo_3d_text_parameters, english,    0x18);
    ASSERT_OFFSETOF(igo_3d_text_parameters, spanish,    0x28);
} // treyarch
