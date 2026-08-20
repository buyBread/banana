#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"
#include "treyarch/ngl/debug/performance_info.hh"

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

    namespace debug {
        void init();
    } // debug

    namespace references {
        inline ::util::memory_reference<debug_state>      debug                    { 0x01123810 };
        inline ::util::memory_reference<debug_state>      synchronized_debug       { 0x0112383C };
        inline ::util::memory_reference<performance_info> performance              { 0x01123868 };
        inline ::util::memory_reference<performance_info> synchronized_performance { 0x011238C8 };
    } // references
}} // treyarch::ngl
