#include <cmath>
#include <cstring>

#include "retail.hh"
#include "treyarch/nal/field_interfaces.hh"
#include "treyarch/shared/math/vmath/quaternion.hh"
#include "treyarch/shared/math/vmath/vector.hh"

using namespace treyarch;

// sub_9D06E0
void nal::field_position_interface::post_decode_derive(nal::pose* pose,
                                                       nal::anim* anim,
                                                       u32        present_fields_count,
                                                       u16*       present_fields) {

    (void)anim;

    auto data = (math::vector4*)pose->get_field_data_ptr(pose_field_fk_position, 0);

    for (u32 i = 0; i < present_fields_count; i++)
        data[present_fields[i]][3] = 0.0f;
}

// sub_9CDC10
void nal::field_position_interface::blend(u32        field_type,
                                          nal::pose* result,
                                          f32        alpha,
                                          nal::pose* pose_a,
                                          nal::pose* pose_b,
                                          f32*       weights_a,
                                          f32*       weights_b) {

    retail::sub_9CDC10(field_type, (u32*)result, alpha, (u32*)pose_a, (u32*)pose_b, weights_a, weights_b);
}

// sub_9D0780
void nal::field_position_interface::map(u32        field_type,
                                        nal::pose* target,
                                        nal::pose* source,
                                        i32*       field_map,
                                        i32*       bone_map) {

    (void)bone_map;

    f32* source_data = source->get_field_data_ptr(field_type, 0);
    f32* value       = target->get_field_data_ptr(field_type, 0);
    u32  values      = values_per_field[field_type];
    f32* end         = value + values * target->pose_layout->field_repeat_counts[field_type];

    for (i32* index = field_map; value < end; value += values, index++) {
        if (*index == -1)
            continue;

        for (u32 i = 0; i < values; i++)
            value[i] = source_data[i + values * *index];
    }

    remap_presence(field_type, field_map, target, source);
}

// sub_9CD5B0
void nal::field_quaternion_interface::post_decode_derive(nal::pose* pose,
                                                         nal::anim* anim,
                                                         u32        present_fields_count,
                                                         u16*       present_fields) {

    (void)anim;

    auto data = (math::quaternion*)pose->get_field_data_ptr(pose_field_fk_quaternion, 0);

    for (u32 i = 0; i < present_fields_count; i++) {
        math::quaternion &value = data[present_fields[i]];

        const f32 x = value[0];
        const f32 y = value[1];
        const f32 z = value[2];
        const f32 w = value[3];

        const f32 length = std::sqrt((f32)((f64)x * x + (f64)y * y + (f64)z * z + (f64)w * w));

        value[0] = x / length;
        value[1] = y / length;
        value[2] = z / length;
        value[3] = w / length;
    }
}

// sub_9CD6A0
void nal::field_quaternion_interface::blend(u32        field_type,
                                            nal::pose* result,
                                            f32        alpha,
                                            nal::pose* pose_a,
                                            nal::pose* pose_b,
                                            f32*       weights_a,
                                            f32*       weights_b) {

    retail::sub_9CD6A0(field_type, (u32*)result, alpha, (u32*)pose_a, (u32*)pose_b, weights_a, weights_b);
}

// sub_9D0640
void nal::field_quaternion_interface::map(u32        field_type,
                                          nal::pose* target,
                                          nal::pose* source,
                                          i32*       field_map,
                                          i32*       bone_map) {

    (void)bone_map;

    u32  values      = values_per_field[field_type];
    auto source_data = (math::quaternion*)source->get_field_data_ptr(field_type, 0);
    f32* value       = target->get_field_data_ptr(field_type, 0);
    f32* end         = value + values * target->pose_layout->field_repeat_counts[field_type];

    for (i32* index = field_map; value < end; value += values, index++) {
        if (*index != -1)
            *(math::quaternion*)value = source_data[*index];
    }

    remap_presence(field_type, field_map, target, source);
}

// sub_9CD780
void nal::field_axis1_angle_interface::post_decode_derive(nal::pose*, nal::anim*, u32, u16*) {}

// sub_9CD790
void nal::field_axis1_angle_interface::blend(u32        field_type,
                                             nal::pose* result,
                                             f32        alpha,
                                             nal::pose* pose_a,
                                             nal::pose* pose_b,
                                             f32*       weights_a,
                                             f32*       weights_b) {

    retail::sub_9CD790(field_type, (u32*)result, alpha, (u32*)pose_a, (u32*)pose_b, weights_a, weights_b);
}

