#include <cstring>

#include "treyarch/nal/bitstream.hh"
#include "treyarch/nal/nal2_decoder_state.hh"
#include "treyarch/nal/transport_list.hh"
#include "treyarch/shared/memory/memory.hh"
#include "treyarch/shared/tl_defs.hh"

using namespace treyarch;

// sub_9D08A0
void nal::nal2_decoder_state::init(nal::anim* animation, u32 state_memory_size) {
    anim        = animation;
    frame_count = animation->frame_count;

    nal::pose_layout* layout = animation->transport_list->pose_layout;

    // the index lists start past the three value buffers' worst case
    auto lists = (u16*)((u8*)this + 0x240 + 12 * animation->total_field_values_encoded);

    *(u32*)((u8*)this + state_memory_size - 4) = 0xDEADBEEF;

    for (u32 field = 0; field < 11; field++) {
        u16 count = layout->field_repeat_counts[field];

        const_list[field]    = lists;
        const_count[field]   = 0;
        varying_count[field] = 0;
        varying_list[field]  = lists + count;
        lists               += 2 * count;

        u32 constants = 0;
        u32 varyings  = 0;

        // constant instances come from the default pose, the others present are decoded
        for (u32 i = 0; i < count; i++) {
            i32 bit      = i + layout->field_bit_offsets[field];
            u32 present  = anim->present_mask.is_one_bool(bit);
            u32 constant = anim->constant_mask.is_one_bool(bit);

            const_count[field]   += constant;
            varying_count[field] += present & ~constant;

            if (constant)
                const_list[field][constants++] = (u16)i;
            else if (present)
                varying_list[field][varyings++] = (u16)i;
        }
    }

    // x1, x0 and the codes follow the state, each 16-aligned
    x1            = (i32*)tl_align((u8*)this + 0x210, 16);
    x0            = (i32*)tl_align((u8*)(x1 + anim->total_field_values_encoded), 16);
    decoder_index = (u16*)tl_align((u8*)(x0 + anim->total_field_values_encoded), 16);

    u32 pose_size = anim->transport_list->pose_layout->pose_size;

    for (i32 i = 0; i < 3; i++) {
        cached_pose_data[i] = (u8*)memory::allocate(pose_size, 16, 0);
        cached_frame[i]     = -1;
    }

    reset();

    initialized = 1;
}

// sub_9CDEE0
void nal::nal2_decoder_state::reset() {
    if (anim->lane_streams) {
        for (i32 k = 0; k < 8; k++) {
            lane_ptr[k] = anim->lane_streams + (anim->lane_starts[k] >> 4);
            lane_bit[k] = anim->lane_starts[k] & 0xF;
        }
    }

    main_ptr = anim->bitstream;
    main_bit = 0;

    bitstream stream = { main_ptr, main_bit };
    u32       count  = anim->total_field_values_encoded;

    // frame 0, then frame 1 as deltas on it, then the code every later frame uses
    for (u16 i = 0; i < count; i++)
        x0[i] = stream.get_value((u16)stream.get(6));

    for (u16 i = 0; i < count; i++) {
        x1[i]  = stream.get_value((u16)stream.get(6));
        x1[i] += x0[i];
    }

    for (u16 i = 0; i < count; i++)
        decoder_index[i] = (u16)stream.get(6);

    main_ptr  = anim->bitstream;
    main_bit  = anim->initial_bit_offset;
    t_for_x_1 = 1;
}

// sub_9CE3E0
void nal::nal2_decoder_state::seek(u32 t) {
    if (t < t_for_x_1 - 1)
        reset();

    u16 takeout_count = anim->takeout_count;
    u32 lane_values   = anim->total_field_values_encoded - takeout_count;
    u32 groups        = lane_values >> 3;

    lane_bitstream lanes[8];

    for (i32 k = 0; k < 8; k++) {
        lanes[k].ptr = lane_ptr[k] + (lane_bit[k] >> 4);
        lanes[k].bit = lane_bit[k] & 0xF;
    }

    while (t_for_x_1 < t) {
        i16 residuals[1024];

        // value i of the lane block comes from lane i % 8
        if (anim->lane_streams) {
            for (u32 group = 0; group < groups; group++) {
                for (i32 k = 0; k < 8; k++)
                    residuals[8 * group + k] = lanes[k].get_value(decoder_index[8 * group + k]);
            }
        } else
            std::memset(residuals, 0, 2 * lane_values);

        // each frame is a residual on 2 x1 - x0
        for (u32 i = 0; i < 8 * groups; i++) {
            i32 value = residuals[i] + 2 * x1[i] - x0[i];

            x0[i] = x1[i];
            x1[i] = value;
        }

        // the leftovers and the takeouts come from the main stream
        bitstream stream = { main_ptr + (main_bit >> 5), main_bit & 0x1F };

        for (u32 i = 8 * groups; i < 8 * groups + takeout_count + (lane_values & 7); i++) {
            i32 value = stream.get_value(decoder_index[i]) + 2 * x1[i] - x0[i];

            x0[i] = x1[i];
            x1[i] = value;
        }

        t_for_x_1++;

        main_ptr = stream.ptr;
        main_bit = stream.bit;
    }

    for (i32 k = 0; k < 8; k++) {
        lane_ptr[k] = lanes[k].ptr;
        lane_bit[k] = lanes[k].bit;
    }
}
