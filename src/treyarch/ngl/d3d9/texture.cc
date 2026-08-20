#include <cstring>

#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/texture.hh"

using namespace treyarch;

bool ngl::d3d9::create_texture_resource(texture_resource* value) {
    D3DPOOL pool = (value->usage & 0x0B) ?
        D3DPOOL_DEFAULT : (D3DPOOL)(((value->usage & 0x10) != 0) + 1);

    DWORD usage = value->usage & ~0x18;
    
    HRESULT result;

    switch (value->resource_type) {
        case D3DRTYPE_TEXTURE:
            result = references::device.get()->CreateTexture
                (value->width,
                 value->height,
                 value->level_count,
                 usage,
                 value->format,
                 pool,
                 (IDirect3DTexture9**)&value->resource,
                 nullptr);

            break;
        case D3DRTYPE_CUBETEXTURE:
            result = references::device.get()->CreateCubeTexture
                (value->width,
                 value->level_count,
                 usage,
                 value->format,
                 pool,
                 (IDirect3DCubeTexture9**)&value->resource,
                 nullptr);

            break;
        case D3DRTYPE_VOLUMETEXTURE:
            result = references::device.get()->CreateVolumeTexture
                (value->width,
                 value->height,
                 value->depth,
                 value->level_count,
                 usage,
                 value->format,
                 pool,
                 (IDirect3DVolumeTexture9**)&value->resource,
                 nullptr);

            break;
        default:
            return value->resource != nullptr;
    }

    if (result != D3D_OK)
        value->resource = nullptr;

    return value->resource != nullptr;
}

u32 ngl::d3d9::get_surface_size(D3DFORMAT format, u32 width, u32 height) {
    switch (format) {
        case D3DFMT_DXT1: {
            u32 block_width  = (width  + 3) >> 2;
            u32 block_height = (height + 3) >> 2;

            if (!block_width)
                block_width = 1;

            if (!block_height)
                block_height = 1;

            return (8 * block_width * block_height + 3) & ~3;
        }

        case D3DFMT_DXT2:
        case D3DFMT_DXT3:
        case D3DFMT_DXT4:
        case D3DFMT_DXT5: {
            u32 block_width  = (width  + 3) >> 2;
            u32 block_height = (height + 3) >> 2;

            if (!block_width)
                block_width = 1;

            if (!block_height)
                block_height = 1;

            return (16 * block_width * block_height + 3) & ~3;
        }

        case D3DFMT_R8G8B8:
            return (3 * (width * height + 1)) & ~3;

        case D3DFMT_A8R8G8B8:
        case D3DFMT_X8R8G8B8:
        case D3DFMT_A2B10G10R10:
        case D3DFMT_A8B8G8R8:
        case D3DFMT_X8B8G8R8:
        case D3DFMT_G16R16:
        case D3DFMT_A2R10G10B10:
        case D3DFMT_X8L8V8U8:
        case D3DFMT_Q8W8V8U8:
        case D3DFMT_V16U16:
        case D3DFMT_A2W10V10U10:
        case D3DFMT_D32:
        case D3DFMT_D24S8:
        case D3DFMT_D24X8:
        case D3DFMT_D24X4S4:
        case D3DFMT_D32F_LOCKABLE:
        case D3DFMT_D24FS8:
        case D3DFMT_G16R16F:
        case D3DFMT_R32F:
            return 4 * width * height;

        case D3DFMT_R5G6B5:
        case D3DFMT_X1R5G5B5:
        case D3DFMT_A1R5G5B5:
        case D3DFMT_A4R4G4B4:
        case D3DFMT_A8R3G3B2:
        case D3DFMT_X4R4G4B4:
        case D3DFMT_A8P8:
        case D3DFMT_A8L8:
        case D3DFMT_V8U8:
        case D3DFMT_L6V5U5:
        case D3DFMT_D16_LOCKABLE:
        case D3DFMT_D15S1:
        case D3DFMT_D16:
        case D3DFMT_L16:
        case D3DFMT_R16F:
        case D3DFMT_CxV8U8:
        case D3DFMT_G8R8_G8B8:
        case D3DFMT_R8G8_B8G8:
            return (2 * width * height + 3) & ~3;

        case D3DFMT_R3G3B2:
        case D3DFMT_A8:
        case D3DFMT_P8:
        case D3DFMT_L8:
        case D3DFMT_A4L4:
            return (width * height + 3) & ~3;

        case D3DFMT_A16B16G16R16:
        case D3DFMT_Q16W16V16U16:
        case D3DFMT_A16B16G16R16F:
        case D3DFMT_G32R32F:
            return 8 * width * height;

        case D3DFMT_A32B32G32R32F:
            return 16 * width * height;

        default:
            return 0;
    }
}

void ngl::d3d9::upload_texture(texture_resource* value, const void* &source) {
    if (!value->resource)
        return;

    IDirect3DTexture9* resource = (IDirect3DTexture9*)value->resource;
    
    u32 level_count = resource->GetLevelCount();

    for (u32 level = 0; level < level_count; ++level) {
        IDirect3DSurface9* surface;
        D3DSURFACE_DESC    description;
        D3DLOCKED_RECT     locked;

        resource->GetSurfaceLevel(level, &surface);
        surface->GetDesc(&description);
        resource->LockRect(level, &locked, nullptr, 0);

        u32 size = get_surface_size(description.Format,
                                    description.Width,
                                    description.Height);

        std::memcpy(locked.pBits, source, size);
        
        source = (const u8*)source + size;

        resource->UnlockRect(level);
        surface->Release();
    }
}

void ngl::d3d9::upload_cube_texture(texture_resource* value, const void* &source) {
    D3DCUBEMAP_FACES faces[] {
        D3DCUBEMAP_FACE_POSITIVE_X,
        D3DCUBEMAP_FACE_NEGATIVE_X,
        D3DCUBEMAP_FACE_POSITIVE_Y,
        D3DCUBEMAP_FACE_NEGATIVE_Y,
        D3DCUBEMAP_FACE_POSITIVE_Z,
        D3DCUBEMAP_FACE_NEGATIVE_Z
    };

    IDirect3DCubeTexture9* resource = (IDirect3DCubeTexture9*)value->resource;
    
    u32 level_count = resource->GetLevelCount();

    for (u32 face_index = 0; face_index < 6; ++face_index) {
        D3DCUBEMAP_FACES face = faces[face_index];

        for (u32 level = 0; level < level_count; ++level) {
            IDirect3DSurface9* surface;
            D3DSURFACE_DESC    description;
            D3DLOCKED_RECT     locked;

            resource->GetCubeMapSurface(face, level, &surface);
            surface->GetDesc(&description);
            resource->LockRect(face, level, &locked, nullptr, 0);

            u32 size = get_surface_size(description.Format,
                                        description.Width,
                                        description.Height);

            std::memcpy(locked.pBits, source, size);
            source = (const u8*)source + size;

            resource->UnlockRect(face, level);
            surface->Release();
        }
    }
}
