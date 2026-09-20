#pragma once

#include "treyarch/shared/math/types/vector4.hh"
#include "util/types.hh"

namespace treyarch {
    inline u32 pack_color(const vector4 &color) {
        u32 red   = (u32)(color.x * 255.0f);
        u32 green = (u32)(color.y * 255.0f);
        u32 blue  = (u32)(color.z * 255.0f);
        u32 alpha = (u32)(color.w * 255.0f);

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
}