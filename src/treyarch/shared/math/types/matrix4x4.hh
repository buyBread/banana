#pragma once

#include "util/types.hh"
#include "treyarch/shared/math/types/vector3.hh"
#include "treyarch/shared/math/types/vector4.hh"
#include "treyarch/shared/mash/mash.hh"

namespace treyarch {
    using row = vector4;

    class matrix4x4 { // L15637: SM3 .ii

    public:
        row x, y, z, w;

        matrix4x4() {}
        matrix4x4(vector3 const &x_row,
                  vector3 const &y_row,
                  vector3 const &z_row,
                  vector3 const &w_row = vector3(0.0f, 0.0f, 0.0f))
            : x(x_row, 0.0f),
              y(y_row, 0.0f),
              z(z_row, 0.0f),
              w(w_row, 1.0f) {}
        matrix4x4(f32 _00, f32 _01, f32 _02, f32 _03,
                  f32 _10, f32 _11, f32 _12, f32 _13,
                  f32 _20, f32 _21, f32 _22, f32 _23,
                  f32 _30, f32 _31, f32 _32, f32 _33)
            : x(_00, _01, _02, _03)
            , y(_10, _11, _12, _13)
            , z(_20, _21, _22, _23)
            , w(_30, _31, _32, _33) {}

        const row &operator[](int i) const { return (&x)[i]; }
              row &operator[](int i)       { return (&x)[i]; }

              vector3 &x_row()       { return *(vector3*)&x; }
        const vector3 &x_row() const { return *(vector3*)&x; }
              vector3 &y_row()       { return *(vector3*)&y; }
        const vector3 &y_row() const { return *(vector3*)&y; }
              vector3 &z_row()       { return *(vector3*)&z; }
        const vector3 &z_row() const { return *(vector3*)&z; }
              vector3 &w_row()       { return *(vector3*)&w; }
        const vector3 &w_row() const { return *(vector3*)&w; }        

        void make_projection(f32 field_of_view=1.570795f, f32 aspect=1.0f, f32 near_plane=1.0f, f32 far_plane=1000.0f, f32 push=0.0f);
        void make_frustum(f32 left=-1.0f, f32 top=-1.0f, f32 right=1.0f, f32 bottom=1.0f, f32 znear=1.0f,f32 zfar=1000.0f, f32 push=0.0f); /* hey, what are you? */ 

        matrix4x4& operator*=(f32 s) {
            x.x *= s; x.y *= s; x.z *= s; x.w *= s;
            y.x *= s; y.y *= s; y.z *= s; y.w *= s;
            z.x *= s; z.y *= s; z.z *= s; z.w *= s;
            w.x *= s; w.y *= s; w.z *= s; w.w *= s;

            return *this;
        }

        // retail sums each element in double and rounds it once
        matrix4x4 operator*(const matrix4x4 &b) const {
            const matrix4x4 &a = *this;

            auto element = [&](i32 row, i32 column) {
                return (f32)((f64)a[row][0] * b[0][column] +
                             (f64)a[row][1] * b[1][column] +
                             (f64)a[row][2] * b[2][column] +
                             (f64)a[row][3] * b[3][column]);
            };

            return matrix4x4(element(0, 0), element(0, 1), element(0, 2), element(0, 3),
                             element(1, 0), element(1, 1), element(1, 2), element(1, 3),
                             element(2, 0), element(2, 1), element(2, 2), element(2, 3),
                             element(3, 0), element(3, 1), element(3, 2), element(3, 3));
        }

        void make_translate(const vector3 &t); /* hey, what are you? */
        void make_rotate(const vector3 &u, f32 a); /* hey, what are you? */
        void make_scale(const vector3 &s); /* hey, what are you? */
        void make_mirror(const vector3 &n, f32 d); /* hey, what are you? */

        void identity() {
            *this = matrix4x4(1.0f, 0.0f, 0.0f, 0.0f,
                              0.0f, 1.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 1.0f, 0.0f,
                              0.0f, 0.0f, 0.0f, 1.0f);
        }

        void translate(const vector3 &t); /* hey, what are you? */
        void rotate(const vector3 &u, f32 a); /* hey, what are you? */
        void scale(const vector3 &s); /* hey, what are you? */
        void scale(f32 s); /* hey, what are you? */
        void orthonormalize(); /* hey, what are you? */
        void mash_convert(mash::generic_mash_info *inf, void *begin_image); /* hey, what are you? */

        matrix4x4 inverse() const;

        // retail affine packets leave the fourth column uninitialized,
        // read only xyz from each row before using one in a full matrix product
        matrix4x4 affine() const {
            return matrix4x4(x_row(), y_row(), z_row(), w_row());
        }

        // callers that need scale removal must do it first; inverse() is not equivalent (0x0044D470)
        matrix4x4 inverse_orthonormal() const {
            return matrix4x4(x.x, y.x, z.x, 0.0f,
                             x.y, y.y, z.y, 0.0f,
                             x.z, y.z, z.z, 0.0f,
                             // w
                             -(f32)((f64)w.x * x.x + (f64)w.y * x.y + (f64)w.z * x.z),
                             -(f32)((f64)w.x * y.x + (f64)w.y * y.y + (f64)w.z * y.z),
                             -(f32)((f64)w.x * z.x + (f64)w.y * z.y + (f64)w.z * z.z),
                             1.0f);
        }

        matrix4x4 transpose() const {
            return matrix4x4(x.x, y.x, z.x, w.x,
                             x.y, y.y, z.y, w.y,
                             x.z, y.z, z.z, w.z,
                             x.w, y.w, z.w, w.w);
        }

        // the cofactor matrix, transposed into the adjugate when asked to
        matrix4x4 cofactors(bool transposed) const;

        matrix4x4 cof()      const { return cofactors(false); }
        matrix4x4 adjugate() const { return cofactors(true);  }

        f32 determinant3() const {
            return (f32)((f64)x.x * y.y * z.z - (f64)y.x * z.z * x.y +
                         (f64)y.x * z.y * x.z - (f64)z.x * y.y * x.z +
                         (f64)z.x * x.y * y.z - (f64)x.x * z.y * y.z);
        };

        f32 determinant() const;
    };
} // treyarch
