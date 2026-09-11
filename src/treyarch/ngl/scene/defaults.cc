#include <cstring>

#include "treyarch/ngl/d3d9/display.hh"
#include "treyarch/ngl/d3d9/framebuffer.hh"
#include "treyarch/ngl/lighting/context.hh"
#include "treyarch/ngl/scene/defaults.hh"
#include "treyarch/ngl/scene/references.hh"

using namespace treyarch;

ngl::scene* set_color_target(ngl::texture* target) {
    ngl::scene* value = ngl::references::current_scene.read();

    value->color_target  = target;
    value->cube_map_face = 0;

    if (!target)
        value->options = 0;

    return value;
}

ngl::scene* set_depth_target(ngl::texture* target) {
    ngl::scene* value = ngl::references::current_scene.read();

    value->depth_target = target;

    if (!target)
        value->options = 0;

    return value;
}

ngl::scene* set_auxiliary_target(ngl::texture* target) {
    ngl::scene* value = ngl::references::current_scene.read();

    value->auxiliary_target = target;

    if (!target)
        value->options = 0;

    return value;
}

ngl::scene* set_camera_matrix(const matrix4x4* camera_to_world) {
    ngl::scene* value = ngl::references::current_scene.read();

    value->world_to_view = camera_to_world->inverse_orthonormal();

    value->derived_matrices_dirty = 1;
    
    return value;
}


ngl::scene* ngl::set_default_scene_state() {
    scene* value = references::current_scene.read();
    
    d3d9::framebuffer_state &framebuffers =
        d3d9::references::framebuffers.get();

    value->clear_stencil = 0;
    set_depth_target(framebuffers.active_depth_buffer);

    set_color_target(framebuffers.back_buffer);
    value->derived_matrices_dirty = 1;

    set_auxiliary_target(framebuffers.back_buffer);
    value->auxiliary_target = framebuffers.active_color_buffer;

    value->viewport_left   = -1.0f;
    value->viewport_top    = -1.0f;
    value->viewport_right  = 1.0f;
    value->viewport_bottom = 1.0f;
    value->derived_matrices_dirty = 1;

    value->scissor_left   = -1.0f;
    value->scissor_top    = -1.0f;
    value->scissor_right  = 1.0f;
    value->scissor_bottom = 1.0f;
    value->derived_matrices_dirty = 1;

    value->aspect_ratio = d3d9::references::selected_display_mode.get().widescreen ?
        1.7777777f : 1.3333334f;
    value->derived_matrices_dirty = 1;

    value->field_of_view   = 50.0f;
    value->ortho_width     = 0.0f;
    value->ortho_height    = 0.0f;
    value->projection_type = projection_perspective;
    value->near_plane      = 1.0f;
    value->far_plane       = 10000.0f;

    matrix4x4 camera_to_world;
              camera_to_world.identity();
    set_camera_matrix(&camera_to_world);

    value->clear_flags   = 6;
    value->clear_color.x = 0.0f;
    value->clear_color.y = 0.0f;
    value->clear_color.z = 0.0f;
    value->clear_color.w = 0.0f;
    value->clear_depth   = 1.0f;

    value->animation_time         = 0.0f;
    value->current_animation_time = 0.0f;
    value->framebuffer_write_mask = 0x0F;
    value->z_write_enabled        = 1;
    value->z_test_enabled         = 1;
    value->depth_bias             = 0.0f;
    value->slope_scale_depth_bias = 0.0f;
    value->ifl_frame              = 0;

    lighting::set_scene_context(lighting::references::default_context.read(),
                                value);

    value->scene_state_424 = 1;
    value->scene_state_428 = 1;
    value->callback_state  = nullptr;
    value->scalar_4d4      = 0.75f;
    value->derived_matrices_dirty = 1;

    return value;
}

ngl::scene* ngl::clear_and_set_default_scene_state(scene* value) {
    std::memset(value, 0, sizeof(scene));
    
    return set_default_scene_state();
}
