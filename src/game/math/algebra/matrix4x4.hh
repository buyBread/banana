#pragma once

#include "game/math/algebra/vector4d.hh"
#include "treyarch/math/rotation.hh"

namespace treyarch {
    typedef math::mat_class<4, 4, math::mat_kind_full, math::rep_sm2> matrix4x4;
} // treyarch

namespace treyarch { namespace math {
    template<>
    class mat_class<4, 4, mat_kind_full, rep_sm2> { // matrix4x4

    protected:
        typedef vector4 row_type;

    public:
        typedef f32      T;
        typedef vector4d row;

        row x, y, z, w;

        mat_class() {}

        typedef row_type get_x_type;
        typedef row_type get_y_type;
        typedef row_type get_z_type;
        typedef row_type get_w_type;

        const get_x_type &get_x() const { return reinterpret_cast<const row_type&>(x); }
        const get_y_type &get_y() const { return reinterpret_cast<const row_type&>(y); }
        const get_z_type &get_z() const { return reinterpret_cast<const row_type&>(z); }
        const get_w_type &get_w() const { return reinterpret_cast<const row_type&>(w); }

        get_x_type &get_x() { return reinterpret_cast<row_type&>(x); }
        get_y_type &get_y() { return reinterpret_cast<row_type&>(y); }
        get_z_type &get_z() { return reinterpret_cast<row_type&>(z); }
        get_w_type &get_w() { return reinterpret_cast<row_type&>(w); }

        void get_xyz(get_x_type &_x, get_y_type &_y, get_z_type &_z) const {
            _x = get_x();
            _y = get_y();
            _z = get_z();
        }

        void get_xyzw(get_x_type &_x, get_y_type &_y, get_z_type &_z, get_w_type &_w) const {
            _x = get_x();
            _y = get_y();
            _z = get_z();
            _w = get_w();
        }

        template<typename V> void set_x(const V &r) { reinterpret_cast<row_type&>(x) = r; }
        template<typename V> void set_y(const V &r) { reinterpret_cast<row_type&>(y) = r; }
        template<typename V> void set_z(const V &r) { reinterpret_cast<row_type&>(z) = r; }
        template<typename V> void set_w(const V &r) { reinterpret_cast<row_type&>(w) = r; }

        template<typename I> const row &operator[](I i) const { return (&x)[i]; }
        template<typename I>       row &operator[](I i)       { return (&x)[i]; }

        typedef const mat_class<4, 4, mat_kind_full, rep_constant>  constant;
        typedef       mat_class<4, 4, mat_kind_full, rep_std<true>> packed;

        template<i32 R1, i32 C1, typename T1, typename P1>
        mat_class(const mat_class<R1, C1, T1, P1> &a) {
            typename mat_class<R1, C1, T1, P1>::get_x_type x1;
            typename mat_class<R1, C1, T1, P1>::get_y_type y1;
            typename mat_class<R1, C1, T1, P1>::get_z_type z1;
            typename mat_class<R1, C1, T1, P1>::get_w_type w1;

            a.get_xyzw(x1, y1, z1, w1);

            set_x(x1);
            set_y(y1);
            set_z(z1);
            set_w(w1);
        }

        mat_class(const row_type &v0, const row_type &v1, const row_type &v2, const row_type &v3) {
            set_x(v0);
            set_y(v1);
            set_z(v2);
            set_w(v3);
        }

        template<i32 R1, i32 C1, typename T1, typename P1>
        mat_class &operator=(const mat_class<R1, C1, T1, P1> &a) {
            typename mat_class<R1, C1, T1, P1>::get_x_type x1;
            typename mat_class<R1, C1, T1, P1>::get_y_type y1;
            typename mat_class<R1, C1, T1, P1>::get_z_type z1;
            typename mat_class<R1, C1, T1, P1>::get_w_type w1;

            a.get_xyzw(x1, y1, z1, w1);

            set_x(x1);
            set_y(y1);
            set_z(z1);
            set_w(w1);

            return *this;
        }

        template<typename TT> mat_class &operator+=(const TT &a) { return *this = *this + a; }
        template<typename TT> mat_class &operator-=(const TT &a) { return *this = *this - a; }
        template<typename TT> mat_class &operator*=(const TT &a) { return *this = *this * a; }
        template<typename TT> mat_class &operator/=(const TT &a) { return *this = *this / a; }

        mat_class(T _00, T _01, T _02, T _03,
                  T _10, T _11, T _12, T _13,
                  T _20, T _21, T _22, T _23,
                  T _30, T _31, T _32, T _33) : x(_00, _01, _02, _03),
                                                y(_10, _11, _12, _13),
                                                z(_20, _21, _22, _23),
                                                w(_30, _31, _32, _33) {}

