#include <algorithm>
#include <cstring>

#include "treyarch/ngl/display.hh"
#include "treyarch/ngl/gpu/d3d9/framebuffer.hh"
#include "treyarch/ngl/gpu/d3d9/scene.hh"
#include "treyarch/ngl/lighting/lighting.hh"
#include "treyarch/ngl/list/list.hh"
#include "treyarch/ngl/scene/scene.hh"
#include "banana/util/memory_reference.hh"

namespace treyarch { namespace ngl { namespace references {
    util::memory_reference<f32*> viewport_tops    { 0x01115C40 };
    util::memory_reference<f32*> viewport_bottoms { 0x01115C44 };
    util::memory_reference<f32*> viewport_rights  { 0x01115C48 };
    util::memory_reference<f32*> viewport_lefts   { 0x01115C4C };

    util::memory_reference<u32> active_viewport_index     { 0x01115C8C };
    util::memory_reference<u8>  viewport_override_enabled { 0x01115C98 };
}}} // treyarch::ngl::references

using namespace treyarch;

// sub_9D54C0
ngl::scene* ngl::set_render_target(ngl::texture* target) {
    ngl::scene* build_scene = d3d9::ngli_set_render_target(target);

    ngl::references::build_scene.read()->derived_matrices_dirty = 1;

    return build_scene;
}

// sub_9D3810
ngl::scene* ngl::set_z_target(ngl::texture* target) {
    return d3d9::ngli_set_z_target(target);
}

// sub_9D7D30
ngl::scene* ngl::set_camera_matrix(const math::rot_tran_mat43 &camera_to_world) {
    ngl::scene* build_scene = ngl::references::build_scene.read();

    build_scene->world_to_view = math::inv(camera_to_world);
    build_scene->derived_matrices_dirty = 1;
    
    return build_scene;
}

// sub_9D7D00
math::rot_tran_mat43* ngl::set_world_to_view_matrix(const math::rot_tran_mat43 &world_to_view) {
    scene* build_scene = references::build_scene.read();

    build_scene->world_to_view = world_to_view;
    build_scene->derived_matrices_dirty = 1;

    return &build_scene->world_to_view;
}

// sub_9D3820
u32 ngl::set_clear_stencil(u32 stencil) {
    ngl::references::build_scene.read()->clear_stencil = stencil;

    return stencil;
}

// sub_9D38C0
ngl::scene* ngl::set_clear_z(f32 depth) {
    ngl::scene* build_scene = ngl::references::build_scene.read();

    build_scene->clear_depth = depth;

    return build_scene;
}

// sub_9D7C10
ngl::scene* ngl::set_aspect_ratio(f32 ratio) {
    ngl::scene* build_scene = ngl::references::build_scene.read();

    build_scene->aspect_ratio = ratio;
    build_scene->derived_matrices_dirty = 1;

    return build_scene;
}

// sub_9D7C40
ngl::scene* ngl::set_perspective_matrix(f32 field_of_view,
                                        f32 near_plane,
                                        f32 far_plane) {

    ngl::scene* build_scene = ngl::references::build_scene.read();

    build_scene->field_of_view          = field_of_view;
    build_scene->ortho_width            = 0.0f;
    build_scene->ortho_height           = 0.0f;
    build_scene->near_plane             = near_plane;
    build_scene->projection_type        = projection_perspective;
    build_scene->far_plane              = far_plane;
    build_scene->derived_matrices_dirty = 1;

    return build_scene;
}

// sub_9D7CA0
ngl::scene* ngl::set_ortho_matrix(f32 ortho_width,
                                  f32 ortho_height,
                                  f32 near_plane,
                                  f32 far_plane) {
                                        
    ngl::scene* build_scene = ngl::references::build_scene.read();

    build_scene->field_of_view   = 0.0f;
    build_scene->ortho_width     = ortho_width;
    build_scene->ortho_height    = ortho_height;
    build_scene->near_plane      = near_plane;
    build_scene->projection_type = projection_orthographic;
    build_scene->far_plane       = far_plane;
    build_scene->derived_matrices_dirty = 1;

    return build_scene;
}

/*
    two words say which values are present, followed by the values themselves.
    a value can still be present when it's zero, so checking for it and reading it have to stay separate.
*/
bool ngl::has_scene_parameter(const scene_parameters* parameters, u32 id) {
    const u32* words = (const u32*)parameters;

    return (words[id >> 5] & (1u << (id & 31))) != 0;
}

void* ngl::get_scene_parameter(const scene_parameters* parameters, u32 id) {
    return (void*)parameters->values[id];
}

void* ngl::find_scene_parameter(const scene_parameters* parameters, u32 id) {
    return has_scene_parameter(parameters, id) ?
        get_scene_parameter(parameters, id) : nullptr;
}

u32 ngl::get_scene_parameter_set_size() {
    return 4 * references::scene_parameter_count.read() + 8;
}

ngl::scene_parameters* ngl::allocate_scene_parameters() {
    return (scene_parameters*)list_alloc(get_scene_parameter_set_size(), 16);
}

void ngl::clear_scene_parameters(scene_parameters* parameters) {
    parameters->valid_low  = 0;
    parameters->valid_high = 0;
}

void ngl::copy_scene_parameters(      scene_parameters* destination,
                                const scene_parameters* source) {

    std::memcpy(destination, source, get_scene_parameter_set_size());
}

