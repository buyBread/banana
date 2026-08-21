#pragma once

#include <d3d9.h>

namespace treyarch { namespace ngl { namespace shaders { namespace sm_babyphat {
    enum class e_vertex_variant {
        material,
        depth_shadow,
    };

    enum class e_pixel_variant {
        unshadowed,
        shadow_1,
        shadow_2,
    };

    bool initialize();
    IDirect3DVertexShader9* get_vertex_program(e_vertex_variant variant);
    IDirect3DPixelShader9* get_pixel_program(e_pixel_variant variant);
}}}} // treyarch::ngl::shaders::sm_babyphat
