#pragma once

#include "treyarch/shared/math/algebra/quaternion.hh"

namespace treyarch { namespace math {
    inline dir3 point_line_closest_point(const dir3 &point, const dir3 &o, const dir3 &v, bool clip_edges = false, f32* pt = nullptr) {
        float1 t1 = (v * (point - o)) / (v * v);
        f32    t  = (f32)t1;

        if (pt)
            *pt = t;

        if (clip_edges) {
            if (t < 0.0f)
                t1 = float1_zero;
            else if (t > 1.0f)
                t1 = float1_one;
        }

        return o + t1 * v;
    }

    inline vector3d::T dot(const vector3d &a, const vector3d &b) {
        vector3d::T t = (f32)((f64)a.x * b.x + (f64)a.y * b.y + (f64)a.z * b.z);

        return t;
    }

    template<i32 S1, i32 L1, typename A1, typename U1, typename R1,
             i32 S2, i32 L2, typename A2, typename U2, typename R2>
    float1 dot(const vec_class<S1, L1, A1, U1, R1> &a, const vec_class<S2, L2, A2, U2, R2> &b) {
        return a * b;
    }

    inline vector3d::T xz_dot(const vector3d &a, const vector3d &b) {
        vector3d::T t = a.x * b.x + a.z * b.z;

        return t;
    }

    template<i32 L1, typename A1, typename U1, typename R1,
             i32 L2, typename A2, typename U2, typename R2>
    dir3 cross(const vec_class<3, L1, A1, U1, R1> &a, const vec_class<3, L2, A2, U2, R2> &b) {
        return cross(dir3(a), dir3(b));
    }

    inline dir3 cross(const vector3d &a, const vector3d &b) {
        return dir3((f32)((f64)a.y * b.z - (f64)a.z * b.y),
                    (f32)((f64)a.z * b.x - (f64)a.x * b.z),
                    (f32)((f64)a.x * b.y - (f64)a.y * b.x));
    }

    inline dir3 lerp(const dir3 &a, const dir3 &b, const float1 &t) {
        return a + (b - a) * t;
    }

    inline vector3d get_triangle_normal(const vector3d &p1, const vector3d &p2, const vector3d &p3) {
        vector3d e0(p1 - p2);
        vector3d e1(p3 - p2);
        vector3d n = cross(e0, e1);

        n.normalize();

        return n;
    }

    template<i32 S1, i32 L1, typename A1, typename U1, typename R1,
             i32 S2, i32 L2, typename A2, typename U2, typename R2>
    bool approx_equals(const vec_class<S1, L1, A1, U1, R1> &a, const vec_class<S2, L2, A2, U2, R2> &b, f32 dist) {
        return abs_squared(a - b) <= dist * dist;
    }

    inline bool approx_equals(const matrix4x4 &a, const matrix4x4 &b, matrix4x4::T dist) {
        if (approx_equals(a[0], b[0], dist))
            if (approx_equals(a[1], b[1], dist))
                if (approx_equals(a[2], b[2], dist))
                    if (approx_equals(a[3], b[3], dist))
                        return true;

        return false;
    }

    template<i32 S1, i32 L1, typename A1, typename U1, typename R1,
             i32 S2, i32 L2, typename A2, typename U2, typename R2>
    f32 angle_between(const vec_class<S1, L1, A1, U1, R1> &v1, const vec_class<S2, L2, A2, U2, R2> &v2) {
        return atan_upper(abs(cross(v1, v2)), v1 * v2);
    }

    template<i32 S1, i32 L1, typename A1, i32 U1, typename R1,
             i32 S2, i32 L2, typename A2, i32 U2, typename R2>
    f32 angle_between(const vec_class<S1, L1, A1, vec_unit<U1>, R1> &v1, const vec_class<S2, L2, A2, vec_unit<U2>, R2> &v2) {
        return acos(v1 * v2);
    }

    inline const float4::constant cos_deg_inv360 = { 1.0f / 360.0f, 1.0f / 360.0f, 1.0f / 360.0f, 1.0f / 360.0f };

    inline float1 cos_deg(f32 a) { return cos(float4(a), cos_deg_inv360).get_x(); }
    inline float1 sin_deg(f32 a) { return cos_deg(a + 270.0f); }

    template<typename A1, typename U1, typename R1, typename A2, typename U2, typename R2>
    float1 dot_xz(const vec_class<3, 0, A1, U1, R1> &a, const vec_class<3, 0, A2, U2, R2> &b) {
        float4 val = float4(a.val34()) * float4(b.val34());

        return val.get_x() + val.get_z();
    }

    template<typename A, typename U, typename R>
    float1 abs_squared_xz(const vec_class<3, 0, A, U, R> &a) {
        float4 val = a.val34();
        val *= val;

        return val.get_x() + val.get_z();
    }

    template<typename A, typename U, typename R>
    float1 abs_xz(const vec_class<3, 0, A, U, R> &a) {
        return sqrt(abs_squared_xz(a));
    }

    enum e_elmt {
        elmt_x,
        elmt_y,
        elmt_z,
        elmt_w
    };
}} // treyarch::math
