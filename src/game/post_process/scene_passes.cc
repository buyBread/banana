#include "game/post_process/post_process.hh"
#include "treyarch/ngl/gpu/d3d9/device.hh"
#include "treyarch/ngl/gpu/d3d9/framebuffer.hh"
#include "treyarch/ngl/gpu/d3d9/state_cache.hh"
#include "treyarch/ngl/gpu/d3d9/texture.hh"
#include "treyarch/ngl/list/render_callback.hh"
#include "treyarch/ngl/scene/scene.hh"
#include "treyarch/ngl/shaders/program_exports.hh"

using namespace treyarch;

// sub_72E7A0
void post_process::render_pause_menu_blur(f32 strength) {
    const blur_taps &horizontal        = references::pause_menu_blur_horizontal_taps.get();
    const blur_taps &vertical          = references::pause_menu_blur_vertical_taps.get();
          blur_taps &scaled_horizontal = references::pause_menu_blur_scaled_horizontal_taps.get();
          blur_taps &scaled_vertical   = references::pause_menu_blur_scaled_vertical_taps.get();

    for (u32 tap = 0; tap < 13; ++tap) {
        scaled_horizontal.taps[tap][0] = (f32)((f64)horizontal.taps[tap][0] * (f64)strength);
        scaled_vertical.taps[tap][1]   = (f32)((f64)vertical.taps[tap][1]   * (f64)strength);
    }

    const ngl::sort_info sorting { ngl::sort_translucent, { 0 } };

    ngl::list_begin_scene(ngl::scene_parameter_defaults);
    ngl::set_scene_name("post_process::render_pause_menu_blur");
    ngl::set_clear_flags(0);
    ngl::set_z_test_enable(false);
    ngl::set_z_write_enable(false);

    device_resource_state &state = references::device_resources.get();

    ngl::lock_texture(state.render_targets[1]);
    ngl::lock_texture(state.render_targets[2]);

    ngl::scene*   current = ngl::references::build_scene.read();
    ngl::texture* source  = current->auxiliary_target ? current->auxiliary_target : current->color_target;

    ngl::lock_texture(source);

    ngl::set_z_target(nullptr);

    ngl::render_callback::list_add_custom_node(draw_pause_menu_blur, nullptr, &sorting);

    ngl::list_end_scene();
}

// sub_72EC20
void post_process::render_zoom_map_effect() {
    const ngl::sort_info sorting { ngl::sort_translucent, { 0 } };

    ngl::list_begin_scene(ngl::scene_parameter_defaults);
    ngl::set_scene_name("post_process::render_zoom_map_effect");
    ngl::set_clear_flags(0);
    ngl::set_z_test_enable(false);
    ngl::set_z_write_enable(false);

    ngl::lock_texture(references::device_resources.get().render_targets[1]);
    ngl::lock_texture(ngl::references::build_scene.read()->color_target);

    ngl::set_z_target(nullptr);

    ngl::render_callback::list_add_custom_node(draw_zoom_map_effect, nullptr, &sorting);

    ngl::list_end_scene();
}

// sub_72E390
void post_process::set_render_target(ngl::texture* target) {
    IDirect3DSurface9* surface = nullptr;

    if (target)
        ((IDirect3DTexture9*)target->gpu_texture.resource)->GetSurfaceLevel(0, &surface);

    IDirect3DDevice9* device = ngl::d3d9::references::dev.get();

    device->SetRenderTarget(0, surface);
    device->SetDepthStencilSurface(nullptr);

    if (surface)
        surface->Release();
}

