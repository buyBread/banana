#pragma once

#include "game/frontend/igo/igo_3d_widget.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_letterbox : public igo_3d_widget {

    public:
        f32 alpha;          // the bars' colour alpha
        f32 cur_height;     // each bar's height on a 640 x 480 screen
        f32 total_time;     // the update runs while it's above 0
        f32 elapsed;
        f32 desired_height;
        u8  reserved_164[0x0C];

        void letterbox(f32 time, f32 height); // a time under 0.0001 jumps straight to the height
    };

    ASSERT_SIZEOF  (igo_3d_letterbox,                 0x170);
    ASSERT_OFFSETOF(igo_3d_letterbox, alpha,          0x150);
    ASSERT_OFFSETOF(igo_3d_letterbox, total_time,     0x158);
    ASSERT_OFFSETOF(igo_3d_letterbox, desired_height, 0x160);
} // treyarch
