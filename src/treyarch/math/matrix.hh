#pragma once

#include "treyarch/math/vector.hh"

namespace treyarch { namespace math {
    class mat_kind_full;

    template<typename Op, typename T1, typename T2 = void> class rep_lazy;

    template<i32 rows, i32 cols, typename kind = mat_kind_full, typename rep = rep_std<>> class mat_class;

    template<typename Op, typename T1, typename T2 = void, typename T3 = void>
    class mat_result_kind {

    public:
        typedef mat_kind_full type;
    };

    template<typename T, bool fast_mul = true> class mat_kind_tran;

    template<typename M>
    auto get_m33_of(const M &a) -> decltype(get_m33(a)) {
        return get_m33(a);
    }

    template<typename T, bool fast_mul, bool P>
    class mat_class<4, 3, mat_kind_tran<T, fast_mul>, rep_std<P>> {

    public:
        typedef mat_class<3, 3, T, rep_std<P>>          get_m33_type;
        typedef typename get_m33_type::get_x_type       get_x_type;
        typedef typename get_m33_type::get_y_type       get_y_type;
        typedef typename get_m33_type::get_z_type       get_z_type;
        typedef vec_class<3, 1, void, void, rep_std<P>> get_w_type;

    protected:
        get_m33_type m;
        get_w_type   t;

    public:
        mat_class() {}

        const get_m33_type &get_m33() const { return m; }
              get_m33_type &get_m33()       { return m; }

        template<typename V> void set_m33(const V &v) { m = v; }

        template<typename T1, typename P1>
        mat_class(const mat_class<3, 3, T1, P1> &_m) : m(_m), t(position3_zero()) {}

        template<typename T1, typename P1, typename A2, typename U2, typename R2>
        mat_class(const mat_class<3, 3, T1, P1> &_m, const vec_class<3, 1, A2, U2, R2> &_t) : m(_m), t(_t) {}

        template<typename T2, typename P2>
        mat_class(const mat_class<4, 3, T2, P2> &a) : m(get_m33_of(a)), t(a.get_w()) {}

        template<typename T1, typename T2, typename T3, typename T4>
        mat_class(const T1 &a, const T2 &b, const T3 &c, const T4 &d) : m(a, b, c), t(d) {}

              get_x_type  get_x() const { return m.get_x(); }
              get_y_type  get_y() const { return m.get_y(); }
              get_z_type  get_z() const { return m.get_z(); }
        const get_w_type &get_w() const { return t; }

        get_x_type &get_x() { return m.get_x(); }
        get_y_type &get_y() { return m.get_y(); }
        get_z_type &get_z() { return m.get_z(); }
        get_w_type &get_w() { return t; }

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

        template<typename V> void set_x(const V &v) { m.set_x(v); }
        template<typename V> void set_y(const V &v) { m.set_y(v); }
        template<typename V> void set_z(const V &v) { m.set_z(v); }
        template<typename V> void set_w(const V &v) { t = v; }

        typedef const mat_class<4, 3, mat_kind_tran<T, fast_mul>, rep_constant> constant;
        typedef mat_class<4, 3, mat_kind_tran<T, fast_mul>, rep_std<true>>       packed;

        template<typename T2, typename P2>
        mat_class &operator=(const mat_class<4, 3, T2, P2> &a) {
            m = get_m33_of(a);
            t = a.get_w();

            return *this;
        }

        template<typename T2, typename P2>
        mat_class &operator=(const mat_class<3, 3, T2, P2> &a) {
            m = a;
            t = position3_zero();

            return *this;
        }

        template<typename T2> mat_class &operator+=(const T2 &a) { return *this = *this + a; }
        template<typename T2> mat_class &operator-=(const T2 &a) { return *this = *this - a; }
        template<typename T2> mat_class &operator*=(const T2 &a) { return *this = *this * a; }
        template<typename T2> mat_class &operator/=(const T2 &a) { return *this = *this / a; }
    };

    template<typename T, bool fast_mul>
    class mat_class<4, 3, mat_kind_tran<T, fast_mul>, rep_constant> {

    public:
        typedef mat_class<3, 3, T, rep_constant>         get_m33_type;
        typedef typename get_m33_type::get_x_type        get_x_type;
        typedef typename get_m33_type::get_y_type        get_y_type;
        typedef typename get_m33_type::get_z_type        get_z_type;
        typedef vec_class<3, 1, void, void, rep_constant> get_w_type;

        get_m33_type m_;
        get_w_type   t_;

              get_x_type  get_x() const { return m_.get_x(); }
              get_y_type  get_y() const { return m_.get_y(); }
              get_z_type  get_z() const { return m_.get_z(); }
        const get_w_type &get_w() const { return t_; }

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

        const get_m33_type &get_m33() const { return m_; }
    };

    template<typename T, bool fast_mul, typename P>
    mat_class<4, 3, mat_kind_tran<T, fast_mul>> inv(const mat_class<4, 3, mat_kind_tran<T, fast_mul>, P> &a) {
        typedef mat_class<4, 3, mat_kind_tran<T, fast_mul>> inverse_type;

        inverse_type result;
        result.set_m33(inv(get_m33(a)));
        result.set_w(-a.get_w() * get_m33(result));

        return result;
    }

    template<typename T, typename P1, typename P2>
    mat_class<4, 3, mat_kind_tran<T>> operator*(const mat_class<4, 3, mat_kind_tran<T>, P1> &a,
                                                const mat_class<4, 3, mat_kind_tran<T>, P2> &b) {

        return mat_class<4, 3, mat_kind_tran<T>>(get_m33(a) * get_m33(b), a.get_w() * get_m33(b) + b.get_w());
    }

    template<typename T, typename P1, typename P2>
    mat_class<4, 3, mat_kind_tran<T>> operator*(const mat_class<3, 3, T, P1>                &a,
                                                const mat_class<4, 3, mat_kind_tran<T>, P2> &b) {

        return mat_class<4, 3, mat_kind_tran<T>>(a * get_m33(b), b.get_w());
    }

    template<typename T, typename P1, typename P2>
    mat_class<4, 3, mat_kind_tran<T>> operator*(const mat_class<4, 3, mat_kind_tran<T>, P1> &a,
                                                const mat_class<3, 3, T, P2>                &b) {

        return mat_class<4, 3, mat_kind_tran<T>>(get_m33(a) * b, a.get_w() * b);
    }

