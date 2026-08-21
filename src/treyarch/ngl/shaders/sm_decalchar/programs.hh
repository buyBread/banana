#pragma once

#include <d3d9.h>

namespace treyarch { namespace ngl { namespace shaders { namespace sm_decalchar {
    enum class e_vertex_variant {
        material,
        depth_shadow,
    };

    bool initialize();
    IDirect3DVertexShader9* get_vertex_program(e_vertex_variant variant);
    IDirect3DPixelShader9* get_pixel_program();
}}}} // treyarch::ngl::shaders::sm_decalchar
