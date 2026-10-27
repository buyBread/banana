#pragma once

#include "game/color.hh"
#include "treyarch/mash/types.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    namespace mash {
        class mash_info_struct;
    } // mash

    struct road_lod_scale_offset {
        f32 scale;
        f32 offset;
    };

    class sm_road_lod_param {

    public:
        enum e_wrapping : i32 {
            wrapping_wrap,
            wrapping_clamp,
            wrapping_border_white,
            wrapping_border_black
        };

        void* self; // the resource handler points this at the parameters themselves

        e_wrapping            wrapping;
        road_lod_scale_offset diffuse;
        road_lod_scale_offset specularity;
        road_lod_scale_offset specularexponent;
        road_lod_scale_offset grunge;
        road_lod_scale_offset noise;
        road_lod_scale_offset micro;
        road_lod_scale_offset height;
        road_lod_scale_offset lightmap;
        f32                   fresnel[2];
        f32                   micro_scale;
        f32                   grunge_scale;
        f32                   height_scale;
        f32                   micro_texels_per_meter;
        f32                   grunge_texels_per_meter;
        f32                   noise_texels_per_meter;
        color                 grunge_diffuse_low_rgb;
        f32                   micro_diffuse_level;
        f32                   noise_diffuse_level;

        // sub_7D3AE0
        void construct_mashed_class() {
            self = nullptr;
        }

        // sub_7C2DE0
        void destruct_mashed_class() {}

        // sub_636AB0
        void unmash(mash::mash_info_struct*, void*, mash::buffer_type) {}
    };

    ASSERT_SIZEOF  (road_lod_scale_offset, 0x08);

    ASSERT_SIZEOF  (sm_road_lod_param,                         0x80);
    ASSERT_OFFSETOF(sm_road_lod_param, diffuse,                0x08);
    ASSERT_OFFSETOF(sm_road_lod_param, fresnel,                0x48);
    ASSERT_OFFSETOF(sm_road_lod_param, grunge_diffuse_low_rgb, 0x68);
    ASSERT_OFFSETOF(sm_road_lod_param, noise_diffuse_level,    0x7C);
} // treyarch
