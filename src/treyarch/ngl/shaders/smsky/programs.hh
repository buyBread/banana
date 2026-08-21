#pragma once

#include <d3d9.h>

#include "util/types.hh"

namespace treyarch { namespace ngl { namespace shaders { namespace smsky {
    enum class e_pixel_variant : u8 {
        daylight,
        night,
        combined,
    };

    bool initialize();
    IDirect3DVertexShader9* get_vertex_program();
    IDirect3DPixelShader9* get_pixel_program(e_pixel_variant variant);
}}}} // treyarch::ngl::shaders::smsky
