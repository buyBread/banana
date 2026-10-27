#pragma once

#include "treyarch/math/matrix.hh"

namespace treyarch { namespace math {
    class mat_kind_diag;
    class rep_identity;

    typedef mat_class<3, 3, mat_kind_vec<mat_kind_diag>>               diag_mat33;
    typedef mat_class<4, 4, mat_kind_vec<mat_kind_diag>>               diag_mat44;
    typedef mat_class<3, 3, mat_kind_vec<mat_kind_diag>, rep_identity> identity_mat33;

    typedef const diag_mat33      diag_mat33_arg;
    typedef const diag_mat44      diag_mat44_arg;
    typedef const identity_mat33 &identity_mat33_arg;

    template<bool P>
    class mat_class<3, 3, mat_kind_vec<mat_kind_diag>, rep_std<P>> {

    public:
        typedef vec_class<3, 1, void, void, rep_std<P>> vec_type;

    protected:
        vec_type v;

    public:
        mat_class() {}

        template<i32 S1, i32 L1, typename A1, typename U1, typename R1>
        explicit mat_class(const vec_class<S1, L1, A1, U1, R1> &_v) : v(vec_type(_v)) {}

        template<typename TX, typename TY, typename TZ>
        mat_class(TX x, TY y, TZ z) : v(x, y, z) {}

        template<typename P1>
        mat_class(const mat_class<3, 3, mat_kind_vec<mat_kind_diag>, P1> &a) { v = a.vec(); }

        template<typename P1>
        explicit mat_class(const mat_class<4, 4, mat_kind_vec<mat_kind_diag>, P1> &a) { v = a.vec(); }

        const vec_type &vec() const { return v; }

        typedef vec_class<3, 0, vec_axis<0>, void, rep_type<f32>> get_x_type;
        typedef vec_class<3, 0, vec_axis<1>, void, rep_type<f32>> get_y_type;
        typedef vec_class<3, 0, vec_axis<2>, void, rep_type<f32>> get_z_type;
        typedef position3_zero                                    get_w_type;

        get_x_type get_x() const { return get_x_type(v.get_x()); }
        get_y_type get_y() const { return get_y_type(v.get_y()); }
        get_z_type get_z() const { return get_z_type(v.get_z()); }
        get_w_type get_w() const { return get_w_type(); }

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

        typedef const mat_class<3, 3, mat_kind_vec<mat_kind_diag>, rep_constant>  constant;
        typedef       mat_class<3, 3, mat_kind_vec<mat_kind_diag>, rep_std<true>> packed;

        template<typename P1>
        mat_class &operator=(const mat_class<3, 3, mat_kind_vec<mat_kind_diag>, P1> &a) { v = a.vec(); return *this; }

        template<typename T> mat_class &operator+=(const T &a) { return *this = *this + a; }
        template<typename T> mat_class &operator-=(const T &a) { return *this = *this - a; }
        template<typename T> mat_class &operator*=(const T &a) { return *this = *this * a; }
        template<typename T> mat_class &operator/=(const T &a) { return *this = *this / a; }
    };

    template<>
    class mat_class<3, 3, mat_kind_vec<mat_kind_diag>, rep_constant> {

    public:
        f32 x_, y_, z_;
        f32 padw_;

        typedef position3 vec_type;

        const vec_type &vec() const { return reinterpret_cast<const vec_type&>(*this); }

        typedef vec_class<3, 0, vec_axis<0>, void, rep_type<f32>> get_x_type;
        typedef vec_class<3, 0, vec_axis<1>, void, rep_type<f32>> get_y_type;
        typedef vec_class<3, 0, vec_axis<2>, void, rep_type<f32>> get_z_type;
        typedef position3_zero                                    get_w_type;

        operator const mat_class<3, 3, mat_kind_vec<mat_kind_diag>>&() const { return reinterpret_cast<const mat_class<3, 3, mat_kind_vec<mat_kind_diag>>&>(*this); }

        get_x_type get_x() const { return get_x_type(x_); }
        get_y_type get_y() const { return get_y_type(y_); }
        get_z_type get_z() const { return get_z_type(z_); }
        get_w_type get_w() const { return get_w_type(); }

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

    template<bool P>
    class mat_class<4, 4, mat_kind_vec<mat_kind_diag>, rep_std<P>> {

