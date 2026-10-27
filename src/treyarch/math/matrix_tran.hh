#pragma once

#include "treyarch/math/matrix_diag.hh"

namespace treyarch { namespace math {
    class mat_kind_vec_tran;

    typedef mat_class<4, 3, mat_kind_vec<mat_kind_vec_tran>> tran_mat43;
    typedef mat_class<4, 4, mat_kind_vec<mat_kind_vec_tran>> tran_mat44;

    typedef const tran_mat43 tran_mat43_arg;
    typedef const tran_mat44 tran_mat44_arg;

    template<i32 c, bool P>
    class mat_class<4, c, mat_kind_vec<mat_kind_vec_tran>, rep_std<P>> {

    public:
        typedef vec_class<c, (c == 3 ? 1 : -1), void, void, rep_std<P>> vec_type;

    protected:
        vec_type v;

    public:
        mat_class() {}

        template<i32 S1, i32 L1, typename A1, typename U1, typename R1>
        explicit mat_class(const vec_class<S1, L1, A1, U1, R1> &_v) : v(vec_type(_v)) {}

        template<typename TX, typename TY, typename TZ>
        mat_class(TX x, TY y, TZ z) : v(x, y, z) {}

        template<typename TX, typename TY, typename TZ, typename TW>
        mat_class(TX x, TY y, TZ z, TW w) : v(x, y, z, w) {}

        template<i32 c2, typename P1>
        mat_class(const mat_class<4, c2, mat_kind_vec<mat_kind_vec_tran>, P1> &a) { v = a.vec(); }

        const vec_type &vec() const { return v; }

        typedef unit_dir_x get_x_type;
        typedef unit_dir_y get_y_type;
        typedef unit_dir_z get_z_type;
        typedef vec_type   get_w_type;

              get_x_type  get_x() const { return get_x_type(); }
              get_y_type  get_y() const { return get_y_type(); }
              get_z_type  get_z() const { return get_z_type(); }
        const get_w_type &get_w() const { return v; }

        get_w_type &get_w() { return v; }

        void get_xyz(get_x_type&, get_y_type&, get_z_type&) const {}
        void get_xyzw(get_x_type&, get_y_type&, get_z_type&, get_w_type &_w) const { _w = get_w(); }

        typedef const mat_class<4, c, mat_kind_vec<mat_kind_vec_tran>, rep_constant>  constant;
        typedef       mat_class<4, c, mat_kind_vec<mat_kind_vec_tran>, rep_std<true>> packed;

        template<i32 c2, typename P1>
        mat_class &operator=(const mat_class<4, c2, mat_kind_vec<mat_kind_vec_tran>, P1> &a) { v = a.vec(); return *this; }

        template<typename T> mat_class &operator+=(const T &a) { return *this = *this + a; }
        template<typename T> mat_class &operator-=(const T &a) { return *this = *this - a; }
        template<typename T> mat_class &operator*=(const T &a) { return *this = *this * a; }
        template<typename T> mat_class &operator/=(const T &a) { return *this = *this / a; }
    };

    template<i32 c>
    class mat_class<4, c, mat_kind_vec<mat_kind_vec_tran>, rep_constant> {

    public:
        f32 x_, y_, z_, w_;

        typedef vec_class<c, (c == 3 ? 1 : -1)> vec_type;

        const vec_type &vec() const { return reinterpret_cast<const vec_type&>(*this); }

        typedef unit_dir_x get_x_type;
        typedef unit_dir_y get_y_type;
        typedef unit_dir_z get_z_type;
        typedef vec_type   get_w_type;

        operator const mat_class<4, c, mat_kind_vec<mat_kind_vec_tran>>&() const {
            return reinterpret_cast<const mat_class<4, c, mat_kind_vec<mat_kind_vec_tran>>&>(*this);
        }

              get_x_type  get_x() const { return get_x_type(); }
              get_y_type  get_y() const { return get_y_type(); }
              get_z_type  get_z() const { return get_z_type(); }
        const get_w_type &get_w() const { return vec(); }

        void get_xyz(get_x_type&, get_y_type&, get_z_type&) const {}
        void get_xyzw(get_x_type&, get_y_type&, get_z_type&, get_w_type &_w) const { _w = get_w(); }
    };

    template<typename P>
    class result_type<op_get_m33, mat_class<4, 3, mat_kind_vec<mat_kind_vec_tran>, P>> {

    public:
        typedef identity_mat33 type;
    };

    template<typename P>
    identity_mat33 get_m33(const mat_class<4, 3, mat_kind_vec<mat_kind_vec_tran>, P>&) {
        return identity_mat33();
    }

    template<typename P>
    class result_type<op_get_m33, mat_class<4, 4, mat_kind_vec<mat_kind_vec_tran>, P>> {

    public:
        typedef identity_mat33 type;
    };

