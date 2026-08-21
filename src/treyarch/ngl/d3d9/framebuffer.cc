#include <cstring>

#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/display.hh"
#include "treyarch/ngl/d3d9/framebuffer.hh"
#include "treyarch/ngl/d3d9/texture.hh"
#include "treyarch/shared/hash/algo.hh"
#include "treyarch/shared/memory/memory.hh"
#include "treyarch/shared/four_cc.hh"

using namespace treyarch;

static constexpr u32 runtime_texture_owned         = 0x00000020;
static constexpr u32 runtime_texture_render_target = 0x00000040;
static constexpr u32 runtime_texture_surface_only  = 0x00000080;
static constexpr u32 runtime_texture_auto_depth    = 0x00000100;
static constexpr u32 runtime_texture_named_target  = 0x00000200;
static constexpr u32 runtime_texture_depth_texture = 0x00002000;
static constexpr u32 runtime_texture_surface_level = 0x00004000;

static bool is_depth_surface_format(D3DFORMAT format) {
    switch ((u32)format) {
        case D3DFMT_D16_LOCKABLE:
        case D3DFMT_D32:
        case D3DFMT_D15S1:
        case D3DFMT_D24S8:
        case D3DFMT_D24X8:
        case D3DFMT_D24X4S4:
        case D3DFMT_D16:
        case D3DFMT_D32F_LOCKABLE:
        case D3DFMT_D24FS8:
        case (D3DFORMAT)four_cc('D', 'F', '2', '4'):
        case (D3DFORMAT)four_cc('D', 'F', '1', '6'):
        case (D3DFORMAT)four_cc('I', 'N', 'T', 'Z'):
        case (D3DFORMAT)four_cc('R', 'A', 'W', 'Z'):
            return true;

        default:
            return false;
    }
}