    template<typename T, typename P1, typename P2>
    mat_class<4, 3, mat_kind_tran<T>> operator/(const mat_class<4, 3, mat_kind_tran<T>, P1> &a,
                                                const mat_class<4, 3, mat_kind_tran<T>, P2> &b) {

        mat_class<3, 3, T> invb33(inv(get_m33(b)));

        return mat_class<4, 3, mat_kind_tran<T>>(get_m33(a) * invb33, position3((a.get_w() - b.get_w()) * invb33));
    }

    template<typename T, typename P1, typename P2>
    mat_class<4, 3, mat_kind_tran<T>> operator/(const mat_class<3, 3, T, P1>                &a,
                                                const mat_class<4, 3, mat_kind_tran<T>, P2> &b) {

        mat_class<3, 3, T> invb33(inv(get_m33(b)));

        return mat_class<4, 3, mat_kind_tran<T>>(a * invb33, -b.get_w() * invb33);
    }

    template<typename T, typename P1, typename P2>
    mat_class<4, 3, mat_kind_tran<T>> operator/(const mat_class<4, 3, mat_kind_tran<T>, P1> &a,
                                                const mat_class<3, 3, T, P2>                &b) {

        mat_class<3, 3, T> invb(inv(b));

        return mat_class<4, 3, mat_kind_tran<T>>(get_m33(a) * invb, a.get_w() * invb);
    }

    typedef mat_class<3, 3, mat_kind_full>                       mat33;
    typedef mat_class<3, 4, mat_kind_full>                       mat34;
    typedef mat_class<4, 3, mat_kind_tran<mat_kind_full, false>> mat43;
    typedef mat_class<4, 4, mat_kind_full>                       mat44;

    typedef const mat33 &mat33_arg;
    typedef const mat34 &mat34_arg;
    typedef const mat43 &mat43_arg;
    typedef const mat44 &mat44_arg;

    template<i32 c, bool P>
    class mat_class<3, c, mat_kind_full, rep_std<P>> {

    protected:
        typedef vec_class<c, (c == 3 ? 0 : -1), void, void, rep_std<P>>    row_type;
        typedef typename if_type<c == 3, position3_zero, unit_dir_w>::type last_row_type;

        row_type v[3];

    public:
        mat_class() {}

        template<i32 R1, i32 C1, typename T1, typename P1>
        mat_class(const mat_class<R1, C1, T1, P1> &a) {
            typename mat_class<R1, C1, T1, P1>::get_x_type x1;
            typename mat_class<R1, C1, T1, P1>::get_y_type y1;
            typename mat_class<R1, C1, T1, P1>::get_z_type z1;

            a.get_xyz(x1, y1, z1);

            v[0] = x1;
            v[1] = y1;
            v[2] = z1;
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

        typedef const mat_class<3, c, mat_kind_full, rep_constant> constant;
        typedef mat_class<3, c, mat_kind_full, rep_std<true>>       packed;

        template<i32 R1, i32 C1, typename T1, typename P1>
        mat_class &operator=(const mat_class<R1, C1, T1, P1> &a) {
            typename mat_class<R1, C1, T1, P1>::get_x_type x1;
            typename mat_class<R1, C1, T1, P1>::get_y_type y1;
            typename mat_class<R1, C1, T1, P1>::get_z_type z1;

            a.get_xyz(x1, y1, z1);

            v[0] = x1;
            v[1] = y1;
            v[2] = z1;

            return *this;
        }

        template<typename T> mat_class &operator+=(const T &a) { return *this = *this + a; }
        template<typename T> mat_class &operator-=(const T &a) { return *this = *this - a; }
        template<typename T> mat_class &operator*=(const T &a) { return *this = *this * a; }
        template<typename T> mat_class &operator/=(const T &a) { return *this = *this / a; }
    };

    template<i32 c>
    class mat_class<3, c, mat_kind_full, rep_constant> {

    public:
        typedef typename if_type<c == 3, dir3, vector4>::type              row_type;
        typedef typename if_type<c == 3, position3_zero, unit_dir_w>::type last_row_type;

        struct { f32 x, y, z, w; } r0_;
        struct { f32 x, y, z, w; } r1_;
        struct { f32 x, y, z, w; } r2_;

        typedef row_type      get_x_type;
        typedef row_type      get_y_type;
        typedef row_type      get_z_type;
        typedef last_row_type get_w_type;

        operator const mat_class<3, c, mat_kind_full>&() const { return reinterpret_cast<const mat_class<3, c, mat_kind_full>&>(*this); }

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

    template<i32 c, bool P>
    class mat_class<4, c, mat_kind_full, rep_std<P>> {

    protected:
        typedef vec_class<c, (c == 3 ? 0 : -1), void, void, rep_std<P>> row_type;
        typedef vec_class<c, (c == 3 ? 1 : -1), void, void, rep_std<P>> last_row_type;

        row_type v[4];

    public:
        mat_class() {}

        // sub_5FC100
        // sub_5FC080
        // sub_7ACFF0
        template<i32 R1, i32 C1, typename T1, typename P1>
        mat_class(const mat_class<R1, C1, T1, P1> &a) {
            typename mat_class<R1, C1, T1, P1>::get_x_type x1;
            typename mat_class<R1, C1, T1, P1>::get_y_type y1;
            typename mat_class<R1, C1, T1, P1>::get_z_type z1;
            typename mat_class<R1, C1, T1, P1>::get_w_type w1;

            a.get_xyzw(x1, y1, z1, w1);

            v[0] = x1;
            v[1] = y1;
            v[2] = z1;

            reinterpret_cast<last_row_type&>(v[3]) = w1;
        }

        // sub_401BA0
        mat_class(const row_type &v0, const row_type &v1, const row_type &v2, const last_row_type &v3) {
            v[0] = v0;
            v[1] = v1;
            v[2] = v2;
            v[3] = reinterpret_cast<const row_type&>(v3);
        }

        typedef row_type      get_x_type;
        typedef row_type      get_y_type;
        typedef row_type      get_z_type;
        typedef last_row_type get_w_type;

        const get_x_type &get_x() const { return v[0]; }
        const get_y_type &get_y() const { return v[1]; }
        const get_z_type &get_z() const { return v[2]; }
        const get_w_type &get_w() const { return reinterpret_cast<const last_row_type&>(v[3]); }

        get_x_type &get_x() { return v[0]; }
        get_y_type &get_y() { return v[1]; }
        get_z_type &get_z() { return v[2]; }
        get_w_type &get_w() { return reinterpret_cast<last_row_type&>(v[3]); }

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
        template<typename V> void set_w(const V &r) { reinterpret_cast<last_row_type&>(v[3]) = r; }

        template<typename T> const row_type &operator[](T i) const { return v[i]; }
        template<typename T>       row_type &operator[](T i)       { return v[i]; }

