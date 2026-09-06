#pragma once

#include "treyarch/ngl/math/types/matrix4x4.hh"

namespace treyarch { namespace ngl { namespace math {
    inline matrix4x4 make_perspective(f32 horizontal_scale,
                                      f32 vertical_scale,
                                      f32 near_plane,
                                      f32 far_plane) {

        f32 depth_scale = far_plane / (far_plane - near_plane);

        return matrix4x4(horizontal_scale, 0.0f,           0.0f,                     0.0f,
                         0.0f,             vertical_scale, 0.0f,                     0.0f,
                         0.0f,             0.0f,           depth_scale,              1.0f,
                         0.0f,             0.0f,          -near_plane * depth_scale, 0.0f);
    }

    inline matrix4x4 make_orthographic(f32 horizontal_scale,
                                       f32 vertical_scale,
                                       f32 near_plane,
                                       f32 far_plane) {

        f32 depth_scale = 1.0f / (far_plane - near_plane);

        return matrix4x4(horizontal_scale, 0.0f,           0.0f,                     0.0f,
                         0.0f,             vertical_scale, 0.0f,                     0.0f,
                         0.0f,             0.0f,           depth_scale,              0.0f,
                         0.0f,             0.0f,          -near_plane * depth_scale, 1.0f);
    }

    inline matrix4x4 make_viewport(f32 left,
                                   f32 top,
                                   f32 right,
                                   f32 bottom) {

        return matrix4x4((right - left) * 0.5f,  0.0f,                 0.0f, 0.0f,
                          0.0f,                 (top - bottom) * 0.5f, 0.0f, 0.0f,
                          0.0f,                  0.0f,                 1.0f, 0.0f,
                         (left + right) * 0.5f, (top + bottom) * 0.5f, 0.0f, 1.0f);
    }
}}} // treyarch::ngl::math
