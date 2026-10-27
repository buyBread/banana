#pragma once

#include "treyarch/math/float4.hh"

namespace treyarch { namespace math {
    inline const f32 math_pi = 3.14159265f;

    inline const float4::constant cos_inv2pi  = { 0.15915494f, 0.15915494f, 0.15915494f, 0.15915494f };
    inline const float4::constant cos_half    = { 0.5f, 0.5f, 0.5f, 0.5f };
    inline const float4::constant cos_quarter = { 0.25f, 0.25f, 0.25f, 0.25f };
    inline const float4::constant cos_k1      = { 6.283185f, 6.283185f, 6.283185f, 6.283185f };
    inline const float4::constant cos_k3      = { -41.341675f, -41.341675f, -41.341675f, -41.341675f };
    inline const float4::constant cos_k5      = { 81.602226f, 81.602226f, 81.602226f, 81.602226f };
    inline const float4::constant cos_k7      = { -76.574959f, -76.574959f, -76.574959f, -76.574959f };
    inline const float4::constant cos_k9      = { 39.710659f, 39.710659f, 39.710659f, 39.710659f };

    // sub_408390
    inline float4 cos(const float4 &radians, const float4 &frequency = cos_inv2pi) {
        float4 nabs_radians = -abs(radians);

        float4_acc acc;
        acc.load_mul(nabs_radians, frequency);

        float4 scaled = acc.ret();

        acc.ceil_neg();
        acc.sub(scaled);

        float4 x1(acc.ret_sub(reinterpret_cast<const float4&>(cos_half)));

        x1  = abs(x1);
        x1 -= cos_quarter;

        float4 x2(x1 * x1);
        float4 x4(x2 * x2);
        float4 x3(x1 * x2);
        float4 x5(x1 * x4);
        float4 x7(x3 * x4);
        float4 x9(x5 * x4);

        acc.load_mul(x9, reinterpret_cast<const float4&>(cos_k9));
        acc.addmul(x7, reinterpret_cast<const float4&>(cos_k7));
        acc.addmul(x5, reinterpret_cast<const float4&>(cos_k5));
        acc.addmul(x3, reinterpret_cast<const float4&>(cos_k3));

        return acc.ret_addmul(x1, reinterpret_cast<const float4&>(cos_k1));
    }

    inline float1 cos(float1 radians) {
        return cos(float4(radians)).get_x();
    }

    inline const float4::constant sin_3pi_over_2 = { 3.0f * 1.5707964f, 3.0f * 1.5707964f, 3.0f * 1.5707964f, 3.0f * 1.5707964f };

    inline float4 sin(const float4 &radians) {
        return cos(radians + sin_3pi_over_2);
    }

    inline float1 sin(float1 radians) {
        return sin(float4(radians)).get_x();
    }

    enum e_sin_cos_type {
        sin_cos_cos    = 0,
        sin_cos_negsin = 1,
        sin_cos_negcos = 2,
        sin_cos_sin    = 3,
        sin_cos_any    = 0
    };

    template<e_sin_cos_type ex, e_sin_cos_type ey, e_sin_cos_type ez, e_sin_cos_type ew>
    float4 sin_cos(const float4 &radians) {
        const f32 pi_over_2 = 1.570796f;

        return cos(radians + float4((f32)ex * pi_over_2, (f32)ey * pi_over_2, (f32)ez * pi_over_2, (f32)ew * pi_over_2));
    }

    template<>
    inline float4 sin_cos<sin_cos_cos, sin_cos_cos, sin_cos_cos, sin_cos_cos>(const float4 &radians) {
        return cos(radians);
    }

    template<>
    inline float4 sin_cos<sin_cos_sin, sin_cos_sin, sin_cos_sin, sin_cos_sin>(const float4 &radians) {
        return sin(radians);
    }

    inline const float4::constant sin_cos_scnsnc = { 3.0f * 1.570796f, 0.0f * 1.570796f, 1.0f * 1.570796f, 2.0f * 1.570796f };

    template<>
    inline float4 sin_cos<sin_cos_sin, sin_cos_cos, sin_cos_negsin, sin_cos_negcos>(const float4 &radians) {
        return cos(radians + sin_cos_scnsnc);
    }

