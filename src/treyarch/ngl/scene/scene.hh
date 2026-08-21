#pragma once

#include "treyarch/ngl/lighting/context.hh"
#include "treyarch/ngl/texture/texture.hh"
#include "treyarch/ngl/math/types/vector4.hh"
#include "treyarch/ngl/math/types/matrix4x4.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl {
    union render_node_sort_key {
        u32 integer;
        f32 floating;
    };

    struct render_node {
        void*                vtable;
        render_node*         next;
        render_node_sort_key sort_key;
    };

    struct scene_parameters {
        u32 valid_low;
        u32 valid_high;
        u32 values[1];
    };

    struct scene_callback {
        void (__cdecl* function)(void* context);
        void* context;
    };

    enum e_projection_type : u32 {
        projection_orthographic = 0,
        projection_perspective  = 1
    };

    struct scene {
        u32                      cube_map_face;
        u8                       platform_scene_state_004[0x0C];
        matrix4x4                projection;
        matrix4x4                view;
        matrix4x4                device;
        matrix4x4                view_to_world;
        matrix4x4                view_to_screen;
        matrix4x4                world_to_view;
        matrix4x4                world_to_screen;
        matrix4x4                viewport_to_world;
        matrix4x4                ui_to_device;
        matrix4x4                derived_matrix_250;
        matrix4x4                derived_matrix_290;
        matrix4x4                derived_matrix_2d0;
        vector4                  view_position;
        vector4                  view_direction;
        vector4                  clip_planes[6];
        scene*                   parent;
        scene*                   next_sibling;
        scene*                   first_child;
        scene*                   last_child;
        const char*              name;
        scene_callback           callbacks[5];
        texture*                 color_target;
        texture*                 depth_target;
        texture*                 auxiliary_target;
        u32                      target_width;
        u32                      target_height;
        e_projection_type        projection_type;
        render_node*             opaque_render_list;
        render_node*             translucent_render_list;
        u32                      opaque_render_list_count;
        u32                      translucent_render_list_count;
        render_node*             specialized_render_list_0;
        u32                      specialized_render_list_count_0;
        render_node*             specialized_render_list_1;
        u32                      specialized_render_list_count_1;
        render_node*             specialized_render_list_2;
        u32                      specialized_render_list_count_2;
        render_node*             specialized_render_list_3;
        u32                      specialized_render_list_count_3;
        render_node*             specialized_render_list_4;
        u32                      specialized_render_list_count_4;
        u32                      local_list_state;
        u32                      options;
        u32                      scene_state_424;
        u32                      scene_state_428;
        void*                    callback_state;
        u32                      local_state_430;
        lighting::light_context* light_context;
        u8                       platform_scene_state_438[0x10];
        f32                      viewport_left;
        f32                      viewport_top;
        f32                      viewport_right;
        f32                      viewport_bottom;
        f32                      scissor_left;
        f32                      scissor_top;
        f32                      scissor_right;
        f32                      scissor_bottom;
        f32                      pixel_scissor_left;
        f32                      pixel_scissor_top;
        f32                      pixel_scissor_right;
        f32                      pixel_scissor_bottom;
        f32                      viewport_half_width;
        f32                      viewport_half_height;
        u32                      clear_flags;
        f32                      clear_depth;
        u32                      clear_stencil;
        u32                      clear_reserved;
        vector4                  clear_color;
        u32                      framebuffer_write_mask;
        u8                       z_write_enabled;
        u8                       z_test_enabled;
        u8                       fog_enabled;
        u8                       derived_matrices_dirty;
        f32                      aspect_ratio;
        f32                      field_of_view;
        f32                      ortho_width;
        f32                      ortho_height;
        f32                      near_plane;
        f32                      far_plane;
        f32                      depth_bias;
        f32                      slope_scale_depth_bias;
        f32                      animation_time;
        f32                      current_animation_time;
        i32                      ifl_frame;
        f32                      scalar_4d4;
        scene_parameters*        parameters;
        u32                      platform_scene_state_4dc;
    };

    ASSERT_SIZEOF  (render_node_sort_key,     0x04);
    ASSERT_SIZEOF  (render_node,              0x0C);
    ASSERT_OFFSETOF(render_node, next,        0x04);
    ASSERT_OFFSETOF(render_node, sort_key,    0x08);
    ASSERT_SIZEOF  (scene_callback,           0x08);
    ASSERT_OFFSETOF(scene_callback, function, 0x00);
    ASSERT_OFFSETOF(scene_callback, context,  0x04);

    ASSERT_SIZEOF  (scene,                            0x4E0);
    ASSERT_OFFSETOF(scene, cube_map_face,             0x000);
    ASSERT_OFFSETOF(scene, projection,                0x010);
    ASSERT_OFFSETOF(scene, view,                      0x050);
    ASSERT_OFFSETOF(scene, device,                    0x090);
    ASSERT_OFFSETOF(scene, view_to_world,             0x0D0);
    ASSERT_OFFSETOF(scene, view_to_screen,            0x110);
    ASSERT_OFFSETOF(scene, world_to_view,             0x150);
    ASSERT_OFFSETOF(scene, world_to_screen,           0x190);
    ASSERT_OFFSETOF(scene, viewport_to_world,         0x1D0);
    ASSERT_OFFSETOF(scene, ui_to_device,              0x210);
    ASSERT_OFFSETOF(scene, derived_matrix_250,        0x250);
    ASSERT_OFFSETOF(scene, derived_matrix_290,        0x290);
    ASSERT_OFFSETOF(scene, derived_matrix_2d0,        0x2D0);
    ASSERT_OFFSETOF(scene, view_position,             0x310);
    ASSERT_OFFSETOF(scene, view_direction,            0x320);
    ASSERT_OFFSETOF(scene, clip_planes,               0x330);
    ASSERT_OFFSETOF(scene, parent,                    0x390);
    ASSERT_OFFSETOF(scene, next_sibling,              0x394);
    ASSERT_OFFSETOF(scene, first_child,               0x398);
    ASSERT_OFFSETOF(scene, last_child,                0x39C);
    ASSERT_OFFSETOF(scene, name,                      0x3A0);
    ASSERT_OFFSETOF(scene, callbacks,                 0x3A4);
    ASSERT_OFFSETOF(scene, color_target,              0x3CC);
    ASSERT_OFFSETOF(scene, depth_target,              0x3D0);
    ASSERT_OFFSETOF(scene, auxiliary_target,          0x3D4);
    ASSERT_OFFSETOF(scene, target_width,              0x3D8);
    ASSERT_OFFSETOF(scene, target_height,             0x3DC);
    ASSERT_OFFSETOF(scene, projection_type,           0x3E0);
    ASSERT_OFFSETOF(scene, opaque_render_list,        0x3E4);
    ASSERT_OFFSETOF(scene, translucent_render_list,   0x3E8);
    ASSERT_OFFSETOF(scene, specialized_render_list_0, 0x3F4);
    ASSERT_OFFSETOF(scene, specialized_render_list_4, 0x414);
    ASSERT_OFFSETOF(scene, local_list_state,          0x41C);
    ASSERT_OFFSETOF(scene, options,                   0x420);
    ASSERT_OFFSETOF(scene, scene_state_424,           0x424);
    ASSERT_OFFSETOF(scene, scene_state_428,           0x428);
    ASSERT_OFFSETOF(scene, callback_state,            0x42C);
    ASSERT_OFFSETOF(scene, local_state_430,           0x430);
    ASSERT_OFFSETOF(scene, light_context,             0x434);
    ASSERT_OFFSETOF(scene, viewport_left,             0x448);
    ASSERT_OFFSETOF(scene, scissor_left,              0x458);
    ASSERT_OFFSETOF(scene, pixel_scissor_left,        0x468);
    ASSERT_OFFSETOF(scene, clear_flags,               0x480);
    ASSERT_OFFSETOF(scene, clear_depth,               0x484);
    ASSERT_OFFSETOF(scene, clear_stencil,             0x488);
    ASSERT_OFFSETOF(scene, clear_color,               0x490);
    ASSERT_OFFSETOF(scene, framebuffer_write_mask,    0x4A0);
    ASSERT_OFFSETOF(scene, z_write_enabled,           0x4A4);
    ASSERT_OFFSETOF(scene, fog_enabled,               0x4A6);
    ASSERT_OFFSETOF(scene, derived_matrices_dirty,    0x4A7);
    ASSERT_OFFSETOF(scene, aspect_ratio,              0x4A8);
    ASSERT_OFFSETOF(scene, near_plane,                0x4B8);
    ASSERT_OFFSETOF(scene, depth_bias,                0x4C0);
    ASSERT_OFFSETOF(scene, animation_time,            0x4C8);
    ASSERT_OFFSETOF(scene, current_animation_time,    0x4CC);
    ASSERT_OFFSETOF(scene, ifl_frame,                 0x4D0);
    ASSERT_OFFSETOF(scene, scalar_4d4,                0x4D4);
    ASSERT_OFFSETOF(scene, parameters,                0x4D8);
    ASSERT_OFFSETOF(scene, platform_scene_state_4dc,  0x4DC);
}} // treyarch::ngl
