#pragma once

#include <d3d9.h>

#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace ngl { namespace d3d9 {
    // identities unknown rn
    struct internal_program_set {
        IDirect3DVertexShader9* vertex_program_4;
        IDirect3DVertexShader9* vertex_program_2;
        IDirect3DPixelShader9*  pixel_program_2;
        IDirect3DVertexShader9* vertex_program_3;
        IDirect3DVertexShader9* vertex_program_1;
        IDirect3DPixelShader9*  pixel_program_10;
        IDirect3DVertexShader9* vertex_program_6;
        IDirect3DVertexShader9* vertex_program_0;
        IDirect3DPixelShader9*  pixel_program_7;
        IDirect3DPixelShader9*  pixel_program_0;
        IDirect3DPixelShader9*  pixel_program_1;
        IDirect3DPixelShader9*  pixel_program_9;
        IDirect3DVertexShader9* vertex_program_5;
        IDirect3DPixelShader9*  pixel_program_5;
        IDirect3DPixelShader9*  pixel_program_8;
        IDirect3DPixelShader9*  pixel_program_4;
        IDirect3DPixelShader9*  pixel_program_6;
        IDirect3DPixelShader9*  pixel_program_3;
    };

    void initialize_internal_programs();

    namespace references {
        inline ::util::memory_reference<internal_program_set> internal_programs { 0x0111A9E0 };
    } // references

    ASSERT_SIZEOF  (internal_program_set,                    0x48);
    ASSERT_OFFSETOF(internal_program_set, vertex_program_4,  0x00);
    ASSERT_OFFSETOF(internal_program_set, pixel_program_2,   0x08);
    ASSERT_OFFSETOF(internal_program_set, pixel_program_10,  0x14);
    ASSERT_OFFSETOF(internal_program_set, vertex_program_0,  0x1C);
    ASSERT_OFFSETOF(internal_program_set, pixel_program_0,   0x24);
    ASSERT_OFFSETOF(internal_program_set, vertex_program_5,  0x30);
    ASSERT_OFFSETOF(internal_program_set, pixel_program_3,   0x44);
}}} // treyarch::ngl::d3d9
