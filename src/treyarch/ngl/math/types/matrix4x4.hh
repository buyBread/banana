#pragma once

#include "util/types.hh"
#include "treyarch/ngl/math/types/vector3.hh"
#include "treyarch/ngl/math/types/vector4.hh"
#include "treyarch/shared/mash/mash.hh"

namespace treyarch { namespace ngl {
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

        const row& operator [] (int i) const { return (&x)[i]; }
              row& operator [] (int i)       { return (&x)[i]; }

              vector3 &x_row()       { return *(vector3*)&x; }
        const vector3 &x_row() const { return *(vector3*)&x; }
              vector3 &y_row()       { return *(vector3*)&y; }
        const vector3 &y_row() const { return *(vector3*)&y; }
              vector3 &z_row()       { return *(vector3*)&z; }
        const vector3 &z_row() const { return *(vector3*)&z; }
              vector3 &w_row()       { return *(vector3*)&w; }
        const vector3 &w_row() const { return *(vector3*)&w; }        

        void make_projection(f32 field_of_view=1.570795f, f32 aspect=1.0f, f32 near_plane=1.0f, f32 far_plane=1000.0f, f32 push=0.0f); /* hey, what are you? */
        void make_frustum(f32 left=-1.0f, f32 top=-1.0f, f32 right=1.0f, f32 bottom=1.0f, f32 znear=1.0f,f32 zfar=1000.0f, f32 push=0.0f); /* hey, what are you? */ 

        matrix4x4& operator*=(f32 s) {
            x.x *= s; x.y *= s; x.z *= s; x.w *= s;
            y.x *= s; y.y *= s; y.z *= s; y.w *= s;
            z.x *= s; z.y *= s; z.z *= s; z.w *= s;
            w.x *= s; w.y *= s; w.z *= s; w.w *= s;

            return *this;
        }

        matrix4x4 operator*(const matrix4x4& b) const {
            const matrix4x4 &a = *this;

            return matrix4x4(a[0][0]*b[0][0] + a[0][1]*b[1][0] + a[0][2]*b[2][0] + a[0][3]*b[3][0], // x row
                             a[0][0]*b[0][1] + a[0][1]*b[1][1] + a[0][2]*b[2][1] + a[0][3]*b[3][1],
                             a[0][0]*b[0][2] + a[0][1]*b[1][2] + a[0][2]*b[2][2] + a[0][3]*b[3][2],
                             a[0][0]*b[0][3] + a[0][1]*b[1][3] + a[0][2]*b[2][3] + a[0][3]*b[3][3],

                             a[1][0]*b[0][0] + a[1][1]*b[1][0] + a[1][2]*b[2][0] + a[1][3]*b[3][0], // y row
                             a[1][0]*b[0][1] + a[1][1]*b[1][1] + a[1][2]*b[2][1] + a[1][3]*b[3][1],
                             a[1][0]*b[0][2] + a[1][1]*b[1][2] + a[1][2]*b[2][2] + a[1][3]*b[3][2],
                             a[1][0]*b[0][3] + a[1][1]*b[1][3] + a[1][2]*b[2][3] + a[1][3]*b[3][3],

                             a[2][0]*b[0][0] + a[2][1]*b[1][0] + a[2][2]*b[2][0] + a[2][3]*b[3][0], // z row
                             a[2][0]*b[0][1] + a[2][1]*b[1][1] + a[2][2]*b[2][1] + a[2][3]*b[3][1],
                             a[2][0]*b[0][2] + a[2][1]*b[1][2] + a[2][2]*b[2][2] + a[2][3]*b[3][2],
                             a[2][0]*b[0][3] + a[2][1]*b[1][3] + a[2][2]*b[2][3] + a[2][3]*b[3][3],

                             a[3][0]*b[0][0] + a[3][1]*b[1][0] + a[3][2]*b[2][0] + a[3][3]*b[3][0], // w row
                             a[3][0]*b[0][1] + a[3][1]*b[1][1] + a[3][2]*b[2][1] + a[3][3]*b[3][1],
                             a[3][0]*b[0][2] + a[3][1]*b[1][2] + a[3][2]*b[2][2] + a[3][3]*b[3][2],
                             a[3][0]*b[0][3] + a[3][1]*b[1][3] + a[3][2]*b[2][3] + a[3][3]*b[3][3]);
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

        matrix4x4 inverse() {
            f32 det = determinant();

            if (det != 0.0f) {
                matrix4x4 result = adjugate();
                result *= 1.0f / det;

                return result;
            }

            matrix4x4 result;
            result.identity();

            return result;
        }

        matrix4x4 transpose() {
            return matrix4x4(x.x, y.x, z.x, w.x,
                             x.y, y.y, z.y, w.y,
                             x.z, y.z, z.z, w.z,
                             x.w, y.w, z.w, w.w);
        }

        matrix4x4 cof() {
            matrix4x4 result;

            for (i32 row = 0; row < 4; ++row) {
                for (i32 column = 0; column < 4; ++column)
                    result[row][column] = cofactor(row, column);
            }

            return result;
        }

        matrix4x4 adjugate() {
            matrix4x4 result = cof();

            return result.transpose();
        }

        f32 cofactor(int excluded_row, int excluded_column) const {
            f32 minor[3][3];
            i32 minor_row = 0;

            for (i32 row = 0; row < 4; ++row) {
                if (row == excluded_row)
                    continue;

                i32 minor_column = 0;

                for (i32 column = 0; column < 4; ++column) {
                    if (column == excluded_column)
                        continue;

                    minor[minor_row][minor_column] = (*this)[row][column];
                    
                    ++minor_column;
                }

                ++minor_row;
            }

            f32 sign = ((excluded_row ^ excluded_column) & 1) ? -1.0f : 1.0f;

            f32 minor_determinant = minor[0][0] * minor[1][1] * minor[2][2] +
                                    minor[0][1] * minor[1][2] * minor[2][0] +
                                    minor[0][2] * minor[1][0] * minor[2][1] -
                                    minor[0][2] * minor[1][1] * minor[2][0] -
                                    minor[0][0] * minor[1][2] * minor[2][1] -
                                    minor[0][1] * minor[1][0] * minor[2][2];

            return sign * minor_determinant;
        }

        f32 determinant3() const {
            return x.x * y.y * z.z - y.x * z.z * x.y +
                   y.x * z.y * x.z - z.x * y.y * x.z +
                   z.x * x.y * y.z - x.x * z.y * y.z;
        };
        f32 determinant() const {
            f32 result = 0.0f;

            for (i32 column = 0; column < 4; ++column)
                result += (*this)[0][column] * cofactor(0, column);

            return result;
        }
    };
}}
