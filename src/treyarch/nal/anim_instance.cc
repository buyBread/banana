#include <cstdlib>
#include <cstring>

#include "treyarch/nal/anim_instance.hh"
#include "treyarch/nal/blend.hh"
#include "treyarch/nal/field_interfaces.hh"
#include "treyarch/nal/skeleton_map.hh"
#include "treyarch/nal/transport_list.hh"
#include "treyarch/shared/math/vmath/matrix_diag.hh"
#include "treyarch/shared/math/vmath/vector.hh"
#include "treyarch/shared/memory/memory.hh"
#include "treyarch/shared/tl_defs.hh"

using namespace treyarch;

// sub_9D1480
nal::anim_instance::anim_instance(nal::anim* animation, u32 instance_flags, nal::skeleton_map* rig_map) {
    skeleton_map = rig_map;
    flags        = instance_flags;
    anim         = animation;
    next         = nullptr;

    u32 state_memory_size = (4 * (3 * animation->total_field_values_encoded + animation->transport_list->pose_layout->total_fields) + 0x253) & ~0xF;

    decoder = (nal2_decoder_state*)memory::allocate(state_memory_size, 16, 0);
    decoder->init(animation, state_memory_size);

    last_trajectory_po             = math::identity_mat33();
    last_trajectory_po_initialized = false;
    force_looping                  = false;

    auto end = (instance_observer*)&instance_observers;

    for (auto observer = (instance_observer*)instance_observers.next; observer != end; observer = (instance_observer*)observer->next)
        observer->on_create(this);
}

// sub_9CD240
nal::anim_instance::~anim_instance() {
    auto end = (instance_observer*)&instance_observers;

    for (auto observer = (instance_observer*)instance_observers.next; observer != end; observer = (instance_observer*)observer->next)
        observer->on_destroy(this);

    if (!decoder)
        return;

    for (i32 i = 0; i < 3; i++)
        memory::free(decoder->cached_pose_data[i]);

    memory::free(decoder);

    decoder = nullptr;
}

// sub_9CF290
void nal::anim_instance::get_pose(i32 frame, nal::pose* pose) {
    u8 stack_memory[0x2000];

    nal::pose temp(anim->transport_list->pose_layout, tl_align(stack_memory, 16));

    nal::pose_layout* layout = anim->transport_list->pose_layout;
    nal::pose*        target = pose;
    bool              remap  = false;

    // a pose of another rig is decoded into the temp pose and remapped at the end
    if (layout->name.hash != pose->pose_layout->name.hash) {
        target = &temp;
        remap  = true;
    }

    i32 slot = 0;

    while (slot < 3 && decoder->cached_frame[slot] != frame)
        slot++;

    if (slot < 3) {
        std::memcpy(target->pose_data, decoder->cached_pose_data[slot], anim->transport_list->pose_layout->pose_size);

        target->present     = decoder->cached_present[slot];
        target->pose_layout = decoder->cached_layout[slot];
    } else {
        i32 q_pose[max_pose_tracks];
        i32 lanes[4];

        get_q_pose(frame, q_pose);

        target->present = decoder->anim->present_mask;

        u8*  data  = target->pose_data;
        i32* value = q_pose;

        for (u32 field = 0; field < 11; field++) {
            // constant instances come from the transport list's default pose
            for (u32 i = 0; i < decoder->const_count[field]; i++) {
                u16  index   = decoder->const_list[field][i];
                f32* written = target->get_field_data_ptr(field, index);
                f32* source  = layout->default_pose.get_field_data_ptr(field, index);

                for (u32 k = 0; k < values_per_field[field]; k++)
                    written[k] = source[k];
            }

            f32* scales  = anim->transport_list->pose_layout->dequant_scale[field];
            u16  encoded = encoded_values_per_field[field];

            // the first five fields with 3 or 4 values write all four lanes, a fourth one left over from before
            if ((u32)(encoded - 3) > 1 || field >= 5) {
                for (u32 i = 0; i < decoder->varying_count[field]; i++) {
                    u16  index   = decoder->varying_list[field][i];
                    f32  scale   = scales[index];
                    f32* written = target->get_field_data_ptr(field, index);

                    for (u32 k = 0; k < encoded; k++)
                        written[k] = (f32)*value++ * scale;
                }
            } else {
                for (u32 i = 0; i < decoder->varying_count[field]; i++) {
                    u16  index   = decoder->varying_list[field][i];
                    f32  scale   = scales[index];
                    auto written = (math::vector4*)target->get_field_data_ptr(field, index);

                    if (encoded)
                        std::memcpy(lanes, value, 4 * encoded);

                    *written = math::vector4((f32)lanes[0] * scale,
                                             (f32)lanes[1] * scale,
                                             (f32)lanes[2] * scale,
                                             (f32)lanes[3] * scale);

                    value += encoded;
                }
            }
        }

        for (u32 i = 0; i < anim->rescale_count; i++)
            *(f32*)(data + anim->rescales[i].offset) *= anim->rescales[i].scale;

        for (u32 field = 0; field < 11; field++)
            field_interfaces[field]->post_decode_derive(target, anim, decoder->varying_count[field], decoder->varying_list[field]);

        // the first slot that's empty or holds a frame outside the two before to the one after; slot 0 otherwise
        slot = 0;

        for (i32 i = 0; i < 3; i++) {
            i32 cached = decoder->cached_frame[i];

            if (cached == -1) {
                slot = i;

                break;
            }

            i32 distance;

            if (frame > cached) {
                distance = frame - cached;
            } else {
                i32 wrapped = frame + (i32)anim->frame_count - cached;

                distance = std::abs(wrapped) >= cached - frame ? frame - cached : wrapped;
            }

            if ((u32)(distance + 1) > 3) {
                slot = i;

                break;
            }
        }

        decoder->cached_frame[slot] = frame;

        std::memcpy(decoder->cached_pose_data[slot], target->pose_data, anim->transport_list->pose_layout->pose_size);

        decoder->cached_present[slot] = target->present;
        decoder->cached_layout[slot]  = target->pose_layout;
    }

    if (remap)
        remap_pose(target, pose);
}