    inline const float4::constant sin_cos_sc = { 3.0f * 1.570796f, 0.0f * 1.570796f, 0.0f * 1.570796f, 0.0f * 1.570796f };

    template<>
    inline float4 sin_cos<sin_cos_sin, sin_cos_cos, sin_cos_cos, sin_cos_cos>(const float4 &radians) {
        return cos(radians + sin_cos_sc);
    }

    template<typename T>
    float4 tan(const T &radians) {
        return sin(radians) / cos(radians);
    }

    // sub_697C20
    inline float1 tan(float1 radians) {
        float4 tmp = sin_cos<sin_cos_sin, sin_cos_cos, sin_cos_sin, sin_cos_cos>(radians);

        return tmp.get_x() / tmp.get_y();
    }

    // inlined @ sub_401DA0
    inline f32 asin_upper(f32 y) {
        f32 a;
        f32 x1 = y;

        if (x1 < 0.5f) {
            f32 x2 = x1 * x1;
            f32 x3 = x2 * x1;
            f32 x5 = x2 * x3;
            f32 x7 = x2 * x5;

            a = 5.398124e-2f * x7;
            a = (f32)((f64)a + (f64)0.075f     * x5);
            a = (f32)((f64)a + (f64).1666667f * x3);
            a += x1;
        } else {
            x1 = sqrt_abs((f32)((1.0 - (f64)x1) * 0.5));

            f32 x2 = x1 * x1;
            f32 x3 = x2 * x1;
            f32 x5 = x2 * x3;
            f32 x7 = x2 * x5;

            a = -.1079625f * x7;
            a = (f32)((f64)a + (f64)-.15f      * x5);
            a = (f32)((f64)a + (f64)-.3333333f * x3);
            a = (f32)((f64)a + -2.0             * x1);
            a += 1.570796f;
        }

        return a;
    }

    // sub_401DA0
    inline f32 asin(f32 y) {
        f32 a = asin_upper(abs(y));

        if (y < 0.0f)
            return -a;

        return a;
    }

    inline f32 acos(f32 x)       { return (math_pi / 2.0f) - asin(x); }
    inline f32 acos_upper(f32 x) { return (math_pi / 2.0f) - asin_upper(x); }

    inline f32 atan(f32 y, f32 x) {
        f32 xa = abs(x);
        f32 ya = abs(y);

        if (xa + ya == 0.0)
            return 0.0f;

        f32 invr = rsqrt(x * x + y * y);
        f32 a    = (xa < ya) ? acos_upper(xa * invr) : asin_upper(ya * invr);

        if (x < 0)
            a = math_pi - a;

        if (y < 0)
            a = -a;

        return a;
    }

    inline f32 atan_upper(f32 ya, f32 x) {
        f32 xa = abs(x);

        if (xa + ya == 0.0)
            return 0.0f;

        f32 invr = rsqrt(x * x + ya * ya);

        if (xa < ya)
            return acos(x * invr);

        f32 a = asin_upper(ya * invr);

        if (x < 0)
            a = math_pi - a;

        return a;
    }

    inline f32 atan_unit(f32 y, f32 x) {
        f32 xa = abs(x);
        f32 ya = abs(y);
        f32 a  = (xa < ya) ? acos_upper(xa) : asin_upper(ya);

        if (x < 0)
            a = math_pi - a;

        if (y < 0)
            a = -a;

        return a;
    }

    inline f32 atan_unit_upper(f32 ya, f32 x) {
        f32 xa = abs(x);

        if (xa < ya)
            return acos(x);

        f32 a = asin_upper(ya);

        if (x < 0)
            a = math_pi - a;

        return a;
    }

    inline f32 cos_atan(f32 y, f32 x) { return x * rsqrt_safe(x * x + y * y); }
    inline f32 sin_atan(f32 y, f32 x) { return y * rsqrt_safe(x * x + y * y); }
    inline f32 cos_atan(f32 y)        { return rsqrt(1.0f + y * y); }
    inline f32 sin_atan(f32 y)        { return y * cos_atan(y); }

    inline f32 atan(f32 y) {
        f32 c  = cos_atan(y);
        f32 ya = abs(y);
        f32 a  = (1.0f < ya) ? acos_upper(c) : asin_upper(ya * c);

        if (y < 0.0f)
            return -a;

        return a;
    }
}} // treyarch::math
