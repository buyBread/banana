#include <cstring>

#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/framebuffer.hh"
#include "treyarch/ngl/d3d9/texture.hh"
#include "treyarch/ngl/texture/runtime.hh"
#include "treyarch/shared/hash/algo.hh"
#include "treyarch/shared/memory/memory.hh"

using namespace treyarch;

void initialize_2d_resource(ngl::d3d9::texture_resource &resource,
                            u32                          width,
                            u32                          height,
                            u32                          level_count,
                            D3DFORMAT                    format,
                            u8                           creation_flags) {

    DWORD render_target = (creation_flags & 2) != 0;

    resource.resource      = nullptr;
    resource.width         = width;
    resource.height        = height;
    resource.depth         = 1;
    resource.level_count   = level_count;
    resource.format        = format;
    resource.usage         = render_target;
    resource.resource_type = D3DRTYPE_TEXTURE;

    if (creation_flags & 1)
        resource.usage = render_target | 0x10;
    else if (creation_flags & 6)
        resource.usage = render_target | 0x08;

    if (creation_flags & 8)
        resource.usage |= D3DUSAGE_DEPTHSTENCIL;
}

ngl::texture* ngl::create_runtime_texture(u32       flags,
                                          D3DFORMAT format,
                                          u32       width,
                                          u32       height,
                                          u32       level_count) {

    texture* value = (texture*)memory::allocate(sizeof(texture), 8, 0);

    std::memset(value, 0, sizeof(texture));

    value->flags = flags;

    u8 creation_flags = (flags & 0x00100000) != 0;

    if (flags & 0x00001000)
        creation_flags |= 4;

    if (flags & runtime_texture_render_target)
        creation_flags |= 2;

    if (flags & 0x000000C0)
        creation_flags |= 2;

    if (flags & runtime_texture_surface_only) {
        if (flags & runtime_texture_surface_level) {
            initialize_2d_resource(value->gpu_texture,
                                   width,
                                   height,
                                   1,
                                   format,
                                   (creation_flags & 0xF5) | 8);

            d3d9::create_texture_resource(&value->gpu_texture);
            ( (IDirect3DTexture9*)value->gpu_texture.resource )->GetSurfaceLevel
                (0, &value->render_target);

            value->flags |= runtime_texture_owned;
            value->last_frame_reference = -1;

            return value;
        }

        value->gpu_texture.width       = width;
        value->gpu_texture.height      = height;
        value->gpu_texture.depth       = 1;
        value->gpu_texture.level_count = 1;
        value->gpu_texture.format      = format;
        value->gpu_texture.usage       = d3d9::is_depth_surface_format(format) ?
            D3DUSAGE_DEPTHSTENCIL : D3DUSAGE_RENDERTARGET;

        d3d9::create_surface_resource(&value->render_target,
                                      width,
                                      height,
                                      format);
    } else
        initialize_2d_resource(value->gpu_texture,
                               width,
                               height,
                               level_count,
                               format,
                               creation_flags);

    d3d9::create_texture_resource(&value->gpu_texture);
    value->flags |= runtime_texture_owned;

    if (value->flags & runtime_texture_render_target)
        ((IDirect3DTexture9*)value->gpu_texture.resource)->GetSurfaceLevel
            (0, &value->render_target);

    if (flags & runtime_texture_auto_depth) {
        u32 depth_flags = (flags & 0x210) | runtime_texture_surface_only;
        D3DFORMAT depth_format = D3DFMT_D24S8;

        if (flags & runtime_texture_depth_texture) {
            depth_flags = (flags & 0x210) |
                          runtime_texture_surface_only |
                          runtime_texture_surface_level;
            depth_format = d3d9::references::framebuffers.get().depth_texture_format;
        }

        value->depth_target = create_runtime_texture(depth_flags,
                                                     depth_format,
                                                     width,
                                                     height,
                                                     1);
    }

    value->last_frame_reference = -1;

    return value;
}

void ngl::name_runtime_texture(texture* value, const char* name) {
    value->name.text  = nullptr;
    value->name.hash  = string_hash(hash::djb2(name));
    value->flags     |= runtime_texture_named_target;
}

void ngl::register_runtime_texture(texture* value, const char* name) {
    value->name.text = (char*)name;
    value->name.hash = string_hash(hash::djb2(name));

    references::textures.get().insert(value);
}

void ngl::release_runtime_texture_device_resources(texture* value) {
    if (!value)
        return;

    if (value->depth_target)
        release_runtime_texture_device_resources(value->depth_target);

    if (value->render_target) {
        value->render_target->Release();
        value->render_target = nullptr;
    }

    d3d9::texture_resource &resource = value->gpu_texture;

    if (resource.resource &&
        d3d9::get_texture_pool(&resource) == D3DPOOL_DEFAULT) {

        resource.resource->Release();
        resource.resource = nullptr;
    }
}

void ngl::restore_runtime_texture_device_resources(texture* value) {
    if (!value)
        return;

    d3d9::texture_resource &resource = value->gpu_texture;

    if (!resource.resource &&
        d3d9::get_texture_pool(&resource) == D3DPOOL_DEFAULT) {

        d3d9::create_texture_resource(&resource);
    }

    if (!value->render_target) {
        if (value->flags & runtime_texture_surface_level) {
            ((IDirect3DTexture9*)resource.resource)->GetSurfaceLevel
                (0, &value->render_target);
        } else if (value->flags & runtime_texture_surface_only) {
            d3d9::create_surface_resource(&value->render_target,
                                          resource.width,
                                          resource.height,
                                          resource.format);
        } else if (value->flags & runtime_texture_render_target) {
            ((IDirect3DTexture9*)resource.resource)->GetSurfaceLevel
                (0, &value->render_target);
        }
    }

    if (value->depth_target)
        restore_runtime_texture_device_resources(value->depth_target);
}
