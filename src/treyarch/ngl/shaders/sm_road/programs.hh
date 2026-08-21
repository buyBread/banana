#pragma once

#include <d3d9.h>

namespace treyarch { namespace ngl { namespace shaders { namespace sm_road {
    enum class e_pixel_variant {
        unshadowed,
        shadow_1,
        shadow_2,
        sample_texture,
        sample_auxiliary_alpha,
    };

    bool initialize();
    IDirect3DVertexShader9* get_vertex_program();
    IDirect3DPixelShader9* get_pixel_program(e_pixel_variant variant);
}}}} // treyarch::ngl::shaders::sm_road
