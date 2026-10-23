#include <cstdlib>
#include <cstring>

#include "treyarch/nal/anim_instance.hh"
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
