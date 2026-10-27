#pragma once

#include "game/math/algebra/vector3d.hh"

namespace treyarch {
    typedef math::vec_class<4, -1, void, void, math::rep_sm2> vector4d;
} // treyarch

namespace treyarch { namespace math {
    template<typename U0>
    class vec_class<4, -1, void, U0, rep_sm2> { // vector4d

    public:
        typedef vec_class<4, -1, void, U0, rep_constant> constant;

        typedef f32 T;

        f32 x, y, z, w;

        explicit vec_class(const float4 &_v) { _v.store4((f32*)this); }

        float4 baseval() const {
            float4 result;
            result.load4((f32*)this);

            return result;
        }

        float4 val4()  const { return baseval(); }
        float4 val34() const { return baseval(); }

        operator const vec_class<4>() const { return vec_class<4>(baseval()); }

        typedef f32 get_x_type;
        typedef f32 get_y_type;
        typedef f32 get_z_type;
        typedef f32 get_w_type;

        const get_x_type &get_x() const { return x; }
        const get_y_type &get_y() const { return y; }
        const get_z_type &get_z() const { return z; }
        const get_w_type &get_w() const { return w; }

        template<typename V> void set_x(const V &val) { x = val; }
        template<typename V> void set_y(const V &val) { y = val; }
        template<typename V> void set_z(const V &val) { z = val; }
        template<typename V> void set_w(const V &val) { w = val; }

        vec_class() {}

        template<typename X, typename Y, typename Z, typename W>
        vec_class(const X &_x, const Y &_y, const Z &_z, const W &_w) : x(_x),
                                                                        y(_y),
                                                                        z(_z),
                                                                        w(_w) {}

        template<i32 L, typename A, typename U, typename R>
        vec_class(const vec_class<3, L, A, U, R> &_v, f32 f) : w(f) {
            float4(_v.val34()).store3((f32*)this);
        }

        template<i32 L, typename A, typename U, typename R>
        explicit vec_class(const vec_class<3, L, A, U, R> &_v) {
            float4(_v.val4()).store4((f32*)this);
        }

        template<i32 S, i32 L, typename A, typename U, typename R>
        vec_class(const vec_class<S, L, A, U, R> &_v) {
            float4(_v.val4()).store4((f32*)this);
        }

        template<typename U>
        vec_class &operator=(const vec_class<4, -1, void, U> &_v) {
            float4(_v.val4()).store4((f32*)this); return *this;
        }

        template<i32 S, i32 L, typename A, typename U, typename R>
        vec_class &operator=(const vec_class<S, L, A, U, R> &_v) {
            float4(_v.val4()).store4((f32*)this); return *this;
        }

        template<typename I> const f32 &operator[](I i) const { return ((const f32*)this)[i]; }
        template<typename I>       f32 &operator[](I i)       { return ((f32*)this)[i]; }

        template<typename TT> vec_class &operator+=(const TT &a) { return *this = *this + a; }
        template<typename TT> vec_class &operator-=(const TT &a) { return *this = *this - a; }
        template<typename TT> vec_class &operator*=(const TT &a) { return *this = *this * a; }
        template<typename TT> vec_class &operator/=(const TT &a) { return *this = *this / a; }

        explicit vec_class(f32 d) { x = y = z = w = d; }

        f32 length2() const { return (f32)((f64)x * x + (f64)y * y + (f64)z * z + (f64)w * w); }
        f32 length()  const { return sqrt(length2()); }

        vec_class &normalize() {
            *this = unitize(*this);

            return *this;
        }

        vec_class &homogenize() {
            w = inv_safe(w);

            x *= w;
            y *= w;
            z *= w;

            return *this;
        }

        vec_class &set_length(f32 newlen = 1.0f) {
            *this *= newlen * rsqrt_safe(length2());

            return *this;
        }

        const vector2d &get_xy()  const { return *(vector2d*)&x; }
        const vector3d &get_xyz() const { return *(vector3d*)&x; }

        template<i32 L, typename A, typename U, typename R>
        void set_xyz(const vec_class<3, L, A, U, R> &new_xyz) {
            *(vector3d*)this = new_xyz;
        }

        void set_xy(const vector2d &new_xy) {
            x = new_xy.x;
            y = new_xy.y;
        }

        bool is_valid() const {
            return x > -1e30f && x < 1e30f &&
                   y > -1e30f && y < 1e30f &&
                   z > -1e30f && z < 1e30f &&
                   w > -1e30f && w < 1e30f;
        }
    };

    inline vector4d::T dot(const vector4d &a, const vector4d &b) {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }
}} // treyarch::math

namespace treyarch {
    ASSERT_SIZEOF(vector4d, 0x10);
} // treyarch
