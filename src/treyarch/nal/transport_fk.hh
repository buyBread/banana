#pragma once

#include "treyarch/nal/transport_list.hh"
#include "game/fixed_string.hh"
#include "treyarch/math/matrix.hh"
#include "treyarch/math/quaternion.hh"
#include "treyarch/math/vector.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace nal {
    struct pose;

    constexpr u32 max_bones = 512;

    // nalBone
    struct bone {
        math::vector4    axis1;  // what FK_AXIS1_ROTATION turns about
        math::quaternion default_rotation;
        math::vector4    default_position;
        i32              parent_index;  // -1 for a root
        fixed_string     name;
        u8               pad_3c[4];
    };

    // nalTransportFK, transport id 0
    struct transport_fk : transport {
        u32   bone_count;
        bone* bones;                           // parents come first
        u32   unk_10[2];
        u16*  position_bones;                  // per FK_POSITION instance
        u16*  quaternion_bones;                // per FK_QUATERNION instance
        u16*  axis1_bones;                     // per FK_AXIS1_ROTATION instance
        u32   trajectory_position_instance;
        u32   trajectory_quaternion_instance;
        i32   trajectory_bone;                 // -1 for none
        void* unk_30;
        u32   unk_34;
        u32   unk_38;
        void* unk_3c;
        i32   root_bone;                       // -1 for none
        u16*  mirror_bones;                    // per bone, 0xFFFF for none
        u16*  mirror_flags;                    // per bone
        i16*  position_instances;              // per bone, -1 for none
        i16*  quaternion_instances;            // per bone, -1 for none
        i16*  axis1_instances;                 // per bone, -1 for none

        bool get_bone_index(fixed_string* name, u32* index);
        void get_bone_matrices(nal::pose* pose, math::mat43* matrices);
    };

    ASSERT_SIZEOF  (bone,                   0x40);
    ASSERT_OFFSETOF(bone, axis1,            0x00);
    ASSERT_OFFSETOF(bone, default_rotation, 0x10);
    ASSERT_OFFSETOF(bone, default_position, 0x20);
    ASSERT_OFFSETOF(bone, parent_index,     0x30);
    ASSERT_OFFSETOF(bone, name,             0x34);

    ASSERT_OFFSETOF(transport_fk, bone_count,                     0x08);
    ASSERT_OFFSETOF(transport_fk, bones,                          0x0C);
    ASSERT_OFFSETOF(transport_fk, position_bones,                 0x18);
    ASSERT_OFFSETOF(transport_fk, quaternion_bones,               0x1C);
    ASSERT_OFFSETOF(transport_fk, axis1_bones,                    0x20);
    ASSERT_OFFSETOF(transport_fk, trajectory_position_instance,   0x24);
    ASSERT_OFFSETOF(transport_fk, trajectory_quaternion_instance, 0x28);
    ASSERT_OFFSETOF(transport_fk, trajectory_bone,                0x2C);
    ASSERT_OFFSETOF(transport_fk, root_bone,                      0x40);
    ASSERT_OFFSETOF(transport_fk, mirror_bones,                   0x44);
    ASSERT_OFFSETOF(transport_fk, mirror_flags,                   0x48);
    ASSERT_OFFSETOF(transport_fk, position_instances,             0x4C);
    ASSERT_OFFSETOF(transport_fk, quaternion_instances,           0x50);
    ASSERT_OFFSETOF(transport_fk, axis1_instances,                0x54);
}} // treyarch::nal
