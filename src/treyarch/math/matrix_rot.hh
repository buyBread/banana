#pragma once

#include "treyarch/math/matrix_diag.hh"
#include "treyarch/math/quaternion.hh"

namespace treyarch { namespace math {
    template<typename T> class mat_kind_rot;

    template<i32 U1, typename R1, typename T, typename P>
    class result_type<op_mul, vec_class<3, 0, void, vec_unit<U1>, R1>, mat_class<3, 3, mat_kind_rot<T>, P>> {

    public:
        typedef unit_dir3 type;
    };

    template<i32 U1, typename R1, typename T, bool fast_mul, typename P>
    class result_type<op_mul, vec_class<3, 0, void, vec_unit<U1>, R1>, mat_class<4, 3, mat_kind_tran<mat_kind_rot<T>, fast_mul>, P>> {

    public:
        typedef unit_dir3 type;
    };

    template<i32 A1, i32 U1, typename R1, typename T, typename P>
    class result_type<op_mul, vec_class<3, 0, vec_axis<A1>, vec_unit<U1>, R1>, mat_class<3, 3, mat_kind_rot<T>, P>> {

    public:
        typedef unit_dir3 type;
    };

    template<i32 A1, i32 U1, typename R1, typename T, bool fast_mul, typename P>
    class result_type<op_mul, vec_class<3, 0, vec_axis<A1>, vec_unit<U1>, R1>, mat_class<4, 3, mat_kind_tran<mat_kind_rot<T>, fast_mul>, P>> {

    public:
        typedef unit_dir3 type;
    };

    class mat_kind_rot_full;

    typedef mat_class<3, 3, mat_kind_rot<mat_kind_rot_full>>                       rot_mat33;
    typedef mat_class<4, 3, mat_kind_tran<mat_kind_rot<mat_kind_rot_full>, false>> rot_tran_mat43;

    typedef const rot_mat33      &rot_mat33_arg;
    typedef const rot_tran_mat43 &rot_tran_mat43_arg;

    template<bool P>
    class mat_class<3, 3, mat_kind_rot<mat_kind_rot_full>, rep_std<P>> {

    protected:
        typedef vec_class<3, 0, void, vec_unit<1>, rep_std<P>> row_type;
        typedef position3_zero                                 last_row_type;

        row_type v[3];

    public:
        mat_class() {}

        template<i32 R1, i32 C1, typename T1, typename P1>
        explicit mat_class(const mat_class<R1, C1, T1, P1> &a) {
            typename mat_class<R1, C1, T1, P1>::get_x_type x1;
            typename mat_class<R1, C1, T1, P1>::get_y_type y1;
            typename mat_class<R1, C1, T1, P1>::get_z_type z1;

            a.get_xyz(x1, y1, z1);

            unit_dir3 z = unitize((dir3)z1);
            unit_dir3 x = unitize(cross((dir3)y1, z));
            unit_dir3 y = declare_unit(cross(z, x));

            v[0] = x;
            v[1] = y;
            v[2] = z;
        }

        template<typename T1, typename P1>
        mat_class(const mat_class<3, 3, mat_kind_rot<T1>, P1> &a) {
            typename mat_class<3, 3, mat_kind_rot<T1>, P1>::get_x_type x1;
            typename mat_class<3, 3, mat_kind_rot<T1>, P1>::get_y_type y1;
            typename mat_class<3, 3, mat_kind_rot<T1>, P1>::get_z_type z1;

            a.get_xyz(x1, y1, z1);

            v[0] = x1;
            v[1] = y1;
            v[2] = z1;
        }

        template<i32 d>
        mat_class(const mat_class<d, d, mat_kind_vec<mat_kind_diag>, rep_identity>&) {
            v[0] = unit_dir_x();
            v[1] = unit_dir_y();
            v[2] = unit_dir_z();
        }

        mat_class(const row_type &v0, const row_type &v1, const row_type &v2) {
            v[0] = v0;
            v[1] = v1;
            v[2] = v2;
        }

