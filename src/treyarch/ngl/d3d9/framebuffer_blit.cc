#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/framebuffer.hh"
#include "treyarch/ngl/d3d9/internal_programs.hh"
#include "treyarch/ngl/d3d9/state_cache.hh"
#include "treyarch/ngl/d3d9/vertex_formats.hh"
#include "treyarch/ngl/ngl.hh"
#include "treyarch/shared/four_cc.hh"
#include "util/memory_reference.hh"

using namespace treyarch;

struct fullscreen_vertex {
    f32 x, y, z, u, v;
};

static util::memory_reference<u32> texture_animation_frame { 0x01118800 };

static void disable_blending() {
    ngl::d3d9::set_render_state(D3DRS_ALPHABLENDENABLE, FALSE);
    ngl::d3d9::set_render_state(D3DRS_ALPHATESTENABLE, FALSE);
}

static ngl::texture* select_texture_frame(ngl::texture* value) {
    if (value->flags & ngl::texture_animated)
        value = value->frames[texture_animation_frame.read() % value->frame_count];

    i32 frame_epoch = (i32)ngl::references::frame_epoch.read();

    value->last_frame_reference = frame_epoch;

    if (value->owner_file)
        value->owner_file->last_frame_reference = frame_epoch;

    return value;
}

static void bind_blit_texture(ngl::texture* value, bool linear_filter) {
    using namespace ngl::d3d9;

    value = select_texture_frame(value);

    set_texture(0, value->gpu_texture.resource);
    set_sampler_state(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    set_sampler_state(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
    set_sampler_state(0, D3DSAMP_MAGFILTER,
                        linear_filter ? D3DTEXF_LINEAR : D3DTEXF_POINT);
    set_sampler_state(0, D3DSAMP_MINFILTER,
                        linear_filter ? D3DTEXF_LINEAR : D3DTEXF_POINT);
    set_sampler_state(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
    set_sampler_state(0, D3DSAMP_MAXANISOTROPY, 3);
}

static void prepare_fullscreen_draw(ngl::texture*          source,
                                    bool                   linear_filter,
                                    IDirect3DPixelShader9* pixel_program) {

    using namespace ngl::d3d9;

    set_render_state(D3DRS_FILLMODE,     D3DFILL_SOLID);
    set_render_state(D3DRS_CULLMODE,     D3DCULL_NONE);
    set_render_state(D3DRS_ZENABLE,      FALSE);
    set_render_state(D3DRS_ZWRITEENABLE, FALSE);
    set_render_state(D3DRS_FOGENABLE,    FALSE);

    disable_blending();

    bind_blit_texture(source, linear_filter);

    internal_program_set &programs = references::internal_programs.get();

    set_vertex_program(programs.framebuffer_copy_vertex_program);
    set_pixel_program(pixel_program);
}

static void draw_fullscreen_quad(const fullscreen_vertex* vertices) {
    using namespace ngl::d3d9;

    binding_cache &bindings = references::bindings.get();

    bindings.stream_source      = nullptr;
    bindings.vertex_declaration = nullptr;

    IDirect3DDevice9* device = references::device.get();

    device->SetVertexDeclaration(references::quad_position_uv_format.get().declaration);
    device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,
                            2,
                            vertices,
                            sizeof(fullscreen_vertex));
}

void ngl::d3d9::blit_texture(IDirect3DSurface9* destination,
                             texture*           source,
                             bool               linear_filter) {

    DWORD z_write_enabled;

    IDirect3DDevice9* device = references::device.get();

    device->GetRenderState(D3DRS_ZWRITEENABLE, &z_write_enabled);

    prepare_fullscreen_draw(source,
                            linear_filter,
                            references::internal_programs.get().framebuffer_copy_pixel_program);

    device->SetRenderTarget(0, destination);

    f32 half_u = 0.5f / (f32)source->gpu_texture.width;
    f32 half_v = 0.5f / (f32)source->gpu_texture.height;

    fullscreen_vertex vertices[4] {
        { -1.0f,  1.0f, 0.0f, half_u,        half_v        },
        {  1.0f,  1.0f, 0.0f, half_u + 1.0f, half_v        },
        { -1.0f, -1.0f, 0.0f, half_u,        half_v + 1.0f },
        {  1.0f, -1.0f, 0.0f, half_u + 1.0f, half_v + 1.0f }
    };

    draw_fullscreen_quad(vertices);

    set_render_state(D3DRS_ZWRITEENABLE, z_write_enabled);
    set_render_state(D3DRS_ZENABLE, TRUE);

    poison_bindings();
}

void ngl::d3d9::copy_active_depth() {
    framebuffer_state &framebuffers = references::framebuffers.get();

    if (!framebuffers.depth_texture_supported)
        return;

    IDirect3DSurface9* destination = nullptr;

    ( (IDirect3DTexture9*)framebuffers.linear_depth_buffer->gpu_texture.resource )
        ->GetSurfaceLevel(0, &destination);

    IDirect3DDevice9* device = references::device.get();

    IDirect3DSurface9* previous_color = nullptr;
    IDirect3DSurface9* previous_depth = nullptr;
    DWORD              z_write_enabled;

    device->GetRenderTarget(0, &previous_color);
    device->GetDepthStencilSurface(&previous_depth);
    device->GetRenderState(D3DRS_ZWRITEENABLE, &z_write_enabled);

    IDirect3DPixelShader9* pixel_program =
        framebuffers.depth_texture_format == (D3DFORMAT)four_cc('R', 'A', 'W', 'Z') ?
            references::internal_programs.get().rawz_depth_copy_pixel_program :
            references::internal_programs.get().depth_copy_pixel_program;

    prepare_fullscreen_draw(framebuffers.active_depth_buffer, false, pixel_program);

    device->SetDepthStencilSurface(nullptr);
    device->SetRenderTarget(0, destination);

    f32 half_u = 0.5f / (f32)framebuffers.active_depth_buffer->gpu_texture.width;
    f32 half_v = 0.5f / (f32)framebuffers.active_depth_buffer->gpu_texture.height;

    fullscreen_vertex vertices[4] {
        { -1.0f,  1.0f, 0.0f, half_u,        half_v        },
        {  1.0f,  1.0f, 0.0f, half_u + 1.0f, half_v        },
        { -1.0f, -1.0f, 0.0f, half_u,        half_v + 1.0f },
        {  1.0f, -1.0f, 0.0f, half_u + 1.0f, half_v + 1.0f }
    };

    draw_fullscreen_quad(vertices);

    device->SetRenderTarget(0, previous_color);
    previous_color->Release();
    device->SetDepthStencilSurface(previous_depth);
    previous_depth->Release();

    destination->Release();

    set_render_state(D3DRS_ZWRITEENABLE, z_write_enabled);
    set_render_state(D3DRS_ZENABLE, TRUE);

    poison_bindings();
}

void ngl::d3d9::generate_mipmaps(texture* value) {
    if (!value)
        return;

    auto* resource = (IDirect3DTexture9*)value->gpu_texture.resource;
    
    u32 level_count = resource->GetLevelCount();

    if (level_count <= 1)
        return;

    set_render_state(D3DRS_CULLMODE, D3DCULL_NONE);
    disable_blending();
    bind_blit_texture(value, true);

    internal_program_set &programs = references::internal_programs.get();

    set_vertex_program(programs.framebuffer_copy_vertex_program);
    set_pixel_program(programs.framebuffer_copy_pixel_program);

    fullscreen_vertex vertices[4] {
        { -1.0f,  1.0f, 0.0f, 0.0f, 0.0f },
        {  1.0f,  1.0f, 0.0f, 1.0f, 0.0f },
        { -1.0f, -1.0f, 0.0f, 0.0f, 1.0f },
        {  1.0f, -1.0f, 0.0f, 1.0f, 1.0f }
    };

    for (u32 level = 1; level < level_count; ++level) {
        IDirect3DSurface9* surface = nullptr;

        resource->GetSurfaceLevel(level, &surface);
        references::device.get()->SetRenderTarget(0, surface);
        surface->Release();

        draw_fullscreen_quad(vertices);
    }

    poison_bindings();
}
