#pragma once

#include <cmath>

#include "util/types.hh"
#include "treyarch/ngl/math/math.hh"
#include "treyarch/shared/mash/mash.hh"

namespace treyarch { namespace ngl {
    class vector2 { // L15079: SM3 .ii

public:
        f32 x, y;

        vector2() {}
        vector2(const vector2 &v) {
            x = v.x;
            y = v.y;
        }
        vector2(f32 _x, f32 _y) {
            x = _x;
            y = _y;
        }
        explicit vector2(f32 d) { x = y = d; }

        vector2& operator = (const vector2 &v) {
            x = v.x;
            y = v.y;
            
            return *this;
        }

        vector2& operator += (const vector2 &v) {
            x += v.x;
            y += v.y;
            
            return *this;
        }

        vector2& operator -= (const vector2 &v) {
            x -= v.x;
            y -= v.y;
            
            return *this;
        }

        vector2& operator *= (f32 d) {
            x *= d;
            y *= d;
            
            return *this;
        }

        vector2& operator /= (f32 d) {
            f32 d_inv = 1.0f / d;
            
            x *= d_inv;
            y *= d_inv;
            
            return *this;
        }

        vector2 operator - () const {  return vector2(-x, -y); }

        const f32& operator [] (int i) const { return (&x)[i]; }
              f32& operator [] (int i)       { return (&x)[i]; }

        f32 length2() const {
            return x*x + y*y;
        }

        f32 length() const {
            return (f32)sqrtf(x*x + y*y);
        }

        vector2 &normalize() {
            f32 l2 = length2();

            if (l2 > SMALL_DIST*SMALL_DIST)
                *this *= math::fast_recip_sqrt(l2);
            
            return *this;
        }
        vector2 &set_length(f32 newlen=1.0f) {
            f32 l2 = length2();

            if (l2 > SMALL_DIST*SMALL_DIST)
                *this *= newlen * math::fast_recip_sqrt(l2);
            
            return *this;
        }

        vector2 perp() const {
            return vector2(-y, x);
        }

        bool is_valid() const {
            return x > -HUGE_DIST && x < HUGE_DIST &&
                   y > -HUGE_DIST && y < HUGE_DIST; }

        void mash_convert(mash::generic_mash_info *inf, void *begin_image); /* hey, what are you? */
    };
}} // treyarch::ngl