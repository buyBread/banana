#pragma once

#include "treyarch/ngl/texture/texture.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

/*
    refactor into classes where appropriate (and when)?
*/

namespace treyarch { namespace ngl {
    union render_node_sort_key {
        u32 integer;
        f32 floating;
    };

    ASSERT_SIZEOF(render_node_sort_key, 0x04);

    struct render_node {
        void*                    vtable;
        render_node*         next;
        render_node_sort_key sort_key;
    };

    ASSERT_SIZEOF  (render_node,           0x0C);
    ASSERT_OFFSETOF(render_node, next,     0x04);
    ASSERT_OFFSETOF(render_node, sort_key, 0x08);

    struct light_context {
        u8 storage[0x50];
    };

    ASSERT_SIZEOF(light_context, 0x50);

    struct scene_parameters {
        u32 valid_low;
        u32 valid_high;
        u32 values[1];
    };

    struct scene_callback {
        void (__cdecl* function)(void* context);
        void* context;
    };

    struct scene {
        u32               target_face;
        u8                reserved_004[0x14C];
        f32               world_to_view[16];
        u8                reserved_190[0x200];
        scene*            parent;
        scene*            next_sibling;
        scene*            first_child;
        scene*            last_child;
        const char*       name;
        scene_callback    callbacks[5];
        texture*          color_target;
        texture*          depth_target;
        texture*          auxiliary_target;
        u32               target_width;
        u32               target_height;
        u32               target_scalar;
        render_node*      integer_nodes;
        render_node*      float_nodes;
        u32               integer_node_count;
        u32               float_node_count;
        render_node*      material_integer_nodes;
        u32               material_integer_node_count;
        render_node*      material_float_nodes;
        u32               material_float_node_count;
        render_node*      geometry_float_nodes;
        u32               geometry_float_node_count;
        render_node*      geometry_integer_nodes;
        u32               geometry_integer_node_count;
        render_node*      geometry_staging_nodes;
        u32               geometry_staging_node_count;
        u32               local_state;
        u32               options;
        u32               inherited_state;
        u32               derived_state;
        void*             callback_state;
        u32               local_state_430;
        light_context*    light_context;
        u8                reserved_438[0x10];
        f32               viewport_left;
        f32               viewport_top;
        f32               viewport_right;
        f32               viewport_bottom;
        f32               scissor_left;
        f32               scissor_top;
        f32               scissor_right;
        f32               scissor_bottom;
        u8                reserved_468[0x18];
        u32               clear_flags;
        f32               clear_depth;
        u32               clear_stencil;
        u32               reserved_48c;
        f32               clear_color[4];
        u32               framebuffer_write_mask;
        u8                depth_mode_a;
        u8                depth_mode_b;
        u8                depth_bias_enabled;
        u8                derived_matrices_dirty;
        f32               aspect_ratio;
        f32               field_of_view;
        f32               ortho_width;
        f32               ortho_height;
        f32               near_plane;
        f32               far_plane;
        f32               depth_bias;
        f32               slope_scale_depth_bias;
        f32               animation_time_source;
        f32               animation_time;
        i32               animation_frame;
        u32               reserved_4d4;
        scene_parameters* parameters;
        u32               reserved_4dc;
    };
}}