#pragma once

#include <d3d9.h>

#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace ngl { namespace d3d9 {
    struct internal_program_set {
        IDirect3DVertexShader9* screen_pcuv4_vertex_program;
        IDirect3DVertexShader9* framebuffer_copy_vertex_program;
        IDirect3DPixelShader9*  framebuffer_copy_pixel_program;
        IDirect3DVertexShader9* screen_pcuv_vertex_program;
        IDirect3DVertexShader9* screen_pc_vertex_program;
        IDirect3DPixelShader9*  fog_pixel_program;
        IDirect3DVertexShader9* font_vertex_program;
        IDirect3DVertexShader9* quad_pc_vertex_program;
        IDirect3DPixelShader9*  white_pixel_program;
        IDirect3DPixelShader9*  quad_pc_pixel_program;
        IDirect3DPixelShader9*  texture_modulate_pixel_program;
        IDirect3DPixelShader9*  four_texture_average_pixel_program;
        IDirect3DVertexShader9* screen_puv_material_vertex_program;
        IDirect3DPixelShader9*  movie_yuv_pixel_program;
        IDirect3DPixelShader9*  restore_depth_pixel_program;
        IDirect3DPixelShader9*  rawz_depth_copy_pixel_program;
        IDirect3DPixelShader9*  screen_pc_pixel_program;
        IDirect3DPixelShader9*  depth_copy_pixel_program;
    };

    void initialize_internal_programs();

    namespace references {
        inline util::memory_reference<internal_program_set> internal_programs { 0x0111A9E0 };
    } // references

    ASSERT_SIZEOF  (internal_program_set,                                     0x48);
    ASSERT_OFFSETOF(internal_program_set, screen_pcuv4_vertex_program,        0x00);
    ASSERT_OFFSETOF(internal_program_set, framebuffer_copy_vertex_program,    0x04);
    ASSERT_OFFSETOF(internal_program_set, framebuffer_copy_pixel_program,     0x08);
    ASSERT_OFFSETOF(internal_program_set, screen_pcuv_vertex_program,         0x0C);
    ASSERT_OFFSETOF(internal_program_set, screen_pc_vertex_program,           0x10);
    ASSERT_OFFSETOF(internal_program_set, fog_pixel_program,                  0x14);
    ASSERT_OFFSETOF(internal_program_set, font_vertex_program,                0x18);
    ASSERT_OFFSETOF(internal_program_set, quad_pc_vertex_program,             0x1C);
    ASSERT_OFFSETOF(internal_program_set, white_pixel_program,                0x20);
    ASSERT_OFFSETOF(internal_program_set, quad_pc_pixel_program,              0x24);
    ASSERT_OFFSETOF(internal_program_set, texture_modulate_pixel_program,     0x28);
    ASSERT_OFFSETOF(internal_program_set, four_texture_average_pixel_program, 0x2C);
    ASSERT_OFFSETOF(internal_program_set, screen_puv_material_vertex_program, 0x30);
    ASSERT_OFFSETOF(internal_program_set, movie_yuv_pixel_program,            0x34);
    ASSERT_OFFSETOF(internal_program_set, restore_depth_pixel_program,        0x38);
    ASSERT_OFFSETOF(internal_program_set, rawz_depth_copy_pixel_program,      0x3C);
    ASSERT_OFFSETOF(internal_program_set, screen_pc_pixel_program,            0x40);
    ASSERT_OFFSETOF(internal_program_set, depth_copy_pixel_program,           0x44);
}}} // treyarch::ngl::d3d9
