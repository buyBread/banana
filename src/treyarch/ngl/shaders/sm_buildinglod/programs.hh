#pragma once

#include <d3d9.h>

namespace treyarch { namespace ngl { namespace shaders { namespace sm_buildinglod {
    enum class e_pixel_variant {
        adaptive_alpha,
        opaque,
    };

    bool initialize();
    IDirect3DVertexShader9* get_vertex_program();
    IDirect3DPixelShader9* get_pixel_program(e_pixel_variant variant);
}}}} // treyarch::ngl::shaders::sm_buildinglod
