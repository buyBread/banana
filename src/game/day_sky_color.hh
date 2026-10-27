#pragma once

#include "game/gradient.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector_basic.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class day_sky_color {

    public:
        void*                   self; // the resource handler points this at the colors themselves
        mash::vector_basic<f32> ray_scaler;
        mash::vector_basic<f32> mie_scaler;
        mash::vector_basic<f32> inscatter_scaler;
        f32                     extinction_scaler;
        mash::vector_basic<f32> g;
        mash::vector_basic<f32> sun_intensity;
        mash::vector_basic<f32> turbidity;
        mash::vector_basic<f32> skydome_zh_power;
        mash::vector_basic<f32> skydome_zenith;
        mash::vector_basic<f32> skydome_horizon;
        mash::vector_basic<f32> atmospheric_wave_length_r;
        mash::vector_basic<f32> atmospheric_wave_length_g;
        mash::vector_basic<f32> atmospheric_wave_length_b;
        f32                     sun_rise;
        f32                     sun_set;
        mash::vector_basic<f32> sun_radius_inner0;
        mash::vector_basic<f32> sun_radius_outer0;
        gradient                sun_color0;
        mash::vector_basic<f32> sun_color_intensity0;
        mash::vector_basic<f32> sun_radius_inner1;
        mash::vector_basic<f32> sun_radius_outer1;
        gradient                sun_color1;
        mash::vector_basic<f32> sun_color_intensity1;
        mash::vector_basic<f32> sun_radius_inner2;
        mash::vector_basic<f32> sun_radius_outer2;
        gradient                sun_color2;
        mash::vector_basic<f32> sun_color_intensity2;
        mash::vector_basic<f32> sun_radius_inner3;
        mash::vector_basic<f32> sun_radius_outer3;
        gradient                sun_color3;
        mash::vector_basic<f32> sun_color_intensity3;
        mash::vector_basic<f32> god_ray_look_at_inner_angle;
        mash::vector_basic<f32> god_ray_look_at_outer_angle;
        mash::vector_basic<f32> god_ray_screen_area_x_min;
        mash::vector_basic<f32> god_ray_screen_area_x_max;
        mash::vector_basic<f32> god_ray_screen_area_y_min;
        mash::vector_basic<f32> god_ray_screen_area_y_max;
        gradient                god_ray_color;
        mash::vector_basic<f32> god_ray_intensity;
        mash::vector_basic<f32> sun_ring_scale_inner;
        mash::vector_basic<f32> sun_ring_scale_outer;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (day_sky_color,                       0x270);
    ASSERT_OFFSETOF(day_sky_color, extinction_scaler,    0x034);
    ASSERT_OFFSETOF(day_sky_color, g,                    0x038);
    ASSERT_OFFSETOF(day_sky_color, sun_rise,             0x0C8);
    ASSERT_OFFSETOF(day_sky_color, sun_radius_inner0,    0x0D0);
    ASSERT_OFFSETOF(day_sky_color, sun_color0,           0x0F0);
    ASSERT_OFFSETOF(day_sky_color, god_ray_color,        0x230);
    ASSERT_OFFSETOF(day_sky_color, sun_ring_scale_outer, 0x260);
} // treyarch
