#pragma once

#include <d3d9.h>

#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"

/*
    temporary file until tlInitList and program framework is in
*/

namespace treyarch { namespace ngl { namespace d3d9 {
    struct internal_shader_set {
        IDirect3DVertexShader9* vertex_shader_4;
        IDirect3DVertexShader9* vertex_shader_2;
        IDirect3DPixelShader9*  pixel_shader_2;
        IDirect3DVertexShader9* vertex_shader_3;
        IDirect3DVertexShader9* vertex_shader_1;
        IDirect3DPixelShader9*  pixel_shader_10;
        IDirect3DVertexShader9* vertex_shader_6;
        IDirect3DVertexShader9* vertex_shader_0;
        IDirect3DPixelShader9*  pixel_shader_7;
        IDirect3DPixelShader9*  pixel_shader_0;
        IDirect3DPixelShader9*  pixel_shader_1;
        IDirect3DPixelShader9*  pixel_shader_9;
        IDirect3DVertexShader9* vertex_shader_5;
        IDirect3DPixelShader9*  pixel_shader_5;
        IDirect3DPixelShader9*  pixel_shader_8;
        IDirect3DPixelShader9*  pixel_shader_4;
        IDirect3DPixelShader9*  pixel_shader_6;
        IDirect3DPixelShader9*  pixel_shader_3;
    };

    void initialize_internal_shaders();

    namespace references {
        inline ::util::memory_reference<internal_shader_set> internal_shaders { 0x0111A9E0 };
    } // references

    ASSERT_SIZEOF  (internal_shader_set,                   0x48);
    ASSERT_OFFSETOF(internal_shader_set, vertex_shader_4,  0x00);
    ASSERT_OFFSETOF(internal_shader_set, pixel_shader_2,   0x08);
    ASSERT_OFFSETOF(internal_shader_set, pixel_shader_10,  0x14);
    ASSERT_OFFSETOF(internal_shader_set, vertex_shader_0,  0x1C);
    ASSERT_OFFSETOF(internal_shader_set, pixel_shader_0,   0x24);
    ASSERT_OFFSETOF(internal_shader_set, vertex_shader_5,  0x30);
    ASSERT_OFFSETOF(internal_shader_set, pixel_shader_3,   0x44);
}}} // treyarch::ngl::d3d9
