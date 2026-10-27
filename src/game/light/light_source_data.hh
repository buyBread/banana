#pragma once

#include "game/math/algebra.hh"
#include "treyarch/ngl/lighting/lighting.hh"
#include "treyarch/math/matrix.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    struct sky_data {
        u8       reserved_000[0x40];
        vector4d color_040;
        vector4d direction_050;
        vector4d unk_060;
        vector4d direction_070;
        vector4d unk_080;
        u8       reserved_090[0x20];
        vector4d unk_0b0;
        u8       reserved_0c0[0x08];
        f32      unk_0c8;
        u8       reserved_0cc[0x0C];
        void*    resource_0d8;
        void*    resource_0dc;
        vector4d color_0e0;
        u8       reserved_0f0[0x20];
        vector4d color_110;
    };

    // the light-source scene parameter, as resolved for a position by 0x007D75C0
    struct light_source_data {
        vector4d                             ambient_color_lo[8];
        vector4d                             ambient_color_hi[8];
        math::mat43                          light_matrix;
        u8                                   reserved_140[0xC0];
        f32                                  exposure;
        u8                                   reserved_204[0x0C];
        vector4d                             world_to_ambient;
        vector4d                             ambient_direction;
        vector4d                             fog_normal;
        vector4d                             fog_position_0;
        vector4d                             fog_position_1;
        vector4d                             color_260;
        f32                                  unk_270;
        f32                                  unk_274;
        f32                                  unk_278;
        f32                                  unk_27c;
        u8                                   reserved_280[0x50];
        vector4d                             depth_fog_color;
        f32                                  depth_fog_start;
        f32                                  depth_fog_end;
        f32                                  unk_2e8;
        f32                                  unk_2ec;
        ngl::lighting::generated_light_data* directional_lights;
        ngl::lighting::generated_light_data* generated_lights[4];
        i32                                  generated_light_counts[4];
        i32                                  directional_light_count;
        f32                                  fogs_per_meter;
        f32                                  unk_31c;
        u8                                   reserved_320[0x0C];
        sky_data*                            sky;
    };

    ASSERT_OFFSETOF(sky_data, color_040,     0x040);
    ASSERT_OFFSETOF(sky_data, direction_050, 0x050);
    ASSERT_OFFSETOF(sky_data, unk_060,       0x060);
    ASSERT_OFFSETOF(sky_data, direction_070, 0x070);
    ASSERT_OFFSETOF(sky_data, unk_080,       0x080);
    ASSERT_OFFSETOF(sky_data, unk_0b0,       0x0B0);
    ASSERT_OFFSETOF(sky_data, unk_0c8,       0x0C8);
    ASSERT_OFFSETOF(sky_data, resource_0d8,  0x0D8);
    ASSERT_OFFSETOF(sky_data, resource_0dc,  0x0DC);
    ASSERT_OFFSETOF(sky_data, color_0e0,     0x0E0);
    ASSERT_OFFSETOF(sky_data, color_110,     0x110);

    ASSERT_OFFSETOF(light_source_data, ambient_color_lo,        0x000);
    ASSERT_OFFSETOF(light_source_data, ambient_color_hi,        0x080);
    ASSERT_OFFSETOF(light_source_data, light_matrix,            0x100);
    ASSERT_OFFSETOF(light_source_data, exposure,                0x200);
    ASSERT_OFFSETOF(light_source_data, world_to_ambient,        0x210);
    ASSERT_OFFSETOF(light_source_data, ambient_direction,       0x220);
    ASSERT_OFFSETOF(light_source_data, fog_normal,              0x230);
    ASSERT_OFFSETOF(light_source_data, fog_position_0,          0x240);
    ASSERT_OFFSETOF(light_source_data, fog_position_1,          0x250);
    ASSERT_OFFSETOF(light_source_data, color_260,               0x260);
    ASSERT_OFFSETOF(light_source_data, unk_270,                 0x270);
    ASSERT_OFFSETOF(light_source_data, unk_27c,                 0x27C);
    ASSERT_OFFSETOF(light_source_data, depth_fog_color,         0x2D0);
    ASSERT_OFFSETOF(light_source_data, depth_fog_start,         0x2E0);
    ASSERT_OFFSETOF(light_source_data, unk_2ec,                 0x2EC);
    ASSERT_OFFSETOF(light_source_data, directional_lights,      0x2F0);
    ASSERT_OFFSETOF(light_source_data, generated_lights,        0x2F4);
    ASSERT_OFFSETOF(light_source_data, generated_light_counts,  0x304);
    ASSERT_OFFSETOF(light_source_data, directional_light_count, 0x314);
    ASSERT_OFFSETOF(light_source_data, fogs_per_meter,          0x318);
    ASSERT_OFFSETOF(light_source_data, unk_31c,                 0x31C);
    ASSERT_OFFSETOF(light_source_data, sky,                     0x32C);
} // treyarch