        typedef row_type      get_x_type;
        typedef row_type      get_y_type;
        typedef row_type      get_z_type;
        typedef last_row_type get_w_type;

        const get_x_type &get_x() const { return v[0]; }
        const get_y_type &get_y() const { return v[1]; }
        const get_z_type &get_z() const { return v[2]; }
              get_w_type  get_w() const { return get_w_type(); }

        get_x_type &get_x() { return v[0]; }
        get_y_type &get_y() { return v[1]; }
        get_z_type &get_z() { return v[2]; }

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

        template<typename V> void set_x(const V &r) { v[0] = r; }
        template<typename V> void set_y(const V &r) { v[1] = r; }
        template<typename V> void set_z(const V &r) { v[2] = r; }

        template<typename T> const row_type &operator[](T i) const { return v[i]; }
        template<typename T>       row_type &operator[](T i)       { return v[i]; }

        typedef const mat_class<3, 3, mat_kind_rot<mat_kind_rot_full>, rep_constant>  constant;
        typedef       mat_class<3, 3, mat_kind_rot<mat_kind_rot_full>, rep_std<true>> packed;

        template<typename T1, typename P1>
        mat_class &operator=(const mat_class<3, 3, mat_kind_rot<T1>, P1> &a) {
            typename mat_class<3, 3, mat_kind_rot<T1>, P1>::get_x_type x1;
            typename mat_class<3, 3, mat_kind_rot<T1>, P1>::get_y_type y1;
            typename mat_class<3, 3, mat_kind_rot<T1>, P1>::get_z_type z1;

            a.get_xyz(x1, y1, z1);

            v[0] = x1;
            v[1] = y1;
            v[2] = z1;

            return *this;
        }

        template<typename T> mat_class &operator*=(const T &a) { return *this = *this * a; }
        template<typename T> mat_class &operator/=(const T &a) { return *this = *this / a; }
    };

    template<>
    class mat_class<3, 3, mat_kind_rot<mat_kind_rot_full>, rep_constant> {

    public:
        typedef unit_dir3      row_type;
        typedef position3_zero last_row_type;

        struct { f32 x, y, z, w; } r0_;
        struct { f32 x, y, z, w; } r1_;
        struct { f32 x, y, z, w; } r2_;

        typedef row_type      get_x_type;
        typedef row_type      get_y_type;
        typedef row_type      get_z_type;
        typedef last_row_type get_w_type;

        operator const mat_class<3, 3, mat_kind_rot<mat_kind_rot_full>>&() const { return reinterpret_cast<const mat_class<3, 3, mat_kind_rot<mat_kind_rot_full>>&>(*this); }

        const get_x_type &get_x() const { return reinterpret_cast<const row_type&>(r0_); }
        const get_y_type &get_y() const { return reinterpret_cast<const row_type&>(r1_); }
        const get_z_type &get_z() const { return reinterpret_cast<const row_type&>(r2_); }
              get_w_type  get_w() const { return get_w_type(); }

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
    };

    template<typename T1, typename T2>
    class mat_result_kind<op_mul, mat_kind_rot<T1>, mat_kind_rot<T2>> {

    public:
        typedef mat_kind_rot<mat_kind_rot_full> type;
    };

    template<typename T1, typename T2, bool fast_mul1>
    class mat_result_kind<op_mul, mat_kind_tran<mat_kind_rot<T1>, fast_mul1>, mat_kind_rot<T2>> {

    public:
        typedef mat_kind_tran<mat_kind_rot<mat_kind_rot_full>, false> type;
    };

    template<typename T1, typename T2, bool fast_mul2>
    class mat_result_kind<op_mul, mat_kind_rot<T1>, mat_kind_tran<mat_kind_rot<T2>, fast_mul2>> {

    public:
        typedef mat_kind_tran<mat_kind_rot<mat_kind_rot_full>, false> type;
    };

