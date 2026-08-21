#pragma once

#include <d3d9.h>

#include "treyarch/ngl/shaders/sm_decal/configuration.hh"

namespace treyarch { namespace ngl { namespace shaders { namespace sm_decal {
    bool initialize();

    IDirect3DVertexShader9* get_vertex_program();
    IDirect3DPixelShader9* get_material_program(const material_configuration &configuration);
    IDirect3DPixelShader9* get_utility_program(e_utility_variant variant);
}}}} // treyarch::ngl::shaders::sm_decal