    public:
        typedef vec_class<4, -1, void, void, rep_std<P>> vec_type;

    protected:
        vec_type v;

    public:
        mat_class() {}

        template<i32 S1, i32 L1, typename A1, typename U1, typename R1>
        explicit mat_class(const vec_class<S1, L1, A1, U1, R1> &_v) : v(_v) {}

        template<typename TX, typename TY, typename TZ, typename TW>
        mat_class(TX x, TY y, TZ z, TW w) : v(x, y, z, w) {}

        template<i32 d, typename P1>
        mat_class(const mat_class<d, d, mat_kind_vec<mat_kind_diag>, P1> &a) { v = a.vec(); }

        const vec_type &vec() const { return v; }

        typedef vec_class<3,  0, vec_axis<0>, void, rep_type<f32>> get_x_type;
        typedef vec_class<3,  0, vec_axis<1>, void, rep_type<f32>> get_y_type;
        typedef vec_class<3,  0, vec_axis<2>, void, rep_type<f32>> get_z_type;
        typedef vec_class<4, -1, vec_axis<3>, void, rep_type<f32>> get_w_type;

        get_x_type get_x() const { return get_x_type(v.get_x()); }
        get_y_type get_y() const { return get_y_type(v.get_y()); }
        get_z_type get_z() const { return get_z_type(v.get_z()); }
        get_w_type get_w() const { return get_w_type(v.get_w()); }

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

        typedef const mat_class<4, 4, mat_kind_vec<mat_kind_diag>, rep_constant> constant;
        typedef mat_class<4, 4, mat_kind_vec<mat_kind_diag>, rep_std<true>>       packed;

        template<i32 d, typename P1>
        mat_class &operator=(const mat_class<d, d, mat_kind_vec<mat_kind_diag>, P1> &a) { v = a.vec(); return *this; }

        template<typename T> mat_class &operator+=(const T &a) { return *this = *this + a; }
        template<typename T> mat_class &operator-=(const T &a) { return *this = *this - a; }
        template<typename T> mat_class &operator*=(const T &a) { return *this = *this * a; }
        template<typename T> mat_class &operator/=(const T &a) { return *this = *this / a; }
    };

    template<>
    class mat_class<4, 4, mat_kind_vec<mat_kind_diag>, rep_constant> {

    public:
        f32 x_, y_, z_, w_;

        typedef vector4 vec_type;

        const vec_type &vec() const { return reinterpret_cast<const vec_type&>(*this); }

        typedef vec_class<3,  0, vec_axis<0>, void, rep_type<f32>> get_x_type;
        typedef vec_class<3,  0, vec_axis<1>, void, rep_type<f32>> get_y_type;
        typedef vec_class<3,  0, vec_axis<2>, void, rep_type<f32>> get_z_type;
        typedef vec_class<4, -1, vec_axis<3>, void, rep_type<f32>> get_w_type;

        operator const mat_class<4, 4, mat_kind_vec<mat_kind_diag>>&() const { return reinterpret_cast<const mat_class<4, 4, mat_kind_vec<mat_kind_diag>>&>(*this); }

        get_x_type get_x() const { return get_x_type(x_); }
        get_y_type get_y() const { return get_y_type(y_); }
        get_z_type get_z() const { return get_z_type(z_); }
        get_w_type get_w() const { return get_w_type(w_); }

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

    template<>
    class mat_class<3, 3, mat_kind_vec<mat_kind_diag>, rep_identity> { // identity_mat33

    public:
        typedef position3 vec_type;

        const vec_type &vec() const { return reinterpret_cast<const vec_type&>(float4_ones); }

        typedef unit_dir_x     get_x_type;
        typedef unit_dir_y     get_y_type;
        typedef unit_dir_z     get_z_type;
        typedef position3_zero get_w_type;

        const get_x_type get_x() const { return get_x_type(); }
        const get_y_type get_y() const { return get_y_type(); }
        const get_z_type get_z() const { return get_z_type(); }
        const get_w_type get_w() const { return get_w_type(); }

        void get_xyz(get_x_type&, get_y_type&, get_z_type&) const {}
        void get_xyzw(get_x_type&, get_y_type&, get_z_type&, get_w_type&) const {}
    };

    template<i32 d, typename P>
    const mat_class<d, d, mat_kind_vec<mat_kind_diag>, P> &operator~(const mat_class<d, d, mat_kind_vec<mat_kind_diag>, P> &a) {
        return a;
    }

