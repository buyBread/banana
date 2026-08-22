#pragma once

#include <d3d9.h>

#include "treyarch/ngl/texture/texture.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl { namespace d3d9 {
    struct framebuffer_state {
        D3DFORMAT depth_texture_format;
        texture*  front_buffer;
        texture*  linear_depth_buffer;
        texture*  back_buffer;
        texture*  secondary_hdr_buffer;
        texture*  secondary_ldr_buffer;
        texture*  active_depth_buffer;
        u8        depth_texture_supported;
        u8        pad_01d[3];
        texture*  active_color_buffer;
    };

    ULONG initialize_framebuffers();

    namespace references {
        inline util::memory_reference<u8>                particle_depth_texture_requested { 0x00F4CD42 };
        inline util::memory_reference<framebuffer_state> framebuffers                     { 0x01123A10 };
    } // references

    ASSERT_SIZEOF  (framebuffer_state,                         0x24);
    ASSERT_OFFSETOF(framebuffer_state, depth_texture_format,    0x00);
    ASSERT_OFFSETOF(framebuffer_state, front_buffer,            0x04);
    ASSERT_OFFSETOF(framebuffer_state, linear_depth_buffer,     0x08);
    ASSERT_OFFSETOF(framebuffer_state, back_buffer,             0x0C);
    ASSERT_OFFSETOF(framebuffer_state, secondary_hdr_buffer,    0x10);
    ASSERT_OFFSETOF(framebuffer_state, secondary_ldr_buffer,    0x14);
    ASSERT_OFFSETOF(framebuffer_state, active_depth_buffer,     0x18);
    ASSERT_OFFSETOF(framebuffer_state, depth_texture_supported, 0x1C);
    ASSERT_OFFSETOF(framebuffer_state, active_color_buffer,     0x20);
}}} // treyarch::ngl::d3d9
