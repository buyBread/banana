#pragma once

#include <array>
#include <cstddef>
#include <d3d9.h>

#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"
#include "treyarch/ngl/d3d9/vertex_definition.hh"

namespace treyarch { namespace ngl { namespace shaders { namespace program_exports {
namespace pcuv {
    inline util::memory_reference<IDirect3DVertexShader9*> vertex_program { 0x01114350 };
    inline util::memory_reference<IDirect3DPixelShader9*>  pixel_program  { 0x01114354 };
    inline util::memory_reference<vertex_definition>       format         { 0x010F7C50 };
} // pcuv

namespace puv {
    inline util::memory_reference<IDirect3DVertexShader9*> vertex_program { 0x01114394 };
    inline util::memory_reference<IDirect3DPixelShader9*>  pixel_program  { 0x01114390 };
} // puv

namespace sm_simple {
    inline util::memory_reference<IDirect3DVertexShader9*> vertex_program { 0x01114410 };
    inline util::memory_reference<IDirect3DPixelShader9*>  pixel_program  { 0x01114414 };
} // sm_simple

namespace sm_translucent {
    inline util::memory_reference<IDirect3DVertexShader9*> vertex_program { 0x01114450 };
    inline util::memory_reference<IDirect3DPixelShader9*>  pixel_program  { 0x01114454 };
} // sm_translucent

namespace sm_depth_shadow {
    inline util::memory_reference<IDirect3DVertexShader9*> vertex_program { 0x01114794 };
    inline util::memory_reference<IDirect3DPixelShader9*>  pixel_program  { 0x01114790 };
} // sm_depth_shadow

namespace smsky {
    inline util::memory_reference<IDirect3DVertexShader9*> vertex_program         { 0x011143D8 };
    inline util::memory_reference<IDirect3DPixelShader9*>  daylight_pixel_program { 0x011143D0 };
    inline util::memory_reference<IDirect3DPixelShader9*>  night_pixel_program    { 0x011143D4 };
    inline util::memory_reference<IDirect3DPixelShader9*>  combined_pixel_program { 0x011143DC };
} // smsky

namespace road_lights {
    inline util::memory_reference<IDirect3DVertexShader9*> vertex_program { 0x01114D24 };
    inline util::memory_reference<IDirect3DPixelShader9*>  pixel_program  { 0x01114D20 };
} // road_lights

namespace fake_peds {
    inline util::memory_reference<IDirect3DVertexShader9*> vertex_program { 0x01114A74 };
    inline util::memory_reference<IDirect3DPixelShader9*>  pixel_program  { 0x01114A70 };
} // fake_peds

namespace sm_citylod {
    inline util::memory_reference<IDirect3DVertexShader9*> vertex_program { 0x01114C64 };
    inline util::memory_reference<IDirect3DPixelShader9*>  pixel_program  { 0x01114C60 };
} // sm_citylod

namespace sm_retrofit {
    inline util::memory_reference<IDirect3DVertexShader9*> vertex_program { 0x01114CA4 };
    inline util::memory_reference<IDirect3DPixelShader9*>  pixel_program  { 0x01114CA0 };
} // sm_retrofit

namespace sm_buildinglod {
    inline util::memory_reference<IDirect3DVertexShader9*> vertex_program               { 0x01114C10 };
    inline util::memory_reference<IDirect3DPixelShader9*>  adaptive_alpha_pixel_program { 0x01114C1C };
    inline util::memory_reference<IDirect3DPixelShader9*>  opaque_pixel_program         { 0x01114C20 };
} // sm_buildinglod

namespace sm_roadlod {
    inline util::memory_reference<IDirect3DVertexShader9*> vertex_program                    { 0x01114BD4 };
    inline util::memory_reference<IDirect3DPixelShader9*>  surface_pixel_program             { 0x01114BD0 };
    inline util::memory_reference<IDirect3DPixelShader9*>  sampler_passthrough_pixel_program { 0x01114BD8 };
} // sm_roadlod

namespace sm_road {
    struct program_exports {
        IDirect3DPixelShader9*  sample_texture_pixel_program;
        IDirect3DPixelShader9*  unshadowed_pixel_program;
        IDirect3DPixelShader9*  sample_auxiliary_alpha_pixel_program;
        IDirect3DVertexShader9* vertex_program;
        IDirect3DPixelShader9*  shadow_2_pixel_program;
        IDirect3DPixelShader9*  shadow_1_pixel_program;
    };