    template<typename P>
    mat_class<3, 3, mat_kind_vec<mat_kind_diag>> inv(const mat_class<3, 3, mat_kind_vec<mat_kind_diag>, P> &a) {
        return mat_class<3, 3, mat_kind_vec<mat_kind_diag>>(inv(a.vec()));
    }

    template<typename P>
    mat_class<4, 4, mat_kind_vec<mat_kind_diag>> inv(const mat_class<4, 4, mat_kind_vec<mat_kind_diag>, P> &a) {
        return mat_class<4, 4, mat_kind_vec<mat_kind_diag>>(inv(a.vec()));
    }

    inline identity_mat33 operator~(const identity_mat33&) { return identity_mat33(); }
    inline identity_mat33 inv(const identity_mat33&)       { return identity_mat33(); }

    template<i32 d1, typename P1, i32 d2, typename P2>
    mat_class<4, 4, mat_kind_vec<mat_kind_diag>> operator*(const mat_class<d1, d1, mat_kind_vec<mat_kind_diag>, P1> &a,
                                                           const mat_class<d2, d2, mat_kind_vec<mat_kind_diag>, P2> &b) {

        return mat_class<4, 4, mat_kind_vec<mat_kind_diag>>(vector4(float4(a.vec().val4()) * float4(b.vec().val4())));
    }

    template<typename P1, typename P2>
    mat_class<3, 3, mat_kind_vec<mat_kind_diag>> operator*(const mat_class<3, 3, mat_kind_vec<mat_kind_diag>, P1> &a,
                                                           const mat_class<3, 3, mat_kind_vec<mat_kind_diag>, P2> &b) {

        return mat_class<3, 3, mat_kind_vec<mat_kind_diag>>(dir3(float4(a.vec().val34()) * float4(b.vec().val34())));
    }

    template<i32 R, i32 C, typename T, typename P>
    const mat_class<R, C, T, P> &operator*(const mat_class<R, C, T, P> &a, const identity_mat33&) {
        return a;
    }

    template<i32 R, i32 C, typename T, typename P>
    const mat_class<R, C, T, P> &operator*(const identity_mat33&, const mat_class<R, C, T, P> &a) {
        return a;
    }

    inline identity_mat33 operator*(const identity_mat33&, const identity_mat33&) { return identity_mat33(); }

    template<i32 d1, typename P1, i32 d2, typename P2>
    mat_class<4, 4, mat_kind_vec<mat_kind_diag>> operator/(const mat_class<d1, d1, mat_kind_vec<mat_kind_diag>, P1> &a,
                                                           const mat_class<d2, d2, mat_kind_vec<mat_kind_diag>, P2> &b) {

        return mat_class<4, 4, mat_kind_vec<mat_kind_diag>>(vector4(float4(a.vec().val4()) / float4(b.vec().val4())));
    }

    template<typename P1, typename P2>
    mat_class<3, 3, mat_kind_vec<mat_kind_diag>> operator/(const mat_class<3, 3, mat_kind_vec<mat_kind_diag>, P1> &a,
                                                           const mat_class<3, 3, mat_kind_vec<mat_kind_diag>, P2> &b) {

        return mat_class<3, 3, mat_kind_vec<mat_kind_diag>>(dir3(float4(a.vec().val34()) / float4(b.vec().val34())));
    }

    template<i32 R, i32 C, typename T, typename P>
    const mat_class<R, C, T, P> &operator/(const mat_class<R, C, T, P> &a, const identity_mat33&) {
        return a;
    }

    template<i32 R, i32 C, typename T, typename P>
    typename result_type<op_inv, mat_class<R, C, T, P>>::type operator/(const identity_mat33&, const mat_class<R, C, T, P> &a) {
        return inv(a);
    }

    inline identity_mat33 operator/(const identity_mat33&, const identity_mat33&) { return identity_mat33(); }

    template<i32 S, i32 L, typename A, typename U, typename R>
    const vec_class<S, L, A, U, R> &operator*(const vec_class<S, L, A, U, R> &a, const identity_mat33&) {
        return a;
    }

    template<i32 S, i32 L, typename A, typename U, typename R>
    const vec_class<S, L, A, U, R> &operator/(const vec_class<S, L, A, U, R> &a, const identity_mat33&) {
        return a;
    }

