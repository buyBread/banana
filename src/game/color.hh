#pragma once

#include "treyarch/math/vector.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    struct color {
        f32 r, g, b, a;
    };

    struct color32 {
        u8 b, g, r, a;

        // sub_405DC0
        color32 &set_clamped(i32 red, i32 green, i32 blue, i32 alpha) { // each channel kept within a byte
            r = (u8)(red   < 0 ? 0 : red   > 255 ? 255 : red);
            g = (u8)(green < 0 ? 0 : green > 255 ? 255 : green);
            b = (u8)(blue  < 0 ? 0 : blue  > 255 ? 255 : blue);
            a = (u8)(alpha < 0 ? 0 : alpha > 255 ? 255 : alpha);

            return *this;
        }
    };

    ASSERT_SIZEOF(color, 0x10);

    ASSERT_SIZEOF  (color32,    0x04);
    ASSERT_OFFSETOF(color32, r, 0x02);

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
