#include "retail.hh"
#include "treyarch/game/post_process/post_process.hh"
#include "treyarch/ngl/list/render_callback.hh"
#include "treyarch/ngl/scene/lifecycle.hh"
#include "treyarch/ngl/scene/references.hh"
#include "treyarch/ngl/scene/scene.hh"

using namespace treyarch;

// sub_72E7A0
void post_process::render_pause_menu_blur(f32 strength) {
    const blur_taps &horizontal        = references::pause_menu_blur_horizontal_taps.get();
    const blur_taps &vertical          = references::pause_menu_blur_vertical_taps.get();
          blur_taps &scaled_horizontal = references::pause_menu_blur_scaled_horizontal_taps.get();
          blur_taps &scaled_vertical   = references::pause_menu_blur_scaled_vertical_taps.get();

    for (u32 tap = 0; tap < 13; ++tap) {
        scaled_horizontal.taps[tap].x = (f32)((f64)horizontal.taps[tap].x * (f64)strength);
        scaled_vertical.taps[tap].y   = (f32)((f64)vertical.taps[tap].y   * (f64)strength);
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

    ngl::scene*   current = ngl::references::current_scene.read();
    ngl::texture* source  = current->auxiliary_target ? current->auxiliary_target : current->color_target;

    ngl::lock_texture(source);

    ngl::set_depth_target(nullptr);

    ngl::render_callback::list_add_custom_node((ngl::render_callback::function)retail::sub_72E3F0, nullptr, &sorting);

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
    ngl::lock_texture(ngl::references::current_scene.read()->color_target);

    ngl::set_depth_target(nullptr);

    ngl::render_callback::list_add_custom_node((ngl::render_callback::function)retail::sub_72EA70, nullptr, &sorting);

    ngl::list_end_scene();
}