// sub_9CFF70
void nal::field_axis1_angle_interface::map(u32        field_type,
                                           nal::pose* target,
                                           nal::pose* source,
                                           i32*       field_map,
                                           i32*       bone_map) {

    (void)bone_map;

    f32* source_data = source->get_field_data_ptr(field_type, 0);
    f32* value       = target->get_field_data_ptr(field_type, 0);
    f32* end         = value + values_per_field[field_type] * target->pose_layout->field_repeat_counts[field_type];

    for (i32* index = field_map; value < end; value++, index++) {
        if (*index != -1)
            *value = source_data[*index];
    }

    remap_presence(field_type, field_map, target, source);
}

// sub_9CDB30
void nal::field_fk_correction_interface::post_decode_derive(nal::pose* pose,
                                                            nal::anim* anim,
                                                            u32        present_fields_count,
                                                            u16*       present_fields) {

    auto data = (math::vector4*)pose->get_field_data_ptr(pose_field_fk_error_correction, 0);

    if (!present_fields_count)
        return;

    auto fk_data = (transport_data_fk*)anim->get_transport_data(0);

    if (!fk_data || !fk_data->ecc_count)
        return;

    // w holds the bone index
    for (u32 i = 0; i < present_fields_count; i++)
        *(u32*)&data[present_fields[i]][3] = fk_data->ecc_bones[i];
}

// sub_9D05F0
void nal::field_fk_correction_interface::blend(u32        field_type,
                                               nal::pose* result,
                                               f32        alpha,
                                               nal::pose* pose_a,
                                               nal::pose* pose_b,
                                               f32*       weights_a,
                                               f32*       weights_b) {

    retail::sub_9D05F0(field_type, (u32*)result, alpha, (u32*)pose_a, (u32*)pose_b, weights_a, weights_b);
}

// sub_9D0620
void nal::field_fk_correction_interface::map(u32        field_type,
                                             nal::pose* target,
                                             nal::pose* source,
                                             i32*       field_map,
                                             i32*       bone_map) {

    (void)field_map;

    remap_sparse(field_type, target, source, bone_map);
}

// sub_9CD780
void nal::field_fk_trajectory_interface::post_decode_derive(nal::pose*, nal::anim*, u32, u16*) {}

// sub_9CDBA0
void nal::field_fk_trajectory_interface::blend(u32        field_type,
                                               nal::pose* result,
                                               f32        alpha,
                                               nal::pose* pose_a,
                                               nal::pose* pose_b,
                                               f32*       weights_a,
                                               f32*       weights_b) {

    retail::sub_9CDBA0(field_type, (u32*)result, alpha, (u32*)pose_a, (u32*)pose_b, weights_a, weights_b);
}

// sub_9CFF00
void nal::field_fk_trajectory_interface::map(u32        field_type,
                                             nal::pose* target,
                                             nal::pose* source,
                                             i32*       field_map,
                                             i32*       bone_map) {

    (void)bone_map;

    *(po*)target->get_field_data_ptr(field_type, 0) = *(po*)source->get_field_data_ptr(field_type, 0);

    remap_presence(field_type, field_map, target, source);
}

// sub_9CD780
void nal::field_default_interface::post_decode_derive(nal::pose*, nal::anim*, u32, u16*) {}

// sub_9CDC10
void nal::field_default_interface::blend(u32        field_type,
                                         nal::pose* result,
                                         f32        alpha,
                                         nal::pose* pose_a,
                                         nal::pose* pose_b,
                                         f32*       weights_a,
                                         f32*       weights_b) {

    retail::sub_9CDC10(field_type, (u32*)result, alpha, (u32*)pose_a, (u32*)pose_b, weights_a, weights_b);
}

// sub_9CFDB0
void nal::field_default_interface::map(u32        field_type,
                                       nal::pose* target,
                                       nal::pose* source,
                                       i32*       field_map,
                                       i32*       bone_map) {

    (void)bone_map;

    // copied in order, the field map only decides presence
    if (target->pose_layout->field_repeat_counts[field_type] && source->pose_layout->field_repeat_counts[field_type]) {
        u32  values       = values_per_field[field_type];
        f32* value        = target->get_field_data_ptr(field_type, 0);
        f32* source_value = source->get_field_data_ptr(field_type, 0);
        f32* end          = value        + values * target->pose_layout->field_repeat_counts[field_type];
        f32* source_end   = source_value + values * source->pose_layout->field_repeat_counts[field_type];

        for (; value < end && source_value < source_end; value += values, source_value += values) {
            for (u32 i = 0; i < values; i++)
                value[i] = source_value[i];
        }
    }

    remap_presence(field_type, field_map, target, source);
}

