#pragma once

#include "treyarch/mash/string.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector.hh"
#include "game/math/algebra.hh"
#include "treyarch/math/vector.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    struct igo_3d_widget_parameters_unk_98 {
        mash::string unk_00;
        mash::string unk_0c;

        // inlined @ sub_6F1D60
        void construct_mashed_class() {
            unk_00.construct_mashed_class();
            unk_0c.construct_mashed_class();
        }

        // inlined @ sub_6D7330
        void destruct_mashed_class() {
            unk_00.destruct_mashed_class();
            unk_0c.destruct_mashed_class();
        }

        // inlined @ sub_6E6BC0
        void unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
            unk_00.unmash(mash_info, this, buffer);
            unk_0c.unmash(mash_info, this, buffer);
        }
    };

    struct igo_3d_widget_parameters_unk_ac {
        mash::string unk_00;
        mash::string unk_0c;

        // inlined @ sub_6F1D60
        void construct_mashed_class() {
            unk_00.construct_mashed_class();
            unk_0c.construct_mashed_class();
        }

        // inlined @ sub_6D73C0
        void destruct_mashed_class() {
            unk_00.destruct_mashed_class();
            unk_0c.destruct_mashed_class();
        }

        // inlined @ sub_6E6BC0
        void unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
            unk_00.unmash(mash_info, this, buffer);
            unk_0c.unmash(mash_info, this, buffer);
        }
    };

    class igo_3d_widget_parameters {

    public:
        enum e_wide_align_flags : i32 {
            wide_align_center,
            wide_align_left,
            wide_align_right,
            wide_align_proportional,
            num_wide_align_flags
        };

        void*              self; // the resource handler points this at the parameters themselves
        mash::string       unk_04;
        mash::string       unk_10;
        math::dir3::packed position;
        f32                tilt_angle;
        f32                rotation_angle;
        f32                spin_angle;
        f32                scale;
        e_wide_align_flags widescreen_align;
        u32                unk_3c;
        u32                unk_40;
        mash::string       unk_44;
        mash::string       unk_50;
        mash::string       unk_5c;
        u32                unk_68[10];
        matrix4x4*         matrix;
        u32                unk_94;

        mash::vector
            <igo_3d_widget_parameters_unk_98> unk_98;
        mash::vector
            <igo_3d_widget_parameters_unk_ac> unk_ac;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (igo_3d_widget_parameters_unk_98, 0x18);
    ASSERT_SIZEOF  (igo_3d_widget_parameters_unk_ac, 0x18);

    ASSERT_SIZEOF  (igo_3d_widget_parameters,                   0xC0);
    ASSERT_OFFSETOF(igo_3d_widget_parameters, unk_10,           0x10);
    ASSERT_OFFSETOF(igo_3d_widget_parameters, position,         0x1C);
    ASSERT_OFFSETOF(igo_3d_widget_parameters, scale,            0x34);
    ASSERT_OFFSETOF(igo_3d_widget_parameters, widescreen_align, 0x38);
    ASSERT_OFFSETOF(igo_3d_widget_parameters, unk_44,           0x44);
    ASSERT_OFFSETOF(igo_3d_widget_parameters, unk_68,           0x68);
    ASSERT_OFFSETOF(igo_3d_widget_parameters, matrix,           0x90);
    ASSERT_OFFSETOF(igo_3d_widget_parameters, unk_98,           0x98);
    ASSERT_OFFSETOF(igo_3d_widget_parameters, unk_ac,           0xAC);
} // treyarch