// sub_9D8CC0
ngl::scene* ngl::set_default_scene_params() {
    scene* value = references::build_scene.read();
    
    d3d9::framebuffer_state &framebuffers =
        d3d9::references::framebuffers.get();

    d3d9::set_default_depth_target();

    d3d9::ngli_set_render_target(framebuffers.back_buffer);
    value->derived_matrices_dirty = 1;

    d3d9::set_auxiliary_target(framebuffers.back_buffer);
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

    value->aspect_ratio = is_display_widescreen() ?
        1.7777777f : 1.3333334f;
    value->derived_matrices_dirty = 1;

    value->field_of_view   = 50.0f;
    value->ortho_width     = 0.0f;
    value->ortho_height    = 0.0f;
    value->projection_type = projection_perspective;
    value->near_plane      = 1.0f;
    value->far_plane       = 10000.0f;

    set_camera_matrix(math::identity_mat33());

    value->clear_flags = 6;
    value->clear_color.set_x(0.0f);
    value->clear_color.set_y(0.0f);
    value->clear_color.set_z(0.0f);
    value->clear_color.set_w(0.0f);
    value->clear_depth = 1.0f;

    value->animation_time         = 0.0f;
    value->current_animation_time = 0.0f;
    value->framebuffer_write_mask = 0x0F;
    value->z_write_enabled        = 1;
    value->z_test_enabled         = 1;
    value->depth_bias             = 0.0f;
    value->slope_scale_depth_bias = 0.0f;
    value->ifl_frame              = 0;

    lighting::set_light_context(lighting::references::default_light_context.read(),
                                value);

    value->scene_state_424 = 1;
    value->scene_state_428 = 1;
    value->callback_state  = nullptr;
    value->scalar_4d4      = 0.75f;
    value->derived_matrices_dirty = 1;

    return value;
}

// sub_9E9AA0
ngl::scene* ngl::clear_and_set_default_scene_state(scene* value) {
    std::memset(value, 0, sizeof(scene));
    
    return set_default_scene_params();
}

bool ngl::is_viewport_override_enabled() {
    return references::viewport_override_enabled.read() != 0;
}

// sub_9D79A0
void ngl::set_viewport(f32 left, f32 top, f32 right, f32 bottom) {
    scene* value = references::build_scene.read();

    value->viewport_left   = left;
    value->viewport_top    = top;
    value->viewport_right  = right;
    value->viewport_bottom = bottom;

    references::build_scene.get()->derived_matrices_dirty = true;
}

// sub_9D79F0
ngl::scene* ngl::set_scissor(f32 left, f32 top, f32 right, f32 bottom) {
    scene* value = references::build_scene.read();

    value->scissor_left   = std::clamp(left,   -1.0f, 1.0f);
    value->scissor_top    = std::clamp(top,    -1.0f, 1.0f);
    value->scissor_right  = std::clamp(right,  -1.0f, 1.0f);
    value->scissor_bottom = std::clamp(bottom, -1.0f, 1.0f);
    value->derived_matrices_dirty = 1;

    return value;
}

// sub_9D7AA0
ngl::scene* ngl::set_pixel_viewport(f32 left, f32 top, f32 right, f32 bottom) {
    scene* value = references::build_scene.read();

    validate_matrices(value);

    f32 target_width  = (f32)value->target_width;
    f32 target_height = (f32)value->target_height;

    f32 viewport_left = (f32)((f64)left / target_width  * 2.0 - 1.0);
    f32 viewport_top  = (f32)((f64)top  / target_height * 2.0 - 1.0);
    f32 viewport_right  = (f32)(((f64)right  + 1.0) / target_width  * 2.0 - 1.0);
    f32 viewport_bottom = (f32)(((f64)bottom + 1.0) / target_height * 2.0 - 1.0);

    set_viewport(viewport_left, viewport_top, viewport_right, viewport_bottom);

    return set_scissor(viewport_left, viewport_top, viewport_right, viewport_bottom);
}

// sub_9D8AF0
void ngl::apply_active_viewport() {
    u32 active_viewport_index = references::active_viewport_index.read();

    f32 top    = references::viewport_tops   .read()[active_viewport_index];
    f32 right  = references::viewport_rights .read()[active_viewport_index];
    f32 bottom = references::viewport_bottoms.read()[active_viewport_index];
    f32 left   = references::viewport_lefts  .read()[active_viewport_index];

    set_viewport(left, top, right, bottom);
}

// sub_9DA420
void ngl::sort_scene(scene* value) {
    for (scene* child = value->first_child; child; child = child->next_sibling)
        sort_scene(child);

    sort_list(value->opaque_render_list,        value->opaque_render_list_count,        opaque_compare());
    sort_list(value->specialized_render_list_0, value->specialized_render_list_count_0, opaque_compare());
    sort_list(value->specialized_render_list_3, value->specialized_render_list_count_3, opaque_compare());
    sort_list(value->specialized_render_list_2, value->specialized_render_list_count_2, trans_compare());
    sort_list(value->translucent_render_list,   value->translucent_render_list_count,   trans_compare());
    sort_list(value->specialized_render_list_1, value->specialized_render_list_count_1, trans_compare());
}

void ngl::scene_dump_start() {
    // maybe impl
}
