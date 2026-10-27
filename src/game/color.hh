#pragma once

#include "treyarch/math/vector.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    struct color {
        f32 r, g, b, a;
    };

    ASSERT_SIZEOF(color, 0x10);

    inline u32 pack_color(const math::vector4 &color) {
        u32 red   = (u32)(color.get_x() * 255.0f);
        u32 green = (u32)(color.get_y() * 255.0f);
        u32 blue  = (u32)(color.get_z() * 255.0f);
        u32 alpha = (u32)(color.get_w() * 255.0f);

        return blue | green << 8 | red << 16 | alpha << 24;
    }

    inline u32 pack_color(const f32 &r, const f32 &g, const f32 &b, const f32 &a) {
        u32 red   = (u32)(r * 255.0f);
        u32 green = (u32)(g * 255.0f);
        u32 blue  = (u32)(b * 255.0f);
        u32 alpha = (u32)(a * 255.0f);

        return blue | green << 8 | red << 16 | alpha << 24;
    }

    inline u32 pack_color(const u8 &r, const u8 &g, const u8 &b, const u8 &a) {
        return (u32)b | (u32)g << 8 | (u32)r << 16 | (u32)a << 24;
    }
} // treyarch
