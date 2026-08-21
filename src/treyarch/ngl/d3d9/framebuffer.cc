#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/display.hh"
#include "treyarch/ngl/d3d9/framebuffer.hh"
#include "treyarch/ngl/texture/runtime.hh"
#include "treyarch/shared/four_cc.hh"

using namespace treyarch;

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

    state.front_buffer = ngl::create_runtime_texture(runtime_texture_render_target,
                                                     D3DFMT_A8R8G8B8,
                                                     mode.width,
                                                     mode.height,
                                                     1);
    ngl::name_runtime_texture(state.front_buffer, "nglFrontBuffer");

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
        state.linear_depth_buffer = ngl::create_runtime_texture
            (runtime_texture_render_target,
             D3DFMT_R32F,
             mode.width,
             mode.height,
             1);
        back_buffer_flags |= runtime_texture_depth_texture;
    }

    state.back_buffer = ngl::create_runtime_texture(back_buffer_flags,
                                                    D3DFMT_A8R8G8B8,
                                                    mode.width,
                                                    mode.height,
                                                    1);
    ngl::name_runtime_texture(state.back_buffer, "nglBackBuffer");

    state.active_color_buffer = state.back_buffer;
    state.active_depth_buffer = state.back_buffer->depth_target;

    state.secondary_ldr_buffer = ngl::create_runtime_texture
        (runtime_texture_render_target | runtime_texture_auto_depth,
         D3DFMT_A8R8G8B8,
         mode.width,
         mode.height,
         1);
    ngl::name_runtime_texture(state.secondary_ldr_buffer, "nglBackBufferLDR2");

    state.secondary_hdr_buffer = ngl::create_runtime_texture
        (runtime_texture_render_target | runtime_texture_auto_depth,
         D3DFMT_A8R8G8B8,
         mode.width,
         mode.height,
         1);
    ngl::name_runtime_texture(state.secondary_hdr_buffer, "nglBackBufferHDR2");

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
