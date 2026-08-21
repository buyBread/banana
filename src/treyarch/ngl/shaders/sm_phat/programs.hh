#pragma once

#include <d3d9.h>

#include "treyarch/ngl/shaders/sm_phat/configuration.hh"

namespace treyarch { namespace ngl { namespace shaders { namespace sm_phat {
    bool initialize();
    IDirect3DVertexShader9* get_vertex_program(e_vertex_variant variant);
    IDirect3DPixelShader9* get_pixel_program(const material_configuration &material, const lighting_configuration &lighting);
    IDirect3DPixelShader9* get_debug_program(e_debug_variant variant);
}}}} // treyarch::ngl::shaders::sm_phat