// sub_9D1860
void nal::anim_instance::get_pose(f32 t, f32 t_prev, nal::pose* pose, nal::transport_fk* fk) {
    nal::pose_layout* anim_layout = anim->transport_list->pose_layout;
    nal::pose_layout* pose_layout = pose->pose_layout;
    bool              looping     = force_looping || (anim->flags & anim_flag_looping);

    f32 frame;
    i32 frame_0;
    i32 frame_1;
    f32 fraction;
    i32 wraps;

    time_to_frames(anim, t_prev, t, &frame, &frame_0, &frame_1, &fraction, &wraps, looping);

    u8 stack_memory_a[0x2000];
    u8 stack_memory_b[0x2000];

    nal::pose pose_a(pose_layout, tl_align(stack_memory_a, 16));
    nal::pose pose_b(pose_layout, tl_align(stack_memory_b, 16));

    bool first_only  = fraction < 0.01f;
    bool second_only = fraction > 0.99f;

    if (first_only)
        get_pose(frame_0, pose);
    else if (second_only)
        get_pose(frame_1, pose);
    else {
        get_pose(frame_0, &pose_a);
        get_pose(frame_1, &pose_b);
    }

    nal::transport_list* transports = anim->transport_list;

    auto skeleton_fk = (nal::transport_fk*)(transports->has_transport(0) ? transports->get_transport(0) : nullptr);
    auto fk_data     = (nal::transport_data_fk*)(anim->has_transport_data(0) ? anim->get_transport_data(0) : nullptr);

    u32 position_instance   = (u32)-1;
    u32 quaternion_instance = (u32)-1;
    i32 trajectory_bone     = -1;

    // the anim has to carry the skeleton's trajectory instances
    if (skeleton_fk && fk) {
        trajectory_bone     = fk->trajectory_bone;
        quaternion_instance = fk->trajectory_quaternion_instance;
        position_instance   = fk->trajectory_position_instance;

        if (!anim->present_mask.is_one(skeleton_fk->trajectory_position_instance + anim_layout->field_bit_offsets[pose_field_fk_position]) ||
            !anim->present_mask.is_one(skeleton_fk->trajectory_quaternion_instance + anim_layout->field_bit_offsets[pose_field_fk_quaternion]))

            trajectory_bone = -1;
    }

    // across the loop seam the later frame's trajectory continues from the anim's end
    if (frame_1 < frame_0) {
        nal::pose* later = second_only ? pose : &pose_b;

        if (trajectory_bone != -1) {
            auto rotation = (math::quaternion*)later->get_field_data_ptr(pose_field_fk_quaternion, quaternion_instance);
            auto position = (math::vector4*)later->get_field_data_ptr(pose_field_fk_position, position_instance);

            po sample;

            *(math::quaternion*)&sample.get_m33().get_quaternion() = *rotation;
            *(math::vector4*)&sample.get_w()                       = *position;

            po continued = po_multiply(fk_data->trajectory_po_at_anim_end, sample);

            const math::vector4 &translation = *(const math::vector4*)&continued.get_w();

            *rotation = *(const math::quaternion*)&continued.get_m33().get_quaternion();
            *position = math::vector4(translation[0], translation[1], translation[2], 1.0f);
        }
    }

    if (first_only || second_only)
        single_frame_poses++;
    else {
        blended_poses++;

        nal::blend(pose, fraction, &pose_a, &pose_b);
    }

    if (trajectory_bone != -1) {
        auto rotation   = (math::quaternion*)pose->get_field_data_ptr(pose_field_fk_quaternion, quaternion_instance);
        auto position   = (math::vector4*)pose->get_field_data_ptr(pose_field_fk_position, position_instance);
        auto trajectory = (po*)pose->get_field_data_ptr(pose_field_fk_trajectory, 0);

        pose->present.set_one(pose_layout->field_bit_offsets[pose_field_fk_trajectory]);

        po current;

        *(math::quaternion*)&current.get_m33().get_quaternion() = *rotation;
        *(math::vector4*)&current.get_w()                       = *position;

        // the field holds the change since the last sample
        if (!last_trajectory_po_initialized) {
            if (t_prev <= 0.0001f)
                *trajectory = current;
            else
                *trajectory = *(const po*)&identity_po;
        } else if (wraps <= 0)
            *trajectory = po_multiply(current, po_inverse(last_trajectory_po));
        else {
            po to_end = po_multiply(fk_data->trajectory_po_at_anim_end, po_inverse(last_trajectory_po));

            last_trajectory_po = math::identity_mat33();

            *trajectory = po_multiply(current, to_end);
        }

        last_trajectory_po             = current;
        last_trajectory_po_initialized = true;
    }

    if (flags & instance_flag_mirror)
        mirror_pose(this, pose);
}

