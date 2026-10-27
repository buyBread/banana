#pragma once

#include <d3d9.h>

#include "treyarch/ngl/shaders/description.hh"

namespace treyarch { namespace ngl { namespace d3d9 {
    class vertex_program {
        IDirect3DVertexShader9* shader_ = nullptr;

    public:
        bool create(shaders::shader_key key);
        IDirect3DVertexShader9* get() const;
    };

    class pixel_program {
        IDirect3DPixelShader9* shader_ = nullptr;

    public:
        bool create(shaders::shader_key key);
        IDirect3DPixelShader9* get() const;
    };
}}} // treyarch::ngl::d3d9
