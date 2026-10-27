#include <cstring>

#include "retail.hh"
#include "treyarch/nal/pose.hh"
#include "treyarch/nal/transport_fk.hh"
#include "treyarch/math/matrix_rot.hh"
#include "treyarch/math/rotation.hh"
#include "treyarch/base/tl_defs.hh"

using namespace treyarch;

// sub_9CD1F0
bool nal::transport_fk::get_bone_index(fixed_string* name, u32* index) {
    for (u32 i = 0; i < bone_count; i++) {
        if (bones[i].name.hash == name->hash) {
            *index = i;

            return true;
        }
    }

    *index = 0;

    return false;
}

// sub_9D0A30
void nal::transport_fk::get_bone_matrices(nal::pose* pose, math::mat43* matrices) { // model space, parents first
    bool position_set[max_bones];
    bool rotation_set[max_bones];

    // conversions and products share it, so the rows' fourth lanes carry over
    math::mat43 scratch;

    tl_prefetch(pose, 0);
    std::memset(position_set, 0, bone_count);
    tl_prefetch(&pose->pose_layout->field_offsets[pose_field_fk_error_correction], 0);
    tl_prefetch(&pose->present, 0);
    tl_prefetch(&position_bones, 0);
    std::memset(rotation_set, 0, bone_count);

    u32 count = pose->pose_layout->field_repeat_counts[pose_field_fk_position];
    u16 bit   = pose->pose_layout->field_bit_offsets[pose_field_fk_position];

    for (u32 i = 0; i < count; i++) {
        if (!pose->present.is_one(bit + i))
            continue;

        u16 bone = position_bones[i];

        position_set[bone] = true;

        *(math::vector4*)&matrices[bone].get_w() = *(math::vector4*)pose->get_field_data_ptr(pose_field_fk_position, i);
    }

    count = pose->pose_layout->field_repeat_counts[pose_field_fk_axis1_rotation];
    bit   = pose->pose_layout->field_bit_offsets[pose_field_fk_axis1_rotation];

    for (u32 i = 0; i < count; i++) {
        if (!pose->present.is_one(bit + i))
            continue;

        u16 bone = axis1_bones[i];

        rotation_set[bone] = true;

        f32                   angle    = *pose->get_field_data_ptr(pose_field_fk_axis1_rotation, i);
        math::unit_quaternion turn     = math::axis_angle_to_quaternion(*(math::unit_dir3*)&bones[bone].axis1, angle);
        math::quaternion      rotation = bones[bone].default_rotation * turn;

        auto rows = (math::rot_mat33*)retail::sub_48D800((u32*)&scratch, (f32*)&rotation);

        *(math::rot_mat33*)&matrices[bone] = *rows;
    }

    count = pose->pose_layout->field_repeat_counts[pose_field_fk_quaternion];
    bit   = pose->pose_layout->field_bit_offsets[pose_field_fk_quaternion];

    for (u32 i = 0; i < count; i++) {
        if (!pose->present.is_one(bit + i))
            continue;

        u16              bone     = quaternion_bones[i];
        math::quaternion rotation = *(math::quaternion*)pose->get_field_data_ptr(pose_field_fk_quaternion, i);

        rotation_set[bone] = true;

        auto rows = (math::rot_mat33*)retail::sub_48D800((u32*)&scratch, (f32*)&rotation);

        *(math::rot_mat33*)&matrices[bone] = *rows;
    }

    bit = pose->pose_layout->field_bit_offsets[pose_field_fk_error_correction];

    // a prefix: the list ends at the first instance that isn't present; w holds the bone index
    for (u32 i = 0; i < pose->pose_layout->field_repeat_counts[pose_field_fk_error_correction]; i++) {
        if (!pose->present.is_one(bit + i))
            break;

        auto value = (math::vector4*)pose->get_field_data_ptr(pose_field_fk_error_correction, i);
        i32  bone  = *(i32*)&(*value)[3];

        *(math::vector4*)&matrices[bone].get_w() = math::vector4((*value)[0], (*value)[1], (*value)[2], 0.0f);

        position_set[bone] = true;
    }

    for (u32 i = 0; i < bone_count; i++) {
        if (!position_set[i])
            *(math::vector4*)&matrices[i].get_w() = bones[i].default_position;

        if (!rotation_set[i]) {
            math::quaternion rotation = bones[i].default_rotation;

            auto rows = (math::rot_mat33*)retail::sub_48D800((u32*)&scratch, (f32*)&rotation);

            *(math::rot_mat33*)&matrices[i] = *rows;
        }
    }

    // local times parent; the rows' fourth lanes are left as the last conversion wrote them
    for (u32 i = 0; i < bone_count; i++) {
        i32 parent_index = bones[i].parent_index;

        if (parent_index == -1)
            continue;

        auto local  = (math::vector4*)&matrices[i];
        auto parent = (math::vector4*)&matrices[parent_index];
        auto result = (math::vector4*)&scratch;

        for (i32 row = 0; row < 3; row++) {
            for (i32 k = 0; k < 3; k++)
                result[row][k] = (f32)((f64)parent[0][k] * local[row][0] + (f64)parent[1][k] * local[row][1] + (f64)parent[2][k] * local[row][2]);
        }

        for (i32 k = 0; k < 4; k++)
            result[3][k] = (f32)((f64)parent[0][k] * local[3][0] + (f64)parent[1][k] * local[3][1] + (f64)parent[2][k] * local[3][2] + (f64)parent[3][k]);

        matrices[i] = scratch;
    }
}
