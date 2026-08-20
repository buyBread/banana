#pragma once

#include <algorithm>
#include <imgui.h>

#include <util/types.hh>

namespace banana { namespace imgui {
namespace util { namespace vec2 {
    inline ImVec2 add(const ImVec2 &left, const ImVec2 &right) {
        return ImVec2(left.x + right.x,
                      left.y + right.y);
    }

    inline ImVec2 subtract(const ImVec2 &left, const ImVec2 &right) {
        return ImVec2(left.x - right.x,
                      left.y - right.y);
    }

    inline ImVec2 multiply(const ImVec2 &vec, f32 scale) {
        return ImVec2(vec.x * scale,
                      vec.y * scale);
    }

    inline bool point_in_rectangle(const ImVec2 &point,
                                   const ImVec2 &minimum,
                                   const ImVec2 &maximum) {

        return point.x >= minimum.x &&
               point.y >= minimum.y &&
               point.x <= maximum.x &&
               point.y <= maximum.y;
    }

    inline ImVec2 rectangle_edge(const ImVec2 &minimum,
                                 const ImVec2 &maximum,
                                 const ImVec2 &toward) {

        ImVec2 center((minimum.x + maximum.x) * 0.5f,
                      (minimum.y + maximum.y) * 0.5f);

        ImVec2 delta       = subtract(toward, center);
        f32    half_width  = (maximum.x - minimum.x) * 0.5f;
        f32    half_height = (maximum.y - minimum.y) * 0.5f;
        f32    horizontal  = half_width  > 0.0f ? std::abs(delta.x) / half_width  : 0.0f;
        f32    vertical    = half_height > 0.0f ? std::abs(delta.y) / half_height : 0.0f;
        f32    divisor     = std::max(horizontal, vertical);

        if (divisor <= 0.0f)
            return center;

        return add(center, multiply(delta, 1.0f / divisor));
    }
}} // util::vec2
}} // banana::imgui