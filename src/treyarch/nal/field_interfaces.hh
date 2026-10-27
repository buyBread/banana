#pragma once

#include "treyarch/nal/anim.hh"
#include "treyarch/nal/pose.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace nal {
    // nalFieldTypeInterface
    struct field_type_interface {
        virtual void post_decode_derive(nal::pose* pose,
                                        nal::anim* anim,
                                        u32        present_fields_count,
                                        u16*       present_fields) = 0;

        virtual void blend(u32        field_type,
                           nal::pose* result,
                           f32        alpha,
                           nal::pose* pose_a,
                           nal::pose* pose_b,
                           f32*       weights_a,
                           f32*       weights_b) = 0;

        virtual void map(u32        field_type,
                         nal::pose* target,
                         nal::pose* source,
                         i32*       field_map,
                         i32*       bone_map) = 0;
    };

    // nalFieldPositionInterface
    struct field_position_interface : field_type_interface {
        void post_decode_derive(nal::pose* pose,
                                nal::anim* anim,
                                u32        present_fields_count,
                                u16*       present_fields) override;

        void blend(u32        field_type,
                   nal::pose* result,
                   f32        alpha,
                   nal::pose* pose_a,
                   nal::pose* pose_b,
                   f32*       weights_a,
                   f32*       weights_b) override;

        void map(u32        field_type,
                 nal::pose* target,
                 nal::pose* source,
                 i32*       field_map,
                 i32*       bone_map) override;
    };

    // nalFieldQuaternionInterface
    struct field_quaternion_interface : field_type_interface {
        void post_decode_derive(nal::pose* pose,
                                nal::anim* anim,
                                u32        present_fields_count,
                                u16*       present_fields) override;

        void blend(u32        field_type,
                   nal::pose* result,
                   f32        alpha,
                   nal::pose* pose_a,
                   nal::pose* pose_b,
                   f32*       weights_a,
                   f32*       weights_b) override;

        void map(u32        field_type,
                 nal::pose* target,
                 nal::pose* source,
                 i32*       field_map,
                 i32*       bone_map) override;
    };

    // nalFieldAxis1AngleInterface
    struct field_axis1_angle_interface : field_type_interface {
        void post_decode_derive(nal::pose* pose,
                                nal::anim* anim,
                                u32        present_fields_count,
                                u16*       present_fields) override;

        void blend(u32        field_type,
                   nal::pose* result,
                   f32        alpha,
                   nal::pose* pose_a,
                   nal::pose* pose_b,
                   f32*       weights_a,
                   f32*       weights_b) override;

        void map(u32        field_type,
                 nal::pose* target,
                 nal::pose* source,
                 i32*       field_map,
                 i32*       bone_map) override;
    };

    // nalFieldFKCorrectionInterface
    struct field_fk_correction_interface : field_type_interface {
        void post_decode_derive(nal::pose* pose,
                                nal::anim* anim,
                                u32        present_fields_count,
                                u16*       present_fields) override;

        void blend(u32        field_type,
                   nal::pose* result,
                   f32        alpha,
                   nal::pose* pose_a,
                   nal::pose* pose_b,
                   f32*       weights_a,
                   f32*       weights_b) override;

        void map(u32        field_type,
                 nal::pose* target,
                 nal::pose* source,
                 i32*       field_map,
                 i32*       bone_map) override;
    };

    // nalFieldFKTrajectoryInterface
    struct field_fk_trajectory_interface : field_type_interface {
        void post_decode_derive(nal::pose* pose,
                                nal::anim* anim,
                                u32        present_fields_count,
                                u16*       present_fields) override;

        void blend(u32        field_type,
                   nal::pose* result,
                   f32        alpha,
                   nal::pose* pose_a,
                   nal::pose* pose_b,
                   f32*       weights_a,
                   f32*       weights_b) override;

        void map(u32        field_type,
                 nal::pose* target,
                 nal::pose* source,
                 i32*       field_map,
                 i32*       bone_map) override;
    };

    // nalFieldDefaultInterface
    struct field_default_interface : field_type_interface {
        void post_decode_derive(nal::pose* pose,
                                nal::anim* anim,
                                u32        present_fields_count,
                                u16*       present_fields) override;

        void blend(u32        field_type,
                   nal::pose* result,
                   f32        alpha,
                   nal::pose* pose_a,
                   nal::pose* pose_b,
                   f32*       weights_a,
                   f32*       weights_b) override;

        void map(u32        field_type,
                 nal::pose* target,
                 nal::pose* source,
                 i32*       field_map,
                 i32*       bone_map) override;
    };

    // nalFieldScaleInterface
    struct field_scale_interface : field_type_interface {
        void post_decode_derive(nal::pose* pose,
                                nal::anim* anim,
                                u32        present_fields_count,
                                u16*       present_fields) override;

        void blend(u32        field_type,
                   nal::pose* result,
                   f32        alpha,
                   nal::pose* pose_a,
                   nal::pose* pose_b,
                   f32*       weights_a,
                   f32*       weights_b) override;

        void map(u32        field_type,
                 nal::pose* target,
                 nal::pose* source,
                 i32*       field_map,
                 i32*       bone_map) override;
    };

    void remap_presence(u32        field_type,
                        i32*       field_map,
                        nal::pose* target,
                        nal::pose* source);

    void remap_sparse(u32        field_type,
                      nal::pose* target,
                      nal::pose* source,
                      i32*       bone_map);

    void blend_sparse(u32        field_type,
                      nal::pose* result,
                      f32        alpha,
                      nal::pose* pose_a,
                      nal::pose* pose_b);

    // one object per pose field, like retail's
    inline constinit field_position_interface      fk_position_interface;
    inline constinit field_quaternion_interface    fk_quaternion_interface;
    inline constinit field_axis1_angle_interface   fk_axis1_rotation_interface;
    inline constinit field_fk_correction_interface fk_error_correction_interface;
    inline constinit field_fk_trajectory_interface fk_trajectory_interface;
    inline constinit field_default_interface       floor_offset_interface;
    inline constinit field_default_interface       camera_data_interface;
    inline constinit field_default_interface       morph_sliders_interface;
    inline constinit field_scale_interface         bone_scale_interface;
    inline constinit field_default_interface       omnilight_data_interface;
    inline constinit field_default_interface       spotlight_data_interface;

    inline field_type_interface* const field_interfaces[11] = { &fk_position_interface,
                                                                &fk_quaternion_interface,
                                                                &fk_axis1_rotation_interface,
                                                                &fk_error_correction_interface,
                                                                &fk_trajectory_interface,
                                                                &floor_offset_interface,
                                                                &camera_data_interface,
                                                                &morph_sliders_interface,
                                                                &bone_scale_interface,
                                                                &omnilight_data_interface,
                                                                &spotlight_data_interface };
}} // treyarch::nal
