#include <cmath>

#include "treyarch/shared/math/vmath.hh"

using namespace treyarch;

// sub_408390
vector4 math::cos(const vector4 &radians, const vector4 &frequency) {
    vector4 result;

    for (i32 lane = 0; lane < 4; ++lane) {
        f32 negative_radians = -std::fabs(radians[lane]);
        f32 scaled           = negative_radians * frequency[lane];

        // fold the angle into [-0.25, 0.25] turns
        f32 x1 = std::ceil(scaled) - scaled;
            x1 = std::fabs(x1 - 0.5f) - 0.25f;

        f32 x2 = x1 * x1;
        f32 x4 = x2 * x2;
        f32 x3 = x1 * x2;
        f32 x5 = x1 * x4;
        f32 x7 = x3 * x4;
        f32 x9 = x5 * x4;

        result[lane] = x9 * cos_k9 + x7 * cos_k7 + x5 * cos_k5 + x3 * cos_k3 + x1 * cos_k1;
    }

    return result;
}

// sub_697C20
f32 math::tan(f32 radians) {
    // SinCos<_sin, _cos, _sin, _cos>, where cos(x + 3pi/2) is sin(x)
    f32 sin_radians = radians + 3.0f * sin_cos_pi_over_two;

    vector4 values = cos(vector4(sin_radians, radians, sin_radians, radians));

    return values.x / values.y;
}

// sub_9D2B70
matrix4x4 math::inverse(const matrix4x4 &value) {
    const matrix4x4 &m = value;

    // 2x2 minors of the bottom two rows
    f32 lower_yz = m.z.y * m.w.z - m.z.z * m.w.y;
    f32 lower_zx = m.z.z * m.w.x - m.z.x * m.w.z;
    f32 lower_xy = m.z.x * m.w.y - m.z.y * m.w.x;
    f32 lower_zw = m.z.z * m.w.w - m.z.w * m.w.z;
    f32 lower_yw = m.z.y * m.w.w - m.z.w * m.w.y;
    f32 lower_xw = m.z.x * m.w.w - m.z.w * m.w.x;

    // and of the top two rows
    f32 upper_yz = m.x.y * m.y.z - m.x.z * m.y.y;
    f32 upper_zx = m.x.z * m.y.x - m.x.x * m.y.z;
    f32 upper_xy = m.x.x * m.y.y - m.x.y * m.y.x;
    f32 upper_zw = m.x.z * m.y.w - m.x.w * m.y.z;
    f32 upper_yw = m.x.y * m.y.w - m.x.w * m.y.y;
    f32 upper_xw = m.x.x * m.y.w - m.x.w * m.y.x;

    f32 cofactor_xx = m.y.w * lower_yz + m.y.y * lower_zw - m.y.z * lower_yw;
    f32 cofactor_xy = m.y.w * lower_zx + m.y.z * lower_xw - m.y.x * lower_zw;
    f32 cofactor_xz = m.y.w * lower_xy + m.y.x * lower_yw - m.y.y * lower_xw;

    f32 determinant = cofactor_xx * m.x.x + cofactor_xy * m.x.y + cofactor_xz * m.x.z;

    matrix4x4 result( cofactor_xx,
                      cofactor_xy,
                      cofactor_xz,
                     -(lower_yz * m.y.x + lower_zx * m.y.y + lower_xy * m.y.z),

                      lower_yw * m.x.z - lower_zw * m.x.y - m.x.w * lower_yz,
                      lower_zw * m.x.x - lower_xw * m.x.z - m.x.w * lower_zx,
                      lower_xw * m.x.y - lower_yw * m.x.x - m.x.w * lower_xy,
                      lower_yz * m.x.x + lower_zx * m.x.y + lower_xy * m.x.z,

                      m.w.w * upper_yz + upper_zw * m.w.y - upper_yw * m.w.z,
                      m.w.w * upper_zx + upper_xw * m.w.z - upper_zw * m.w.x,
                      m.w.w * upper_xy + upper_yw * m.w.x - upper_xw * m.w.y,
                     -(upper_yz * m.w.x + upper_zx * m.w.y + upper_xy * m.w.z),

                      upper_yw * m.z.z - upper_zw * m.z.y - m.z.w * upper_yz,
                      upper_zw * m.z.x - upper_xw * m.z.z - m.z.w * upper_zx,
                      upper_xw * m.z.y - upper_yw * m.z.x - m.z.w * upper_xy,
                      upper_yz * m.z.x + upper_zx * m.z.y + upper_xy * m.z.z);

    // a singular matrix keeps the unscaled cofactors
    if (determinant != 0.0f)
        result *= 1.0f / determinant;

    return result;
}
