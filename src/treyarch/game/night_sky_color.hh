#pragma once

#include "treyarch/game/gradient.hh"
#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector_basic.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class night_sky_color {

    public:
        void*                   self; // the resource handler points this at the colors themselves
        mash::vector_basic<f32> dome_scale0;
        mash::vector_basic<f32> dome_scale1;
        mash::vector_basic<f32> dome_scale2;
        mash::vector_basic<f32> dome_scale3;
        mash::vector_basic<f32> dome_power0;
        mash::vector_basic<f32> dome_power1;
        mash::vector_basic<f32> dome_power2;
        mash::vector_basic<f32> dome_power3;
        gradient                dome_color0;
        mash::vector_basic<f32> dome_color0_intensity;
        gradient                dome_color1;
        mash::vector_basic<f32> dome_color1_intensity;
        gradient                dome_color2;
        mash::vector_basic<f32> dome_color2_intensity;
        gradient                dome_color3;
        mash::vector_basic<f32> dome_color3_intensity;
        gradient                dome_base_color;
        mash::vector_basic<f32> dome_color_base_intensity;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (night_sky_color,                            0x124);
    ASSERT_OFFSETOF(night_sky_color, dome_power0,               0x044);
    ASSERT_OFFSETOF(night_sky_color, dome_color0,               0x084);
    ASSERT_OFFSETOF(night_sky_color, dome_base_color,           0x104);
    ASSERT_OFFSETOF(night_sky_color, dome_color_base_intensity, 0x114);
} // treyarch
