#include <cmath>

#include "treyarch/shared/math/types/matrix4x4.hh"
#include "treyarch/shared/math/vmath.hh"

using namespace treyarch;

// sub_95D960
void matrix4x4::make_projection(f32 field_of_view,
                                f32 aspect,
                                f32 near_plane,
                                f32 far_plane,
                                f32 push) {

    f32 horizontal_scale = 1.0f / math::tan(field_of_view * 0.5f);
    f32 depth_scale      = far_plane / (far_plane - near_plane);

    *this = matrix4x4(horizontal_scale, 0.0f,                      0.0f,                              0.0f,
                      0.0f,             horizontal_scale / aspect, 0.0f,                              0.0f,
                      0.0f,             0.0f,                      depth_scale,                       1.0f,
                      0.0f,             0.0f,                      (push - near_plane) * depth_scale, 0.0f);
}

// sub_9D4380
f32 matrix4x4::determinant() const {
    // 2x2 minors of the z and w columns
    f32 minor_zw = z.z * w.w - z.w * w.z;
    f32 minor_yw = y.z * w.w - y.w * w.z;
    f32 minor_yz = y.z * z.w - y.w * z.z;
    f32 minor_xz = x.z * z.w - x.w * z.z;
    f32 minor_xw = x.z * w.w - x.w * w.z;
    f32 minor_xy = x.z * y.w - x.w * y.z;

    f32 z_term = (w.y * minor_xy + (x.y * minor_yw - y.y * minor_xw)) * z.x;
    f32 x_term = (y.y * minor_zw - z.y * minor_yw + w.y * minor_yz) * x.x;
    f32 y_term = (x.y * minor_zw - z.y * minor_xw + w.y * minor_xz) * y.x;
    f32 w_term = (x.y * minor_yz - y.y * minor_xz + z.y * minor_xy) * w.x;

    return z_term + (x_term - y_term) - w_term;
}

// sub_9D4520
matrix4x4 matrix4x4::cofactors(bool transposed) const {
    // 2x2 minors named by their rows, then columns
    f32 zw_zw = z.z * w.w - z.w * w.z;
    f32 yw_zw = y.z * w.w - y.w * w.z;
    f32 yz_zw = y.z * z.w - y.w * z.z;
    f32 xw_zw = x.z * w.w - x.w * w.z;
    f32 xz_zw = x.z * z.w - x.w * z.z;
    f32 xy_zw = x.z * y.w - x.w * y.z;
    f32 zw_yw = z.y * w.w - z.w * w.y;
    f32 yw_yw = y.y * w.w - y.w * w.y;
    f32 yz_yw = y.y * z.w - y.w * z.y;
    f32 xw_yw = x.y * w.w - x.w * w.y;
    f32 xz_yw = x.y * z.w - x.w * z.y;
    f32 xy_yw = x.y * y.w - x.w * y.y;
    f32 zw_yz = z.y * w.z - z.z * w.y;
    f32 yw_yz = y.y * w.z - y.z * w.y;
    f32 yz_yz = y.y * z.z - y.z * z.y;
    f32 xw_yz = x.y * w.z - x.z * w.y;
    f32 xz_yz = x.y * z.z - x.z * z.y;
    f32 xy_yz = x.y * y.z - x.z * y.y;

    matrix4x4 result( zw_zw * y.y - yw_zw * z.y + yz_zw * w.y,
                    -(zw_zw * y.x - yw_zw * z.x + yz_zw * w.x),
                      zw_yw * y.x - yw_yw * z.x + yz_yw * w.x,
                    -(zw_yz * y.x - yw_yz * z.x + yz_yz * w.x),

                    -(zw_zw * x.y - xw_zw * z.y + xz_zw * w.y),
                      zw_zw * x.x - xw_zw * z.x + xz_zw * w.x,
                    -(zw_yw * x.x - xw_yw * z.x + xz_yw * w.x),
                      zw_yz * x.x - xw_yz * z.x + xz_yz * w.x,

                      yw_zw * x.y - xw_zw * y.y + xy_zw * w.y,
                    -(yw_zw * x.x - xw_zw * y.x + xy_zw * w.x),
                      yw_yw * x.x - xw_yw * y.x + xy_yw * w.x,
                    -(yw_yz * x.x - xw_yz * y.x + xy_yz * w.x),

                    -(yz_zw * x.y - xz_zw * y.y + xy_zw * z.y),
                      yz_zw * x.x - xz_zw * y.x + xy_zw * z.x,
                    -(yz_yw * x.x - xz_yw * y.x + xy_yw * z.x),
                      yz_yz * x.x - xz_yz * y.x + xy_yz * z.x);

    return transposed ? result.transpose() : result;
}

// sub_9D4DE0
matrix4x4 matrix4x4::inverse() const {
    f32 value = determinant();

    // retail returns an uninitialized stack matrix here
    if (value == 0.0f) {
        matrix4x4 result;
        result.identity();

        return result;
    }

    matrix4x4 result = adjugate();
    result *= 1.0f / value;

    return result;
}