    template<typename T1, typename T2, bool fast_mul1, bool fast_mul2>
    class mat_result_kind<op_mul, mat_kind_tran<mat_kind_rot<T1>, fast_mul1>, mat_kind_tran<mat_kind_rot<T2>, fast_mul2>> {

    public:
        typedef mat_kind_tran<mat_kind_rot<mat_kind_rot_full>, false> type;
    };

    template<typename T>
    class mat_result_kind<op_mul, mat_kind_rot<T>, mat_kind_rot<T>> {

    public:
        typedef mat_kind_rot<T> type;
    };

    template<typename T>
    class mat_result_kind<op_mul, mat_kind_tran<mat_kind_rot<T>, true>, mat_kind_rot<T>> {

    public:
        typedef mat_kind_tran<mat_kind_rot<T>, true> type;
    };

    template<typename T>
    class mat_result_kind<op_mul, mat_kind_rot<T>, mat_kind_tran<mat_kind_rot<T>, true>> {

    public:
        typedef mat_kind_tran<mat_kind_rot<T>, true> type;
    };

    template<typename T>
    class mat_result_kind<op_mul, mat_kind_tran<mat_kind_rot<T>, true>, mat_kind_tran<mat_kind_rot<T>, true>> {

    public:
        typedef mat_kind_tran<mat_kind_rot<T>, true> type;
    };

    template<typename T, typename P>
    typename result_type<op_inv, mat_class<3, 3, mat_kind_rot<T>, P>>::type operator~(const mat_class<3, 3, mat_kind_rot<T>, P> &a) {
        return inv(a);
    }

    template<typename P>
    rot_mat33 inv(const mat_class<3, 3, mat_kind_rot<mat_kind_rot_full>, P> &a) {
        typename mat_class<3, 3, mat_kind_rot<mat_kind_rot_full>, P>::get_x_type x1;
        typename mat_class<3, 3, mat_kind_rot<mat_kind_rot_full>, P>::get_y_type y1;
        typename mat_class<3, 3, mat_kind_rot<mat_kind_rot_full>, P>::get_z_type z1;

        a.get_xyz(x1, y1, z1);

        float4 r0, r1, r2;
        transpose(r0, r1, r2, float4(x1.val34()), float4(y1.val34()), float4(z1.val34()));

        return rot_mat33(unit_dir3(r0), unit_dir3(r1), unit_dir3(r2));
    }

    template<i32 R, i32 C, typename P>
    class result_type<op_get_quaternion, mat_class<R, C, mat_kind_rot<mat_kind_rot_full>, P>> {

    public:
        typedef unit_quaternion type;
    };

    template<i32 R, i32 C, typename P>
    unit_quaternion get_quaternion(const mat_class<R, C, mat_kind_rot<mat_kind_rot_full>, P> &rot) {
        f32 r00 = rot.get_x()[0];
        f32 r11 = rot.get_y()[1];
        f32 r22 = rot.get_z()[2];

        const float4 &r0 = float4(rot.get_x().val34());
        const float4 &r1 = float4(rot.get_y().val34());
        const float4 &r2 = float4(rot.get_z().val34());

        f32 tr = r00 + r11 + r22;

        if (tr >= -0.333333f) {
            tr += 1.0f;

            return declare_unit(quaternion(r2.get_y() - r1.get_z(), r0.get_z() - r2.get_x(), r1.get_x() - r0.get_y(), (float1)tr) * (0.5f * rsqrt(tr)));
        } else if (r00 < r11) {
            if (r22 < r11) {
                tr = r11 - r00 - r22 + 1.0f;

                return declare_unit(quaternion(r1.get_x() + r0.get_y(), (float1)tr, r2.get_y() + r1.get_z(), r0.get_z() - r2.get_x()) * (0.5f * rsqrt(tr)));
            }
        } else if (r22 < r00) {
            tr = r00 - r11 - r22 + 1.0f;

            return declare_unit(quaternion((float1)tr, r1.get_x() + r0.get_y(), r0.get_z() + r2.get_x(), r2.get_y() - r1.get_z()) * (0.5f * rsqrt(tr)));
        }

        tr = r22 - r00 - r11 + 1.0f;

        return declare_unit(quaternion(r0.get_z() + r2.get_x(), r2.get_y() + r1.get_z(), (float1)tr, r1.get_x() - r0.get_y()) * (0.5f * rsqrt(tr)));
    }

