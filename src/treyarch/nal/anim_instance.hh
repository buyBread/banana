#pragma once

#include "treyarch/nal/anim.hh"
#include "treyarch/nal/nal2_decoder_state.hh"
#include "treyarch/nal/pose.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace nal {
    struct skeleton_map;

#pragma pack(push, 1)
    // nalAnimInstance
    struct anim_instance {
        nal::po                  last_trajectory_po;
        bool                     last_trajectory_po_initialized;
        u32                      flags;
        nal::skeleton_map*       skeleton_map;  // may be null
        nal::anim*               anim;
        nal::nal2_decoder_state* decoder;

        void get_q_pose(u32 t, i32* q_pose);
        void unshuffle_q_pose(i32* q_pose);
    };
#pragma pack(pop)

    ASSERT_OFFSETOF(anim_instance, last_trajectory_po,             0x00);
    ASSERT_OFFSETOF(anim_instance, last_trajectory_po_initialized, 0x20);
    ASSERT_OFFSETOF(anim_instance, flags,                          0x21);
    ASSERT_OFFSETOF(anim_instance, skeleton_map,                   0x25);
    ASSERT_OFFSETOF(anim_instance, anim,                           0x29);
    ASSERT_OFFSETOF(anim_instance, decoder,                        0x2D);
}} // treyarch::nal
