#pragma once

#include "treyarch/mash/string.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector.hh"
#include "game/math/algebra.hh"
#include "treyarch/math/vector.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    struct igo_3d_text_child {
        mash::string member_name;
        mash::string record_name;

        // inlined @ sub_6F1D60
        void construct_mashed_class() {
            member_name.construct_mashed_class();
            record_name.construct_mashed_class();
        }

        // inlined @ sub_6D7330
        void destruct_mashed_class() {
            member_name.destruct_mashed_class();
            record_name.destruct_mashed_class();
        }

        // inlined @ sub_6E6BC0
        void unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
            member_name.unmash(mash_info, this, buffer);
            record_name.unmash(mash_info, this, buffer);
        }
    };

    struct igo_3d_widget_child {
        mash::string member_name;
        mash::string record_name;

        // inlined @ sub_6F1D60
        void construct_mashed_class() {
            member_name.construct_mashed_class();
            record_name.construct_mashed_class();
        }

        // inlined @ sub_6D73C0
        void destruct_mashed_class() {
            member_name.destruct_mashed_class();
            record_name.destruct_mashed_class();
        }

        // inlined @ sub_6E6BC0
        void unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
            member_name.unmash(mash_info, this, buffer);
            record_name.unmash(mash_info, this, buffer);
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

        void* self; // the resource handler points this at the parameters themselves

        mash::string              name;
        mash::string              entity_name;
        math::dir3::packed        position;
        f32                       tilt_angle;
        f32                       rotation_angle;
        f32                       spin_angle;
        f32                       scale;
        e_wide_align_flags        widescreen_align;
        u32                       suppression_mask; // igo_widget_suppression_condition bits
        u8                        anims_enabled;
        u8                        pad_41[0x03];
        mash::string              spawn_anim;
        mash::string              anim_50;
        mash::string              anim_5c;
        i32                       value_source;
        i32                       value_display;
        f32                       value_min;
        f32                       value_max;
        u32                       unk_78;
        f32                       ui_light_near_dist;
        f32                       ui_light_far_dist;
        vector3d                  ui_light_color;
        matrix4x4*                matrix;
        u32                       unk_94;
        mash::vector
            <igo_3d_text_child>   text_children;
        mash::vector
            <igo_3d_widget_child> widget_children;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);

        void update_transform_matrix();
    };

    ASSERT_SIZEOF  (igo_3d_text_child,   0x18);
    ASSERT_SIZEOF  (igo_3d_widget_child, 0x18);

    ASSERT_SIZEOF  (igo_3d_widget_parameters,                     0xC0);
    ASSERT_OFFSETOF(igo_3d_widget_parameters, name,               0x04);
    ASSERT_OFFSETOF(igo_3d_widget_parameters, entity_name,        0x10);
    ASSERT_OFFSETOF(igo_3d_widget_parameters, position,           0x1C);
    ASSERT_OFFSETOF(igo_3d_widget_parameters, scale,              0x34);
    ASSERT_OFFSETOF(igo_3d_widget_parameters, widescreen_align,   0x38);
    ASSERT_OFFSETOF(igo_3d_widget_parameters, anims_enabled,      0x40);
    ASSERT_OFFSETOF(igo_3d_widget_parameters, spawn_anim,         0x44);
    ASSERT_OFFSETOF(igo_3d_widget_parameters, value_source,       0x68);
    ASSERT_OFFSETOF(igo_3d_widget_parameters, ui_light_near_dist, 0x7C);
    ASSERT_OFFSETOF(igo_3d_widget_parameters, ui_light_color,     0x84);
    ASSERT_OFFSETOF(igo_3d_widget_parameters, matrix,             0x90);
    ASSERT_OFFSETOF(igo_3d_widget_parameters, text_children,      0x98);
    ASSERT_OFFSETOF(igo_3d_widget_parameters, widget_children,    0xAC);
} // treyarch
