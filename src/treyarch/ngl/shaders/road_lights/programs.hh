#pragma once

#include <d3d9.h>

namespace treyarch { namespace ngl { namespace shaders { namespace road_lights {
    bool initialize();

    IDirect3DVertexShader9* get_vertex_program();
    IDirect3DPixelShader9* get_pixel_program();
}}}} // treyarch::ngl::shaders::road_lights
