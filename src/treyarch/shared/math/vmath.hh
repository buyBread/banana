#pragma once

#include "treyarch/shared/math/types/matrix4x4.hh"
#include "treyarch/shared/math/types/vector4.hh"
#include "util/types.hh"

// the Float4 helpers from SM3's math/include/vmath.h, the way the PC build compiled them
namespace treyarch { namespace math {
    inline constexpr f32 cos_inverse_two_pi = 0.15915494f;
    inline constexpr f32 cos_k1             = 6.283185f;
    inline constexpr f32 cos_k3             = -41.341675f;
    inline constexpr f32 cos_k5             = 81.602226f;
    inline constexpr f32 cos_k7             = -76.574959f;
    inline constexpr f32 cos_k9             = 39.710659f;

    // SinCos offsets are multiples of this, not of the more precise 1.5707964f Sin() uses
    inline constexpr f32 sin_cos_pi_over_two = 1.570796f;

    vector4 cos(const vector4 &radians,
                const vector4 &frequency = vector4(cos_inverse_two_pi));

    f32 tan(f32 radians);

    matrix4x4 inverse(const matrix4x4 &value);
}} // treyarch::math