    static_assert(sizeof(program_exports) == 0x18, ASSERT_FAIL_SANITY);

    inline util::memory_reference<program_exports> programs { 0x01114870 };
} // sm_road

namespace sm_phatpalettecharnormal {
    struct program_exports {
        IDirect3DPixelShader9*  shadow_2_pixel_program;
        IDirect3DPixelShader9*  shadow_1_pixel_program;
        IDirect3DPixelShader9*  sample_auxiliary_alpha_pixel_program;
        IDirect3DPixelShader9*  sample_texture_pixel_program;
        IDirect3DVertexShader9* material_vertex_program;
        IDirect3DPixelShader9*  unshadowed_pixel_program;
        IDirect3DVertexShader9* depth_shadow_vertex_program;
    };

    static_assert(sizeof(program_exports) == 0x1C, ASSERT_FAIL_SANITY);

    inline util::memory_reference<program_exports> programs { 0x01114820 };
} // sm_phatpalettecharnormal

namespace sm_phatpalettechar {
    struct program_exports {
        IDirect3DPixelShader9*  shadow_1_pixel_program;
        IDirect3DPixelShader9*  shadow_2_pixel_program;
        IDirect3DPixelShader9*  sample_texture_pixel_program;
        IDirect3DVertexShader9* material_vertex_program;
        IDirect3DPixelShader9*  unshadowed_pixel_program;
        IDirect3DPixelShader9*  sample_auxiliary_alpha_pixel_program;
        IDirect3DVertexShader9* depth_shadow_vertex_program;
    };

    static_assert(sizeof(program_exports) == 0x1C, ASSERT_FAIL_SANITY);

    inline util::memory_reference<program_exports> programs { 0x011147D0 };
} // sm_phatpalettechar

namespace sm_phat_palette {
    struct program_exports {
        IDirect3DPixelShader9*  shadow_1_pixel_program;
        u32                     reserved_004;
        IDirect3DPixelShader9*  sample_texture_pixel_program;
        IDirect3DPixelShader9*  shadow_2_pixel_program;
        IDirect3DVertexShader9* material_vertex_program;
        IDirect3DPixelShader9*  unshadowed_pixel_program;
        IDirect3DVertexShader9* depth_shadow_vertex_program;
    };

    static_assert(sizeof(program_exports) == 0x1C, ASSERT_FAIL_SANITY);

    static_assert(offsetof(program_exports, sample_texture_pixel_program) == 0x08, ASSERT_FAIL_SANITY);
    static_assert(offsetof(program_exports, depth_shadow_vertex_program)  == 0x18, ASSERT_FAIL_SANITY);

    inline util::memory_reference<program_exports> programs { 0x011145F0 };
} // sm_phat_palette

namespace sm_phatcharnormal {
    struct program_exports {
        IDirect3DVertexShader9* depth_shadow_vertex_program;
        IDirect3DPixelShader9*  unshadowed_pixel_program;
        IDirect3DVertexShader9* material_vertex_program;
        IDirect3DPixelShader9*  sample_green_pixel_program;
        IDirect3DPixelShader9*  shadow_1_pixel_program;
        IDirect3DPixelShader9*  sample_texture_pixel_program;
        IDirect3DPixelShader9*  shadow_2_pixel_program;
    };

    static_assert(sizeof(program_exports) == 0x1C, ASSERT_FAIL_SANITY);