        typedef const mat_class<4, c, mat_kind_full, rep_constant> constant;
        typedef mat_class<4, c, mat_kind_full, rep_std<true>>      packed;

        template<i32 R1, i32 C1, typename T1, typename P1>
        mat_class &operator=(const mat_class<R1, C1, T1, P1> &a) {
            typename mat_class<R1, C1, T1, P1>::get_x_type x1;
            typename mat_class<R1, C1, T1, P1>::get_y_type y1;
            typename mat_class<R1, C1, T1, P1>::get_z_type z1;
            typename mat_class<R1, C1, T1, P1>::get_w_type w1;

            a.get_xyzw(x1, y1, z1, w1);

            v[0] = x1;
            v[1] = y1;
            v[2] = z1;

            reinterpret_cast<last_row_type&>(v[3]) = w1;

            return *this;
        }

        template<typename T> mat_class &operator+=(const T &a) { return *this = *this + a; }
        template<typename T> mat_class &operator-=(const T &a) { return *this = *this - a; }
        template<typename T> mat_class &operator*=(const T &a) { return *this = *this * a; }
        template<typename T> mat_class &operator/=(const T &a) { return *this = *this / a; }
    };

    template<i32 c>
    class mat_class<4, c, mat_kind_full, rep_constant> {

    public:
        typedef typename if_type<c == 3, dir3, vector4>::type      row_type;
        typedef typename if_type<c == 3, position3, vector4>::type last_row_type;

        struct { f32 x, y, z, w; } r0_;
        struct { f32 x, y, z, w; } r1_;
        struct { f32 x, y, z, w; } r2_;
        struct { f32 x, y, z, w; } r3_;

        typedef row_type      get_x_type;
        typedef row_type      get_y_type;
        typedef row_type      get_z_type;
        typedef last_row_type get_w_type;

        operator const mat_class<4, c, mat_kind_full>&() const { return reinterpret_cast<const mat_class<4, c, mat_kind_full>&>(*this); }

        const get_x_type &get_x() const { return reinterpret_cast<const row_type&>(r0_); }
        const get_y_type &get_y() const { return reinterpret_cast<const row_type&>(r1_); }
        const get_z_type &get_z() const { return reinterpret_cast<const row_type&>(r2_); }
        const get_w_type &get_w() const { return reinterpret_cast<const last_row_type&>(r3_); }

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

    template<i32 r, i32 c, typename T, typename T1, typename T2>
    class mat_class<r, c, T, rep_lazy<op_add, T1, T2>> {

    protected:
        const T1 &a;
        const T2 &b;

    public:
        mat_class(const T1 &_a, const T2 &_b) : a(_a), b(_b) {}

        typedef typename mat_class<r, c, T>::get_x_type get_x_type;
        typedef typename mat_class<r, c, T>::get_y_type get_y_type;
        typedef typename mat_class<r, c, T>::get_z_type get_z_type;
        typedef typename mat_class<r, c, T>::get_w_type get_w_type;

        get_x_type get_x() const { return a.get_x() + b.get_x(); }
        get_y_type get_y() const { return a.get_y() + b.get_y(); }
        get_z_type get_z() const { return a.get_z() + b.get_z(); }
        get_w_type get_w() const { return a.get_w() + b.get_w(); }

        void get_xyz(get_x_type &_x, get_y_type &_y, get_z_type &_z) const {
            typename T1::get_x_type x1; typename T2::get_x_type x2;
            typename T1::get_y_type y1; typename T2::get_y_type y2;
            typename T1::get_z_type z1; typename T2::get_z_type z2;

            a.get_xyz(x1, y1, z1);
            b.get_xyz(x2, y2, z2);

            _x = get_x_type(x1 + x2);
            _y = get_y_type(y1 + y2);
            _z = get_z_type(z1 + z2);
        }

        void get_xyzw(get_x_type &_x, get_y_type &_y, get_z_type &_z, get_w_type &_w) const {
            typename T1::get_x_type x1; typename T2::get_x_type x2;
            typename T1::get_y_type y1; typename T2::get_y_type y2;
            typename T1::get_z_type z1; typename T2::get_z_type z2;
            typename T1::get_w_type w1; typename T2::get_w_type w2;

            a.get_xyzw(x1, y1, z1, w1);
            b.get_xyzw(x2, y2, z2, w2);

            _x = get_x_type(x1 + x2);
            _y = get_y_type(y1 + y2);
            _z = get_z_type(z1 + z2);
            _w = get_w_type(w1 + w2);
        }
    };

    template<i32 r, i32 c, typename T, typename T1, typename T2>
    class mat_class<r, c, T, rep_lazy<op_sub, T1, T2>> {

    protected:
        const T1 &a;
        const T2 &b;

    public:
        mat_class(const T1 &_a, const T2 &_b) : a(_a), b(_b) {}

        typedef typename mat_class<r, c, T>::get_x_type get_x_type;
        typedef typename mat_class<r, c, T>::get_y_type get_y_type;
        typedef typename mat_class<r, c, T>::get_z_type get_z_type;
        typedef typename mat_class<r, c, T>::get_w_type get_w_type;

        get_x_type get_x() const { return a.get_x() - b.get_x(); }
        get_y_type get_y() const { return a.get_y() - b.get_y(); }
        get_z_type get_z() const { return a.get_z() - b.get_z(); }
        get_w_type get_w() const { return a.get_w() - b.get_w(); }

        void get_xyz(get_x_type &_x, get_y_type &_y, get_z_type &_z) const {
            typename T1::get_x_type x1; typename T2::get_x_type x2;
            typename T1::get_y_type y1; typename T2::get_y_type y2;
            typename T1::get_z_type z1; typename T2::get_z_type z2;

            a.get_xyz(x1, y1, z1);
            b.get_xyz(x2, y2, z2);

            _x = get_x_type(x1 - x2);
            _y = get_y_type(y1 - y2);
            _z = get_z_type(z1 - z2);
        }

        void get_xyzw(get_x_type &_x, get_y_type &_y, get_z_type &_z, get_w_type &_w) const {
            typename T1::get_x_type x1; typename T2::get_x_type x2;
            typename T1::get_y_type y1; typename T2::get_y_type y2;
            typename T1::get_z_type z1; typename T2::get_z_type z2;
            typename T1::get_w_type w1; typename T2::get_w_type w2;

            a.get_xyzw(x1, y1, z1, w1);
            b.get_xyzw(x2, y2, z2, w2);

            _x = get_x_type(x1 - x2);
            _y = get_y_type(y1 - y2);
            _z = get_z_type(z1 - z2);
            _w = get_w_type(w1 - w2);
        }
    };