// sub_9CDE70
void nal::field_scale_interface::post_decode_derive(nal::pose* pose,
                                                    nal::anim* anim,
                                                    u32        present_fields_count,
                                                    u16*       present_fields) {

    auto data = (math::vector4*)pose->get_field_data_ptr(pose_field_bone_scale, 0);

    if (!present_fields_count)
        return;

    auto fk_data = (transport_data_fk*)anim->get_transport_data(0);

    if (!fk_data || !fk_data->scale_count)
        return;

    // w holds the bone index
    for (u32 i = 0; i < present_fields_count; i++)
        *(u32*)&data[present_fields[i]][3] = fk_data->scale_bones[i];
}

// sub_9D05F0
void nal::field_scale_interface::blend(u32        field_type,
                                       nal::pose* result,
                                       f32        alpha,
                                       nal::pose* pose_a,
                                       nal::pose* pose_b,
                                       f32*       weights_a,
                                       f32*       weights_b) {

    retail::sub_9D05F0(field_type, (u32*)result, alpha, (u32*)pose_a, (u32*)pose_b, weights_a, weights_b);
}

// sub_9D0620
void nal::field_scale_interface::map(u32        field_type,
                                     nal::pose* target,
                                     nal::pose* source,
                                     i32*       field_map,
                                     i32*       bone_map) {

    (void)field_map;

    remap_sparse(field_type, target, source, bone_map);
}

// sub_9CFC60
void nal::remap_presence(u32        field_type,
                         i32*       field_map,
                         nal::pose* target,
                         nal::pose* source) {

    u16 count      = target->pose_layout->field_repeat_counts[field_type];
    u16 target_bit = target->pose_layout->field_bit_offsets[field_type];
    u16 source_bit = source->pose_layout->field_bit_offsets[field_type];

    if (field_map) {
        for (u32 i = 0; i < count; i++) {
            if (field_map[i] == -1 || !source->present.is_one(field_map[i] + source_bit))
                target->present.set_zero(target_bit + i);
            else
                target->present.set_one(target_bit + i);
        }
    } else {
        u16 source_count = source->pose_layout->field_repeat_counts[field_type];

        for (u16 i = 0; i < count; i++) {
            if (i < source_count && source->present.is_one(i + source_bit))
                target->present.set_one(target_bit + i);
            else
                target->present.set_zero(target_bit + i);
        }
    }
}

// sub_9D0440
void nal::remap_sparse(u32        field_type,
                       nal::pose* target,
                       nal::pose* source,
                       i32*       bone_map) {

    u16  target_bit   = target->pose_layout->field_bit_offsets[field_type];
    u16  source_bit   = source->pose_layout->field_bit_offsets[field_type];
    auto target_data  = (math::vector4*)target->get_field_data_ptr(field_type, 0);
    auto source_data  = (math::vector4*)source->get_field_data_ptr(field_type, 0);
    u32  target_count = target->pose_layout->field_repeat_counts[field_type];

    u32 count     = 0;
    i32 last_bone = -1;

    // the list ends at the first instance that isn't present; w holds the bone index
    for (u32 i = 0; i < target_count; i++) {
        if (!source->present.is_one(source_bit + i))
            break;

        i32 bone = bone_map[*(u32*)&source_data[i][3]];

        if (bone == -1)
            continue;

        target->present.set_one(target_bit + count);

        if (bone >= last_bone) {
            target_data[count] = source_data[i];
            *(i32*)&target_data[count][3] = bone;

            last_bone = bone;
        } else {
            // sorted insert, with no lower bound on the search
            i32 slot = count - 1;

            while (*(u32*)&target_data[slot][3] > (u32)bone)
                slot--;

            slot++;

            // retail's memcpy, which copes with the overlap
            std::memmove(&target_data[slot + 1], &target_data[slot], (count - slot) * sizeof(math::vector4));

            target_data[slot] = source_data[i];
            *(i32*)&target_data[slot][3] = bone;
        }

        count++;
    }
}
