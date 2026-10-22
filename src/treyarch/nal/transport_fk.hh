#pragma once

#include "treyarch/nal/transport_list.hh"
#include "treyarch/shared/fixed_string.hh"
#include "treyarch/shared/math/vmath/quaternion.hh"
#include "treyarch/shared/math/vmath/vector.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace nal {
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
        bone* bones;  // parents come first

        bool get_bone_index(fixed_string* name, u32* index);
    };

    ASSERT_SIZEOF  (bone,                   0x40);
    ASSERT_OFFSETOF(bone, axis1,            0x00);
    ASSERT_OFFSETOF(bone, default_rotation, 0x10);
    ASSERT_OFFSETOF(bone, default_position, 0x20);
    ASSERT_OFFSETOF(bone, parent_index,     0x30);
    ASSERT_OFFSETOF(bone, name,             0x34);

    ASSERT_OFFSETOF(transport_fk, bone_count, 0x08);
    ASSERT_OFFSETOF(transport_fk, bones,      0x0C);
}} // treyarch::nal
