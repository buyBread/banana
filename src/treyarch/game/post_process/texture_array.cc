#include <cstdlib>

#include "treyarch/game/post_process/texture_array.hh"
#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/framebuffer.hh"

using namespace treyarch;

// sub_730EC0
post_process::surface_texture::surface_texture() {
    target_index        = 0;
    saved_render_target = nullptr;
    render_target       = nullptr;
    owns_render_target  = 0;
}

// sub_730EE0
post_process::surface_texture::~surface_texture() {
    IDirect3DSurface9* surface = render_target;

    target_index        = 0;
    saved_render_target = nullptr;

    if (surface && owns_render_target)
        surface->Release();
}

// sub_73DBD0
bool post_process::surface_texture::create(u32       width,
                                           u32       height,
                                           u32       level_count,
                                           D3DFORMAT surface_format,
                                           D3DFORMAT format,
                                           u8        creation_flags) {

    ngl::d3d9::texture_resource resource;

    ngl::d3d9::initialize_2d_resource(resource, width, height, level_count, format, creation_flags);

    if (!ngl::d3d9::create_texture_resource(&resource))
        return false;

    texture = resource;

    ngl::d3d9::create_surface_resource(&render_target,
                                       texture.width,
                                       texture.height,
                                       surface_format ? surface_format : texture.format);
    owns_render_target = 1;

    return true;
}

// sub_73DC80
u32 post_process::surface_texture::create_level(u8*       work_buffer,
                                                u32       width,
                                                u32       height,
                                                u32       level_count,
                                                D3DFORMAT surface_format,
                                                D3DFORMAT format,
                                                u8        creation_flags) {

    ngl::d3d9::texture_resource resource;

    if (!work_buffer)
        return ngl::d3d9::initialize_2d_resource(resource, width, height, level_count, format, creation_flags);

    ngl::d3d9::initialize_2d_resource(resource, width, height, level_count, format, creation_flags);

    u32 created = ngl::d3d9::create_texture_resource(&resource);

    if (!created)
        return created;

    texture = resource;

    ngl::d3d9::create_surface_resource(&render_target,
                                       texture.width,
                                       texture.height,
                                       surface_format ? surface_format : texture.format);
    owns_render_target = 1;

    return created;
}

// sub_73DD60
void post_process::surface_texture::clear(D3DCOLOR color) {
    IDirect3DDevice9* device = ngl::d3d9::references::device.get();

    target_index        = 0;
    saved_render_target = nullptr;

    IDirect3DSurface9* current_target;
    device->GetRenderTarget(0, &current_target);

    saved_render_target = current_target;

    ngl::d3d9::set_render_target(target_index, &render_target, false);
    device->Clear(0, nullptr, D3DCLEAR_TARGET, color, 0.0f, 0);
    ngl::d3d9::set_render_target(target_index, &saved_render_target, true);
}

// inlined into sub_745930
post_process::texture_array::texture_array() {
    vtable = &references::texture_array_vtable.get();
    count  = 0;
}

// inlined into sub_745A70
post_process::texture_array::~texture_array() {
    vtable = &references::texture_array_vtable.get();
    count  = 0;
}

// sub_745930
post_process::mipmap_texture_array::mipmap_texture_array() {
    width  = 0;
    height = 0;
    vtable = &references::mipmap_texture_array_vtable.get();
}

// sub_745970
u32 post_process::mipmap_texture_array::create(u8*       work_buffer,
                                               u32       level_width,
                                               u32       level_height,
                                               u32       level_count,
                                               D3DFORMAT surface_format,
                                               D3DFORMAT format,
                                               u8        creation_flags) {

    if ((level_width - 1) & level_width) {
        u8 bits = 0;

        for (u32 value = level_width; value; value >>= 1)
            ++bits;

        level_width = 1 << bits;
    }

    if ((level_height - 1) & level_height) {
        u8 bits = 0;

        for (u32 value = level_height; value; value >>= 1)
            ++bits;

        level_height = 1 << bits;
    }

    if (!level_count) {
        u32 largest = level_width <= level_height ? level_height : level_width;

        for (; largest; largest >>= 1)
            ++level_count;
    }

    if (work_buffer) {
        this->width  = level_width;
        this->height = level_height;
        count        = level_count;
    }

    u32 created = 0;

    if (!level_count)
        return created;

    for (u32 level = 0; level < level_count; ++level) {
        u32 level_created = textures[level].create_level(work_buffer,
                                                         level_width,
                                                         level_height,
                                                         1,
                                                         surface_format,
                                                         format,
                                                         creation_flags);
        if (!level_created)
            return 0;

        created += level_created;

        if (work_buffer)
            work_buffer += level_created;

        level_width >>= 1;

        if (!level_width)
            level_width = 1;

        level_height >>= 1;

        if (!level_height)
            level_height = 1;
    }

    return created;
}

void post_process::mipmap_texture_array::destroy() {
    using deleting_destructor = void*(__thiscall*)(mipmap_texture_array*, u8);

    ((deleting_destructor*)vtable)[0](this, 1);
}

// sub_73D3F0
bool post_process::create_noise_texture(ngl::d3d9::texture_resource &resource,
                                        u32                          width,
                                        u32                          height) {

    resource.width         = width;
    resource.height        = height;
    resource.depth         = 1;
    resource.level_count   = 1;
    resource.format        = D3DFMT_A8R8G8B8;
    resource.usage         = 0;
    resource.resource_type = D3DRTYPE_TEXTURE;
    resource.resource      = nullptr;

    if (!ngl::d3d9::create_texture_resource(&resource))
        return false;

    u8* bits;

    switch (resource.resource_type) {
        case D3DRTYPE_TEXTURE: {
            D3DLOCKED_RECT locked;
            ((IDirect3DTexture9*)resource.resource)->LockRect(0, &locked, nullptr, 0);

            bits = (u8*)locked.pBits;

            break;
        }
        case D3DRTYPE_CUBETEXTURE: {
            D3DLOCKED_RECT locked;
            ((IDirect3DCubeTexture9*)resource.resource)->LockRect(D3DCUBEMAP_FACE_POSITIVE_X, 0, &locked, nullptr, 0);

            bits = (u8*)locked.pBits;

            break;
        }
        case D3DRTYPE_VOLUMETEXTURE: {
            D3DLOCKED_BOX locked;
            ((IDirect3DVolumeTexture9*)resource.resource)->LockBox(0, &locked, nullptr, 0);

            bits = (u8*)locked.pBits;

            break;
        }
        default:
            return false;
    }

    if (!bits)
        return false;

    for (u32 row = 0; row < height; ++row) {
        u8* texel = bits;

        for (u32 column = 0; column < width; ++column) {
            texel[0] = (u8)std::rand();
            texel[1] = (u8)(255 * std::rand() / 0x7FFF);
            texel[2] = (u8)std::rand();
            texel[3] = (u8)(255 * std::rand() / 0x7FFF);

            texel += 4;
        }

        bits += 4 * width;
    }

    switch (resource.resource_type) {
        case D3DRTYPE_TEXTURE:
            ((IDirect3DTexture9*)resource.resource)->UnlockRect(0);

            break;
        case D3DRTYPE_CUBETEXTURE:
            ((IDirect3DCubeTexture9*)resource.resource)->UnlockRect(D3DCUBEMAP_FACE_POSITIVE_X, 0);

            break;
        case D3DRTYPE_VOLUMETEXTURE:
            ((IDirect3DVolumeTexture9*)resource.resource)->UnlockBox(0);

            break;
    }

    return true;
}