// sub_9CF1C0
void nal::anim_instance::get_q_pose(u32 t, i32* q_pose) {
    decoder->seek(t);

    i32* x0    = decoder->x0;
    i32* x1    = decoder->x1;
    u32  count = decoder->anim->total_field_values_encoded;

    // the frame before t_for_x_1 is still in x0
    i32* values = t == decoder->t_for_x_1 - 1 ? x0 : x1;

    for (u32 i = 0; i < count; i++)
        q_pose[i] = values[i];

    unshuffle_q_pose(q_pose);
}

// sub_9CD380
void nal::anim_instance::unshuffle_q_pose(i32* q_pose) {
    u16  takeout_count = anim->takeout_count;
    u32  lane_values   = anim->total_field_values_encoded - takeout_count;
    u16* tracks        = anim->takeout_tracks;
    u32  groups        = lane_values >> 3;

    i32 unshuffled[max_pose_tracks];
    i32 count = 0;

    // the lane block goes back lane by lane
    for (i32 lane = 0; lane < 8; lane++) {
        for (u32 group = 0; group < groups; group++)
            unshuffled[count++] = q_pose[lane + 8 * group];
    }

    // then the main stream's leftovers and takeouts, as they were decoded
    u32 begin = 8 * groups;
    u32 end   = takeout_count + 8 * groups + (lane_values & 7);

    if (begin < end)
        std::memcpy(&unshuffled[count], &q_pose[begin], 4 * (end - begin));

    i32  total  = takeout_count + lane_values;
    i32  last   = total - takeout_count - 1;
    i32* source = &q_pose[total - 1];

    // from the last takeout back, each goes in at its track and the values behind it move up
    for (i32 i = takeout_count - 1; i >= 0; i--) {
        i32 track = tracks[i];
        i32 value = *source--;

        while (last >= track) {
            unshuffled[last + i + 1] = unshuffled[last];
            last--;
        }

        last             = tracks[i];
        unshuffled[last] = value;
    }

    if (total)
        std::memcpy(q_pose, unshuffled, 4 * total);
}