    template<i32 r, i32 c, typename T, typename T1, typename T2>
    class mat_class<r, c, T, rep_lazy<op_mul, T1, T2>> {

    public:
        const T1 &a;
        const T2 &b;

        mat_class(const T1 &_a, const T2 &_b) : a(_a), b(_b) {}

        typedef typename mat_class<r, c, T>::get_x_type get_x_type;
        typedef typename mat_class<r, c, T>::get_y_type get_y_type;
        typedef typename mat_class<r, c, T>::get_z_type get_z_type;
        typedef typename mat_class<r, c, T>::get_w_type get_w_type;

        get_x_type get_x() const { return a.get_x() * b; }
        get_y_type get_y() const { return a.get_y() * b; }
        get_z_type get_z() const { return a.get_z() * b; }
        get_w_type get_w() const { return a.get_w() * b; }

        void get_xyz(get_x_type &_x, get_y_type &_y, get_z_type &_z) const {
            typename T1::get_x_type x1;
            typename T1::get_y_type y1;
            typename T1::get_z_type z1;

            a.get_xyz(x1, y1, z1);

            _x = get_x_type(x1 * b);
            _y = get_y_type(y1 * b);
            _z = get_z_type(z1 * b);
        }

        void get_xyzw(get_x_type &_x, get_y_type &_y, get_z_type &_z, get_w_type &_w) const {
            typename T1::get_x_type x1;
            typename T1::get_y_type y1;
            typename T1::get_z_type z1;
            typename T1::get_w_type w1;

            a.get_xyzw(x1, y1, z1, w1);

            _x = get_x_type(x1 * b);
            _y = get_y_type(y1 * b);
            _z = get_z_type(z1 * b);
            _w = get_w_type(w1 * b);
        }
    };

    template<typename Op,
             i32 R1, i32 C1, typename T1, typename P1,
             i32 R2, i32 C2, typename T2, typename P2>
    class result_type<Op, mat_class<R1, C1, T1, P1>, mat_class<R2, C2, T2, P2>> {

        enum { rows = (R1 > R2) ? R1 : R2,
               cols = (C1 > C2) ? C1 : C2 };

    public:
        typedef mat_class<rows, cols, typename mat_result_kind<Op, T1, T2>::type, rep_lazy<Op, mat_class<R1, C1, T1, P1>, mat_class<R2, C2, T2, P2>>> type;
    };

    template<i32 R1, i32 C1, typename T1, typename P1,
             i32 R2, i32 C2, typename T2, typename P2>
    typename result_type<op_add, mat_class<R1, C1, T1, P1>, mat_class<R2, C2, T2, P2>>::type
    operator+(const mat_class<R1, C1, T1, P1> &a, const mat_class<R2, C2, T2, P2> &b) {

        return typename result_type<op_add, mat_class<R1, C1, T1, P1>, mat_class<R2, C2, T2, P2>>::type(a, b);
    }

    template<i32 R1, i32 C1, typename T1, typename P1,
             i32 R2, i32 C2, typename T2, typename P2>
    typename result_type<op_sub, mat_class<R1, C1, T1, P1>, mat_class<R2, C2, T2, P2>>::type
    operator-(const mat_class<R1, C1, T1, P1> &a, const mat_class<R2, C2, T2, P2> &b) {

        return typename result_type<op_sub, mat_class<R1, C1, T1, P1>, mat_class<R2, C2, T2, P2>>::type(a, b);
    }

    template<i32 R1, i32 C1, typename T1, typename P1,
             i32 R2, i32 C2, typename T2, typename P2>
    typename result_type<op_mul, mat_class<R1, C1, T1, P1>, mat_class<R2, C2, T2, P2>>::type
    operator*(const mat_class<R1, C1, T1, P1> &a, const mat_class<R2, C2, T2, P2> &b) {

        return typename result_type<op_mul, mat_class<R1, C1, T1, P1>, mat_class<R2, C2, T2, P2>>::type(a, b);
    }

    template<i32 R1, i32 C1, typename T1, typename P1,
             i32 R2, i32 C2, typename T2, typename P2_o, typename P2_a, typename P2_b>
    mat_class<((R1 > R2) ? R1 : R2), ((C1 > C2) ? C1 : C2), typename mat_result_kind<op_mul, T1, T2>::type>
    operator*(const mat_class<R1, C1, T1, P1> &a, const mat_class<R2, C2, T2, rep_lazy<P2_o, P2_a, P2_b>> &b) {

        const mat_class<R2, C2, T2> concrete_b(b);

        return typename result_type<op_mul, mat_class<R1, C1, T1, P1>, mat_class<R2, C2, T2>>::type(a, concrete_b);
    }

    template<typename subkind> class mat_kind_vec;

    template<typename Op,
             i32 R1, i32 C1, typename P1,
             i32 R2, i32 C2, typename P2,
             typename subkind>
    class result_type<Op, mat_class<R1, C1, mat_kind_vec<subkind>, P1>, mat_class<R2, C2, mat_kind_vec<subkind>, P2>> {

        enum { rows = (R1 > R2) ? R1 : R2,
               cols = (C1 > C2) ? C1 : C2 };

    public:
        typedef mat_class<rows, cols, mat_kind_vec<subkind>> type;
    };

    template<i32 R, i32 C, typename T, typename P>
    mat_class<R, C, mat_kind_vec<T>> operator-(const mat_class<R, C, mat_kind_vec<T>, P> &a) {
        return mat_class<R, C, mat_kind_vec<T>>(-a.vec());
    }

    template<i32 R1, i32 C1, typename P1, i32 R2, i32 C2, typename P2, typename T>
    typename result_type<op_add, mat_class<R1, C1, mat_kind_vec<T>, P1>, mat_class<R2, C2, mat_kind_vec<T>, P2>>::type
    operator+(const mat_class<R1, C1, mat_kind_vec<T>, P1> &a, const mat_class<R2, C2, mat_kind_vec<T>, P2> &b) {

        return typename result_type<op_add, mat_class<R1, C1, mat_kind_vec<T>, P1>, mat_class<R2, C2, mat_kind_vec<T>, P2>>::type(a.vec() + b.vec());
    }

    template<i32 R1, i32 C1, typename P1, i32 R2, i32 C2, typename P2, typename T>
    typename result_type<op_sub, mat_class<R1, C1, mat_kind_vec<T>, P1>, mat_class<R2, C2, mat_kind_vec<T>, P2>>::type
    operator-(const mat_class<R1, C1, mat_kind_vec<T>, P1> &a, const mat_class<R2, C2, mat_kind_vec<T>, P2> &b) {

        return typename result_type<op_sub, mat_class<R1, C1, mat_kind_vec<T>, P1>, mat_class<R2, C2, mat_kind_vec<T>, P2>>::type(a.vec() - b.vec());
    }