              vector3d &xrow()       { return *(vector3d*)&x; }
        const vector3d &xrow() const { return *(vector3d*)&x; }
              vector3d &yrow()       { return *(vector3d*)&y; }
        const vector3d &yrow() const { return *(vector3d*)&y; }
              vector3d &zrow()       { return *(vector3d*)&z; }
        const vector3d &zrow() const { return *(vector3d*)&z; }
              vector3d &wrow()       { return *(vector3d*)&w; }
        const vector3d &wrow() const { return *(vector3d*)&w; }

        // sub_95D960
        void make_projection(T field_of_view = 1.570795f,
                             T aspect        = 1.0f,
                             T near_plane    = 1.0f,
                             T far_plane     = 1000.0f,
                             T push          = 0.0f) {

            x.y = x.z = x.w =
            y.x = y.z = y.w =
            z.x = z.y =
            w.x = w.y = 0.0f;

            x.x = 1.0f / tan(field_of_view * 0.5f);
            y.y = x.x / aspect;
            z.z = (f32)((f64)far_plane / ((f64)far_plane - near_plane));
            z.w = 1.0f;
            w.z = (f32)(((f64)push - near_plane) * z.z);
            w.w = 0.0f;
        }

        void make_frustum(T left   = -1.0f,
                          T top    = -1.0f,
                          T right  =  1.0f,
                          T bottom =  1.0f,
                          T znear  =  1.0f,
                          T zfar   =  1000.0f,
                          T push   =  0.0f) {

            T xq = 1.0f / (right - left);
            T yq = 1.0f / (bottom - top);
            T zq = zfar / (zfar - znear);

            x.y = x.z = x.w =
            y.x = y.z = y.w =
            w.x = w.y = 0.0f;

            x.x = 2.0f * znear * xq;
            y.y = 2.0f * znear * yq;
            z.x = (right + left) * xq;
            z.y = (bottom + top) * yq;
            z.z = zq;
            z.w = 1.0f;
            w.z = (push - znear) * zq;
            w.w = 0.0f;
        }

        mat_class &operator*=(T s) {
            x.x *= s; x.y *= s; x.z *= s; x.w *= s;
            y.x *= s; y.y *= s; y.z *= s; y.w *= s;
            z.x *= s; z.y *= s; z.z *= s; z.w *= s;
            w.x *= s; w.y *= s; w.z *= s; w.w *= s;

            return *this;
        }

        void make_translate(const vector3d &t) {
            x = unit_dir_x();
            y = unit_dir_y();
            z = unit_dir_z();
            w = position3(t);
        }

        void make_rotate(const vector3d &axis, const float1 &angle) {
            const float4 &sc = sin_cos<sin_cos_sin, sin_cos_cos, sin_cos_any, sin_cos_any>(angle);

            float4 r0, r1, r2;
            axis_sin_cos_to_matrix(axis.val34(), sc.get_x(), sc.get_y(), r0, r1, r2);

            set_x(unit_dir3(r0));
            set_y(unit_dir3(r1));
            set_z(unit_dir3(r2));
            set_w(position3_zero());
        }

        void make_scale(const vector3d &s) {
            *this = diag_mat33(s);
        }

        void identity() {
            set_x(unit_dir_x());
            set_y(unit_dir_y());
            set_z(unit_dir_z());
            set_w(unit_dir_w());
        }

        void translate(const vector3d &t) {
            reinterpret_cast<dir3&>(w) += t;
        }

        void rotate(const vector3d &axis, const float1 &angle) {
            *this *= axis_angle_to_rot_mat(declare_unit(axis), angle);
        }

        void scale(const vector3d &s) {
            *this *= diag_mat33(s);
        }

        void scale(const float1 &s) {
            *this *= diag_mat33(dir3(float4(s)));
        }

        mat44 transpose() const { return ~*this; }

        void orthonormalize() {
            zrow().normalize();
            (yrow() -= zrow() * (zrow() * yrow())).normalize();
            xrow() = cross(yrow(), zrow());
        }
    };

    inline void fast_matrix4x4_mul(matrix4x4 &res, const matrix4x4 &a, const matrix4x4 &b) {
        res = a * b;
    }

    inline dir3 xform3d_0(const matrix4x4 &m44, const dir3 &sv) {
        return sv * reinterpret_cast<const mat43&>(m44);
    }

    inline dir3 xform3d_1(const matrix4x4 &m44, const dir3 &sv) {
        return dir3((position3)sv * reinterpret_cast<const mat43&>(m44));
    }

    inline vector3d xform3d_1_homog(const matrix4x4 &m44, const dir3 &sv) {
        vector4 tmp = reinterpret_cast<const position3&>(sv) * m44;

        return vector3d(tmp * inv_safe(tmp.get_w()));
    }

    inline vector4d xform4d(const matrix4x4 &m44, const vector4d &sv) {
        return sv * m44;
    }
}} // treyarch::math

namespace treyarch {
    ASSERT_SIZEOF(matrix4x4, 0x40);
} // treyarch