    inline util::memory_reference<program_exports> programs { 0x011145A0 };
} // sm_phatcharnormal

namespace sm_phatchar {
    struct program_exports {
        IDirect3DPixelShader9*  unshadowed_pixel_program;
        IDirect3DPixelShader9*  sample_green_pixel_program;
        IDirect3DPixelShader9*  shadow_2_pixel_program;
        IDirect3DPixelShader9*  shadow_1_pixel_program;
        IDirect3DVertexShader9* depth_shadow_vertex_program;
        IDirect3DPixelShader9*  sample_texture_pixel_program;
        IDirect3DVertexShader9* material_vertex_program;
    };

    static_assert(sizeof(program_exports) == 0x1C, ASSERT_FAIL_SANITY);

    inline util::memory_reference<program_exports> programs { 0x01114550 };
} // sm_phatchar

namespace sm_phatspiderman {
    struct program_exports {
        IDirect3DPixelShader9*  sample_green_pixel_program;
        IDirect3DPixelShader9*  shadow_2_pixel_program;
        IDirect3DPixelShader9*  sample_texture_pixel_program;
        IDirect3DVertexShader9* material_vertex_program;
        IDirect3DVertexShader9* depth_shadow_vertex_program;
        IDirect3DPixelShader9*  unshadowed_pixel_program;
        IDirect3DPixelShader9*  shadow_1_pixel_program;
    };

    static_assert(sizeof(program_exports) == 0x1C, ASSERT_FAIL_SANITY);

    inline util::memory_reference<program_exports> programs { 0x01114500 };
} // sm_phatspiderman

namespace sm_bright_filter {
    struct program_exports {
        IDirect3DPixelShader9*  scale_color;
        u32                     reserved_004;
        IDirect3DPixelShader9*  downsample_bloom_20;
        IDirect3DPixelShader9*  warped_gaussian_blur_x;
        IDirect3DPixelShader9*  radial_overlay;
        IDirect3DPixelShader9*  damage_luminance;
        IDirect3DPixelShader9*  sample_texture_partial_precision;
        u32                     reserved_01c;
        IDirect3DVertexShader9* fullscreen_position_3d_vertex;
        IDirect3DPixelShader9*  sample_texture_0;
        IDirect3DPixelShader9*  gamma_correction;
        u32                     reserved_02c;
        IDirect3DPixelShader9*  gaussian_blur_13_0;
        IDirect3DPixelShader9*  line_blur_16;
        IDirect3DPixelShader9*  gaussian_vertical_offset;
        u32                     reserved_03c;
        IDirect3DPixelShader9*  high_pass_luminance;
        u32                     reserved_044;
        IDirect3DPixelShader9*  downsample_20;
        IDirect3DPixelShader9*  box_blur_4_0;
        IDirect3DPixelShader9*  downsample_bloom_4;
        IDirect3DPixelShader9*  god_rays;
        u32                     reserved_058;
        IDirect3DPixelShader9*  downsample_rgb_16;
        IDirect3DPixelShader9*  tone_map_desaturated;
        IDirect3DPixelShader9*  tone_map_luminance;
        IDirect3DPixelShader9*  sample_rgb_half_alpha_0;
        u32                     reserved_06c;
        IDirect3DPixelShader9*  sample_texture_1;
        IDirect3DVertexShader9* downsample_offset_vertex;
        u32                     reserved_078;
        IDirect3DPixelShader9*  exposure_target_vignette;
        IDirect3DVertexShader9* texcoord_offset_vertex;
        u32                     reserved_084;
        IDirect3DVertexShader9* ray_adjust_vertex;
        u32                     reserved_08c;
        IDirect3DPixelShader9*  sample_texture_2;
        u32                     reserved_094;
        u32                     reserved_098;
        IDirect3DPixelShader9*  premultiply_alpha;
        IDirect3DPixelShader9*  warped_gaussian_blur_y;
        u32                     reserved_0a4;
        IDirect3DPixelShader9*  box_blur_4_1;
        u32                     reserved_0ac;
        IDirect3DPixelShader9*  depth_gate;
        u32                     reserved_0b4;
        IDirect3DPixelShader9*  line_blur_5;
        IDirect3DVertexShader9* zoom_vertex;
        IDirect3DPixelShader9*  sample_rgb_half_alpha_1;
        IDirect3DPixelShader9*  box_blur_4_2;
        u32                     reserved_0c8;
        IDirect3DPixelShader9*  sample_texture_3;
        u32                     reserved_0d0;
        IDirect3DPixelShader9*  gaussian_blur_3;
        IDirect3DPixelShader9*  bloom_depth_gate;
        IDirect3DPixelShader9*  downsample_alpha_20;
        u32                     reserved_0e0;
        IDirect3DPixelShader9*  gaussian_blur_13_1;
        IDirect3DVertexShader9* fullscreen_position_2d_vertex;
        u32                     reserved_0ec;
        u32                     reserved_0f0;
        u32                     reserved_0f4;
        IDirect3DPixelShader9*  box_blur_rgb_preserve_alpha;
        IDirect3DPixelShader9*  gaussian_vertical_far;
        IDirect3DPixelShader9*  gaussian_vertical_near;
        IDirect3DPixelShader9*  spidey_sense_bloom;
        u32                     reserved_108;
        IDirect3DPixelShader9*  downsample_alpha_4;
        u32                     reserved_110;
        IDirect3DPixelShader9*  depth_mask;
    };

