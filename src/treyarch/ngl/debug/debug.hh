#pragma once

#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace ngl {
    struct debug_platform_state {
        u8  render_single_node;
        u8  pad_001[0x03];
        u32 platform_value_004;
        u32 platform_value_008;
        u32 platform_value_00c;
    };

    ASSERT_SIZEOF  (debug_platform_state,                     0x10);
    ASSERT_OFFSETOF(debug_platform_state, render_single_node, 0x00);
    ASSERT_OFFSETOF(debug_platform_state, platform_value_004, 0x04);
    ASSERT_OFFSETOF(debug_platform_state, platform_value_008, 0x08);
    ASSERT_OFFSETOF(debug_platform_state, platform_value_00c, 0x0C);

    struct debug_state {
        debug_platform_state platform;
        u8                   show_performance_info;
        u8                   show_performance_bar;
        u8                   profile_shaders;
        u8                   dump_scene_file;
        u8                   screenshot;
        u8                   pad_015[0x03];
        u32                  test_node_start;
        u32                  test_node_end;
        u32                  current_node;
        u8                   disable_quads;
        u8                   disable_fonts;
        u8                   disable_meshes;
        u8                   disable_vsync;
        u8                   debug_prints;
        u8                   dump_frame_log;
        u8                   disable_duplicate_material_warning;
        u8                   disable_missing_texture_warning;
    };

    ASSERT_SIZEOF  (debug_state,                                     0x2C);
    ASSERT_OFFSETOF(debug_state, platform,                           0x00);
    ASSERT_OFFSETOF(debug_state, show_performance_info,              0x10);
    ASSERT_OFFSETOF(debug_state, show_performance_bar,               0x11);
    ASSERT_OFFSETOF(debug_state, profile_shaders,                    0x12);
    ASSERT_OFFSETOF(debug_state, dump_scene_file,                    0x13);
    ASSERT_OFFSETOF(debug_state, screenshot,                         0x14);
    ASSERT_OFFSETOF(debug_state, test_node_start,                    0x18);
    ASSERT_OFFSETOF(debug_state, test_node_end,                      0x1C);
    ASSERT_OFFSETOF(debug_state, current_node,                       0x20);
    ASSERT_OFFSETOF(debug_state, disable_quads,                      0x24);
    ASSERT_OFFSETOF(debug_state, disable_fonts,                      0x25);
    ASSERT_OFFSETOF(debug_state, disable_meshes,                     0x26);
    ASSERT_OFFSETOF(debug_state, disable_vsync,                      0x27);
    ASSERT_OFFSETOF(debug_state, debug_prints,                       0x28);
    ASSERT_OFFSETOF(debug_state, dump_frame_log,                     0x29);
    ASSERT_OFFSETOF(debug_state, disable_duplicate_material_warning, 0x2A);
    ASSERT_OFFSETOF(debug_state, disable_missing_texture_warning,    0x2B);

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

    struct debug_primitive_batch_state {
        u32 batch_counter_0;
        u32 batch_counter_1;
        u32 batch_counter_2;
        u32 allocation_bytes;
        u32 batch_counter_3;
        u32 batch_counter_4;
        u8  allocation_overflowed;
        u8  synchronized_allocation_overflowed;
        u8  pad_01a[0x02];
        u32 peak_allocation_bytes;
        f32 synchronized_peak_utilization;
        u32 synchronized_frame_epoch;
    };

    ASSERT_SIZEOF  (debug_primitive_batch_state,                                      0x28);
    ASSERT_OFFSETOF(debug_primitive_batch_state, batch_counter_0,                     0x00);
    ASSERT_OFFSETOF(debug_primitive_batch_state, batch_counter_1,                     0x04);
    ASSERT_OFFSETOF(debug_primitive_batch_state, batch_counter_2,                     0x08);
    ASSERT_OFFSETOF(debug_primitive_batch_state, allocation_bytes,                    0x0C);
    ASSERT_OFFSETOF(debug_primitive_batch_state, batch_counter_3,                     0x10);
    ASSERT_OFFSETOF(debug_primitive_batch_state, batch_counter_4,                     0x14);
    ASSERT_OFFSETOF(debug_primitive_batch_state, allocation_overflowed,               0x18);
    ASSERT_OFFSETOF(debug_primitive_batch_state, synchronized_allocation_overflowed,  0x19);
    ASSERT_OFFSETOF(debug_primitive_batch_state, peak_allocation_bytes,               0x1C);
    ASSERT_OFFSETOF(debug_primitive_batch_state, synchronized_peak_utilization,       0x20);
    ASSERT_OFFSETOF(debug_primitive_batch_state, synchronized_frame_epoch,            0x24);

    namespace debug {
        void init();
        void render();
        void render_performance_info();
        void render_performance_bar();
        void reset_primitive_batches();
        void screen_shot(const char* name = nullptr);
    } // debug

    namespace references {
        inline util::memory_reference<debug_state>      debug          { 0x01123810 };
        inline util::memory_reference<debug_state>      sync_debug     { 0x0112383C };
        inline util::memory_reference<performance_info> perf_info      { 0x01123868 };
        inline util::memory_reference<performance_info> sync_perf_info { 0x011238C8 };

        inline util::memory_reference<debug_primitive_batch_state> debug_primitive_batches      { 0x01123928 };
        inline util::memory_reference<i32>                         performance_bar_vblank_count { 0x00F53734 };
    } // references
}} // treyarch::ngl
