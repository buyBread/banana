#pragma once

#include <d3d9.h>

#include "treyarch/ngl/d3d9/vertex_definition.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"

namespace treyarch {
    // RTTI Blitter
    struct blitter {
        void*                    vtable;
        u32                      reference_count; // one per owning filter, plus the system's
        IDirect3DVertexBuffer9*  vertex_buffer;
        D3DVERTEXELEMENT9        vertex_elements[3];
        ngl::vertex_definition   vertex_format;
        IDirect3DVertexShader9** vertex_program;
        IDirect3DPixelShader9**  texture_program;
        IDirect3DPixelShader9**  solid_color_program;
        IDirect3DPixelShader9**  depth_texture_program;
        IDirect3DPixelShader9**  color_depth_texture_program;

        // the draw sends tap i as vertex constant c[i] = { scale_u[i], scale_v[i], offset_u[i], offset_v[i] };
        // filter setups write texel offsets into the taps, and noise also doubles the scale per tap
        f32 tap_scale_u[16];
        f32 tap_scale_v[16];
        f32 tap_offset_u[16];
        f32 tap_offset_v[16];

        blitter();

        bool initialize();
        void release();
        void draw_fullscreen();
    };

    namespace references {
        inline util::memory_reference<void*> blitter_vtable { 0x00BCA07C };
    } // references

    ASSERT_SIZEOF  (blitter,                              0x144);
    ASSERT_OFFSETOF(blitter, reference_count,             0x04);
    ASSERT_OFFSETOF(blitter, vertex_buffer,               0x08);
    ASSERT_OFFSETOF(blitter, vertex_elements,             0x0C);
    ASSERT_OFFSETOF(blitter, vertex_format,               0x24);
    ASSERT_OFFSETOF(blitter, vertex_program,              0x30);
    ASSERT_OFFSETOF(blitter, texture_program,             0x34);
    ASSERT_OFFSETOF(blitter, color_depth_texture_program, 0x40);
    ASSERT_OFFSETOF(blitter, tap_scale_u,                 0x44);
    ASSERT_OFFSETOF(blitter, tap_scale_v,                 0x84);
    ASSERT_OFFSETOF(blitter, tap_offset_u,                0xC4);
    ASSERT_OFFSETOF(blitter, tap_offset_v,                0x104);
} // treyarch
