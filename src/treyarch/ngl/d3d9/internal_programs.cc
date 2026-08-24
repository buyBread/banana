#include "treyarch/ngl/d3d9/internal_programs.hh"
#include "treyarch/ngl/d3d9/shader_program_cache.hh"

using namespace treyarch;

static util::memory_reference<const DWORD> vertex_program_code_0                { 0x00DBA380 };
static util::memory_reference<const DWORD> vertex_program_code_1                { 0x00DB9AE8 };
static util::memory_reference<const DWORD> framebuffer_copy_vertex_program_code { 0x00DB9F90 };
static util::memory_reference<const DWORD> vertex_program_code_3                { 0x00DB96A8 };
static util::memory_reference<const DWORD> vertex_program_code_4                { 0x00DB9E58 };
static util::memory_reference<const DWORD> vertex_program_code_5                { 0x00DB9BB8 };
static util::memory_reference<const DWORD> vertex_program_code_6                { 0x00DBA060 };

static util::memory_reference<const DWORD> pixel_program_code_0                { 0x00DB9DD8 };
static util::memory_reference<const DWORD> pixel_program_code_1                { 0x00DB9798 };
static util::memory_reference<const DWORD> framebuffer_copy_pixel_program_code { 0x00DB9A28 };
static util::memory_reference<const DWORD> depth_copy_pixel_program_code       { 0x00DB95C0 };
static util::memory_reference<const DWORD> rawz_depth_copy_pixel_program_code  { 0x00DB9CD0 };
static util::memory_reference<const DWORD> pixel_program_code_5                { 0x00DBA1B0 };
static util::memory_reference<const DWORD> pixel_program_code_6                { 0x00DB9450 };
static util::memory_reference<const DWORD> pixel_program_code_7                { 0x00DBA4A0 };
static util::memory_reference<const DWORD> pixel_program_code_8                { 0x00DB94D0 };
static util::memory_reference<const DWORD> pixel_program_code_9                { 0x00DB9260 };
static util::memory_reference<const DWORD> pixel_program_code_10               { 0x00DB9870 };

void ngl::d3d9::initialize_internal_programs() {
    internal_program_set &programs = references::internal_programs.get();

    shader_program_cache::create_vertex_program(&vertex_program_code_0.get(), &programs.vertex_program_0);
    shader_program_cache::create_pixel_program (&pixel_program_code_0.get(),  &programs.pixel_program_0);
    shader_program_cache::create_vertex_program(&vertex_program_code_1.get(), &programs.vertex_program_1);

    shader_program_cache::create_vertex_program(&framebuffer_copy_vertex_program_code.get(), &programs.framebuffer_copy_vertex_program);

    shader_program_cache::create_vertex_program(&vertex_program_code_3.get(), &programs.vertex_program_3);
    shader_program_cache::create_vertex_program(&vertex_program_code_4.get(), &programs.vertex_program_4);
    shader_program_cache::create_vertex_program(&vertex_program_code_5.get(), &programs.vertex_program_5);
    shader_program_cache::create_vertex_program(&vertex_program_code_6.get(), &programs.vertex_program_6);
    shader_program_cache::create_pixel_program (&pixel_program_code_1.get(),  &programs.pixel_program_1);

    shader_program_cache::create_pixel_program(&framebuffer_copy_pixel_program_code.get(), &programs.framebuffer_copy_pixel_program);
    shader_program_cache::create_pixel_program(&depth_copy_pixel_program_code.get(),       &programs.depth_copy_pixel_program);
    shader_program_cache::create_pixel_program(&rawz_depth_copy_pixel_program_code.get(),  &programs.rawz_depth_copy_pixel_program);

    shader_program_cache::create_pixel_program(&pixel_program_code_5.get(),  &programs.pixel_program_5);
    shader_program_cache::create_pixel_program(&pixel_program_code_6.get(),  &programs.pixel_program_6);
    shader_program_cache::create_pixel_program(&pixel_program_code_7.get(),  &programs.pixel_program_7);
    shader_program_cache::create_pixel_program(&pixel_program_code_8.get(),  &programs.pixel_program_8);
    shader_program_cache::create_pixel_program(&pixel_program_code_9.get(),  &programs.pixel_program_9);
    shader_program_cache::create_pixel_program(&pixel_program_code_10.get(), &programs.pixel_program_10);
}
