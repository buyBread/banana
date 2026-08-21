#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/internal_programs.hh"

using namespace treyarch;

static ::util::memory_reference<const DWORD> vertex_program_code_0 { 0x00DBA380 };
static ::util::memory_reference<const DWORD> vertex_program_code_1 { 0x00DB9AE8 };
static ::util::memory_reference<const DWORD> vertex_program_code_2 { 0x00DB9F90 };
static ::util::memory_reference<const DWORD> vertex_program_code_3 { 0x00DB96A8 };
static ::util::memory_reference<const DWORD> vertex_program_code_4 { 0x00DB9E58 };
static ::util::memory_reference<const DWORD> vertex_program_code_5 { 0x00DB9BB8 };
static ::util::memory_reference<const DWORD> vertex_program_code_6 { 0x00DBA060 };

static ::util::memory_reference<const DWORD> pixel_program_code_0  { 0x00DB9DD8 };
static ::util::memory_reference<const DWORD> pixel_program_code_1  { 0x00DB9798 };
static ::util::memory_reference<const DWORD> pixel_program_code_2  { 0x00DB9A28 };
static ::util::memory_reference<const DWORD> pixel_program_code_3  { 0x00DB95C0 };
static ::util::memory_reference<const DWORD> pixel_program_code_4  { 0x00DB9CD0 };
static ::util::memory_reference<const DWORD> pixel_program_code_5  { 0x00DBA1B0 };
static ::util::memory_reference<const DWORD> pixel_program_code_6  { 0x00DB9450 };
static ::util::memory_reference<const DWORD> pixel_program_code_7  { 0x00DBA4A0 };
static ::util::memory_reference<const DWORD> pixel_program_code_8  { 0x00DB94D0 };
static ::util::memory_reference<const DWORD> pixel_program_code_9  { 0x00DB9260 };
static ::util::memory_reference<const DWORD> pixel_program_code_10 { 0x00DB9870 };

void ngl::d3d9::initialize_internal_programs() {
    IDirect3DDevice9*     device   = references::device.get();
    internal_program_set &programs = references::internal_programs.get();

    device->CreateVertexShader(&vertex_program_code_0.get(), &programs.vertex_program_0);
    device->CreatePixelShader (&pixel_program_code_0 .get(), &programs.pixel_program_0 );
    device->CreateVertexShader(&vertex_program_code_1.get(), &programs.vertex_program_1);
    device->CreateVertexShader(&vertex_program_code_2.get(), &programs.vertex_program_2);
    device->CreateVertexShader(&vertex_program_code_3.get(), &programs.vertex_program_3);
    device->CreateVertexShader(&vertex_program_code_4.get(), &programs.vertex_program_4);
    device->CreateVertexShader(&vertex_program_code_5.get(), &programs.vertex_program_5);
    device->CreateVertexShader(&vertex_program_code_6.get(), &programs.vertex_program_6);
    device->CreatePixelShader (&pixel_program_code_1 .get(), &programs.pixel_program_1 );
    device->CreatePixelShader (&pixel_program_code_2 .get(), &programs.pixel_program_2 );
    device->CreatePixelShader (&pixel_program_code_3 .get(), &programs.pixel_program_3 );
    device->CreatePixelShader (&pixel_program_code_4 .get(), &programs.pixel_program_4 );
    device->CreatePixelShader (&pixel_program_code_5 .get(), &programs.pixel_program_5 );
    device->CreatePixelShader (&pixel_program_code_6 .get(), &programs.pixel_program_6 );
    device->CreatePixelShader (&pixel_program_code_7 .get(), &programs.pixel_program_7 );
    device->CreatePixelShader (&pixel_program_code_8 .get(), &programs.pixel_program_8 );
    device->CreatePixelShader (&pixel_program_code_9 .get(), &programs.pixel_program_9 );
    device->CreatePixelShader (&pixel_program_code_10.get(), &programs.pixel_program_10);
}
