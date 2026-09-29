#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    struct igo_startup_projection {
        f32 field_of_view;
        f32 ortho_width;
        f32 ortho_height;
        f32 near_plane;
        u8  orthographic;
        u8  reserved_011[0x03];
        f32 far_plane;
    };

    struct igo_draw_projection {
        f32 ortho_width;
        f32 ortho_height;
        f32 near_plane;
        f32 far_plane;
        u8  orthographic;
    };

    struct igo_view_fov_cache {
        f32 field_of_view;
        u32 state;
    };

    namespace references {
        inline util::memory_reference<igo_startup_projection> startup_projection { 0x00E73BA8 };
        inline util::memory_reference<igo_draw_projection>    draw_projection    { 0x00E73CD8 };
        inline util::memory_reference<igo_view_fov_cache>     view_fov_cache     { 0x0102F924 };
    } // references

    ASSERT_SIZEOF  (igo_startup_projection,            0x18);
    ASSERT_OFFSETOF(igo_startup_projection, far_plane, 0x14);

    ASSERT_SIZEOF  (igo_draw_projection,               0x14);
    ASSERT_OFFSETOF(igo_draw_projection, orthographic, 0x10);

    ASSERT_SIZEOF(igo_view_fov_cache, 0x08);
} // treyarch
