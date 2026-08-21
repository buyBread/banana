#pragma once

#include <d3d9.h>

#include "treyarch/ngl/shaders/sm_bright_filter/configuration.hh"

namespace treyarch { namespace ngl { namespace shaders { namespace sm_bright_filter {
    inline constexpr size_t vertex_variant_count = vertex_configurations.size();
    inline constexpr size_t pixel_variant_count  = pixel_configurations.size();

    bool initialize();

    IDirect3DVertexShader9* get_vertex_program(e_vertex_variant variant);
    IDirect3DPixelShader9* get_pixel_program(e_pixel_variant variant);
}}}} // treyarch::ngl::shaders::sm_bright_filter
