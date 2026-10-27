#pragma once

#include "treyarch/ngl/lighting/lighting.hh"
#include "treyarch/ngl/list/render_node.hh"
#include "treyarch/ngl/texture/texture.hh"
#include "treyarch/math/matrix.hh"
#include "treyarch/math/matrix_rot.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace ngl {
    enum e_scene_callback_type : u32 {
        scene_callback_pre  = 0,
        scene_callback_mid  = 1,
        scene_callback_post = 2,
        scene_callback_3    = 3,
        scene_callback_4    = 4
    };

    using scene_callback_function = void (__cdecl*)(void* context);

    struct scene_parameters {
        u32 valid_low;
        u32 valid_high;
        u32 values[1];
    };

    struct scene_callback {
        scene_callback_function function;
        
        void* context;

        void invoke() const {
            if (function)
                function(context);
        }
    };

    enum e_projection_type : u32 {
        projection_orthographic = 0,
        projection_perspective  = 1
    };

    // old_format is never written on PC
    struct locked_texture_node {
        texture*             texture_data;
        u32                  old_format;
        locked_texture_node* next;
    };

    struct scene {
        u32                      cube_map_face;
        u8                       platform_scene_state_004[0x0C];
        math::mat44              projection;
        math::mat44              view;
        math::mat44              device;
        math::rot_tran_mat43     view_to_world;
        math::mat44              view_to_screen;
        math::rot_tran_mat43     world_to_view;
        math::mat44              world_to_screen;
        math::mat44              viewport_to_world;
        math::mat44              ui_to_device;
        math::mat44              derived_matrix_250;
        math::mat44              derived_matrix_290;
        math::mat44              derived_matrix_2d0;
        math::position3          view_position;
        math::dir3               view_direction;
        math::vector4            clip_planes[6];
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
        locked_texture_node*     locked_textures;
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
        math::vector4            clear_color;
        u32                      framebuffer_write_mask;
        u8                       z_write_enabled;
        u8                       z_test_enabled;
        u8                       depth_bias_enabled;
        u8                       derived_matrices_dirty;
        f32                      aspect_ratio;
        f32                      field_of_view;
        f32                      ortho_width;
        f32                      ortho_height;
        f32                      near_plane;
        f32                      far_plane;
        f32                      slope_scale_depth_bias;
        f32                      depth_bias;
        f32                      animation_time;
        f32                      current_animation_time;
        i32                      ifl_frame;
        f32                      scalar_4d4;
        scene_parameters*        parameters;
        u32                      platform_scene_state_4dc;
    };

    scene* set_render_target(texture* target);
    scene* set_z_target(texture* target);
    u32    set_clear_stencil(u32 stencil);
    scene* set_camera_matrix(const math::rot_tran_mat43 &camera_to_world);
    math::rot_tran_mat43* set_world_to_view_matrix(const math::rot_tran_mat43 &world_to_view);
    scene* set_clear_z(f32 depth);
    scene* set_aspect_ratio(f32 ratio);
    scene* set_perspective_matrix(f32 field_of_view, f32 near_plane, f32 far_plane);
    scene* set_ortho_matrix(f32 ortho_width, f32 ortho_height, f32 near_plane, f32 far_plane);

    enum e_scene_parameter_source : u32 {
        scene_parameter_defaults = 0,
        scene_parameter_parent   = 1,
        scene_parameter_root     = 2
    };

    u32               get_scene_parameter_set_size();
    bool              has_scene_parameter(const scene_parameters* parameters, u32 id);
    void*             get_scene_parameter(const scene_parameters* parameters, u32 id);
    void*             find_scene_parameter(const scene_parameters* parameters, u32 id);
    scene_parameters* allocate_scene_parameters();
    void              clear_scene_parameters(scene_parameters* parameters);
    void              copy_scene_parameters(      scene_parameters* destination,
                                            const scene_parameters* source);

    scene* list_begin_scene(e_scene_parameter_source parameter_source);
    void   list_end_scene();
    scene* list_select_scene(scene* value);
    void   sort_scene(scene* value);

    const char* set_scene_name(const char* name);

    locked_texture_node* lock_texture(texture* value);

    void set_scene_callback(e_scene_callback_type   type,
                            scene_callback_function function,
                            void*                   context = nullptr);

    void set_clear_flags(u32 flags);
    void set_clear_color(f32 red, f32 green, f32 blue, f32 alpha);
    void set_z_write_enable(bool enable);
    void set_z_test_enable(bool enable);
    void set_animation_time(f32 time);

    scene* set_scene_option_group_0(bool first, bool second, bool third);
    scene* set_scene_option_group_1(bool first, bool second, bool third);

    scene* set_default_scene_params();
    scene* clear_and_set_default_scene_state(scene* value);

    void calculate_matrices(scene* value);
    void validate_matrices(scene* value);

    math::mat44* get_matrix_world_to_screen(scene* value);

    void apply_matrix_plane(      math::vector4 &out,
                            const math::mat44   &matrix,
                            const math::vector4 &plane);

    bool   is_viewport_override_enabled();
    void   set_viewport(f32 left, f32 top, f32 right, f32 bottom);
    scene* set_scissor(f32 left, f32 top, f32 right, f32 bottom);
    scene* set_pixel_viewport(f32 left, f32 top, f32 right, f32 bottom);
    void   apply_active_viewport();

    void scene_dump_start();

    namespace references {
        inline util::memory_reference<scene*> build_scene      { 0x01115C54 };
        inline util::memory_reference<scene*> root_build_scene { 0x01115C58 };
        inline util::memory_reference<u8>     in_shadow_scene  { 0x01073DFA };

        inline util::memory_reference<u32> scene_parameter_count { 0x011162AC };

        inline util::memory_reference<scene_callback> end_of_render_callback { 0x01115C70 };
        inline util::memory_reference<scene_callback> end_of_frame_callback  { 0x01115C78 };
        inline util::memory_reference<scene_callback> end_of_vblank_callback { 0x01115C80 };
    } // references

    ASSERT_SIZEOF  (scene_callback,           0x08);
    ASSERT_OFFSETOF(scene_callback, function, 0x00);
    ASSERT_OFFSETOF(scene_callback, context,  0x04);

    ASSERT_SIZEOF  (scene,                                  0x4E0);
    ASSERT_OFFSETOF(scene, cube_map_face,                   0x000);
    ASSERT_OFFSETOF(scene, projection,                      0x010);
    ASSERT_OFFSETOF(scene, view,                            0x050);
    ASSERT_OFFSETOF(scene, device,                          0x090);
    ASSERT_OFFSETOF(scene, view_to_world,                   0x0D0);
    ASSERT_OFFSETOF(scene, view_to_screen,                  0x110);
    ASSERT_OFFSETOF(scene, world_to_view,                   0x150);
    ASSERT_OFFSETOF(scene, world_to_screen,                 0x190);
    ASSERT_OFFSETOF(scene, viewport_to_world,               0x1D0);
    ASSERT_OFFSETOF(scene, ui_to_device,                    0x210);
    ASSERT_OFFSETOF(scene, derived_matrix_250,              0x250);
    ASSERT_OFFSETOF(scene, derived_matrix_290,              0x290);
    ASSERT_OFFSETOF(scene, derived_matrix_2d0,              0x2D0);
    ASSERT_OFFSETOF(scene, view_position,                   0x310);
    ASSERT_OFFSETOF(scene, view_direction,                  0x320);
    ASSERT_OFFSETOF(scene, clip_planes,                     0x330);
    ASSERT_OFFSETOF(scene, parent,                          0x390);
    ASSERT_OFFSETOF(scene, next_sibling,                    0x394);
    ASSERT_OFFSETOF(scene, first_child,                     0x398);
    ASSERT_OFFSETOF(scene, last_child,                      0x39C);
    ASSERT_OFFSETOF(scene, name,                            0x3A0);
    ASSERT_OFFSETOF(scene, callbacks,                       0x3A4);
    ASSERT_OFFSETOF(scene, color_target,                    0x3CC);
    ASSERT_OFFSETOF(scene, depth_target,                    0x3D0);
    ASSERT_OFFSETOF(scene, auxiliary_target,                0x3D4);
    ASSERT_OFFSETOF(scene, target_width,                    0x3D8);
    ASSERT_OFFSETOF(scene, target_height,                   0x3DC);
    ASSERT_OFFSETOF(scene, projection_type,                 0x3E0);
    ASSERT_OFFSETOF(scene, opaque_render_list,              0x3E4);
    ASSERT_OFFSETOF(scene, translucent_render_list,         0x3E8);
    ASSERT_OFFSETOF(scene, opaque_render_list_count,        0x3EC);
    ASSERT_OFFSETOF(scene, translucent_render_list_count,   0x3F0);
    ASSERT_OFFSETOF(scene, specialized_render_list_0,       0x3F4);
    ASSERT_OFFSETOF(scene, specialized_render_list_count_0, 0x3F8);
    ASSERT_OFFSETOF(scene, specialized_render_list_1,       0x3FC);
    ASSERT_OFFSETOF(scene, specialized_render_list_count_1, 0x400);
    ASSERT_OFFSETOF(scene, specialized_render_list_2,       0x404);
    ASSERT_OFFSETOF(scene, specialized_render_list_count_2, 0x408);
    ASSERT_OFFSETOF(scene, specialized_render_list_3,       0x40C);
    ASSERT_OFFSETOF(scene, specialized_render_list_count_3, 0x410);
    ASSERT_OFFSETOF(scene, specialized_render_list_4,       0x414);
    ASSERT_OFFSETOF(scene, specialized_render_list_count_4, 0x418);
    ASSERT_OFFSETOF(scene, locked_textures,                 0x41C);
    ASSERT_OFFSETOF(scene, options,                         0x420);
    ASSERT_OFFSETOF(scene, scene_state_424,                 0x424);
    ASSERT_OFFSETOF(scene, scene_state_428,                 0x428);
    ASSERT_OFFSETOF(scene, callback_state,                  0x42C);
    ASSERT_OFFSETOF(scene, local_state_430,                 0x430);
    ASSERT_OFFSETOF(scene, light_context,                   0x434);
    ASSERT_OFFSETOF(scene, viewport_left,                   0x448);
    ASSERT_OFFSETOF(scene, scissor_left,                    0x458);
    ASSERT_OFFSETOF(scene, pixel_scissor_left,              0x468);
    ASSERT_OFFSETOF(scene, clear_flags,                     0x480);
    ASSERT_OFFSETOF(scene, clear_depth,                     0x484);
    ASSERT_OFFSETOF(scene, clear_stencil,                   0x488);
    ASSERT_OFFSETOF(scene, clear_color,                     0x490);
    ASSERT_OFFSETOF(scene, framebuffer_write_mask,          0x4A0);
    ASSERT_OFFSETOF(scene, z_write_enabled,                 0x4A4);
    ASSERT_OFFSETOF(scene, z_test_enabled,                  0x4A5);
    ASSERT_OFFSETOF(scene, depth_bias_enabled,              0x4A6);
    ASSERT_OFFSETOF(scene, derived_matrices_dirty,          0x4A7);
    ASSERT_OFFSETOF(scene, aspect_ratio,                    0x4A8);
    ASSERT_OFFSETOF(scene, near_plane,                      0x4B8);
    ASSERT_OFFSETOF(scene, slope_scale_depth_bias,          0x4C0);
    ASSERT_OFFSETOF(scene, depth_bias,                      0x4C4);
    ASSERT_OFFSETOF(scene, animation_time,                  0x4C8);
    ASSERT_OFFSETOF(scene, current_animation_time,          0x4CC);
    ASSERT_OFFSETOF(scene, ifl_frame,                       0x4D0);
    ASSERT_OFFSETOF(scene, scalar_4d4,                      0x4D4);
    ASSERT_OFFSETOF(scene, parameters,                      0x4D8);
    ASSERT_OFFSETOF(scene, platform_scene_state_4dc,        0x4DC);

    ASSERT_SIZEOF  (locked_texture_node,       0x0C);
    ASSERT_OFFSETOF(locked_texture_node, next, 0x08);
}} // treyarch::ngl
