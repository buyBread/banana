#pragma once

#include <cmath>

#include "banana/util/types.hh"

namespace treyarch {
    inline constexpr f32 PI   = 3.1415927f;
    inline constexpr f32 _2PI = PI * 2.0f;

    // sub_48DEE0
    inline f32 clip_angle(f32 ang) {
        if (ang >= 0.0f)
            ang = std::fmod(ang, _2PI);
        else
            ang = _2PI - std::fmod(-ang, _2PI);

        return ang;
    }

    inline bool clamp_value(      f32* value,
                            const f32  min_value,
                            const f32  max_value) {

        if (*value < min_value) {
            *value = min_value;

            return true;
        } else if (*value > max_value) {
            *value = max_value;

            return true;
        }

        return false;
    }

    inline bool clamp_value_abs(f32* value, const f32 absv) {
        return clamp_value(value, -absv, absv);
    }
} // treyarch
