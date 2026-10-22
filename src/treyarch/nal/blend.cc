#include "retail.hh"
#include "treyarch/nal/blend.hh"
#include "treyarch/nal/field_interfaces.hh"
#include "treyarch/shared/math/vmath/quaternion.hh"
#include "treyarch/shared/math/vmath/vector.hh"

using namespace treyarch;

// sub_9D15B0
void nal::blend(nal::pose* pose_r,
                f32        alpha,
                nal::pose* pose_a,
                nal::pose* pose_b) {

    nal::pose_layout* layout       = pose_a->pose_layout;
    u16               total_fields = layout->total_fields;
    const f32         one_minus    = 1.0f - alpha;

    for (i32 i = 0; i < pose_r->present.data_count; i++)
        pose_r->present.data[i] = pose_a->present.data[i] | pose_b->present.data[i];

    f32 weights_a[384];
    f32 weights_b[384];

    for (u32 i = 0; i < total_fields; i++) {
        if (pose_a->present.is_one(i)) {
            if (pose_b->present.is_one(i)) {
                weights_a[i] = one_minus;
                weights_b[i] = alpha;
            } else {
                weights_a[i] = 1.0f;
                weights_b[i] = 0.0f;
            }
        } else {
            weights_a[i] = 0.0f;
            weights_b[i] = pose_b->present.is_one(i) ? 1.0f : 0.0f;
        }
    }

    for (u32 i = 0; i < 11; i++) {
        if (!layout->field_repeat_counts[i])
            continue;

        u16 first = layout->field_bit_offsets[i];

        if (i == pose_field_floor_offset) {
            field_type_interface* floor_offset = field_interfaces[pose_field_floor_offset];

            // the floor offset always comes from pose b
            weights_a[first] = 0.0f;
            weights_b[first] = 1.0f;

            floor_offset->blend(pose_field_floor_offset, pose_r, alpha, pose_a, pose_b, &weights_a[first], &weights_b[first]);
        } else {
            field_interfaces[i]->blend(i, pose_r, alpha, pose_a, pose_b, &weights_a[first], &weights_b[first]);
        }
    }
}

// sub_9CF940
void nal::blend_trajectory(nal::pose* pose_r,
                           f32        alpha,
                           nal::pose* pose_a,
                           nal::pose* pose_b) {

    auto value_a = (po*)pose_a->get_field_data_ptr(pose_field_fk_trajectory, 0);
    auto value_b = (po*)pose_b->get_field_data_ptr(pose_field_fk_trajectory, 0);
    auto value   = (po*)pose_r->get_field_data_ptr(pose_field_fk_trajectory, 0);

    const f32 one_minus = 1.0f - alpha;

    // all four lanes of the translation
    *(math::vector4*)&value->get_w() = *(math::vector4*)&value_a->get_w() * one_minus + *(math::vector4*)&value_b->get_w() * alpha;

    math::quaternion blended;

    // quaternion blend
    auto quaternion = (math::quaternion*)retail::sub_48D240((f32*)&blended,
                                                             one_minus,
                                                             alpha,
                                                             (f32*)&value_a->get_m33().get_quaternion(),
                                                             (f32*)&value_b->get_m33().get_quaternion());

    *(math::quaternion*)&value->get_m33().get_quaternion() = *quaternion;
}
