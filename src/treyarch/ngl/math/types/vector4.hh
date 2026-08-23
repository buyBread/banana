#pragma once

#include "util/types.hh"
#include "treyarch/ngl/math/math.hh"
#include "treyarch/ngl/math/types/vector2.hh"
#include "treyarch/ngl/math/types/vector3.hh"
#include "treyarch/shared/mash/mash.hh"

namespace treyarch { namespace ngl {
    class matrix4x4;

    class vector4 { // L15465: SM3 .ii
    
public:
        f32 x, y, z, w;

        f32 get_x() const { return x; }
        f32 get_y() const { return y; }
        f32 get_z() const { return z; }
        f32 get_w() const { return w; }

        template<typename V>
        void set_x(const V &val) { x=val; }
        template<typename V>
        void set_y(const V &val) { y=val; }
        template<typename V>
        void set_z(const V &val) { z=val; }
        template<typename V>
        void set_w(const V &val) { w=val; }

        vector4() {}
        vector4(const vector4& v) {
            x = v.x;
            y = v.y;
            z = v.z;
            w = v.w;
        }
        vector4(f32 _x, f32 _y, f32 _z, f32 _w) {
            x = _x;
            y = _y;
            z = _z;
            w = _w;
        }
        explicit vector4(const vector3 &v, f32 d=1.0f) {
            x = v.x;
            y = v.y;
            z = v.z;
            w = d;
        }
        explicit vector4(f32 d) { x = y = z = w = d; }

        vector4& operator = (const vector4 &v) {
            x = v.x;
            y = v.y;
            z = v.z;
            w = v.w;
            
            return *this;
        }

        vector4& operator += (const vector4 &v) {
            x += v.x;
            y += v.y;
            z += v.z;
            w += v.w;
            
            return *this;
        }

        vector4& operator -= (const vector4 &v) {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            w -= v.w;
            
            return *this;
        }

        vector4& operator *= (f32 d) {
            x *= d;
            y *= d;
            z *= d;
            w *= d;
            
            return *this;
        }

        vector4& operator /= (f32 d) {
            f32 d_inv = 1.0f/d;
            
            x *= d_inv;
            y *= d_inv;
            z *= d_inv;
            w *= d_inv;
            
            return *this;
        }

        vector4 operator - () const {
            return vector4(-x, -y, -z, -w);
        }

        const f32& operator [] (int i) const { return (&x)[i]; }
              f32& operator [] (int i)       { return (&x)[i]; }

        f32 length2() const {
            return x*x + y*y + z*z + w*w;
        }

        f32 length() const {
            return (f32)sqrtf(x*x + y*y + z*z + w*w);
        }

        vector4& normalize() {
            f32 l2 = length2();

            if (l2 > SMALL_DIST*SMALL_DIST)
                *this *= math::fast_recip_sqrt(l2);
        
            return *this;
        }

        vector4 &homogenize() {
            if (w) w=1.0f/w;
                x*=w; y*=w; z*=w;
            
            return *this;
        }

        vector4 &set_length(f32 newlen=1.0f) {
            f32 l2 = length2();

            if (l2 > SMALL_DIST*SMALL_DIST)
                *this *= newlen * math::fast_recip_sqrt(l2);

            return *this;
        }


        const vector2 &get_xy() const {
            return *(vector2*)&x;
        }

        const vector3 &get_xyz() const {
            return *(vector3*)&x;
        }

        void set_xyz(const vector3 &new_xyz) {
            x = new_xyz.x;
            y = new_xyz.y;
            z = new_xyz.z;
        }
        void set_xy(const vector2 &new_xy) {
            x = new_xy.x;
            y = new_xy.y;
        }

        bool is_valid() const {
            return x > -HUGE_DIST && x < HUGE_DIST &&
                   y > -HUGE_DIST && y < HUGE_DIST &&
                   z > -HUGE_DIST && z < HUGE_DIST &&
                   w > -HUGE_DIST && w < HUGE_DIST;
        }

        vector4 transform_plane(const matrix4x4 &matrix) const;

        void mash_convert(mash::generic_mash_info *inf, void *begin_image); /* hey, what are you? */
    };
}}
