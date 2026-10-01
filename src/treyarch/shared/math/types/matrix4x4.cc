#include <cmath>

#include "treyarch/shared/math/types/matrix4x4.hh"

using namespace treyarch;

// sub_95D960
void matrix4x4::make_projection(f32 field_of_view,
                                f32 aspect,
                                f32 near_plane,
                                f32 far_plane,
                                f32 push) {

    f32 horizontal_scale = 1.0f / std::tan(field_of_view * 0.5f);
    f32 depth_scale      = far_plane / (far_plane - near_plane);

    *this = matrix4x4(horizontal_scale, 0.0f,                      0.0f,                              0.0f,
                      0.0f,             horizontal_scale / aspect, 0.0f,                              0.0f,
                      0.0f,             0.0f,                      depth_scale,                       1.0f,
                      0.0f,             0.0f,                      (push - near_plane) * depth_scale, 0.0f);
}