    template<i32 R1, i32 C1, typename P1, i32 R2, i32 C2, typename P2, typename T>
    bool operator==(const mat_class<R1, C1, mat_kind_vec<T>, P1> &a, const mat_class<R2, C2, mat_kind_vec<T>, P2> &b) {
        return a.vec() == b.vec();
    }

    template<i32 R1, i32 C1, typename P1, i32 R2, i32 C2, typename P2, typename T>
    bool operator!=(const mat_class<R1, C1, mat_kind_vec<T>, P1> &a, const mat_class<R2, C2, mat_kind_vec<T>, P2> &b) {
        return a.vec() != b.vec();
    }

    template<i32 R, i32 C, typename T, typename P>
    mat_class<R, C, mat_kind_vec<T>> operator*(const mat_class<R, C, mat_kind_vec<T>, P> &a, f32 b) {
        return mat_class<R, C, mat_kind_vec<T>>(a.vec() * b);
    }

    template<i32 R, i32 C, typename T, typename P>
    mat_class<R, C, mat_kind_vec<T>> operator/(const mat_class<R, C, mat_kind_vec<T>, P> &a, f32 b) {
        return mat_class<R, C, mat_kind_vec<T>>(a.vec() / b);
    }

    template<i32 n, i32 R, i32 C, typename T, typename P>
    typename get_set<n, mat_class<R, C, T, P>>::type get(const mat_class<R, C, T, P> &a) {
        return get_set<n, mat_class<R, C, T, P>>::get(a);
    }

    template<i32 n, i32 R, i32 C, typename T, typename P, typename V>
    void set(mat_class<R, C, T, P> &a, const V &v) {
        get_set<n, mat_class<R, C, T, P>>::set(a, v);
    }

    template<i32 C, typename T, typename P>
    mat_class<4, C> operator-(const mat_class<4, C, T, P> &a) {
        typename mat_class<4, C, T, P>::get_x_type x1;
        typename mat_class<4, C, T, P>::get_y_type y1;
        typename mat_class<4, C, T, P>::get_z_type z1;
        typename mat_class<4, C, T, P>::get_w_type w1;

        a.get_xyzw(x1, y1, z1, w1);

        return mat_class<4, C>(-x1, -y1, -z1, -w1);
    }

    template<i32 C, typename T, typename P>
    mat_class<3, C> operator-(const mat_class<3, C, T, P> &a) {
        typename mat_class<3, C, T, P>::get_x_type x1;
        typename mat_class<3, C, T, P>::get_y_type y1;
        typename mat_class<3, C, T, P>::get_z_type z1;

        a.get_xyz(x1, y1, z1);

        return mat_class<3, C>(-x1, -y1, -z1);
    }

    template<typename T, typename P>
    mat_class<3, 3> operator~(const mat_class<3, 3, T, P> &a) {
        typename mat_class<3, 3, T, P>::get_x_type x1;
        typename mat_class<3, 3, T, P>::get_y_type y1;
        typename mat_class<3, 3, T, P>::get_z_type z1;

        a.get_xyz(x1, y1, z1);

        float4 r0, r1, r2;
        transpose(r0, r1, r2, float4(x1.val34()), float4(y1.val34()), float4(z1.val34()));

        return mat_class<3, 3>(dir3(r0), dir3(r1), dir3(r2));
    }

    template<typename T, typename P>
    mat_class<4, 3> operator~(const mat_class<3, 4, T, P> &a) {
        typename mat_class<3, 4, T, P>::get_x_type x1;
        typename mat_class<3, 4, T, P>::get_y_type y1;
        typename mat_class<3, 4, T, P>::get_z_type z1;

        a.get_xyz(x1, y1, z1);

        float4 r0, r1, r2, r3;
        transpose(r0, r1, r2, r3, float4(x1.val4()), float4(y1.val4()), float4(z1.val4()));

        return mat_class<4, 3>(dir3(r0), dir3(r1), dir3(r2), position3(r3));
    }

    template<typename T, typename P>
    mat_class<3, 4> operator~(const mat_class<4, 3, T, P> &a) {
        typename mat_class<4, 3, T, P>::get_x_type x1;
        typename mat_class<4, 3, T, P>::get_y_type y1;
        typename mat_class<4, 3, T, P>::get_z_type z1;
        typename mat_class<4, 3, T, P>::get_w_type w1;

        a.get_xyzw(x1, y1, z1, w1);

        float4 r0, r1, r2;
        transpose(r0, r1, r2, float4(x1.val34()), float4(y1.val34()), float4(z1.val34()), float4(w1.val34()));

        return mat_class<3, 4>(vector4(r0), vector4(r1), vector4(r2));
    }

    // sub_5FC7B0
    template<typename T, typename P>
    mat_class<4, 4> operator~(const mat_class<4, 4, T, P> &a) {
        typename mat_class<4, 4, T, P>::get_x_type x1;
        typename mat_class<4, 4, T, P>::get_y_type y1;
        typename mat_class<4, 4, T, P>::get_z_type z1;
        typename mat_class<4, 4, T, P>::get_w_type w1;

        a.get_xyzw(x1, y1, z1, w1);

        float4 r0, r1, r2, r3;
        transpose(r0, r1, r2, r3, float4(x1.val4()), float4(y1.val4()), float4(z1.val4()), float4(w1.val4()));

        return mat_class<4, 4>(vector4(r0), vector4(r1), vector4(r2), vector4(r3));
    }

    template<typename T1>
    class mat_result_kind<op_inv, T1> {

    public:
        typedef T1 type;
    };

    template<i32 R1, i32 C1, typename T1, typename P1>
    class result_type<op_inv, mat_class<R1, C1, T1, P1>> {

    public:
        typedef mat_class<R1, C1, typename mat_result_kind<op_inv, T1>::type> type;
    };

    template<typename T, typename P>
    typename result_type<op_inv, mat_class<3, 3, T, P>>::type inv(const mat_class<3, 3, T, P> &a) {
        typename mat_class<3, 3, T, P>::get_x_type x1;
        typename mat_class<3, 3, T, P>::get_y_type y1;
        typename mat_class<3, 3, T, P>::get_z_type z1;

        a.get_xyz(x1, y1, z1);

        float4 d0, d1, d2;
        inv_mat(d0, d1, d2, float4(x1.val34()), float4(y1.val34()), float4(z1.val34()));

        return typename result_type<op_inv, mat_class<3, 3, T, P>>::type(dir3(d0), dir3(d1), dir3(d2));
    }