    template<i32 R, i32 C, typename T, typename P>
    class result_type<op_get_quaternion, mat_class<R, C, mat_kind_rot<T>, P>> {

    public:
        typedef typename mat_class<R, C, mat_kind_rot<T>, P>::get_quaternion_type type;
    };

    template<i32 R, i32 C, typename T, typename P>
    typename result_type<op_get_quaternion, mat_class<R, C, mat_kind_rot<T>, P>>::type get_quaternion(const mat_class<R, C, mat_kind_rot<T>, P> &rot) {
        return rot.get_quaternion();
    }

    template<i32 R, i32 C, typename T, bool fast_mul, typename P>
    class result_type<op_get_quaternion, mat_class<R, C, mat_kind_tran<mat_kind_rot<T>, fast_mul>, P>> {

        typedef typename result_type<op_get_m33, mat_class<R, C, mat_kind_tran<mat_kind_rot<T>, fast_mul>, P>>::type get_m33_type;

    public:
        typedef typename result_type<op_get_quaternion, get_m33_type>::type type;
    };

    template<i32 R, i32 C, typename T, bool fast_mul, typename P>
    typename result_type<op_get_quaternion, mat_class<R, C, mat_kind_tran<mat_kind_rot<T>, fast_mul>, P>>::type
        get_quaternion(const mat_class<R, C, mat_kind_tran<mat_kind_rot<T>, fast_mul>, P> &rot) {

        return get_quaternion(get_m33(rot));
    }

    class mat_kind_rot_quat;

    typedef mat_class<3, 3, mat_kind_rot<mat_kind_rot_quat>>                      quat_mat33;
    typedef mat_class<4, 3, mat_kind_tran<mat_kind_rot<mat_kind_rot_quat>, true>> quat_tran_mat43;

    typedef const quat_mat33       quat_mat33_arg;
    typedef const quat_tran_mat43 &quat_tran_mat43_arg;

    inline const float4::constant float4_nppz = { -1.0f,  1.0f,  1.0f, 0.0f };
    inline const float4::constant float4_pnpz = {  1.0f, -1.0f,  1.0f, 0.0f };
    inline const float4::constant float4_ppnz = {  1.0f,  1.0f, -1.0f, 0.0f };

    template<bool P>
    class mat_class<3, 3, mat_kind_rot<mat_kind_rot_quat>, rep_std<P>> {

    public:
        typedef quat_class<void, true, rep_std<P>> get_quaternion_type;

    protected:
        get_quaternion_type q;

    public:
        mat_class() {}

        template<typename A1, typename R1>
        mat_class(const quat_class<A1, true, R1> &_q) : q(_q) {}

        const get_quaternion_type &get_quaternion() const { return q; }
              get_quaternion_type &get_quaternion()       { return q; }

        template<typename T1, typename P1>
        mat_class(const mat_class<3, 3, mat_kind_rot<T1>, P1> &a) : q(math::get_quaternion(a)) {}

        template<i32 d>
        mat_class(const mat_class<d, d, mat_kind_vec<mat_kind_diag>, rep_identity>&) : q(position3_zero().val4()) {}

        template<typename T1, bool fast_mul1, typename P1>
        explicit mat_class(const mat_class<4, 3, mat_kind_tran<mat_kind_rot<T1>, fast_mul1>, P1> &a) : q(math::get_quaternion(a)) {}

        template<typename A1, i32 U1, typename R1>
        mat_class(const vec_class<3, 0, A1, vec_unit<U1>, R1> &axis, f32 s, f32 c) {
            f32 h = sqrt_abs(2.0f + abs(c + c));

            if (c >= 0.0f)
                q = get_quaternion_type(axis * (-s / h), 0.5f * h);
            else
                q = get_quaternion_type(axis * (-0.5f * h), s / h);
        }