    template<typename U1, typename R1, i32 d, typename P>
    dir3 operator*(const vec_class<3, 0, void, U1, R1> &a, const mat_class<d, d, mat_kind_vec<mat_kind_diag>, P> &b) {
        return dir3(float4(a.baseval()) * float4(b.vec().val34()));
    }

    template<typename U1, typename R1, typename P>
    position3 operator*(const vec_class<3, 1, void, U1, R1> &a, const mat_class<3, 3, mat_kind_vec<mat_kind_diag>, P> &b) {
        return position3(float4(a.val34()) * float4(b.vec().val34()));
    }

    template<typename U1, typename R1, typename P>
    vector4 operator*(const vec_class<3, 1, void, U1, R1> &a, const mat_class<4, 4, mat_kind_vec<mat_kind_diag>, P> &b) {
        return vector4(float4(a.val4()) * float4(b.vec().val34()));
    }

    template<typename U1, typename R1, i32 d, typename P>
    vector4 operator*(const vec_class<4, -1, void, U1, R1> &a, const mat_class<d, d, mat_kind_vec<mat_kind_diag>, P> &b) {
        return vector4(float4(a.baseval()) * float4(b.vec().val4()));
    }

    template<typename R1, typename P>
    const vec_class<4, -1, vec_axis<3>, void, R1> &operator*(const vec_class<4, -1, vec_axis<3>, void, R1> &a,
                                                             const mat_class<3, 3, mat_kind_vec<mat_kind_diag>, P>&) {

        return a;
    }

    template<i32 S1, i32 L1, i32 A1, typename R1, i32 d, typename P>
    vec_class<S1, L1, vec_axis<A1>> operator*(const vec_class<S1, L1, vec_axis<A1>, void, R1> &a,
                                              const mat_class<d, d, mat_kind_vec<mat_kind_diag>, P> &b) {

        return vec_class<S1, L1, vec_axis<A1>>(a.val1() * get<A1>(b.vec()));
    }

    template<i32 S1, i32 L1, i32 A1, typename R1, i32 d, typename P>
    vec_class<S1, L1, vec_axis<A1>, void, rep_type<typename get_set<A1, typename mat_class<d, d, mat_kind_vec<mat_kind_diag>, P>::vec_type>::type>>
    operator*(const vec_class<S1, L1, vec_axis<A1>, vec_unit<1>, R1>&, const mat_class<d, d, mat_kind_vec<mat_kind_diag>, P> &b) {

        return vec_class<S1, L1, vec_axis<A1>, void, rep_type<typename get_set<A1, typename mat_class<d, d, mat_kind_vec<mat_kind_diag>, P>::vec_type>::type>>(get<A1>(b.vec()));
    }

    template<i32 S1, i32 L1, i32 A1, typename R1, i32 d, typename P>
    vec_class<S1, L1, vec_axis<A1>, void, rep_type<typename get_set<A1, typename mat_class<d, d, mat_kind_vec<mat_kind_diag>, P>::vec_type>::type>>
    operator*(const vec_class<S1, L1, vec_axis<A1>, vec_unit<-1>, R1>&, const mat_class<d, d, mat_kind_vec<mat_kind_diag>, P> &b) {

        return vec_class<S1, L1, vec_axis<A1>, void, rep_type<typename get_set<A1, typename mat_class<d, d, mat_kind_vec<mat_kind_diag>, P>::vec_type>::type>>(-get<A1>(b.vec()));
    }

    template<i32 S1, i32 L1, i32 d, typename P>
    vec_class<S1, L1, vec_axis_zero> operator*(const vec_class<S1, L1, vec_axis_zero>&,
                                               const mat_class<d, d, mat_kind_vec<mat_kind_diag>, P>&) {

        return vec_class<S1, L1, vec_axis_zero>();
    }

    template<typename P>
    typename mat_class<4, 4, mat_kind_vec<mat_kind_diag>, P>::get_w_type operator*(const position3_zero&,
                                                                                   const mat_class<4, 4, mat_kind_vec<mat_kind_diag>, P> &b) {

        return b.get_w();
    }

    template<i32 R, i32 C, typename T, typename P>
    identity_mat33 multiply_identity(const mat_class<R, C, T, P>&) {
        return identity_mat33();
    }
}} // treyarch::math