    // sub_9D2B70
    template<typename T, typename P>
    typename result_type<op_inv, mat_class<4, 4, T, P>>::type inv(const mat_class<4, 4, T, P> &a) {
        typename mat_class<4, 4, T, P>::get_x_type x1;
        typename mat_class<4, 4, T, P>::get_y_type y1;
        typename mat_class<4, 4, T, P>::get_z_type z1;
        typename mat_class<4, 4, T, P>::get_w_type w1;

        a.get_xyzw(x1, y1, z1, w1);

        float4 d0, d1, d2, d3;
        inv_mat(d0, d1, d2, d3, float4(x1.val4()), float4(y1.val4()), float4(z1.val4()), float4(w1.val4()));

        return typename result_type<op_inv, mat_class<4, 4, T, P>>::type(vector4(d0), vector4(d1), vector4(d2), vector4(d3));
    }

    template<i32 R1, i32 C1, typename T1, typename P1,
             i32 R2, i32 C2, typename T2, typename P2>
    bool operator==(const mat_class<R1, C1, T1, P1> &a, const mat_class<R2, C2, T2, P2> &b) {
        typename mat_class<R1, C1, T1, P1>::get_x_type x1; typename mat_class<R2, C2, T2, P2>::get_x_type x2;
        typename mat_class<R1, C1, T1, P1>::get_y_type y1; typename mat_class<R2, C2, T2, P2>::get_y_type y2;
        typename mat_class<R1, C1, T1, P1>::get_z_type z1; typename mat_class<R2, C2, T2, P2>::get_z_type z2;
        typename mat_class<R1, C1, T1, P1>::get_w_type w1; typename mat_class<R2, C2, T2, P2>::get_w_type w2;

        a.get_xyzw(x1, y1, z1, w1);
        b.get_xyzw(x2, y2, z2, w2);

        return x1 == x2 && y1 == y2 && z1 == z2 && w1 == w2;
    }

    template<i32 C1, typename T1, typename P1,
             i32 C2, typename T2, typename P2>
    bool operator==(const mat_class<3, C1, T1, P1> &a, const mat_class<3, C2, T2, P2> &b) {
        typename mat_class<3, C1, T1, P1>::get_x_type x1; typename mat_class<3, C2, T2, P2>::get_x_type x2;
        typename mat_class<3, C1, T1, P1>::get_y_type y1; typename mat_class<3, C2, T2, P2>::get_y_type y2;
        typename mat_class<3, C1, T1, P1>::get_z_type z1; typename mat_class<3, C2, T2, P2>::get_z_type z2;

        a.get_xyz(x1, y1, z1);
        b.get_xyz(x2, y2, z2);

        return x1 == x2 && y1 == y2 && z1 == z2;
    }

    template<i32 R1, i32 C1, typename T1, typename P1,
             i32 R2, i32 C2, typename T2, typename P2>
    bool operator!=(const mat_class<R1, C1, T1, P1> &a, const mat_class<R2, C2, T2, P2> &b) {
        return !(a == b);
    }

    template<i32 C, typename T, typename P>
    mat_class<4, C> operator*(const mat_class<4, C, T, P> &a, f32 b) {
        typename mat_class<4, C, T, P>::get_x_type x1;
        typename mat_class<4, C, T, P>::get_y_type y1;
        typename mat_class<4, C, T, P>::get_z_type z1;
        typename mat_class<4, C, T, P>::get_w_type w1;

        a.get_xyzw(x1, y1, z1, w1);

        return mat_class<4, C>(x1 * b, y1 * b, z1 * b, w1 * b);
    }

    template<i32 C, typename T, typename P>
    mat_class<3, C> operator*(const mat_class<3, C, T, P> &a, f32 b) {
        typename mat_class<3, C, T, P>::get_x_type x1;
        typename mat_class<3, C, T, P>::get_y_type y1;
        typename mat_class<3, C, T, P>::get_z_type z1;

        a.get_xyz(x1, y1, z1);

        return mat_class<3, C>(x1 * b, y1 * b, z1 * b);
    }

    template<i32 R, i32 C, typename T, typename P>
    mat_class<R, C> operator*(f32 a, const mat_class<R, C, T, P> &b) {
        return b * a;
    }

    template<i32 R, i32 C, typename T, typename P, typename T2>
    mat_class<R, C> operator/(const mat_class<R, C, T, P> &a, const T2 &b) {
        return a * inv(b);
    }

    template<i32 R1, i32 C1, typename T1, typename P1,
             i32 R2, i32 C2, typename T2, typename P2>
    typename result_type<op_mul, mat_class<R1, C1, T1, P1>, typename result_type<op_inv, mat_class<R2, C2, T2, P2>>::type>::type
        operator/(const mat_class<R1, C1, T1, P1> &a, const mat_class<R2, C2, T2, P2> &b) {

        return a * inv(b);
    }

    template<i32 R1, typename T1, typename P1,
                     typename T2, typename P2>
    mat_class<4, 3, typename mat_result_kind<op_mul, T1, typename mat_result_kind<op_inv, T2>::type>::type>
        operator/(const mat_class<R1, 3, T1, P1> &a, const mat_class<4, 3, T2, P2> &b) {

        typename mat_class<R1, 3, T1, P1>::get_x_type x1;
        typename mat_class<R1, 3, T1, P1>::get_y_type y1;
        typename mat_class<R1, 3, T1, P1>::get_z_type z1;
        typename mat_class<R1, 3, T1, P1>::get_w_type w1;

        a.get_xyzw(x1, y1, z1, w1);

        typename result_type<op_inv, typename result_type<op_get_m33, mat_class<4, 3, T2, P2>>::type>::type invb(inv(get_m33(b)));

        return mat_class<4, 3, typename mat_result_kind<op_mul, T1, typename mat_result_kind<op_inv, T2>::type>::type>
            (x1 * invb, y1 * invb, z1 * invb, position3((w1 - b.get_w()) * invb));
    }

    template<typename U1, typename R1,
             typename T, typename P>
    vector4 default_mul(const vec_class<4, -1, void, U1, R1> &a, const mat_class<4, 4, T, P> &b) {
        typename mat_class<4, 4, T, P>::get_x_type x1;
        typename mat_class<4, 4, T, P>::get_y_type y1;
        typename mat_class<4, 4, T, P>::get_z_type z1;
        typename mat_class<4, 4, T, P>::get_w_type w1;

        b.get_xyzw(x1, y1, z1, w1);

        return vector4(madd(float4(x1.val34()), a.get_x(),
                            float4(y1.val34()), a.get_y(),
                            float4(z1.val34()), a.get_z(),
                            float4(w1.val34()), a.get_w()));
    }