// sub_9CF240
void nal::anim_instance::remap_pose(nal::pose* source, nal::pose* target) {
    skeleton_map_entry* entry = skeleton_map->get_entry(&source->pose_layout->name);

    for (u32 field = 0; field < 11; field++)
        field_interfaces[field]->map(field, target, source, entry->field_maps[field], entry->bone_map);
}

// sub_9CECB0
void nal::mirror_pose(nal::anim_instance* instance, nal::pose* pose) {
    nal::anim*           anim       = instance->anim;
    nal::transport_list* transports = anim->transport_list;

    auto fk      = (nal::transport_fk*)(transports->has_transport(0) ? transports->get_transport(0) : nullptr);
    auto fk_data = anim->has_transport_data(0) ? anim->get_transport_data(0) : nullptr;

    // looked up and never used
    (void)fk_data;

    nal::pose_layout* layout       = transports->pose_layout;
    u16*              mirror_flags = fk->mirror_flags;
    u16*              mirror_bones = fk->mirror_bones;

    // a pair swaps with x and z negated; a bone mirrored onto itself negates them in place, the root swaps its lanes
    for (u32 i = 0; i < layout->field_repeat_counts[pose_field_fk_quaternion]; i++) {
        u16 bone   = fk->quaternion_bones[i];
        u16 mirror = mirror_bones[bone];

        if (mirror == 0xFFFF)
            continue;

        i32 other = fk->quaternion_instances[mirror];

        if (other == -1)
            continue;

        auto rotation = (math::quaternion*)pose->get_field_data_ptr(pose_field_fk_quaternion, i);

        if (mirror != bone) {
            math::quaternion own     = *rotation;
            auto             partner = (math::quaternion*)pose->get_field_data_ptr(pose_field_fk_quaternion, other);

            *rotation = math::quaternion(-(*partner)[0], (*partner)[1], -(*partner)[2], (*partner)[3]);
            *partner  = math::quaternion(-own[0], own[1], -own[2], own[3]);
        } else if (bone == fk->root_bone)
            *rotation = math::quaternion((*rotation)[1], (*rotation)[0], (*rotation)[3], (*rotation)[2]);
        else if (!mirror_flags[bone]) {
            (*rotation)[0] = -(*rotation)[0];
            (*rotation)[2] = -(*rotation)[2];
        }
    }

    // pairs swap angles, a bone mirrored onto itself negates its own
    for (u32 i = 0; i < layout->field_repeat_counts[pose_field_fk_axis1_rotation]; i++) {
        u16 bone   = fk->axis1_bones[i];
        u16 mirror = mirror_bones[bone];

        if (mirror == 0xFFFF)
            continue;

        i32 other = fk->axis1_instances[mirror];

        if (other == -1)
            continue;

        f32* angle = pose->get_field_data_ptr(pose_field_fk_axis1_rotation, i);

        if (mirror == bone)
            *angle = -*angle;
        else {
            f32  own     = *angle;
            f32* partner = pose->get_field_data_ptr(pose_field_fk_axis1_rotation, other);

            *angle   = *partner;
            *partner = own;
        }
    }

    // a pair swaps with x negated; a bone mirrored onto itself negates x for the root and trajectory bones, y otherwise
    for (u32 i = 0; i < layout->field_repeat_counts[pose_field_fk_position]; i++) {
        u16 bone   = fk->position_bones[i];
        u16 mirror = mirror_bones[bone];

        if (mirror == 0xFFFF)
            continue;

        i32 other = fk->position_instances[mirror];

        if (other == -1)
            continue;

        auto position = (math::vector4*)pose->get_field_data_ptr(pose_field_fk_position, i);

        if (mirror != bone) {
            auto          partner = (math::vector4*)pose->get_field_data_ptr(pose_field_fk_position, other);
            math::vector4 own     = *position;
            math::vector4 theirs  = *partner;

            *position = math::vector4(-theirs[0], theirs[1], theirs[2], theirs[3]);
            *partner  = math::vector4(-own[0], own[1], own[2], own[3]);
        } else if (bone == fk->root_bone || bone == fk->trajectory_bone)
            (*position)[0] = -(*position)[0];
        else
            (*position)[1] = -(*position)[1];
    }
}