    static_assert(sizeof(program_exports) == 0x118, ASSERT_FAIL_SANITY);

    inline util::memory_reference<program_exports> programs { 0x01114640 };
} // sm_bright_filter

namespace sm_decalchar {
    inline util::memory_reference<IDirect3DVertexShader9*> material_vertex_program     { 0x01114B98 };
    inline util::memory_reference<IDirect3DVertexShader9*> depth_shadow_vertex_program { 0x01114B94 };
    inline util::memory_reference<IDirect3DPixelShader9*>  pixel_program               { 0x01114B90 };
} // sm_decalchar

namespace sm_bush {
    inline util::memory_reference<IDirect3DPixelShader9*>  material_pixel_program                 { 0x01114A20 };
    inline util::memory_reference<IDirect3DPixelShader9*>  constant_red_pixel_program             { 0x01114A24 };
    inline util::memory_reference<IDirect3DPixelShader9*>  shadowed_material_pixel_program        { 0x01114A28 };
    inline util::memory_reference<IDirect3DPixelShader9*>  sample_texture_pixel_program           { 0x01114A2C };
    inline util::memory_reference<IDirect3DPixelShader9*>  sample_auxiliary_texture_pixel_program { 0x01114A30 };
    inline util::memory_reference<IDirect3DVertexShader9*> material_vertex_program                { 0x01114A34 };
} // sm_bush

namespace sm_babyphatnormal {
    inline util::memory_reference<IDirect3DVertexShader9*> material_vertex_program     { 0x01114B50 };
    inline util::memory_reference<IDirect3DPixelShader9*>  unshadowed_pixel_program    { 0x01114B4C };
    inline util::memory_reference<IDirect3DPixelShader9*>  shadow_1_pixel_program      { 0x01114B48 };
    inline util::memory_reference<IDirect3DPixelShader9*>  shadow_2_pixel_program      { 0x01114B44 };
    inline util::memory_reference<IDirect3DVertexShader9*> depth_shadow_vertex_program { 0x01114B40 };
} // sm_babyphatnormal

namespace sm_babyphat {
    inline util::memory_reference<IDirect3DVertexShader9*> material_vertex_program     { 0x01114AF0 };
    inline util::memory_reference<IDirect3DPixelShader9*>  unshadowed_pixel_program    { 0x01114AF4 };
    inline util::memory_reference<IDirect3DVertexShader9*> depth_shadow_vertex_program { 0x01114AF8 };
    inline util::memory_reference<IDirect3DPixelShader9*>  shadow_2_pixel_program      { 0x01114AFC };
    inline util::memory_reference<IDirect3DPixelShader9*>  shadow_1_pixel_program      { 0x01114B00 };
} // sm_babyphat

namespace sm_decal {
    struct program_exports {
        IDirect3DPixelShader9*  material_horizon;
        IDirect3DPixelShader9*  material_gobo_environment;
        IDirect3DPixelShader9*  material_environment;
        IDirect3DPixelShader9*  facing;
        IDirect3DPixelShader9*  sample_texture_0;
        IDirect3DPixelShader9*  material_horizon_gobo_environment;
        IDirect3DPixelShader9*  ambient_alpha;
        IDirect3DPixelShader9*  sample_texture_1;
        IDirect3DPixelShader9*  material_horizon_gobo;
        IDirect3DPixelShader9*  material_gobo;
        IDirect3DVertexShader9* vertex;
        IDirect3DPixelShader9*  material_horizon_environment;
        IDirect3DPixelShader9*  material;
    };

