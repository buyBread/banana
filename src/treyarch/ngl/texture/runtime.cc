#include <cstring>

#include "retail.hh"
#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/framebuffer.hh"
#include "treyarch/ngl/d3d9/texture.hh"
#include "treyarch/ngl/texture/runtime.hh"
#include "treyarch/shared/hash/algo.hh"
#include "treyarch/shared/memory/heap.hh"
#include "treyarch/shared/memory/memory.hh"

using namespace treyarch;

// sub_9E23D0
bool ngl::initialize_cube_resource(d3d9::texture_resource &resource,
                                   u32                     edge_length,
                                   u32                     level_count,
                                   D3DFORMAT               format,
                                   u8                      creation_flags) {

    DWORD render_target = (creation_flags & 2) != 0;

    resource.width         = edge_length;
    resource.height        = edge_length;
    resource.level_count   = level_count;
    resource.depth         = 6;
    resource.format        = format;
    resource.usage         = render_target;
    resource.resource_type = D3DRTYPE_CUBETEXTURE;
    resource.resource      = nullptr;

    if (creation_flags & 1)
        resource.usage = render_target | 0x10;
    else if (creation_flags & 6)
        resource.usage = render_target | 0x08;

    return true;
}

// sub_9E2440
bool ngl::initialize_volume_resource(d3d9::texture_resource &resource,
                                     u32                     width,
                                     u32                     height,
                                     u32                     depth,
                                     u32                     level_count,
                                     D3DFORMAT               format,
                                     u8                      creation_flags) {

    DWORD render_target = (creation_flags & 2) != 0;

    resource.width         = width;
    resource.height        = height;
    resource.depth         = depth;
    resource.level_count   = level_count;
    resource.format        = format;
    resource.usage         = render_target;
    resource.resource_type = D3DRTYPE_VOLUMETEXTURE;
    resource.resource      = nullptr;

    if (creation_flags & 1)
        resource.usage = render_target | 0x10;
    else if (creation_flags & 6)
        resource.usage = render_target | 0x08;

    return true;
}

// sub_9E25B0
ngl::texture* ngl::create_runtime_texture(u32       flags,
                                          D3DFORMAT format,
                                          u32       width,
                                          u32       height,
                                          u32       depth,
                                          u32       level_count) {

    texture* value = (texture*)memory::allocate(sizeof(texture), 8, 0);

    std::memset(value, 0, sizeof(texture));

    u32 guard = references::created_texture_name_guard.read();

    if (!(guard & 1)) {
        references::created_texture_name_guard.write(guard | 1);

        fixed_string &name = references::created_texture_name.get();

        name.hash = string_hash(hash::djb2("created"));
        name.set_text("created");

        retail::sub_ADE5FD(&ngl::release_created_texture_name); // the game's CRT atexit
    }

    value->name.assign(references::created_texture_name.get());
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
            d3d9::initialize_2d_resource(value->gpu_texture,
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

        // the descriptor stays zeroed; only the surface exists
        d3d9::create_surface_resource(&value->render_target,
                                      width,
                                      height,
                                      format);
    } else if (flags & texture_cube)
        initialize_cube_resource(value->gpu_texture, width, level_count, format, creation_flags);
    else if (flags & texture_volume)
        initialize_volume_resource(value->gpu_texture, width, height, depth, level_count, format, creation_flags);
    else
        d3d9::initialize_2d_resource(value->gpu_texture,
                                     width,
                                     height,
                                     level_count,
                                     format,
                                     creation_flags);

    d3d9::create_texture_resource(&value->gpu_texture);
    value->flags |= runtime_texture_owned;

    if (value->flags & runtime_texture_render_target)
        ( (IDirect3DTexture9*)value->gpu_texture.resource )
            ->GetSurfaceLevel(0, &value->render_target);

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
                                                     1,
                                                     1);
    }

    value->last_frame_reference = -1;

    return value;
}

// sub_B77B50
void ngl::release_created_texture_name() {
    fixed_string &name = references::created_texture_name.get();

    if (name.text)
        memory::heap::free(name.text);
}

// inlined into sub_9E85D0
void ngl::name_runtime_texture(texture* value, const char* name) {
    fixed_string replacement;

    replacement.text = nullptr;
    replacement.hash = string_hash(hash::djb2(name));

    value->name.assign(replacement);
    value->flags |= runtime_texture_named_target;
}

// inlined into sub_9E7AC0
void ngl::register_runtime_texture(texture* value, const fixed_string &name) {
    value->name.assign(name);

    references::textures.get().insert(value);
}