    template<typename P>
    identity_mat33 get_m33(const mat_class<4, 4, mat_kind_vec<mat_kind_vec_tran>, P>&) {
        return identity_mat33();
    }

    template<i32 c, typename P>
    mat_class<4, c, mat_kind_vec<mat_kind_vec_tran>> inv(const mat_class<4, c, mat_kind_vec<mat_kind_vec_tran>, P> &a) {
        return mat_class<4, c, mat_kind_vec<mat_kind_vec_tran>>(-a.vec());
    }

    template<typename P1, i32 c2, typename P2>
    mat_class<4, c2, mat_kind_vec<mat_kind_vec_tran>> operator*(const mat_class<4, 3,  mat_kind_vec<mat_kind_vec_tran>, P1> &a,
                                                                const mat_class<4, c2, mat_kind_vec<mat_kind_vec_tran>, P2> &b) {

        return mat_class<4, c2, mat_kind_vec<mat_kind_vec_tran>>(a.vec() + b.vec());
    }

    template<typename P1, i32 c2, typename P2>
    mat_class<4, c2, mat_kind_vec<mat_kind_vec_tran>> operator*(const mat_class<4, 4,  mat_kind_vec<mat_kind_vec_tran>, P1> &a,
                                                                const mat_class<4, c2, mat_kind_vec<mat_kind_vec_tran>, P2> &b) {

        return mat_class<4, c2, mat_kind_vec<mat_kind_vec_tran>>(dir3(a.vec()) + a.vec().get_w() * b.vec());
    }

    template<typename P1, i32 c2, typename P2>
    mat_class<4, c2, mat_kind_vec<mat_kind_vec_tran>> operator/(const mat_class<4, 3,  mat_kind_vec<mat_kind_vec_tran>, P1> &a,
                                                                const mat_class<4, c2, mat_kind_vec<mat_kind_vec_tran>, P2> &b) {

        return mat_class<4, c2, mat_kind_vec<mat_kind_vec_tran>>(a.vec() - b.vec());
    }

    template<typename P1, i32 c2, typename P2>
    mat_class<4, c2, mat_kind_vec<mat_kind_vec_tran>> operator/(const mat_class<4, 4,  mat_kind_vec<mat_kind_vec_tran>, P1> &a,
                                                                const mat_class<4, c2, mat_kind_vec<mat_kind_vec_tran>, P2> &b) {

        return mat_class<4, c2, mat_kind_vec<mat_kind_vec_tran>>(dir3(a.vec()) - a.vec().get_w() * b.vec());
    }

    template<typename T>
    class mat_result_kind<op_mul, T, mat_kind_vec<mat_kind_vec_tran>> {

    public:
        typedef mat_kind_tran<T, false> type;
    };

    template<typename T>
    class mat_result_kind<op_mul, mat_kind_vec<mat_kind_vec_tran>, T> {

    public:
        typedef mat_kind_tran<T, false> type;
    };

    template<typename T, bool fast_mul>
    class mat_result_kind<op_mul, mat_kind_tran<T, fast_mul>, mat_kind_vec<mat_kind_vec_tran>> {

    public:
        typedef mat_kind_tran<T, fast_mul> type;
    };

    template<typename T, bool fast_mul>
    class mat_result_kind<op_mul, mat_kind_vec<mat_kind_vec_tran>, mat_kind_tran<T, fast_mul>> {

    public:
        typedef mat_kind_tran<T, fast_mul> type;
    };

    template<>
    class mat_result_kind<op_mul, mat_kind_vec<mat_kind_vec_tran>, mat_kind_vec<mat_kind_vec_tran>> {

    public:
        typedef mat_kind_vec<mat_kind_vec_tran> type;
    };

    template<typename U1, typename R1, i32 c, typename P>
    const vec_class<3, 0, void, U1, R1> &operator*(const vec_class<3, 0, void, U1, R1> &a,
                                                   const mat_class<4, c, mat_kind_vec<mat_kind_vec_tran>, P>&) {
        return a;
    }

    template<typename R1, i32 c, typename P>
    typename mat_class<4, c, mat_kind_vec<mat_kind_vec_tran>>::get_w_type operator*(const vec_class<3, 1, void, void, R1>                     &a,
                                                                                    const mat_class<4, c, mat_kind_vec<mat_kind_vec_tran>, P> &b) {

        return dir3(a) + b.vec();
    }

    template<typename R1, i32 c, typename P>
    typename mat_class<4, c, mat_kind_vec<mat_kind_vec_tran>>::get_w_type operator*(const vec_class<4, -1, void, void, R1>                    &a,
                                                                                    const mat_class<4, c, mat_kind_vec<mat_kind_vec_tran>, P> &b) {

        return dir3(a) + a.get_w() * b.vec();
    }
}} // treyarch::math
