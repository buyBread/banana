#pragma once

#include <d3d9.h>

namespace treyarch { namespace ngl { namespace shaders { namespace sm_bush {
    enum class e_pixel_variant {
        material,
        shadowed_material,
        constant_red,
        sample_texture,
        sample_auxiliary_texture,
    };

    bool initialize();
    IDirect3DVertexShader9* get_vertex_program();
    IDirect3DPixelShader9* get_pixel_program(e_pixel_variant variant);
}}}} // treyarch::ngl::shaders::sm_bush