    template<typename U1, typename R1,
             typename T, typename P>
    vector4 default_mul(const vec_class<4, -1, void, U1, R1> &a, const mat_class<4, 3, T, P> &b) {
        typename mat_class<4, 3, T, P>::get_x_type x1;
        typename mat_class<4, 3, T, P>::get_y_type y1;
        typename mat_class<4, 3, T, P>::get_z_type z1;
        typename mat_class<4, 3, T, P>::get_w_type w1;

        b.get_xyzw(x1, y1, z1, w1);

        float4 tmp = madd(float4(x1.val34()), a.get_x(),
                          float4(y1.val34()), a.get_y(),
                          float4(z1.val34()), a.get_z(),
                          float4(w1.val34()), a.get_w());

        tmp.set_w(a.get_w());

        return vector4(tmp);
    }

    template<typename U1, typename R1,
             typename T, typename P>
    vector4 default_mul(const vec_class<4, -1, void, U1, R1> &a, const mat_class<3, 4, T, P> &b) {
        typename mat_class<3, 4, T, P>::get_x_type x1;
        typename mat_class<3, 4, T, P>::get_y_type y1;
        typename mat_class<3, 4, T, P>::get_z_type z1;

        b.get_xyz(x1, y1, z1);

        return vector4(madd(float4(x1.val34()), a.get_x(),
                            float4(y1.val34()), a.get_y(),
                            float4(z1.val34()), a.get_z()) +
                       float4(float4_axis<3>(float4(a.val4()))));
    }

    template<typename U1, typename R1,
             typename T, typename P>
    vector4 default_mul(const vec_class<4, -1, void, U1, R1> &a, const mat_class<3, 3, T, P> &b) {
        typename mat_class<3, 3, T, P>::get_x_type x1;
        typename mat_class<3, 3, T, P>::get_y_type y1;
        typename mat_class<3, 3, T, P>::get_z_type z1;

        b.get_xyz(x1, y1, z1);

        float4 tmp = madd(float4(x1.val34()), a.get_x(),
                          float4(y1.val34()), a.get_y(),
                          float4(z1.val34()), a.get_z());

        tmp.set_w(a.get_w());

        return vector4(tmp);
    }

    template<typename U1, typename R1,
             i32 R, i32 C, typename T, typename P>
    vector4 operator*(const vec_class<4, -1, void, U1, R1> &a, const mat_class<R, C, T, P> &b) {
        return default_mul(a, b);
    }

    template<typename U1, typename R1,
             i32 C, typename T, typename P>
    typename if_type<C == 3, position3, vector4>::type default_mul(const vec_class<3, 1, void, U1, R1> &a, const mat_class<4, C, T, P> &b) {
        typename mat_class<4, C, T, P>::get_x_type x1;
        typename mat_class<4, C, T, P>::get_y_type y1;
        typename mat_class<4, C, T, P>::get_z_type z1;
        typename mat_class<4, C, T, P>::get_w_type w1;

        b.get_xyzw(x1, y1, z1, w1);

        typedef typename if_type<C == 3, position3, vector4>::type last_row_type;

        return last_row_type(madd(float4(x1.val34()), a.get_x(),
                                  float4(y1.val34()), a.get_y(),
                                  float4(z1.val34()), a.get_z(),
                                  float4(w1.val34())));
    }

    template<typename U1, typename R1,
             typename T, typename P>
    vector4 default_mul(const vec_class<3, 1, void, U1, R1> &a, const mat_class<3, 4, T, P> &b) {
        typename mat_class<3, 4, T, P>::get_x_type x1;
        typename mat_class<3, 4, T, P>::get_y_type y1;
        typename mat_class<3, 4, T, P>::get_z_type z1;

        b.get_xyz(x1, y1, z1);

        return vector4(madd(float4(x1.val4()), a.get_x(),
                            float4(y1.val4()), a.get_y(),
                            float4(z1.val4()), a.get_z(),
                            unit_dir_w().val4()));
    }

    template<typename U1, typename R1,
             typename T, typename P>
    position3 default_mul(const vec_class<3, 1, void, U1, R1> &a, const mat_class<3, 3, T, P> &b) {
        typename mat_class<3, 3, T, P>::get_x_type x1;
        typename mat_class<3, 3, T, P>::get_y_type y1;
        typename mat_class<3, 3, T, P>::get_z_type z1;

        b.get_xyz(x1, y1, z1);

        return position3(madd(float4(x1.val34()), a.get_x(),
                              float4(y1.val34()), a.get_y(),
                              float4(z1.val34()), a.get_z()));
    }

    template<typename U1, typename R1,
             i32 R, i32 C, typename T, typename P>
    typename if_type<C == 3, position3, vector4>::type operator*(const vec_class<3, 1, void, U1, R1> &a, const mat_class<R, C, T, P> &b) {
        return default_mul(a, b);
    }

    template<typename U1, typename R1,
             i32 R, i32 C, typename T, typename P>
    class result_type<op_mul, vec_class<3, 0, void, U1, R1>, mat_class<R, C, T, P>> {

    public:
        typedef typename if_type<(C == 3), dir3, vector4>::type type;
    };

    template<typename U1, typename R1,
             i32 R, i32 C, typename T, typename P>
    typename result_type<op_mul, vec_class<3, 0, void, U1, R1>, mat_class<R, C, T, P>>::type
        operator*(const vec_class<3, 0, void, U1, R1> &a, const mat_class<R, C, T, P> &b) {

        typename mat_class<R, C, T, P>::get_x_type x1;
        typename mat_class<R, C, T, P>::get_y_type y1;
        typename mat_class<R, C, T, P>::get_z_type z1;

        b.get_xyz(x1, y1, z1);

        return typename result_type<op_mul, vec_class<3, 0, void, U1, R1>, mat_class<R, C, T, P>>::type(
            madd(float4(x1.val34()), a.get_x(),
                 float4(y1.val34()), a.get_y(),
                 float4(z1.val34()), a.get_z()));
    }

    template<i32 S1, i32 L1, i32 A1, typename R1,
             i32 R, i32 C, typename T, typename P>
    typename if_type<C == 3, dir3, vector4>::type operator*(const vec_class<S1, L1, vec_axis<A1>, void, R1> &a, const mat_class<R, C, T, P> &b) {
        typedef typename if_type<C == 3, dir3, vector4>::type row_type;

        return row_type(float4(get<A1>(b).baseval()) * a.val1());
    }

