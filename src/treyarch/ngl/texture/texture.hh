#pragma once

#include <d3d9.h>

#include "treyarch/shared/fixed_string.hh"
#include "util/types.hh"

/*
    refactor into classes where appropriate (and when)?
*/

namespace treyarch { namespace ngl {
    struct texture_platform_state {
        IDirect3DBaseTexture9* resource;
        u32                    width;
        u32                    height;
        u32                    depth;
        u32                    level_count;
        D3DFORMAT              format;
        DWORD                  usage;
        D3DRESOURCETYPE        resource_type;
    };

    ASSERT_SIZEOF(texture_platform_state, 0x20);

    struct texture_description {
        const char*         name                = nullptr;
        u32                 flags               = 0;
        u32                 width               = 1;
        u32                 height              = 1;
        u32                 depth               = 1;
        u32                 level_count         = 1;
        D3DFORMAT           format              = D3DFMT_A8R8G8B8;
        DWORD               usage               = 0;
        D3DRESOURCETYPE     resource_type       = D3DRTYPE_TEXTURE;
        D3DPOOL             pool                = D3DPOOL_DEFAULT;
        D3DMULTISAMPLE_TYPE multisample_type    = D3DMULTISAMPLE_NONE;
        DWORD               multisample_quality = 0;
        BOOL                discard             = FALSE;
        D3DFORMAT           depth_format        = D3DFMT_D24S8;
        D3DRESOURCETYPE     depth_resource_type = D3DRTYPE_SURFACE;
    };

    struct texture {
        u32                    native_handle;
        u32                    frame_epoch;
        fixed_string           name;
        u32                    flags;
        texture_platform_state platform;
        IDirect3DSurface9*     surface;
        texture*               depth_target;
        u32                    level_count;
        texture**              levels;
    };

    ASSERT_SIZEOF  (texture,               0x44);
    ASSERT_OFFSETOF(texture, name,         0x08);
    ASSERT_OFFSETOF(texture, flags,        0x10);
    ASSERT_OFFSETOF(texture, platform,     0x14);
    ASSERT_OFFSETOF(texture, surface,      0x34);
    ASSERT_OFFSETOF(texture, depth_target, 0x38);
    ASSERT_OFFSETOF(texture, level_count,  0x3C);
    ASSERT_OFFSETOF(texture, levels,       0x40);
}}