#pragma once

#include <d3d9.h>

#include "treyarch/ngl/d3d9/vertex_definition.hh"
#include "treyarch/shared/math/types/matrix4x4.hh"
#include "util/macros/sanity_assert.hh"

namespace treyarch { namespace ngl { namespace d3d9 {
    struct blitter {
        void*                     vtable;
        u32                       active;
        IDirect3DVertexBuffer9*   vertex_buffer;
        u8                        reserved_00c[0x18];
        vertex_definition         vertex_format;
        IDirect3DVertexShader9**  vertex_program;
        u8                        reserved_034[0x10];
        matrix4x4                 transforms[4];

        void draw_fullscreen();
    };

    ASSERT_OFFSETOF(blitter, vertex_buffer,  0x08);
    ASSERT_OFFSETOF(blitter, vertex_format,  0x24);
    ASSERT_OFFSETOF(blitter, vertex_program, 0x30);
    ASSERT_OFFSETOF(blitter, transforms,     0x44);
}}} // treyarch::ngl::d3d9