        typedef unit_dir3      get_x_type;
        typedef unit_dir3      get_y_type;
        typedef unit_dir3      get_z_type;
        typedef position3_zero get_w_type;

        get_x_type get_x() const {
            const float4 q1 = q.val();
            const float4 q2 = q1 + q1;
            const float4 sw = swizzle<3, 2, 1, 0>(madd(q1, q2.get_w(), float4_zzzn));

            return get_x_type(madd(q2, q1.get_x(), reinterpret_cast<const float4&>(float4_pnpz), sw));
        }

        get_y_type get_y() const {
            const float4 q1 = q.val();
            const float4 q2 = q1 + q1;
            const float4 sw = swizzle<2, 3, 0, 0>(madd(q1, q2.get_w(), float4_zzzn));

            return get_y_type(madd(q2, q1.get_y(), reinterpret_cast<const float4&>(float4_ppnz), sw));
        }

        get_z_type get_z() const {
            const float4 q1 = q.val();
            const float4 q2 = q1 + q1;
            const float4 sw = swizzle<2, 3, 0, 0>(madd(q1, q2.get_w(), float4_zzzn));

            return get_z_type(madd(q2, q1.get_z(), reinterpret_cast<const float4&>(float4_nppz), sw));
        }

        void get_xyz(get_x_type &_x, get_y_type &_y, get_z_type &_z) const {
            float4 r0, r1, r2;
            quat_to_matrix(q.val(), r0, r1, r2);

            _x = get_x_type(r0);
            _y = get_y_type(r1);
            _z = get_z_type(r2);
        }

        void get_xyzw(get_x_type &_x, get_y_type &_y, get_z_type &_z, get_w_type&) const {
            get_xyz(_x, _y, _z);
        }

        typedef const mat_class<3, 3, mat_kind_rot<mat_kind_rot_quat>, rep_constant> constant;
        typedef mat_class<3, 3, mat_kind_rot<mat_kind_rot_quat>, rep_std<true>>       packed;

        template<typename T1, typename P1>
        mat_class &operator=(const mat_class<3, 3, mat_kind_rot<T1>, P1> &a) {
            q = math::get_quaternion(a);

            return *this;
        }

        template<typename T> mat_class &operator*=(const T &a) { return *this = *this * a; }
        template<typename T> mat_class &operator/=(const T &a) { return *this = *this / a; }
    };

    template<>
    class mat_class<3, 3, mat_kind_rot<mat_kind_rot_quat>, rep_constant> {

    public:
        typedef unit_quaternion get_quaternion_type;

        struct { f32 x, y, z, w; } q_;

        typedef unit_dir3                                         row_type;
        typedef position3_zero                                    last_row_type;
        typedef mat_class<3, 3, mat_kind_rot<mat_kind_rot_quat>> base_type;

        typedef row_type      get_x_type;
        typedef row_type      get_y_type;
        typedef row_type      get_z_type;
        typedef last_row_type get_w_type;

        const get_quaternion_type &get_quaternion() const { return reinterpret_cast<const get_quaternion_type&>(*this); }

        operator const mat_class<3, 3, mat_kind_rot<mat_kind_rot_quat>>&() const { return reinterpret_cast<const mat_class<3, 3, mat_kind_rot<mat_kind_rot_quat>>&>(*this); }

        get_x_type get_x() const { return reinterpret_cast<const base_type*>(this)->get_x(); }
        get_y_type get_y() const { return reinterpret_cast<const base_type*>(this)->get_y(); }
        get_z_type get_z() const { return reinterpret_cast<const base_type*>(this)->get_z(); }
        get_w_type get_w() const { return get_w_type(); }

        void get_xyz(get_x_type &_x, get_y_type &_y, get_z_type &_z) const {
            reinterpret_cast<const base_type*>(this)->get_xyz(_x, _y, _z);
        }

