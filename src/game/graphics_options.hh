#pragma once

#include "treyarch/mash/types.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    namespace mash {
        class mash_info_struct;
    } // mash

    class graphics_options {

    public:
        struct entities_t {
            f32 fade_in_distance;
            f32 fade_out_distance;
            f32 no_fade_radius;
            f32 fade_altitude_bias;
        };

        struct phat_t {
            enum e_map : i32 {
                map_all,
                map_diffuse,
                map_opacity,
                map_specularity,
                map_specular_exponent,
                map_emissiveness,
                map_normal,
                map_reflectivity,
                map_environment,
                map_gobo,
                map_horizon,
                map_shadow_1,
                map_shadow_2
            };

            e_map map;
            bool  horizon_mapping_enabled;
            bool  gobo_enabled;
            bool  shadow_enabled;
            u8    pad_07;
        };

        struct decal_t {
            bool ambient_occlusion_mode;
            u8   pad_01[3];
        };

        struct light_t {
            bool sun;
            bool moon;
            bool sky;
            bool point;
            bool ambient;
            bool horizon;
            bool directional;
            bool streetlight;
            i32  maximum;
            f32  bounce_ratio;
        };

        struct bloom_t {
            f32 brightness_multiplier;
            i32 blur_iterations;
        };

        struct shadows_t {
            f32 smallest_dim;
            f32 step_factor;
            f32 depth_factor;
        };

        struct buildings_t {
            enum e_map : i32 {
                map_all,
                map_diffuse,
                map_opacity,
                map_special_effects,
                map_normal
            };

            e_map map;
            bool  high_lod_enabled;
            u8    pad_05[3];
            f32   high_lod_distance;
            f32   medium_lod_distance;
            bool  tilemap_indicator;
            bool  meshmap_indicator;
            u8    pad_12[2];
            f32   interior_distance;
            f32   fresnel_min;
            f32   fresnel_max;
            f32   shadow_scale;
        };

        struct fog_t {
            bool depth;
            bool volumetric;
            u8   pad_02[2];
        };

        void*       self; // the resource handler points this at the options themselves
        entities_t  entities;
        phat_t      phat;
        decal_t     decal;
        light_t     light;
        bool        render_diffuse;
        bool        render_specular;
        bool        render_grey;
        bool        render_wireframe;
        bool        autoexposure;
        u8          pad_35[3];
        f32         exposure;
        f32         exposure_speed;
        f32         emissiveness;
        f32         ambience;
        f32         specular_exponent;
        f32         opacity;
        f32         specularity;
        f32         reflectivity;
        bool        local_space_lighting;
        u8          pad_59[3];
        bloom_t     bloom;
        i32         ghosts;
        f32         unk_68[12];
        shadows_t   shadows;
        buildings_t buildings;
        fog_t       fog;
        f32         particle_scale;
        f32         time_scale;

        // sub_7D3AE0
        void construct_mashed_class() {
            self = nullptr;
        }

        // sub_7C2DE0
        void destruct_mashed_class() {}

        // sub_636AB0
        void unmash(mash::mash_info_struct*, void*, mash::buffer_type) {}
    };

    ASSERT_SIZEOF  (graphics_options::phat,      0x08);
    ASSERT_SIZEOF  (graphics_options::decal,     0x04);
    ASSERT_SIZEOF  (graphics_options::light,     0x10);
    ASSERT_SIZEOF  (graphics_options::buildings, 0x24);
    ASSERT_SIZEOF  (graphics_options::fog,       0x04);

    ASSERT_SIZEOF  (graphics_options,                       0xD4);
    ASSERT_OFFSETOF(graphics_options, phat,                 0x14);
    ASSERT_OFFSETOF(graphics_options, light,                0x20);
    ASSERT_OFFSETOF(graphics_options, render_diffuse,       0x30);
    ASSERT_OFFSETOF(graphics_options, exposure,             0x38);
    ASSERT_OFFSETOF(graphics_options, local_space_lighting, 0x58);
    ASSERT_OFFSETOF(graphics_options, bloom,                0x5C);
    ASSERT_OFFSETOF(graphics_options, unk_68,               0x68);
    ASSERT_OFFSETOF(graphics_options, shadows,              0x98);
    ASSERT_OFFSETOF(graphics_options, buildings,            0xA4);
    ASSERT_OFFSETOF(graphics_options, fog,                  0xC8);
    ASSERT_OFFSETOF(graphics_options, time_scale,           0xD0);
} // treyarch
