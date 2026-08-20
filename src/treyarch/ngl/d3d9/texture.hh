#pragma once

#include <d3d9.h>

#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl { namespace d3d9 {
    struct texture_resource {
        IDirect3DBaseTexture9* resource;
        u32                    width;
        u32                    height;
        u32                    depth;
        u32                    level_count;
        D3DFORMAT              format;
        DWORD                  usage;
        D3DRESOURCETYPE        resource_type;
    };

    bool create_texture_resource(texture_resource* value);
    u32 get_surface_size(D3DFORMAT format, u32 width, u32 height);
    void upload_texture(texture_resource* value, const void* &source);
    void upload_cube_texture(texture_resource* value, const void* &source);

    ASSERT_SIZEOF  (texture_resource,                0x20);
    ASSERT_OFFSETOF(texture_resource, resource,      0x00);
    ASSERT_OFFSETOF(texture_resource, width,         0x04);
    ASSERT_OFFSETOF(texture_resource, height,        0x08);
    ASSERT_OFFSETOF(texture_resource, depth,         0x0C);
    ASSERT_OFFSETOF(texture_resource, level_count,   0x10);
    ASSERT_OFFSETOF(texture_resource, format,        0x14);
    ASSERT_OFFSETOF(texture_resource, usage,         0x18);
    ASSERT_OFFSETOF(texture_resource, resource_type, 0x1C);
}}} // treyarch::ngl::d3d9
