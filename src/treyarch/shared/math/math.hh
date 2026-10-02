#pragma once

#include <cmath>

#include "util/types.hh"

// i'm not sure why they have two different precision PIs
#define M_PI          3.1415926535897932384626433832795f 
#define M_PI_TWO      (M_PI * 2.0f)
#define M_PI_OVER_TWO (M_PI * 0.5f)

inline constexpr f32 PI = 3.1415927f;

inline constexpr f32 TWO_PI        = PI * 2.0f;
inline constexpr f32 PI_OVER_TWO   = PI * 0.5f;
inline constexpr f32 PI_OVER_FOUR  = PI * 0.25f;
inline constexpr f32 PI_OVER_EIGHT = PI * 0.125f;

inline constexpr f32 SMALL_ANGLE = 0.00001f;

inline constexpr f32 SMALL_DIST = 0.00001f;
inline constexpr f32 LARGE_DIST = 10.0e+8f;
inline constexpr f32 HUGE_DIST  = 1e30f; // made this one up (the const), the value was probably a macro?

namespace treyarch { namespace math {
    inline bool clamp_value(      f32* value,
                            const f32  min_value,
                            const f32  max_value ) {

        if (*value < min_value){
            *value = min_value;
            
            return true;
        } else if (*value > max_value) {
            *value = max_value;
            
            return true;
        }

        return false;
    }

    inline bool clamp_value_abs(f32 *value, const f32 abs_v) {
        return clamp_value(value, -abs_v, abs_v);
    }

    inline i32 clamp_int(const i32 value, const i32 minimum, const i32 maximum ) {
        if (value < minimum)
            return minimum;
        else if (value > maximum)
            return maximum;

        return value;
    }

    inline f32 sqr(f32 x){
        return x*x;
    }

    inline i32 sign(f32 x) {
        return (x < 0) ? -1 : ((x > 0) ? 1 : 0);
    }

    inline f32 range(f32 v, f32 min, f32 max) {
        if (v < min)
            return min;
        if (v > max)
            return max;

        return v;
    }

    inline bool approx_equals(f32 x, f32 y, f32 epsilon){
        return (fabsf( x - y ) < epsilon);
    }

    template <class T>
    inline T lerp(T v1, T v2, f32 t) {
        return v1 + (v2 - v1) * t;
    }

    inline f32 lerp(f32 v1, f32 v2, f32 t) {
        return (f32)((f64)v1 + ((f64)v2 - v1) * t);
    }

    inline i32 positorial( i32 x ) {
        if (x % 2)
            return ((x + 1) * (x / 2)) + (x / 2) + 1;
        else
            return ((x + 1) * (x / 2));
    }

    inline f32 fast_length2(f32 x, f32 y, f32 z) {
        return (f32)((f64)x*x + (f64)y*y + (f64)z*z);
    }

    inline f32 fast_length(f32 x, f32 y, f32 z) {
        return (f32)sqrtf(fast_length2(x, y, z));
    }

    inline f32 fast_recip_length(f32 x, f32 y, f32 z) {
        return 1.0f / (f32)sqrtf(fast_length2(x, y, z));
    }

    inline f32 fast_distance(f32 x1, f32 y1, f32 z1, f32 v2[3]) {
        f32 dx, dy, dz;

        dx = x1 - v2[0];
        dy = y1 - v2[1];
        dz = z1 - v2[2];

        return (f32)sqrtf(fast_length2(dx, dy, dz));
    }

    inline f32 fast_recip_sqrt(f32 x) {
        return 1.0f / (f32)sqrtf(x);
    }

    // sub_401DA0
    inline f32 arcsin(f32 value) {
        f32 x = (f32)std::fabs((f64)value);
        
        f32 result;

        if (x < 0.5f) {
            f32 x2 = x  * x;
            f32 x3 = x2 * x;
            f32 x5 = x3 * x2;
            f32 x7 = x5 * x2;

            result = (f32)((f64)(f32)
                          ((f64)(f32)((f64)x7 * (f64)0.05398124f) +
                                      (f64)x5 * (f64)0.075f) +
                                      (f64)x3 * (f64)0.1666667f);
            result = (f32)((f64)result + (f64)x);
        } else {
            f32 t  = (f32)std::fabs((f64)(f32)((1.0 - (f64)x) * 0.5));
            f32 s  = (f32)std::sqrt((f64)t);
            f32 s2 = s  * s;
            f32 s3 = s2 * s;
            f32 s5 = s3 * s2;
            f32 s7 = s5 * s2;

            result = (f32)((f64)(f32)((f64)s7 * (f64)-0.1079625f) - (f64)s5 * (f64)0.15f);
            result = (f32)((f64)result - (f64)s3 * (f64)0.3333333f);
            result = (f32)((f64)result - (f64)s * 2.0);
            result = (f32)((f64)result + (f64)1.570796f);
        }

        return value < 0.0f ? -result : result;
    }
}} // treyarch::math
