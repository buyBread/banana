#pragma once

#include "treyarch/shared/fixed_string.hh"
#include "treyarch/shared/tl_fixed_bitvector.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace nal {
    struct pose_layout;

    // nalPose
    struct pose {
        nal::pose_layout*            pose_layout;
        u8*                          pose_data;
        u32                          alloced_data;
        tl_fixed_bitvector<u32, 384> present;  // one bit per field instance

        pose(nal::pose_layout* layout, void* preallocated_memory);
        ~pose();

        void init(nal::pose_layout* layout, void* preallocated_memory);
        void copy(const pose &pose);
    };

    // nalPoseLayout, inside the transport list
    struct pose_layout {
        nal::pose    default_pose;  // its layout is this one
        fixed_string name;          // the transport list's name
        u32          version;
        u32          unk_4c;
        u32          unk_50[3];
        u32          pose_size;
        f32*         dequant_scale[11];
        u16          field_repeat_counts[11];
        u16          field_offsets[11];
        u16          field_bit_offsets[11];
        u16          total_fields;
        u16          total_values;
    };

    ASSERT_SIZEOF  (pose,               0x40);
    ASSERT_OFFSETOF(pose, pose_layout,  0x00);
    ASSERT_OFFSETOF(pose, pose_data,    0x04);
    ASSERT_OFFSETOF(pose, alloced_data, 0x08);
    ASSERT_OFFSETOF(pose, present,      0x0C);

    ASSERT_OFFSETOF(pose_layout, default_pose,        0x00);
    ASSERT_OFFSETOF(pose_layout, name,                0x40);
    ASSERT_OFFSETOF(pose_layout, version,             0x48);
    ASSERT_OFFSETOF(pose_layout, unk_4c,              0x4C);
    ASSERT_OFFSETOF(pose_layout, unk_50,              0x50);
    ASSERT_OFFSETOF(pose_layout, pose_size,           0x5C);
    ASSERT_OFFSETOF(pose_layout, dequant_scale,       0x60);
    ASSERT_OFFSETOF(pose_layout, field_repeat_counts, 0x8C);
    ASSERT_OFFSETOF(pose_layout, field_offsets,       0xA2);
    ASSERT_OFFSETOF(pose_layout, field_bit_offsets,   0xB8);
    ASSERT_OFFSETOF(pose_layout, total_fields,        0xCE);
    ASSERT_OFFSETOF(pose_layout, total_values,        0xD0);
}} // treyarch::nal
