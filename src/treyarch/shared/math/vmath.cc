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

        f32 accumulator = x9 * cos_k9;
            accumulator = (f32)((f64)accumulator + (f64)x7 * cos_k7);
            accumulator = (f32)((f64)accumulator + (f64)x5 * cos_k5);
            accumulator = (f32)((f64)accumulator + (f64)x3 * cos_k3);
            accumulator = (f32)((f64)accumulator + (f64)x1 * cos_k1);

        result[lane] = accumulator;
    }

    return result;
}

// sub_697C20
f32 math::tan(f32 radians) {
    // SinCos<_sin, _cos, _sin, _cos>, where cos(x + 3pi/2) is sin(x)
    f32 sin_radians = radians + 3.0f * sin_cos_pi_over_two;
    f32 cos_radians = radians + 0.0f * sin_cos_pi_over_two;

    vector4 values = cos(vector4(sin_radians, cos_radians, sin_radians, cos_radians));

    return values.x / values.y;
}

// sub_9D2B70
matrix4x4 math::inverse(const matrix4x4 &value) {
    const matrix4x4 &m = value;

    // 2x2 minors of the bottom two rows; the ones with the w column round each product first
    f32 lower_yz = (f32)((f64)m.z.y * m.w.z - (f64)m.z.z * m.w.y);
    f32 lower_zx = (f32)((f64)m.z.z * m.w.x - (f64)m.z.x * m.w.z);
    f32 lower_xy = (f32)((f64)m.z.x * m.w.y - (f64)m.z.y * m.w.x);
    f32 lower_zw = m.z.z * m.w.w - m.z.w * m.w.z;
    f32 lower_yw = m.z.y * m.w.w - m.z.w * m.w.y;
    f32 lower_xw = m.z.x * m.w.w - m.z.w * m.w.x;

    // and of the top two rows
    f32 upper_yz = (f32)((f64)m.x.y * m.y.z - (f64)m.x.z * m.y.y);
    f32 upper_zx = (f32)((f64)m.x.z * m.y.x - (f64)m.x.x * m.y.z);
    f32 upper_xy = (f32)((f64)m.x.x * m.y.y - (f64)m.x.y * m.y.x);
    f32 upper_zw = m.x.z * m.y.w - m.x.w * m.y.z;
    f32 upper_yw = m.x.y * m.y.w - m.x.w * m.y.y;
    f32 upper_xw = m.x.x * m.y.w - m.x.w * m.y.x;

    f32 cofactor_xx = (m.y.w * lower_yz + (f32)((f64)m.y.y * lower_zw - (f64)m.y.z * lower_yw)) + 0.0f;
    f32 cofactor_xy = (m.y.w * lower_zx + (f32)((f64)m.y.z * lower_xw - (f64)m.y.x * lower_zw)) + 0.0f;
    f32 cofactor_xz = (m.y.w * lower_xy + (f32)((f64)m.y.x * lower_yw - (f64)m.y.y * lower_xw)) + 0.0f;

    f32 determinant = (f32)((f64)cofactor_xz * m.x.z + ((f64)cofactor_xy * m.x.y + (f64)cofactor_xx * m.x.x));

    f32 y_w_lane = 0.0f * m.y.w + 0.0f;
    f32 x_w_lane = 0.0f - 0.0f * m.x.w;
    f32 w_w_lane = 0.0f * m.w.w + 0.0f;
    f32 z_w_lane = 0.0f - 0.0f * m.z.w;

    matrix4x4 result(cofactor_xx,
                     cofactor_xy,
                     cofactor_xz,
                     y_w_lane - (f32)((f64)lower_xy * m.y.z + ((f64)lower_zx * m.y.y + (f64)lower_yz * m.y.x)),

                     ((f32)((f64)lower_yw * m.x.z - (f64)lower_zw * m.x.y) - m.x.w * lower_yz) + 0.0f,
                     ((f32)((f64)lower_zw * m.x.x - (f64)lower_xw * m.x.z) - m.x.w * lower_zx) + 0.0f,
                     ((f32)((f64)lower_xw * m.x.y - (f64)lower_yw * m.x.x) - m.x.w * lower_xy) + 0.0f,
                     (f32)((f64)lower_xy * m.x.z + ((f64)lower_zx * m.x.y + (f64)lower_yz * m.x.x)) + x_w_lane,

                     (m.w.w * upper_yz + (f32)((f64)upper_zw * m.w.y - (f64)upper_yw * m.w.z)) + 0.0f,
                     (m.w.w * upper_zx + (f32)((f64)upper_xw * m.w.z - (f64)upper_zw * m.w.x)) + 0.0f,
                     (m.w.w * upper_xy + (f32)((f64)upper_yw * m.w.x - (f64)upper_xw * m.w.y)) + 0.0f,
                     w_w_lane - (f32)((f64)upper_xy * m.w.z + ((f64)upper_yz * m.w.x + (f64)upper_zx * m.w.y)),

                     ((f32)((f64)upper_yw * m.z.z - (f64)upper_zw * m.z.y) - m.z.w * upper_yz) + 0.0f,
                     ((f32)((f64)upper_zw * m.z.x - (f64)upper_xw * m.z.z) - m.z.w * upper_zx) + 0.0f,
                     ((f32)((f64)upper_xw * m.z.y - (f64)upper_yw * m.z.x) - m.z.w * upper_xy) + 0.0f,
                     (f32)((f64)upper_xy * m.z.z + ((f64)upper_yz * m.z.x + (f64)upper_zx * m.z.y)) + z_w_lane);

    // a singular matrix keeps the unscaled cofactors
    if (determinant != 0.0f)
        result *= 1.0f / determinant;

    return result;
}
