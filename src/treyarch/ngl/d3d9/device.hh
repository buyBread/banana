#pragma once

#include <d3d9.h>

#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl { namespace d3d9 {
    struct texture_binding_cache {
        IDirect3DBaseTexture9* values[16];
    };

    void reset_bindings();
    void wait_for_rendering();

    namespace references {
        inline util::memory_reference<IDirect3DDevice9*>       device               { 0x011170BC };
        inline util::memory_reference<IDirect3DVertexBuffer9*> stream_source        { 0x0111AB58 };
        inline util::memory_reference<texture_binding_cache>   textures             { 0x0111AB18 };
        inline util::memory_reference<IDirect3DIndexBuffer9*>  indices              { 0x0111AB98 };
        inline util::memory_reference<IDirect3DPixelShader9*>  pixel_shader         { 0x0111AB9C };
        inline util::memory_reference<IDirect3DVertexShader9*> vertex_shader        { 0x0111ABA0 };
        inline util::memory_reference<u32>                     stream_source_stride { 0x0111ABA4 };
    } // references
}}} // treyarch::ngl::d3d9
