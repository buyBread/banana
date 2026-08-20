#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/internal_shaders.hh"

using namespace treyarch;

static util::memory_reference<const DWORD> vertex_shader_code_0 { 0x00DBA380 };
static util::memory_reference<const DWORD> vertex_shader_code_1 { 0x00DB9AE8 };
static util::memory_reference<const DWORD> vertex_shader_code_2 { 0x00DB9F90 };
static util::memory_reference<const DWORD> vertex_shader_code_3 { 0x00DB96A8 };
static util::memory_reference<const DWORD> vertex_shader_code_4 { 0x00DB9E58 };
static util::memory_reference<const DWORD> vertex_shader_code_5 { 0x00DB9BB8 };
static util::memory_reference<const DWORD> vertex_shader_code_6 { 0x00DBA060 };

static util::memory_reference<const DWORD> pixel_shader_code_0  { 0x00DB9DD8 };
static util::memory_reference<const DWORD> pixel_shader_code_1  { 0x00DB9798 };
static util::memory_reference<const DWORD> pixel_shader_code_2  { 0x00DB9A28 };
static util::memory_reference<const DWORD> pixel_shader_code_3  { 0x00DB95C0 };
static util::memory_reference<const DWORD> pixel_shader_code_4  { 0x00DB9CD0 };
static util::memory_reference<const DWORD> pixel_shader_code_5  { 0x00DBA1B0 };
static util::memory_reference<const DWORD> pixel_shader_code_6  { 0x00DB9450 };
static util::memory_reference<const DWORD> pixel_shader_code_7  { 0x00DBA4A0 };
static util::memory_reference<const DWORD> pixel_shader_code_8  { 0x00DB94D0 };
static util::memory_reference<const DWORD> pixel_shader_code_9  { 0x00DB9260 };
static util::memory_reference<const DWORD> pixel_shader_code_10 { 0x00DB9870 };

void ngl::d3d9::initialize_internal_shaders() {
    IDirect3DDevice9*    device  = references::device.get();
    internal_shader_set &shaders = references::internal_shaders.get();

    device->CreateVertexShader(&vertex_shader_code_0.get(), &shaders.vertex_shader_0);
    device->CreatePixelShader (&pixel_shader_code_0 .get(), &shaders.pixel_shader_0);
    device->CreateVertexShader(&vertex_shader_code_1.get(), &shaders.vertex_shader_1);
    device->CreateVertexShader(&vertex_shader_code_2.get(), &shaders.vertex_shader_2);
    device->CreateVertexShader(&vertex_shader_code_3.get(), &shaders.vertex_shader_3);
    device->CreateVertexShader(&vertex_shader_code_4.get(), &shaders.vertex_shader_4);
    device->CreateVertexShader(&vertex_shader_code_5.get(), &shaders.vertex_shader_5);
    device->CreateVertexShader(&vertex_shader_code_6.get(), &shaders.vertex_shader_6);
    device->CreatePixelShader (&pixel_shader_code_1 .get(), &shaders.pixel_shader_1);
    device->CreatePixelShader (&pixel_shader_code_2 .get(), &shaders.pixel_shader_2);
    device->CreatePixelShader (&pixel_shader_code_3 .get(), &shaders.pixel_shader_3);
    device->CreatePixelShader (&pixel_shader_code_4 .get(), &shaders.pixel_shader_4);
    device->CreatePixelShader (&pixel_shader_code_5 .get(), &shaders.pixel_shader_5);
    device->CreatePixelShader (&pixel_shader_code_6 .get(), &shaders.pixel_shader_6);
    device->CreatePixelShader (&pixel_shader_code_7 .get(), &shaders.pixel_shader_7);
    device->CreatePixelShader (&pixel_shader_code_8 .get(), &shaders.pixel_shader_8);
    device->CreatePixelShader (&pixel_shader_code_9 .get(), &shaders.pixel_shader_9);
    device->CreatePixelShader (&pixel_shader_code_10.get(), &shaders.pixel_shader_10);
}
