#pragma once

#include "treyarch/shared/math/algebra/vector2d.hh"
#include "treyarch/shared/math/vmath/vector.hh"

namespace treyarch { namespace math {
    class rep_sm2;
}} // treyarch::math

namespace treyarch {
    typedef math::vec_class<3, 0, void, void, math::rep_sm2> vector3d;
} // treyarch

namespace treyarch { namespace math {
    template<typename U0>
    class vec_class<3, 0, void, U0, rep_sm2> { // vector3d

    public:
        typedef vec_class<3, 0, void, void, rep_constant> constant;

        typedef f32 T;

        f32 x, y, z;

        explicit vec_class(const float4 &_v) { _v.store3((f32*)this); }

        float4 baseval() const {
            float4 result;
            result.load3((f32*)this);

            return result;
        }

        float4 val4()  const { return float4(x, y, z, 0.0f); }
        float4 val34() const { return baseval(); }

        operator const vec_class<3, 0>() const { return vec_class<3, 0>(baseval()); }

        typedef f32 get_x_type;
        typedef f32 get_y_type;
        typedef f32 get_z_type;
        typedef f32 get_w_type;

        const get_x_type &get_x() const { return x; }
        const get_y_type &get_y() const { return y; }
        const get_z_type &get_z() const { return z; }
              get_w_type  get_w() const { return 0.0f; }

        template<typename V> void set_x(const V &val) { x = val; }
        template<typename V> void set_y(const V &val) { y = val; }
        template<typename V> void set_z(const V &val) { z = val; }

        vec_class() {}

        template<typename X, typename Y, typename Z>
        vec_class(const X &_x, const Y &_y, const Z &_z) : x(_x), y(_y), z(_z) {}

        template<typename A, typename U, typename R>
        vec_class(const vec_class<3, 0, A, U, R> &_v) {
            float4(_v.val34()).store3((f32*)this);
        }

        template<i32 S, i32 L, typename A, typename U, typename R>
        explicit vec_class(const vec_class<S, L, A, U, R> &_v) {
            float4(_v.val34()).store3((f32*)this);
        }

        template<typename A, typename U, typename R>
        vec_class &operator=(const vec_class<3, 0, A, U, R> &_v) {
            float4(_v.val34()).store3((f32*)this); return *this;
        }

        template<typename I> const f32 &operator[](I i) const { return ((const f32*)this)[i]; }
        template<typename I>       f32 &operator[](I i)       { return ((f32*)this)[i]; }

        template<typename TT> vec_class &operator+=(const TT &a) { return *this = *this + a; }
        template<typename TT> vec_class &operator-=(const TT &a) { return *this = *this - a; }
        template<typename TT> vec_class &operator*=(const TT &a) { return *this = *this * a; }
        template<typename TT> vec_class &operator/=(const TT &a) { return *this = *this / a; }

        explicit vec_class(const vector2d &v, f32 d = 1.0f) : x(v.x), y(v.y), z(d) {}
        explicit vec_class(f32 d) : x(d), y(d), z(d) {}

        vec_class &operator=(const vec_class &v) {
            x = v.x;
            y = v.y;
            z = v.z;

            return *this;
        }

        vec_class &operator+=(const vec_class &v) {
            x += v.x;
            y += v.y;
            z += v.z;

            return *this;
        }

        vec_class &operator-=(const vec_class &v) {
            x -= v.x;
            y -= v.y;
            z -= v.z;

            return *this;
        }

        vec_class &operator*=(f32 d) {
            x *= d;
            y *= d;
            z *= d;

            return *this;
        }

        vec_class &operator/=(f32 d) {
            f32 d_inv = inv(d);

            x *= d_inv;
            y *= d_inv;
            z *= d_inv;

            return *this;
        }

        f32 length2() const { return (f32)((f64)x * x + (f64)y * y + (f64)z * z); }
        f32 length()  const { return sqrt(length2()); }

        vec_class &normalize() {
            *this = unitize(*this);

            return *this;
        }

        vec_class &set_length(f32 newlen = 1.0f) {
            *this *= newlen * rsqrt_safe(length2());

            return *this;
        }

        unit_dir3 perp() const {
            f32 ax = abs(x);
            f32 ay = abs(y);
            f32 az = abs(z);

            if (ax < az) {
                if (!(ay < ax)) {
                    f32 inv = rsqrt(y * y + z * z);

                    return unit_dir3(0.0f, z * inv, -y * inv);
                }
            } else if (!(ay < az)) {
                f32 inv = rsqrt_safe(x * x + y * y);

                return unit_dir3(y * inv, -x * inv, 0.0f);
            }

            f32 inv = rsqrt(x * x + z * z);

            return unit_dir3(z * inv, 0.0f, -x * inv);
        }

        const vector2d &get_xy() const { return *(vector2d*)&x; }
        const vector2d &get_yz() const { return *(vector2d*)&y; }
        const vector2d  get_xz() const { return vector2d(x, z); }

        f32 xy_length2() const { return (f32)((f64)x * x + (f64)y * y); }
        f32 yz_length2() const { return (f32)((f64)y * y + (f64)z * z); }
        f32 xz_length2() const { return (f32)((f64)x * x + (f64)z * z); }

        f32 xy_length() const { return sqrt(xy_length2()); }
        f32 yz_length() const { return sqrt(yz_length2()); }
        f32 xz_length() const { return sqrt(xz_length2()); }

        bool is_valid() const {
            return x > -1e30f && x < 1e30f &&
                   y > -1e30f && y < 1e30f &&
                   z > -1e30f && z < 1e30f;
        }
    };

    inline vector3d operator-(const vector3d &a, const vector3d &b) {
        return vector3d(a.x - b.x, a.y - b.y, a.z - b.z);
    }
}} // treyarch::math

namespace treyarch {
    ASSERT_SIZEOF(vector3d, 0x0C);
} // treyarch
