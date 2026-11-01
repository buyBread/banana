#pragma once

#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class wds_time_manager {

    public:
        // the five factors multiplied by get_time_dilation_factor
        f32   unk_000;
        f32   global_time_dilation;   // chuck get/set_global_time_dilation
        f32   reflexes_time_dilation; // the one factor ignore_reflex_dilation skips (time_interface::calc_time_dilation)
        f32   unk_00c;
        u8    reserved_010[0x0C];
        u32   frame_sequence;
        void* unk_020;                // 0xA0 bytes, deleted through its first slot
        f32   unk_024;
        f32   time_inc;               // this frame's undilated step, from frame_advance
        u8    reserved_02c[0x04];
        u8    unk_030[0x0C];

        wds_time_manager();
       ~wds_time_manager();

        /* inlined (frame_advance sub_968530, world_dynamics_system::frame_advance, time_interface::calc_time_dilation).
           SM3 multiplies global and velocity dilation, then reflexes unless ignored, and floors at 0.000001 */
        f32 get_time_dilation_factor(bool ignore_reflex_dilation = false) const {
            f32 factor = (f32)((f64)unk_00c * (f64)global_time_dilation * (f64)unk_024 * (f64)unk_000);

            if (!ignore_reflex_dilation)
                factor = (f32)((f64)factor * (f64)reflexes_time_dilation);

            return factor >= 0.01f ? factor : 0.01f;
        }
    };

    ASSERT_SIZEOF  (wds_time_manager,                         0x3C);
    ASSERT_OFFSETOF(wds_time_manager, global_time_dilation,   0x04);
    ASSERT_OFFSETOF(wds_time_manager, reflexes_time_dilation, 0x08);
    ASSERT_OFFSETOF(wds_time_manager, unk_00c,                0x0C);
    ASSERT_OFFSETOF(wds_time_manager, frame_sequence,         0x1C);
    ASSERT_OFFSETOF(wds_time_manager, unk_020,                0x20);
    ASSERT_OFFSETOF(wds_time_manager, unk_024,                0x24);
    ASSERT_OFFSETOF(wds_time_manager, time_inc,               0x28);
    ASSERT_OFFSETOF(wds_time_manager, unk_030,                0x30);
} // treyarch