    static_assert(sizeof(program_exports) == 0x34, ASSERT_FAIL_SANITY);

    inline util::memory_reference<program_exports> programs { 0x011149B0 };
} // sm_decal

namespace sm_phat_palette_normal {
    inline util::memory_reference<IDirect3DPixelShader9*>  shadow_1_pixel_program            { 0x01114960 };
    inline util::memory_reference<IDirect3DPixelShader9*>  unshadowed_pixel_program          { 0x01114964 };
    inline util::memory_reference<IDirect3DVertexShader9*> depth_shadow_vertex_program       { 0x0111496C };
    inline util::memory_reference<IDirect3DPixelShader9*>  shadow_2_pixel_program            { 0x01114970 };
    inline util::memory_reference<IDirect3DPixelShader9*>  sampler_passthrough_pixel_program { 0x01114974 };
    inline util::memory_reference<IDirect3DVertexShader9*> material_vertex_program           { 0x01114978 };
} // sm_phat_palette_normal

namespace sm_phatnormal {
    inline util::memory_reference<IDirect3DPixelShader9*>  unshadowed_pixel_program    { 0x01114910 };
    inline util::memory_reference<IDirect3DVertexShader9*> material_vertex_program     { 0x01114914 };
    inline util::memory_reference<IDirect3DPixelShader9*>  shadow_1_pixel_program      { 0x01114918 };
    inline util::memory_reference<IDirect3DPixelShader9*>  shadow_2_pixel_program      { 0x0111491C };
    inline util::memory_reference<IDirect3DVertexShader9*> depth_shadow_vertex_program { 0x01114920 };
} // sm_phatnormal

namespace sm_phat {
    struct program_exports {
        IDirect3DPixelShader9*  special_shadow_1;
        IDirect3DVertexShader9* material_vertex;
        IDirect3DVertexShader9* utility_vertex;
        IDirect3DPixelShader9*  debug_normal;
        IDirect3DPixelShader9*  debug_shadow_0;
        IDirect3DPixelShader9*  debug_shadow_1;
        IDirect3DPixelShader9*  debug_environment;
        IDirect3DPixelShader9*  debug_texture_coordinates_1;
        IDirect3DPixelShader9*  debug_texture_coordinates_0;
        IDirect3DPixelShader9*  debug_facing;
        IDirect3DPixelShader9*  special_shadow_2;
        IDirect3DPixelShader9*  special_unshadowed;
        IDirect3DPixelShader9*  debug_gobo;
    };

    struct pixel_pipeline_descriptor {
        u32                 unk_00;
        IDirect3DPixelShader9**  pixel_program_output;
        std::array<u32, 33> unk_08;
    };

    static_assert(sizeof(program_exports)           == 0x34, ASSERT_FAIL_SANITY);
    static_assert(sizeof(pixel_pipeline_descriptor) == 0x8C, ASSERT_FAIL_SANITY);

    inline util::memory_reference<program_exports> programs { 0x01114490 };

    inline util::memory_reference // sob
        <std::array<std::array<pixel_pipeline_descriptor, 21>, 17>> pixel_pipelines { 0x00F3E600 };
} // sm_phat
}}}} // treyarch::ngl::shaders::program_exports
