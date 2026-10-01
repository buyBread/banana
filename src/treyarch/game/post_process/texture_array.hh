#pragma once

#include <d3d9.h>

#include "treyarch/ngl/d3d9/texture.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace post_process {
    // TextureArray element
    struct surface_texture {
        ngl::d3d9::texture_resource texture;
        IDirect3DSurface9*          render_target;
        u32                         target_index;
        IDirect3DSurface9*          saved_render_target;
        u8                          owns_render_target;
        u8                          pad_02d[3];

        surface_texture();
        ~surface_texture();

        bool create(u32       width,
                    u32       height,
                    u32       level_count,
                    D3DFORMAT surface_format,
                    D3DFORMAT format,
                    u8        creation_flags);

        // `work_buffer` is the X360 placement cursor; on PC only its null-ness matters
        u32 create_level(u8*       work_buffer,
                         u32       width,
                         u32       height,
                         u32       level_count,
                         D3DFORMAT surface_format,
                         D3DFORMAT format,
                         u8        creation_flags);

        void clear(D3DCOLOR color);
    };

    // RTTI TextureArray
    struct texture_array {
        void*           vtable;
        u32             count;
        surface_texture textures[16];

        texture_array();
        ~texture_array();
    };

    // RTTI MipmapTextureArray
    struct mipmap_texture_array : texture_array {
        u32 width;
        u32 height;

        mipmap_texture_array();

        u32 create(u8*       work_buffer,
                   u32       level_width,
                   u32       level_height,
                   u32       level_count,
                   D3DFORMAT surface_format,
                   D3DFORMAT format,
                   u8        creation_flags);

        void destroy();
    };

    bool create_noise_texture(ngl::d3d9::texture_resource &resource,
                              u32                          width,
                              u32                          height);

    namespace references {
        inline util::memory_reference<void*> texture_array_vtable        { 0x00BCA394 };
        inline util::memory_reference<void*> mipmap_texture_array_vtable { 0x00BCA47C };
    } // references

    ASSERT_SIZEOF  (surface_texture,                      0x30);
    ASSERT_OFFSETOF(surface_texture, texture,             0x00);
    ASSERT_OFFSETOF(surface_texture, render_target,       0x20);
    ASSERT_OFFSETOF(surface_texture, target_index,        0x24);
    ASSERT_OFFSETOF(surface_texture, saved_render_target, 0x28);
    ASSERT_OFFSETOF(surface_texture, owns_render_target,  0x2C);

    ASSERT_SIZEOF  (texture_array,           0x308);
    ASSERT_OFFSETOF(texture_array, count,    0x004);
    ASSERT_OFFSETOF(texture_array, textures, 0x008);

    ASSERT_SIZEOF  (mipmap_texture_array,         0x310);
    ASSERT_OFFSETOF(mipmap_texture_array, width,  0x308);
    ASSERT_OFFSETOF(mipmap_texture_array, height, 0x30C);
}} // treyarch::post_process