        void get_xyzw(get_x_type &_x, get_y_type &_y, get_z_type &_z, get_w_type &_w) const {
            reinterpret_cast<const base_type*>(this)->get_xyzw(_x, _y, _z, _w);
        }
    };

    template<typename P>
    quat_mat33 inv(const mat_class<3, 3, mat_kind_rot<mat_kind_rot_quat>, P> &a) {
        return quat_mat33(inv(get_quaternion(a)));
    }

    template<typename P>
    quat_mat33 operator*(const mat_class<3, 3, mat_kind_rot<mat_kind_rot_quat>, P> &a,
                         const mat_class<3, 3, mat_kind_rot<mat_kind_rot_quat>, P> &b) {

        return quat_mat33(get_quaternion(a) * get_quaternion(b));
    }

    template<typename P>
    quat_mat33 operator/(const mat_class<3, 3, mat_kind_rot<mat_kind_rot_quat>, P> &a,
                         const mat_class<3, 3, mat_kind_rot<mat_kind_rot_quat>, P> &b) {

        return quat_mat33(get_quaternion(a) / get_quaternion(b));
    }

    template<typename U1, typename R1,
             typename P2>
    vector4 operator*(const vec_class<4, -1, void, U1, R1>                       &a,
                      const mat_class<3, 3, mat_kind_rot<mat_kind_rot_quat>, P2> &b) {

        float4 tmp = quat_rotate(get_quaternion(b).val(), float4(a.val34()));
        tmp.set_w(a.get_w());

        return vector4(tmp);
    }

    template<typename U1, typename R1,
             typename P2>
    vec_class<3, 0, void, U1> operator*(const vec_class<3, 0, void, U1, R1>                        &a,
                                        const mat_class<3, 3, mat_kind_rot<mat_kind_rot_quat>, P2> &b) {

        return vec_class<3, 0, void, U1>(quat_rotate(get_quaternion(b).val(), float4(a.val34())));
    }

    template<typename U1, typename R1, typename P2>
    position3 operator*(const vec_class<3, 1, void, U1, R1>                        &a,
                        const mat_class<3, 3, mat_kind_rot<mat_kind_rot_quat>, P2> &b) {

        return position3(quat_rotate(get_quaternion(b).val(), float4(a.val34())));
    }

    // sub_9D25C0
    inline mat44 operator*(const mat44 &a, const rot_tran_mat43 &b) {
        return a * mat44(b);
    }

    // inlined @ sub_9D5510
    inline mat44 operator*(const rot_tran_mat43 &a, const mat44 &b) {
        return mat44(a) * b;
    }

    // sub_44D470
    inline rot_tran_mat43 inv(const rot_tran_mat43 &value) {
        const unit_dir3 &x = value.get_m33().get_x();
        const unit_dir3 &y = value.get_m33().get_y();
        const unit_dir3 &z = value.get_m33().get_z();
        const position3 &w = value.get_w();

        float4 r0, r1, r2;
        transpose(r0, r1, r2, x.val34(), y.val34(), z.val34());

        rot_mat33 rotation = rot_mat33(unit_dir3(r0), unit_dir3(r1), unit_dir3(r2));

        position3 translation(-(f32)((f64)w[0] * x[0] + (f64)w[1] * x[1] + (f64)w[2] * x[2]),
                              -(f32)((f64)w[0] * y[0] + (f64)w[1] * y[1] + (f64)w[2] * y[2]),
                              -(f32)((f64)w[0] * z[0] + (f64)w[1] * z[1] + (f64)w[2] * z[2]));

        return rot_tran_mat43(rotation, translation);
    }

    ASSERT_SIZEOF(rot_mat33,       0x30);
    ASSERT_SIZEOF(rot_tran_mat43,  0x40);
    ASSERT_SIZEOF(quat_mat33,      0x10);
    ASSERT_SIZEOF(quat_tran_mat43, 0x20);
}} // treyarch::math
