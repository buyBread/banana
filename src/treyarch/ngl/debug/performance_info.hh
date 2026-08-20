#pragma once

#include "util/types.hh"
#include "util/macros/sanity_assert.hh"

namespace treyarch { namespace ngl {
    struct performance_info {
        u32 list_work_bytes_used;
        u32 max_list_work_bytes_used;
        u32 scratch_mesh_bytes_used;
        u32 max_scratch_mesh_bytes_used;
        u64 cpu_start;
        u64 render_start;
        u64 render_finish;
        u64 list_submit_cycles;
        u64 list_send_cycles;
        f32 frames_per_second;
        f32 total_milliseconds;
        f32 total_seconds;
        f32 render_milliseconds;
        f32 cpu_milliseconds;
        f32 frame_milliseconds;
        f32 list_send_milliseconds;
        f32 list_submit_milliseconds;
        u32 total_polygons;
        u32 node_count;
    };

    ASSERT_SIZEOF  (performance_info,                              0x60);
    ASSERT_OFFSETOF(performance_info, list_work_bytes_used,        0x00);
    ASSERT_OFFSETOF(performance_info, max_list_work_bytes_used,    0x04);
    ASSERT_OFFSETOF(performance_info, scratch_mesh_bytes_used,     0x08);
    ASSERT_OFFSETOF(performance_info, max_scratch_mesh_bytes_used, 0x0C);
    ASSERT_OFFSETOF(performance_info, cpu_start,                   0x10);
    ASSERT_OFFSETOF(performance_info, render_start,                0x18);
    ASSERT_OFFSETOF(performance_info, render_finish,               0x20);
    ASSERT_OFFSETOF(performance_info, list_submit_cycles,          0x28);
    ASSERT_OFFSETOF(performance_info, list_send_cycles,            0x30);
    ASSERT_OFFSETOF(performance_info, frames_per_second,           0x38);
    ASSERT_OFFSETOF(performance_info, total_milliseconds,          0x3C);
    ASSERT_OFFSETOF(performance_info, total_seconds,               0x40);
    ASSERT_OFFSETOF(performance_info, render_milliseconds,         0x44);
    ASSERT_OFFSETOF(performance_info, cpu_milliseconds,            0x48);
    ASSERT_OFFSETOF(performance_info, frame_milliseconds,          0x4C);
    ASSERT_OFFSETOF(performance_info, list_send_milliseconds,      0x50);
    ASSERT_OFFSETOF(performance_info, list_submit_milliseconds,    0x54);
    ASSERT_OFFSETOF(performance_info, total_polygons,              0x58);
    ASSERT_OFFSETOF(performance_info, node_count,                  0x5C);
}} // treyarch::ngl