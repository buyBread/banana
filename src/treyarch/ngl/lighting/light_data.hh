#pragma once

#include "treyarch/shared/math/types/matrix4x4.hh"
#include "treyarch/shared/math/types/vector4.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl {
    struct texture;

namespace lighting {
    struct directional_light_data {
        vector4 direction;
        vector4 color;
    };

    struct point_light_data {
        vector4 position;
        vector4 color;
    };

    struct spot_light_data {
        vector4 position;
        vector4 direction;
        vector4 angles;
        vector4 color;
    };

    struct generated_light_data {
        u32       flags;
        u8        reserved_004[0x1C];
        vector4   direction;
        u8        reserved_030[0x10];
        vector4   position;
        matrix4x4 projector_matrix;
        vector4   color;
        texture*  projector_texture;
        f32       activity;
        f32       inner_radius;
        f32       outer_radius;
        u8        reserved_0B0[0x10];
        f32       attenuation_inner;
        f32       attenuation_outer;
        u8        reserved_0C8[0x0C];
        f32       direction_w;
        u8        reserved_0D8[0x04];
        i32       projector_config_0;
        i32       projector_config_1;
        u8        reserved_0E4[0x0C];
    };

    ASSERT_SIZEOF  (directional_light_data,            0x20);
    ASSERT_OFFSETOF(directional_light_data, direction, 0x00);
    ASSERT_OFFSETOF(directional_light_data, color,     0x10);
    ASSERT_SIZEOF  (point_light_data,                  0x20);
    ASSERT_SIZEOF  (spot_light_data,                   0x40);
    ASSERT_SIZEOF  (generated_light_data,              0xF0);
} // lighting
}} // treyarch::ngl
