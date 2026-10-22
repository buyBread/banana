#include <cstring>

#include "treyarch/nal/anim_instance.hh"

using namespace treyarch;

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

    i32 unshuffled[1024];
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
