#pragma once

#include "treyarch/nal/anim.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace nal {
    // nal2DecoderState, one allocation per instance; the value buffers and index lists follow it
    struct nal2_decoder_state {
        u16        lane_bit[8];  // right after reset, the lane start's (word << 4) | bit
        u16*       lane_ptr[8];
        u32        t_for_x_1;    // the frame held in x1
        u32        frame_count;
        u32        initialized;
        nal::anim* anim;
        u32*       main_ptr;
        u32        main_bit;
        u32        const_count[11];
        u32        varying_count[11];
        u16*       const_list[11];
        u16*       varying_list[11];
        u8         unk_f8[0x48];
        u16*       decoder_index;  // the 6-bit code of every encoded value
        i32*       x0;             // the values at t_for_x_1 - 1
        i32*       x1;
        i32        cached_frame[3];
        u8*        cached_pose_data[3];

        tl_fixed_bitvector<u32, 384> cached_present[3];

        nal::pose_layout* cached_layout[3];

        void init(nal::anim* animation, u32 state_memory_size);
        void reset();
        void seek(u32 t);
    };

    ASSERT_OFFSETOF(nal2_decoder_state, lane_bit,         0x000);
    ASSERT_OFFSETOF(nal2_decoder_state, lane_ptr,         0x010);
    ASSERT_OFFSETOF(nal2_decoder_state, t_for_x_1,        0x030);
    ASSERT_OFFSETOF(nal2_decoder_state, frame_count,      0x034);
    ASSERT_OFFSETOF(nal2_decoder_state, initialized,      0x038);
    ASSERT_OFFSETOF(nal2_decoder_state, anim,             0x03C);
    ASSERT_OFFSETOF(nal2_decoder_state, main_ptr,         0x040);
    ASSERT_OFFSETOF(nal2_decoder_state, main_bit,         0x044);
    ASSERT_OFFSETOF(nal2_decoder_state, const_count,      0x048);
    ASSERT_OFFSETOF(nal2_decoder_state, varying_count,    0x074);
    ASSERT_OFFSETOF(nal2_decoder_state, const_list,       0x0A0);
    ASSERT_OFFSETOF(nal2_decoder_state, varying_list,     0x0CC);
    ASSERT_OFFSETOF(nal2_decoder_state, decoder_index,    0x140);
    ASSERT_OFFSETOF(nal2_decoder_state, x0,               0x144);
    ASSERT_OFFSETOF(nal2_decoder_state, x1,               0x148);
    ASSERT_OFFSETOF(nal2_decoder_state, cached_frame,     0x14C);
    ASSERT_OFFSETOF(nal2_decoder_state, cached_pose_data, 0x158);
    ASSERT_OFFSETOF(nal2_decoder_state, cached_present,   0x164);
    ASSERT_OFFSETOF(nal2_decoder_state, cached_layout,    0x200);
}} // treyarch::nal
