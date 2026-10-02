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
    f32 depth_scale      = (f32)((f64)far_plane / ((f64)far_plane - near_plane));

    *this = matrix4x4(horizontal_scale, 0.0f,                      0.0f,                              0.0f,
                      0.0f,             horizontal_scale / aspect, 0.0f,                              0.0f,
                      0.0f,             0.0f,                      depth_scale,                       1.0f,
                      0.0f,             0.0f,                      (f32)(((f64)push - near_plane) * depth_scale), 0.0f);
}

// sub_9D4380
f32 matrix4x4::determinant() const {
    f64 minor_zw = (f64)z.z * w.w - (f64)z.w * w.z;
    f64 minor_yw = (f64)y.z * w.w - (f64)y.w * w.z;
    f64 minor_yz = (f64)y.z * z.w - (f64)y.w * z.z;
    f64 minor_xz = (f64)x.z * z.w - (f64)x.w * z.z;
    f64 minor_xw = (f64)x.z * w.w - (f64)x.w * w.z;
    f64 minor_xy = (f64)x.z * y.w - (f64)x.w * y.z;

    f32 z_term = ((f32)(w.y * minor_xy) + ((f32)(x.y * minor_yw) - (f32)(y.y * minor_xw))) * z.x;
    f32 x_term = ((f32)(y.y * minor_zw) - (f32)(z.y * minor_yw) + (f32)(w.y * minor_yz)) * x.x;
    f32 y_term = ((f32)(x.y * minor_zw) - (f32)(z.y * minor_xw) + (f32)(w.y * minor_xz)) * y.x;
    f32 w_term = ((f32)(x.y * minor_yz) - (f32)(y.y * minor_xz) + (f32)(z.y * minor_xy)) * w.x;

    return z_term + (x_term - y_term) - w_term;
}

// sub_9D4520
matrix4x4 matrix4x4::cofactors(bool transposed) const {
    // 2x2 minors named by their rows, then columns
    f64 zw_zw = (f64)z.z * w.w - (f64)z.w * w.z;
    f64 yw_zw = (f64)y.z * w.w - (f64)y.w * w.z;
    f64 yz_zw = (f64)y.z * z.w - (f64)y.w * z.z;
    f64 xw_zw = (f64)x.z * w.w - (f64)x.w * w.z;
    f64 xz_zw = (f64)x.z * z.w - (f64)x.w * z.z;
    f64 xy_zw = (f64)x.z * y.w - (f64)x.w * y.z;
    f64 zw_yw = (f64)z.y * w.w - (f64)z.w * w.y;
    f64 yw_yw = (f64)y.y * w.w - (f64)y.w * w.y;
    f64 yz_yw = (f64)y.y * z.w - (f64)y.w * z.y;
    f64 xw_yw = (f64)x.y * w.w - (f64)x.w * w.y;
    f64 xz_yw = (f64)x.y * z.w - (f64)x.w * z.y;
    f64 xy_yw = (f64)x.y * y.w - (f64)x.w * y.y;
    f64 zw_yz = (f64)z.y * w.z - (f64)z.z * w.y;
    f64 yw_yz = (f64)y.y * w.z - (f64)y.z * w.y;
    f64 yz_yz = (f64)y.y * z.z - (f64)y.z * z.y;
    f64 xw_yz = (f64)x.y * w.z - (f64)x.z * w.y;
    f64 xz_yz = (f64)x.y * z.z - (f64)x.z * z.y;
    f64 xy_yz = (f64)x.y * y.z - (f64)x.z * y.y;

    matrix4x4 result((f32)(  zw_zw * y.y - yw_zw * z.y + yz_zw * w.y),
                     (f32)(-(zw_zw * y.x - yw_zw * z.x + yz_zw * w.x)),
                     (f32)(  zw_yw * y.x - yw_yw * z.x + yz_yw * w.x),
                     (f32)(-(zw_yz * y.x - yw_yz * z.x + yz_yz * w.x)),

                     (f32)(-(zw_zw * x.y - xw_zw * z.y + xz_zw * w.y)),
                     (f32)(  zw_zw * x.x - xw_zw * z.x + xz_zw * w.x),
                     (f32)(-(zw_yw * x.x - xw_yw * z.x + xz_yw * w.x)),
                     (f32)(  zw_yz * x.x - xw_yz * z.x + xz_yz * w.x),

                     (f32)(  yw_zw * x.y - xw_zw * y.y + xy_zw * w.y),
                     (f32)(-(yw_zw * x.x - xw_zw * y.x + xy_zw * w.x)),
                     (f32)(  yw_yw * x.x - xw_yw * y.x + xy_yw * w.x),
                     (f32)(-(yw_yz * x.x - xw_yz * y.x + xy_yz * w.x)),

                     (f32)(-(yz_zw * x.y - xz_zw * y.y + xy_zw * z.y)),
                     (f32)(  yz_zw * x.x - xz_zw * y.x + xy_zw * z.x),
                     (f32)(-(yz_yw * x.x - xz_yw * y.x + xy_yw * z.x)),
                     (f32)(  yz_yz * x.x - xz_yz * y.x + xy_yz * z.x));

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
