#pragma once

#include "treyarch/ngl/math/types/matrix4x4.hh"
#include "util/macros/sanity_assert.hh"

namespace treyarch { namespace ngl {
    struct texture;
}}

namespace treyarch { namespace ngl { namespace fx {
    // temporary lighting data built for a draw
    struct general_lighting_parameters {
        u8           reserved_000[0x110];
        i32          directional_light_count;
        u8           reserved_114[0x0C];
        vector4      directional_light_direction;
        vector4      directional_light_color;
        vector4      horizon_projection_u;
        vector4      horizon_projection_v;
        vector4      ambient_info;
        vector4      fog_color;
        vector4      fog_control;
        u8*          light_source;
        u8           reserved_194[0x0C];
        i32          light_count;
        u8           reserved_1a4[0x0C];
        vector4      view_position;
        vector4      ambient_direction;
        vector4      reserved_1d0;
        vector4      reserved_1e0;
        vector4      reserved_1f0;
        vector4      reserved_200;
        vector4      light_positions[4];
        vector4      light_directions[4];
        vector4      light_colors[4];
        vector4      light_attenuation[4];
        vector4      horizon_color;
        vector4      horizon_axis;
        vector4      reserved_330;
        vector4      horizon_color_scale;
        vector4      horizon_direction;
        vector4      horizon_range;
        vector4      horizon_direction_scaled;
        vector4      horizon_bias;
        vector4      horizon_scalar;
        vector4      horizon_extra;
        vector4      ambient_colors[8];
        vector4      ambient_colors_with_primary[8];
        vector4      ambient_color;
        matrix4x4    projector_matrix;
        vector4      post_view_position;
        vector4      post_direction;
        vector4      post_plane_0;
        vector4      post_plane_1;
        vector4      post_range;
        vector4      post_color;
        texture*     projector_texture;
        i32          projector_config_0;
        i32          projector_config_1;
        i32          projector_index;
        i32          special_light_index;
        f32          horizon_scale;
        u8           reserved_578[8];
        vector4      ambient_defaults[9];
        texture*     horizon_texture;
    };

    ASSERT_OFFSETOF(general_lighting_parameters, reserved_000, 0x000);
    ASSERT_OFFSETOF(general_lighting_parameters, directional_light_count, 0x110);
    ASSERT_OFFSETOF(general_lighting_parameters, reserved_114, 0x114);
    ASSERT_OFFSETOF(general_lighting_parameters, directional_light_direction, 0x120);
    ASSERT_OFFSETOF(general_lighting_parameters, directional_light_color, 0x130);
    ASSERT_OFFSETOF(general_lighting_parameters, horizon_projection_u, 0x140);
    ASSERT_OFFSETOF(general_lighting_parameters, horizon_projection_v, 0x150);
    ASSERT_OFFSETOF(general_lighting_parameters, ambient_info, 0x160);
    ASSERT_OFFSETOF(general_lighting_parameters, fog_color, 0x170);
    ASSERT_OFFSETOF(general_lighting_parameters, fog_control, 0x180);
    ASSERT_OFFSETOF(general_lighting_parameters, light_source, 0x190);
    ASSERT_OFFSETOF(general_lighting_parameters, reserved_194, 0x194);
    ASSERT_OFFSETOF(general_lighting_parameters, light_count, 0x1A0);
    ASSERT_OFFSETOF(general_lighting_parameters, reserved_1a4, 0x1A4);
    ASSERT_OFFSETOF(general_lighting_parameters, view_position, 0x1B0);
    ASSERT_OFFSETOF(general_lighting_parameters, ambient_direction, 0x1C0);
    ASSERT_OFFSETOF(general_lighting_parameters, reserved_1d0, 0x1D0);
    ASSERT_OFFSETOF(general_lighting_parameters, reserved_1e0, 0x1E0);
    ASSERT_OFFSETOF(general_lighting_parameters, reserved_1f0, 0x1F0);
    ASSERT_OFFSETOF(general_lighting_parameters, reserved_200, 0x200);
    ASSERT_OFFSETOF(general_lighting_parameters, light_positions, 0x210);
    ASSERT_OFFSETOF(general_lighting_parameters, light_directions, 0x250);
    ASSERT_OFFSETOF(general_lighting_parameters, light_colors, 0x290);
    ASSERT_OFFSETOF(general_lighting_parameters, light_attenuation, 0x2D0);
    ASSERT_OFFSETOF(general_lighting_parameters, horizon_color, 0x310);
    ASSERT_OFFSETOF(general_lighting_parameters, horizon_axis, 0x320);
    ASSERT_OFFSETOF(general_lighting_parameters, reserved_330, 0x330);
    ASSERT_OFFSETOF(general_lighting_parameters, horizon_color_scale, 0x340);
    ASSERT_OFFSETOF(general_lighting_parameters, horizon_direction, 0x350);
    ASSERT_OFFSETOF(general_lighting_parameters, horizon_range, 0x360);
    ASSERT_OFFSETOF(general_lighting_parameters, horizon_direction_scaled, 0x370);
    ASSERT_OFFSETOF(general_lighting_parameters, horizon_bias, 0x380);
    ASSERT_OFFSETOF(general_lighting_parameters, horizon_scalar, 0x390);
    ASSERT_OFFSETOF(general_lighting_parameters, horizon_extra, 0x3A0);
    ASSERT_OFFSETOF(general_lighting_parameters, ambient_colors, 0x3B0);
    ASSERT_OFFSETOF(general_lighting_parameters, ambient_colors_with_primary, 0x430);
    ASSERT_OFFSETOF(general_lighting_parameters, ambient_color, 0x4B0);
    ASSERT_OFFSETOF(general_lighting_parameters, projector_matrix, 0x4C0);
    ASSERT_OFFSETOF(general_lighting_parameters, post_view_position, 0x500);
    ASSERT_OFFSETOF(general_lighting_parameters, post_direction, 0x510);
    ASSERT_OFFSETOF(general_lighting_parameters, post_plane_0, 0x520);
    ASSERT_OFFSETOF(general_lighting_parameters, post_plane_1, 0x530);
    ASSERT_OFFSETOF(general_lighting_parameters, post_range, 0x540);
    ASSERT_OFFSETOF(general_lighting_parameters, post_color, 0x550);
    ASSERT_OFFSETOF(general_lighting_parameters, projector_texture, 0x560);
    ASSERT_OFFSETOF(general_lighting_parameters, projector_config_0, 0x564);
    ASSERT_OFFSETOF(general_lighting_parameters, projector_config_1, 0x568);
    ASSERT_OFFSETOF(general_lighting_parameters, projector_index, 0x56C);
    ASSERT_OFFSETOF(general_lighting_parameters, special_light_index, 0x570);
    ASSERT_OFFSETOF(general_lighting_parameters, horizon_scale, 0x574);
    ASSERT_OFFSETOF(general_lighting_parameters, reserved_578, 0x578);
    ASSERT_OFFSETOF(general_lighting_parameters, ambient_defaults, 0x580);
    ASSERT_OFFSETOF(general_lighting_parameters, horizon_texture, 0x610);
    ASSERT_SIZEOF(general_lighting_parameters, 0x614);
}}} // treyarch::ngl::fx
