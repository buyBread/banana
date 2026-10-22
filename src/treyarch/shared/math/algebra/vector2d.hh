#pragma once

#include <cmath>

#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class vector2d {

    public:
        typedef f32 T;

        T x, y;

                 vector2d() {}
                 vector2d(T _x, T _y) : x(_x),
                                        y(_y) {}
        explicit vector2d(T d) : x(d),
                                 y(d) {}

        vector2d &operator+=(const vector2d &v) {
            x += v.x;
            y += v.y;

            return *this;
        }

        vector2d &operator-=(const vector2d &v) {
            x -= v.x;
            y -= v.y;

            return *this;
        }

        vector2d &operator*=(T d) {
            x *= d;
            y *= d;

            return *this;
        }

        vector2d &operator/=(T d) {
            T d_inv = 1.0f / d;

            x *= d_inv;
            y *= d_inv;

            return *this;
        }

        T length2() const {
            return (f32)((f64)x * x + (f64)y * y);
        }

        T length() const {
            return (T)sqrtf(length2());
        }

        vector2d operator-() const { return vector2d(-x, -y); }

        vector2d perp() const { return vector2d(-y, x); }

        const T &operator[](i32 i) const { return (&x)[i]; }
              T &operator[](i32 i)       { return (&x)[i]; }

        bool is_valid() const {
            return x > -1e30f && x < 1e30f &&
                   y > -1e30f && y < 1e30f;
        }
    };

    ASSERT_SIZEOF(vector2d, 0x08);

    inline vector2d operator+(const vector2d &a, const vector2d &b) { return vector2d(a.x + b.x, a.y + b.y); }
    inline vector2d operator-(const vector2d &a, const vector2d &b) { return vector2d(a.x - b.x, a.y - b.y); }

    inline vector2d operator*(const vector2d &a, vector2d::T d) { return vector2d(d * a.x, d * a.y); }
    inline vector2d operator*(vector2d::T d, const vector2d &a) { return vector2d(d * a.x, d * a.y); }

    inline vector2d operator/(const vector2d &a, vector2d::T d) {
        f32 d_inv = 1.0f / d;

        return vector2d(a.x * d_inv, a.y * d_inv);
    }

    inline bool operator==(const vector2d &a, const vector2d &b) { return (a.x == b.x) && (a.y == b.y); }
    inline bool operator!=(const vector2d &a, const vector2d &b) { return (a.x != b.x) || (a.y != b.y); }

    inline vector2d::T dot     (const vector2d &a, const vector2d &b) { return a.x * b.x + a.y * b.y; }
    inline vector2d::T perp_dot(const vector2d &a, const vector2d &b) { return a.y * b.x - a.x * b.y; }

    inline bool approx_equals(const vector2d &a, const vector2d &b, vector2d::T dist) {
        return (a - b).length2() <= dist * dist;
    }
} // treyarch