// sub_72E3F0
void post_process::draw_pause_menu_blur(void*) {
          device_resource_state &state    = references::device_resources.get();
    const auto                  &programs = ngl::shaders::program_exports::sm_bright_filter::programs.get();

    IDirect3DDevice9* device = ngl::d3d9::references::dev.get();

    const f32* weights = &references::pause_menu_blur_weights.get()[0];
    const f32* unknown = &references::unk_00e79590.get()[0];

    ngl::d3d9::set_render_state(D3DRS_CULLMODE, D3DCULL_NONE);
    ngl::d3d9::set_blend_mode(0);

    ngl::texture* back_buffer = ngl::d3d9::get_back_buffer_tex();

    // back buffer into the first half size target
    post_process::set_render_target(state.render_targets[1]);
    ngl::d3d9::set_texture(0, back_buffer, 1, 3);
    ngl::d3d9::set_vertex_program(programs.fullscreen_position_3d_vertex);
    ngl::d3d9::set_pixel_program(programs.sample_texture_1);
    ngl::d3d9::set_vertex_buffer(&state.half_texel_vertex_buffer_0, &state.half_texel_vertex_definition, 0, 0);
    device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

    // horizontal pass into the second one
    post_process::set_render_target(state.render_targets[2]);
    ngl::d3d9::set_texture(0, state.render_targets[1], 1, 3);
    ngl::d3d9::set_pixel_program(programs.gaussian_blur_13_1);
    device->SetPixelShaderConstantF(13, &references::pause_menu_blur_scaled_horizontal_taps.get().taps[0][0], 13);
    device->SetPixelShaderConstantF(0, weights, 13);
    device->SetPixelShaderConstantF(26, unknown, 1);
    ngl::d3d9::set_vertex_buffer(&state.half_texel_vertex_buffer_1, &state.half_texel_vertex_definition, 0, 0);
    device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

    // vertical pass back into the first
    post_process::set_render_target(state.render_targets[1]);
    ngl::d3d9::set_texture(0, state.render_targets[2], 1, 3);
    ngl::d3d9::set_pixel_program(programs.gaussian_blur_13_0);
    device->SetPixelShaderConstantF(13, &references::pause_menu_blur_scaled_vertical_taps.get().taps[0][0], 13);
    device->SetPixelShaderConstantF(0, weights, 13);
    device->SetPixelShaderConstantF(26, unknown, 1);
    ngl::d3d9::set_vertex_buffer(&state.half_texel_vertex_buffer_1, &state.half_texel_vertex_definition, 0, 0);
    device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

    // and the result over the back buffer
    ngl::d3d9::set_blend_mode(0);
    post_process::set_render_target(back_buffer);
    ngl::d3d9::set_texture(0, state.render_targets[1], 1, 3);
    ngl::d3d9::set_pixel_program(programs.sample_texture_1);
    ngl::d3d9::set_vertex_buffer(&state.half_texel_vertex_buffer_0, &state.half_texel_vertex_definition, 0, 0);
    device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
}

// sub_72EA70
void post_process::draw_zoom_map_effect(void*) {
    u32 guard = references::zoom_map_effect_color_guard.read();

    if (!(guard & 1)) {
        references::zoom_map_effect_color_guard.write(guard | 1);

        math::vector4 &color = references::zoom_map_effect_color.get();

        color.set_x(0.75f);
        color.set_y(0.87f);
        color.set_z(1.0f);
        color.set_w(1.0f);
    }

          device_resource_state &state    = references::device_resources.get();
    const auto                  &programs = ngl::shaders::program_exports::sm_bright_filter::programs.get();

    IDirect3DDevice9* device = ngl::d3d9::references::dev.get();

    ngl::d3d9::set_render_state(D3DRS_CULLMODE, D3DCULL_NONE);
    ngl::d3d9::set_blend_mode(0);

    ngl::texture* back_buffer = ngl::d3d9::get_back_buffer_tex();

    post_process::set_render_target(state.render_targets[1]);
    ngl::d3d9::set_texture(0, back_buffer, 1, 3);
    ngl::d3d9::set_vertex_program(programs.fullscreen_position_3d_vertex);
    ngl::d3d9::set_pixel_program(programs.tone_map_desaturated);
    device->SetPixelShaderConstantF(0, &references::zoom_map_effect_color.get()[0], 1);
    ngl::d3d9::set_vertex_buffer(&state.half_texel_vertex_buffer_0, &state.half_texel_vertex_definition, 0, 0);
    device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

    ngl::d3d9::set_blend_mode(0);
    post_process::set_render_target(back_buffer);
    ngl::d3d9::set_texture(0, state.render_targets[1], 1, 3);
    ngl::d3d9::set_pixel_program(programs.sample_texture_1);
    ngl::d3d9::set_vertex_buffer(&state.half_texel_vertex_buffer_0, &state.half_texel_vertex_definition, 0, 0);
    device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
}