    template<i32 S1, i32 L1, i32 A1, typename R1,
             i32 R, i32 C, typename T, typename P>
    typename get_set<A1, mat_class<R, C, T, P>>::type operator*(const vec_class<S1, L1, vec_axis<A1>, vec_unit<1>, R1>&, const mat_class<R, C, T, P> &b) {
        return get<A1>(b);
    }

    template<i32 R, i32 C, typename T, typename P>
    vector4_zero operator*(const vector4_zero&, const mat_class<R, C, T, P>&) {
        return vector4_zero();
    }

    template<i32 R, i32 C, typename T, typename P>
    typename mat_class<R, C, T, P>::get_w_type operator*(const position3_zero&, const mat_class<R, C, T, P> &b) {
        return b.get_w();
    }

    template<i32 R, i32 C, typename T, typename P>
    dir3_zero operator*(const dir3_zero&, const mat_class<R, C, T, P>&) {
        return dir3_zero();
    }

    template<i32 S1, i32 L1, i32 A1, typename R1,
             i32 R, i32 C, typename T, typename P>
    typename result_type<op_neg, typename get_set<A1, mat_class<R, C, T, P>>::type>::type
    operator*(const vec_class<S1, L1, vec_axis<A1>, vec_unit<-1>, R1>&, const mat_class<R, C, T, P> &b) {

        return -get<A1>(b);
    }

    template<i32 S1, i32 L1, typename A1, typename U1, typename R1,
             i32 R, i32 C, typename T, typename P>
    typename if_type<C == 3, position3, vector4>::type operator/(const vec_class<S1, L1, A1, U1, R1> &a,
                                                                 const mat_class<R, C, T, P>         &b) {
        return a * inv(b);
    }

    template<typename A1, typename U1, typename R1,
             i32 R, i32 C, typename T, typename P>
    typename if_type<C == 3, position3, vector4>::type operator/(const vec_class<3, 1, A1, U1, R1> &a,
                                                                 const mat_class<R, C, T, P>       &b) {
        return a * inv(b);
    }

    template<typename A1, typename U1, typename R1,
             typename T, typename P>
    position3 operator/(const vec_class<3, 1, A1, U1, R1> &a,
                        const mat_class<4, 3, T, P>       &b) {

        return position3((a - b.get_w()) * inv(get_m33(b)));
    }

    template<typename A1, typename U1, typename R1,
             typename T, typename P>
    dir3 operator/(const vec_class<3, 0, A1, U1, R1> &a,
                   const mat_class<4, 3, T, P>       &b) {

        return a * inv(get_m33(b));
    }

    template<typename A1, typename U1, typename R1,
             i32 R, i32 C, typename T, typename P>
    typename if_type<C == 3, dir3, vector4>::type operator/(const vec_class<3, 0, A1, U1, R1> &a,
                                                            const mat_class<R, C, T, P>      &b) {

        return a * inv(b);
    }

    template<i32 R, i32 C, typename T, typename P>
    dir3_zero operator/(const dir3_zero&, const mat_class<R, C, T, P>&) {
        return dir3_zero();
    }

    template<typename T, typename P>
    class result_type<op_get_m33, mat_class<4, 3, T, P>> {

    public:
        typedef mat_class<3, 3> type;
    };

    template<typename T, typename P>
    mat_class<3, 3> get_m33(const mat_class<4, 3, T, P> &a) {
        typename mat_class<4, 3, T, P>::get_x_type x1;
        typename mat_class<4, 3, T, P>::get_y_type y1;
        typename mat_class<4, 3, T, P>::get_z_type z1;

        a.get_xyz(x1, y1, z1);

        return mat_class<3, 3>(dir3(x1), dir3(y1), dir3(z1));
    }

    template<typename T, bool fast_mul, typename Po, typename Pa, typename Pb>
    class result_type<op_get_m33, mat_class<4, 3, mat_kind_tran<T, fast_mul>, rep_lazy<Po, Pa, Pb>>> {

    public:
        typedef mat_class<3, 3, T> type;
    };

    template<typename T, bool fast_mul, typename Po, typename Pa, typename Pb>
    mat_class<3, 3, T> get_m33(const mat_class<4, 3, mat_kind_tran<T, fast_mul>, rep_lazy<Po, Pa, Pb>> &a) {
        typename mat_class<4, 3, mat_kind_tran<T, fast_mul>, rep_lazy<Po, Pa, Pb>>::get_x_type x1;
        typename mat_class<4, 3, mat_kind_tran<T, fast_mul>, rep_lazy<Po, Pa, Pb>>::get_y_type y1;
        typename mat_class<4, 3, mat_kind_tran<T, fast_mul>, rep_lazy<Po, Pa, Pb>>::get_z_type z1;

        a.get_xyz(x1, y1, z1);

        return mat_class<3, 3, T>(x1, y1, z1);
    }

    template<typename T, bool fast_mul, typename P>
    class result_type<op_get_m33, mat_class<4, 3, mat_kind_tran<T, fast_mul>, P>> {

    public:
        typedef typename mat_class<4, 3, mat_kind_tran<T, fast_mul>, P>::get_m33_type type;
    };

    template<typename T, bool fast_mul, typename P>
    typename result_type<op_get_m33, mat_class<4, 3, mat_kind_tran<T, fast_mul>, P>>::type get_m33(const mat_class<4, 3, mat_kind_tran<T, fast_mul>, P> &a) {
        return a.get_m33();
    }

    // inlined @ sub_7CB940
    // inlined @ sub_7CC710
    // inlined @ sub_7CD8C0
    // inlined @ sub_7CEA90
    inline mat43 operator*(const mat43 &a, const mat43 &b) {
        auto element = [&](const float4 &row, i32 column) {
            return (f64)row[0] * b.get_x()[column] +
                   (f64)row[1] * b.get_y()[column] +
                   (f64)row[2] * b.get_z()[column];
        };

        auto direction = [&](const dir3 &row) {
            return dir3((f32)element(row.val34(), 0),
                        (f32)element(row.val34(), 1),
                        (f32)element(row.val34(), 2));
        };

        const float4 &w = a.get_w().val34();

        return mat43(direction(a.get_x()),
                     direction(a.get_y()),
                     direction(a.get_z()),
                     position3((f32)(element(w, 0) + b.get_w()[0]),
                               (f32)(element(w, 1) + b.get_w()[1]),
                               (f32)(element(w, 2) + b.get_w()[2])));
    }

    ASSERT_SIZEOF(mat33,         0x30);
    ASSERT_SIZEOF(mat34,         0x30);
    ASSERT_SIZEOF(mat43,         0x40);
    ASSERT_SIZEOF(mat43::packed, 0x30);
    ASSERT_SIZEOF(mat44,         0x40);
}} // treyarch::math
