#pragma once

#include "util/types.hh"
#include "treyarch/ngl/math/math.hh"
#include "treyarch/ngl/math/types/vector2.hh"
#include "treyarch/shared/mash/mash.hh"

namespace treyarch { namespace ngl {
    class phys_vec3;
    
    class vector3 { // L15346: SM3 .ii

public:
        f32 x, y, z;

        f32 get_x() const { return x; }
        f32 get_y() const { return y; }
        f32 get_z() const { return z; }
        f32 get_w() const { return 0.0f; } // ?

        template<typename V>
        void set_x(const V &val) { x=val; }
        template<typename V>
        void set_y(const V &val) { y=val; }
        template<typename V>
        void set_z(const V &val) { z=val; }

        vector3() {}
        vector3(const vector3 &v) {
            x = v.x;
            y = v.y;
            z = v.z;
        }
        vector3(f32 _x, f32 _y, f32 _z) {
            x = _x;
            y = _y;
            z = _z;
        }
        explicit vector3(const vector2& v, f32 d=1.0F) {
            x = v.x;
            y = v.y;
            z = d;
        }
        explicit vector3(f32 d) { x = y = z = d; }

        vector3& operator = (const vector3 &v) {
            x = v.x;
            y = v.y;
            z = v.z;
            
            return *this;
        }

        vector3& operator += (const vector3 &v) {
            x += v.x;
            y += v.y;
            z += v.z;
            
            return *this;
        }

        vector3& operator -= (const vector3 &v) {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            
            return *this;
        }

        vector3& operator *= (f32 d) {
            x *= d;
            y *= d;
            z *= d;
            
            return *this;
        }

        vector3& operator /= (f32 d) {
            f32 d_inv = 1.0f / d;
            
            x *= d_inv;
            y *= d_inv;
            z *= d_inv;
            
            return *this;
        }

        vector3 operator - () const {
            return vector3(-x, -y, -z);
        }

        const f32& operator[] (int i) const { return (&x)[i]; }
              f32& operator[] (int i)       { return (&x)[i]; }

        f32 length2() const {
            return x*x + y*y + z*z;
        }

        f32 length() const {
            return (f32)sqrtf(x*x + y*y + z*z);
        }

        vector3 &normalize(){
            f32 l2 = length2();
            
            if (l2 > SMALL_DIST*SMALL_DIST)
                *this *= math::fast_recip_sqrt(l2);
            
            return *this;
        }

        vector3 &set_length(f32 newlen=1.0f){
            f32 l2 = length2();

            if (l2 > SMALL_DIST*SMALL_DIST)
                *this *= newlen * math::fast_recip_sqrt(l2);
            
            return *this;
        }

        vector3 perp() const; /* hey, what are you? */

        const vector2 &get_xy() const { return *(vector2*)&x; }
        const vector2 &get_yz() const { return *(vector2*)&y; }
        const vector2  get_xz() const { return vector2(x,z); }

        f32 xy_length2() const { return x*x + y*y; }
        f32 yz_length2() const { return y*y + z*z; }
        f32 xz_length2() const { return x*x + z*z; }

        f32 xy_length() const { return (f32)sqrtf(x*x + y*y); }
        f32 yz_length() const { return (f32)sqrtf(y*y + z*z); }
        f32 xz_length() const { return (f32)sqrtf(x*x + z*z); }

        bool is_valid() const {
            return x > -HUGE_DIST && x < HUGE_DIST &&
                   y > -HUGE_DIST && y < HUGE_DIST &&
                   z > -HUGE_DIST && z < HUGE_DIST;
        }

        bool is_normal() const {
            return fabsf(length2() - 1.0f) < 0.01f;
        }

        void mash_convert(mash::generic_mash_info *inf, void *begin_image); /* hey, what are you? */

        operator const phys_vec3() const; /* hey, what are you? */
        
        vector3(const phys_vec3&); /* hey, what are you? */
    };
}} // treyarch::ngl