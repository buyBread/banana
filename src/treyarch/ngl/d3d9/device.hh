#pragma once

#include <d3d9.h>

#include "treyarch/ngl/d3d9/vertex_definition.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl { namespace d3d9 {
    struct binding_cache {
        IDirect3DBaseTexture9*       textures[16];
        IDirect3DVertexBuffer9*      stream_source;
        u32                          unk_044[15];
        IDirect3DIndexBuffer9*       indices;
        IDirect3DPixelShader9*       pixel_shader;
        IDirect3DVertexShader9*      vertex_shader;
        IDirect3DVertexDeclaration9* vertex_declaration;
        u32                          validation;
    };

    void poison_bindings();
    void reset_bindings();
    void wait_for_rendering();
    void reset_device();

    void set_texture(u32 stage, IDirect3DBaseTexture9* value);
    void set_vertex_program(IDirect3DVertexShader9* value);
    void set_pixel_program(IDirect3DPixelShader9* value);
    void set_vertex_definition(const vertex_definition* value);

    namespace references {
        inline util::memory_reference<IDirect3D9*>           d3d9         { 0x011170B8 };
        inline util::memory_reference<IDirect3DDevice9*>     device       { 0x011170BC };
        inline util::memory_reference<D3DPRESENT_PARAMETERS> presentation { 0x01116350 };
        inline util::memory_reference<D3DCAPS9>              capabilities { 0x01116388 };
        inline util::memory_reference<binding_cache>         bindings     { 0x0111AB18 };
    } // references

    ASSERT_SIZEOF  (binding_cache,                       0x94);
    ASSERT_OFFSETOF(binding_cache, textures,             0x00);
    ASSERT_OFFSETOF(binding_cache, stream_source,        0x40);
    ASSERT_OFFSETOF(binding_cache, indices,              0x80);
    ASSERT_OFFSETOF(binding_cache, pixel_shader,         0x84);
    ASSERT_OFFSETOF(binding_cache, vertex_shader,        0x88);
    ASSERT_OFFSETOF(binding_cache, vertex_declaration,   0x8C);
    ASSERT_OFFSETOF(binding_cache, validation,           0x90);
}}} // treyarch::ngl::d3d9
