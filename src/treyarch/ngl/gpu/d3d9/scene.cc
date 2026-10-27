#include <bit>
#include <d3d9.h>

#include "treyarch/ngl/debug/debug.hh"
#include "treyarch/ngl/fx/batch_renderer.hh"
#include "treyarch/ngl/gpu/d3d9/device.hh"
#include "treyarch/ngl/gpu/d3d9/framebuffer.hh"
#include "treyarch/ngl/gpu/d3d9/scene.hh"
#include "treyarch/ngl/gpu/d3d9/state_cache.hh"
#include "treyarch/ngl/list/list.hh"
#include "treyarch/ngl/scene/scene.hh"
#include "treyarch/ngl/timing/frame_timer.hh"
#include "game/color.hh"
#include "banana/util/memory_reference.hh"

namespace treyarch { namespace ngl { namespace d3d9 { namespace references {
    util::memory_reference<u8> rendering_to_texture           { 0x00F53D50 };
    util::memory_reference<u8> fixed_twenty_millisecond_frame { 0x01118559 };
}}}} // treyarch::ngl::d3d9::references

using namespace treyarch;

// sub_9E8FE0
ngl::scene* ngl::d3d9::ngli_set_render_target(texture* target) {
    scene* build_scene = ngl::references::build_scene.read();

    build_scene->color_target  = target;
    build_scene->cube_map_face = 0;

    if (!target)
        build_scene->options = 0;

    return build_scene;
}

// sub_9E9000
ngl::scene* ngl::d3d9::set_auxiliary_target(texture* target) {
    scene* build_scene = ngl::references::build_scene.read();

    build_scene->auxiliary_target = target;

    if (!target)
        build_scene->options = 0;

    return build_scene;
}

// sub_9E9020
ngl::scene* ngl::d3d9::ngli_set_z_target(texture* target) {
    scene* build_scene = ngl::references::build_scene.read();

    build_scene->depth_target = target;

    if (!target)
        build_scene->options = 0;

    return build_scene;
}

// sub_9E9A80
void ngl::d3d9::set_default_depth_target() {
    ngl::set_clear_stencil(0);
    ngl::set_z_target(references::framebuffers.get().active_depth_buffer);
}

LONG normalized_to_pixel(f32 value, u32 extent) {
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
        if (!references::rendering_to_texture.read())
            set_render_state(D3DRS_COLORWRITEENABLE, 0x0F);

        references::rendering_to_texture.write(1);
    } else {
        if (references::rendering_to_texture.read())
            set_render_state(D3DRS_COLORWRITEENABLE, 0);

        references::rendering_to_texture.write(0);
    }

    IDirect3DDevice9* device = references::dev.get();

    if (color_surface)
        device->SetRenderTarget(0, color_surface);

    device->SetDepthStencilSurface(depth_surface);

    if (color_surface)
        color_surface->Release();
}

// sub_9E9250
void ngl::d3d9::setup_scene(scene* value) {
    if (value->animation_time == 0.0f) {
        f32 frame_milliseconds = references::fixed_twenty_millisecond_frame.read() ?
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

    IDirect3DDevice9* device = references::dev.get();

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
                      (DWORD)pack_color(value->clear_color),
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

// sub_9E97E0
void ngl::d3d9::render_scene(scene* value) {
    ++references::scene_recursion.get();

    ngl::references::build_scene.write(value);

    value->callbacks[0].invoke();

    // children render first
    for (scene* child = value->first_child; child; child = child->next_sibling)
        render_scene(child);

    ngl::references::build_scene.write(value);

    if (!value->color_target && !value->depth_target) {
        --references::scene_recursion.get();

        return;
    }

    validate_matrices(value);
    setup_scene(value);
    value->callbacks[1].invoke();

    bool depth_bias_changed = false;

    if (value->depth_bias_enabled && (value->depth_bias != 0.0f || value->slope_scale_depth_bias != 0.0f)) {
        depth_bias_changed = true;

        set_render_state(D3DRS_DEPTHBIAS,
                         std::bit_cast<DWORD>(value->depth_bias));
        set_render_state(D3DRS_SLOPESCALEDEPTHBIAS,
                         std::bit_cast<DWORD>(value->slope_scale_depth_bias));
    }

    if (value->specialized_render_list_4)
        fx::render_batch(value->specialized_render_list_4);

    if (value->opaque_render_list_count)
        render_nodes(value->opaque_render_list);

    // copy depth before drawing translucent nodes
    if (value->options & 0x40)
        copy_active_depth();

    value->callbacks[2].invoke();

    if (value->translucent_render_list_count)
        render_nodes(value->translucent_render_list);

    if (depth_bias_changed) {
        set_render_state(D3DRS_DEPTHBIAS, 0);
        set_render_state(D3DRS_SLOPESCALEDEPTHBIAS, 0);
    }

    // list 3 is only drawn when list 2 is empty
    if (value->specialized_render_list_count_3 && !value->specialized_render_list_count_2)
        render_nodes(value->specialized_render_list_3);

    value->callbacks[3].invoke();
    value->callbacks[4].invoke();

    if (value->color_target && value->color_target->gpu_texture.level_count > 1)
        ngli_gen_mipmaps(value->color_target);

    --references::scene_recursion.get();

    ngl::references::perf_info.get().node_count +=
        value->opaque_render_list_count + value->translucent_render_list_count;
}
