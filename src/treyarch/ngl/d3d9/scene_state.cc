#include <d3d9.h>

#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/scene_state.hh"
#include "treyarch/ngl/d3d9/state_cache.hh"
#include "treyarch/ngl/timing/frame_timer.hh"
#include "util/memory_reference.hh"

using namespace treyarch;

static util::memory_reference<u8> rendering_to_texture           { 0x00F53D50 };
static util::memory_reference<u8> fixed_twenty_millisecond_frame { 0x01118559 };

static DWORD pack_color(const ngl::vector4 &color) {
    u32 red   = (u32)(color.x * 255.0f);
    u32 green = (u32)(color.y * 255.0f);
    u32 blue  = (u32)(color.z * 255.0f);
    u32 alpha = (u32)(color.w * 255.0f);

    return blue | green << 8 | red << 16 | alpha << 24;
}

static LONG normalized_to_pixel(f32 value, u32 extent) {
    return (LONG)((value * 0.5f + 0.5f) * (f32)extent + 0.5f);
}

void ngl::d3d9::bind_scene_targets(scene* value) {
    IDirect3DSurface9* color_surface = nullptr;

    if (value->color_target) {
        texture* color_target = value->color_target;

        if (color_target->flags & texture_cube) {
            ( (IDirect3DCubeTexture9*)color_target->gpu_texture.resource )
                ->GetCubeMapSurface((D3DCUBEMAP_FACES)value->cube_map_face, 0, &color_surface);
        } else {
            ( (IDirect3DTexture9*)color_target->gpu_texture.resource )
                ->GetSurfaceLevel(0, &color_surface);
        }
    }

    IDirect3DSurface9* depth_surface = nullptr;

    if (value->depth_target)
        depth_surface = value->depth_target->render_target;
    else if (value->color_target &&
             value->color_target->depth_target) {

        depth_surface = value->color_target->depth_target->render_target;
    }

    if (color_surface) {
        if (!rendering_to_texture.read())
            set_render_state(D3DRS_COLORWRITEENABLE, 0x0F);

        rendering_to_texture.write(1);
    } else {
        if (rendering_to_texture.read())
            set_render_state(D3DRS_COLORWRITEENABLE, 0);

        rendering_to_texture.write(0);
    }

    IDirect3DDevice9* device = references::device.get();

    if (color_surface)
        device->SetRenderTarget(0, color_surface);

    device->SetDepthStencilSurface(depth_surface);

    if (color_surface)
        color_surface->Release();
}

void ngl::d3d9::apply_scene_state(scene* value) {
    if (value->animation_time == 0.0f) {
        f32 frame_milliseconds = fixed_twenty_millisecond_frame.read() ?
            20.0f : 16.666666f;

        value->current_animation_time =
            frame_milliseconds * (f32)timing::references::list_tick.read() / 1000.0f;
    } else
        value->current_animation_time = value->animation_time;

    bind_scene_targets(value);

    u32 target_width  = value->target_width  ? value->target_width  : 1;
    u32 target_height = value->target_height ? value->target_height : 1;

    D3DVIEWPORT9 viewport {
        0,
        0,
        target_width,
        target_height,
        0.0f,
        1.0f
    };

    IDirect3DDevice9* device = references::device.get();

    device->SetViewport(&viewport);

    if (value->scissor_left   == -1.0f &&
        value->scissor_top    == -1.0f &&
        value->scissor_right  ==  1.0f &&
        value->scissor_bottom ==  1.0f) {

        set_render_state(D3DRS_SCISSORTESTENABLE, FALSE);
    } else {
        RECT rectangle {
            normalized_to_pixel(value->scissor_left,   target_width),
            normalized_to_pixel(value->scissor_top,    target_height),
            normalized_to_pixel(value->scissor_right,  target_width),
            normalized_to_pixel(value->scissor_bottom, target_height)
        };

        set_render_state(D3DRS_SCISSORTESTENABLE, TRUE);
        
        device->SetScissorRect(&rectangle);
    }

    if (value->clear_flags)
        device->Clear(0,
                      nullptr,
                      value->clear_flags,
                      pack_color(value->clear_color),
                      value->clear_depth,
                      value->clear_stencil);

    if (value->z_write_enabled || value->z_test_enabled) {
        set_render_state(D3DRS_ZENABLE, TRUE);

        if (value->z_test_enabled)
            set_render_state(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
        else {
            DWORD stencil_enabled;

            device->GetRenderState(D3DRS_STENCILENABLE, &stencil_enabled);

            if (stencil_enabled)
                set_render_state(D3DRS_STENCILENABLE, FALSE);
            else {
                set_render_state(D3DRS_STENCILENABLE, TRUE);
                set_render_state(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
                set_render_state(D3DRS_STENCILREF, 255);
                set_render_state(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
            }

            set_render_state(D3DRS_ZFUNC, D3DCMP_ALWAYS);
        }
    } else
        set_render_state(D3DRS_ZENABLE, FALSE);

    set_render_state(D3DRS_COLORWRITEENABLE, value->framebuffer_write_mask);

    value->ifl_frame = (i32)(value->current_animation_time * 30.0f);
}