static void initialize_2d_resource(ngl::d3d9::texture_resource &resource,
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

static void create_surface(IDirect3DSurface9** surface,
                           u32                 width,
                           u32                 height,
                           D3DFORMAT           format) {

    IDirect3DDevice9* device = ngl::d3d9::references::device.get();

    if (is_depth_surface_format(format))
        device->CreateDepthStencilSurface(width,
                                          height,
                                          format,
                                          D3DMULTISAMPLE_NONE,
                                          0,
                                          FALSE,
                                          surface,
                                          nullptr);
    else
        device->CreateRenderTarget(width,
                                   height,
                                   format,
                                   D3DMULTISAMPLE_NONE,
                                   0,
                                   FALSE,
                                   surface,
                                   nullptr);
}

static ngl::texture* create_runtime_texture(u32       flags,
                                            D3DFORMAT format,
                                            u32       width,
                                            u32       height,
                                            u32       level_count) {

    ngl::texture* value = (ngl::texture*)memory::allocate
        (sizeof(ngl::texture), 8, 0);

    std::memset(value, 0, sizeof(ngl::texture));

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

            ngl::d3d9::create_texture_resource(&value->gpu_texture);
            ( (IDirect3DTexture9*) value->gpu_texture.resource )->GetSurfaceLevel(0, &value->render_target);

            value->flags |= runtime_texture_owned;
            value->last_frame_reference = -1;

            return value;
        }

        create_surface(&value->render_target, width, height, format);
    } else
        initialize_2d_resource(value->gpu_texture,
                               width,
                               height,
                               level_count,
                               format,
                               creation_flags);

    ngl::d3d9::create_texture_resource(&value->gpu_texture);
    value->flags |= runtime_texture_owned;

    if (value->flags & runtime_texture_render_target)
        ( (IDirect3DTexture9*) value->gpu_texture.resource )->GetSurfaceLevel(0, &value->render_target);

    if (flags & runtime_texture_auto_depth) {
        u32       depth_flags  = (flags & 0x210) | runtime_texture_surface_only;
        D3DFORMAT depth_format = D3DFMT_D24S8;

        if (flags & runtime_texture_depth_texture) {
            depth_flags = (flags & 0x210) |
                          runtime_texture_surface_only |
                          runtime_texture_surface_level;
            depth_format = ngl::d3d9::references::framebuffers.get().depth_texture_format;
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

static void name_framebuffer(ngl::texture* value, const char* name) {
    value->name.text  = nullptr;
    value->name.hash  = string_hash(hash::djb2(name));
    value->flags     |= runtime_texture_named_target;
}

static bool supports_depth_texture(D3DFORMAT format) {
    return ngl::d3d9::references::d3d9.get()->CheckDeviceFormat
        (0,
         D3DDEVTYPE_HAL,
         D3DFMT_X8R8G8B8,
         D3DUSAGE_DEPTHSTENCIL,
         D3DRTYPE_TEXTURE,
         format) >= 0;
}

ULONG ngl::d3d9::initialize_framebuffers() {
    framebuffer_state &state = references::framebuffers.get();
    display_mode      &mode  = references::selected_display_mode.get();

    state.front_buffer = create_runtime_texture(runtime_texture_render_target,
                                                D3DFMT_A8R8G8B8,
                                                mode.width,
                                                mode.height,
                                                1);
    name_framebuffer(state.front_buffer, "nglFrontBuffer");

    D3DFORMAT df24 = (D3DFORMAT)four_cc('D', 'F', '2', '4');
    D3DFORMAT df16 = (D3DFORMAT)four_cc('D', 'F', '1', '6');
    // apparently, these are OG hacks for ATI / NVIDIA to sample depth buffers as textures?
    D3DFORMAT intz = (D3DFORMAT)four_cc('I', 'N', 'T', 'Z');
    D3DFORMAT rawz = (D3DFORMAT)four_cc('R', 'A', 'W', 'Z');

    bool supports_df24 = supports_depth_texture(df24);
    bool supports_df16 = supports_depth_texture(df16);
    bool supports_intz = supports_depth_texture(intz);
    bool supports_rawz = supports_depth_texture(rawz);

    if (supports_intz)
        state.depth_texture_format = intz;
    else if (supports_rawz)
        state.depth_texture_format = rawz;
    else if (supports_df24)
        state.depth_texture_format = df24;
    else if (supports_df16)
        state.depth_texture_format = df16;

    if (references::particle_depth_texture_requested.read()) {
        state.depth_texture_supported =
            supports_df24 || supports_df16 || supports_intz || supports_rawz;
    }

    u32 back_buffer_flags = runtime_texture_render_target |
                            runtime_texture_auto_depth;

    if (state.depth_texture_supported) {
        state.linear_depth_buffer = create_runtime_texture
            (runtime_texture_render_target,
             D3DFMT_R32F,
             mode.width,
             mode.height,
             1);
        back_buffer_flags |= runtime_texture_depth_texture;
    }

    state.back_buffer = create_runtime_texture(back_buffer_flags,
                                               D3DFMT_A8R8G8B8,
                                               mode.width,
                                               mode.height,
                                               1);
    name_framebuffer(state.back_buffer, "nglBackBuffer");

    state.active_color_buffer = state.back_buffer;
    state.active_depth_buffer = state.back_buffer->depth_target;

    state.secondary_ldr_buffer = create_runtime_texture
        (runtime_texture_render_target | runtime_texture_auto_depth,
         D3DFMT_A8R8G8B8,
         mode.width,
         mode.height,
         1);
    name_framebuffer(state.secondary_ldr_buffer, "nglBackBufferLDR2");

    state.secondary_hdr_buffer = create_runtime_texture
        (runtime_texture_render_target | runtime_texture_auto_depth,
         D3DFMT_A8R8G8B8,
         mode.width,
         mode.height,
         1);
    name_framebuffer(state.secondary_hdr_buffer, "nglBackBufferHDR2");

    IDirect3DSurface9* surface;
    ( (IDirect3DTexture9*) state.back_buffer->gpu_texture.resource )->GetSurfaceLevel(0, &surface);

    references::device.get()->SetRenderTarget(0, surface);
    references::device.get()->Clear(0,
                                    nullptr,
                                    D3DCLEAR_TARGET |
                                    D3DCLEAR_ZBUFFER |
                                    D3DCLEAR_STENCIL,
                                    0,
                                    1.0f,
                                    0);

    return surface->Release();
}
