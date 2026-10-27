#pragma once

#include "game/math/algebra.hh"
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

    struct directional_light_data {
        u32      flags;
        u8       reserved_004[0x1C];
        vector3d direction;
        u8       reserved_02c[0xC4];
    };

    // the light-source scene parameter, as resolved for a position by 0x007D75C0
    struct light_source_data {
        u8                      reserved_000[0x200];
        f32                     exposure;
        u8                      reserved_204[0xCC];
        vector4d                color_2d0;
        f32                     depth_fog_start;
        f32                     depth_fog_end;
        f32                     unk_2e8;
        f32                     unk_2ec;
        directional_light_data* directional_lights;
        u8                      reserved_2f4[0x20];
        i32                     directional_light_count;
        u8                      reserved_318[0x14];
        sky_data*               sky;
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

    ASSERT_SIZEOF  (directional_light_data,            0xF0);
    ASSERT_OFFSETOF(directional_light_data, direction, 0x20);

    ASSERT_OFFSETOF(light_source_data, exposure,                0x200);
    ASSERT_OFFSETOF(light_source_data, color_2d0,               0x2D0);
    ASSERT_OFFSETOF(light_source_data, depth_fog_start,         0x2E0);
    ASSERT_OFFSETOF(light_source_data, unk_2ec,                 0x2EC);
    ASSERT_OFFSETOF(light_source_data, directional_lights,      0x2F0);
    ASSERT_OFFSETOF(light_source_data, directional_light_count, 0x314);
    ASSERT_OFFSETOF(light_source_data, sky,                     0x32C);
} // treyarch
