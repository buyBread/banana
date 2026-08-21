#pragma once

#include <d3d9.h>

namespace treyarch { namespace ngl { namespace shaders { namespace sm_roadlod {
    enum class e_pixel_variant {
        surface,
        sampler_passthrough,
    };

    bool initialize();
    IDirect3DVertexShader9* get_vertex_program();
    IDirect3DPixelShader9* get_pixel_program(e_pixel_variant variant);
}}}} // treyarch::ngl::shaders::sm_roadlod
