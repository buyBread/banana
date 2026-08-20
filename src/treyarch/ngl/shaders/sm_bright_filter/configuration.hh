#pragma once

#include <array>

namespace treyarch { namespace ngl { namespace shaders { namespace sm_bright_filter {
    struct shader_configuration {
        const char* source_path;
        const char* source_name;
        const char* definition;
    };

    enum class e_vertex_variant {
        fullscreen_position_3d,
        texcoord_offset,
        fullscreen_position_2d,
        ray_adjust,
        downsample_offset,
        zoom,
        _count,
    };

    enum class e_pixel_variant {
        high_pass_luminance,
        premultiply_alpha,
        gaussian_blur_13,
        sample_texture,
        tone_map_luminance,
        warped_gaussian_blur_x,
        warped_gaussian_blur_y,
        sample_rgb_half_alpha,
        tone_map_desaturated,
        damage_luminance,
        exposure_target_vignette,
        radial_overlay,
        gaussian_vertical_near,
        gaussian_vertical_offset,
        gaussian_vertical_far,
        line_blur_16,
        god_rays,
        depth_gate,
        bloom_depth_gate,
        depth_mask,
        gaussian_blur_3,
        downsample_20,
        downsample_bloom_20,
        downsample_alpha_20,
        box_blur_4,
        box_blur_rgb_preserve_alpha,
        downsample_rgb_16,
        scale_color,
        spidey_sense_bloom,
        sample_texture_partial_precision,
        line_blur_5,
        gamma_correction,
        downsample_bloom_4,
        downsample_alpha_4,
        _count,
    };

    inline constexpr std::array
        <shader_configuration, (size_t)e_vertex_variant::_count>
    vertex_configurations {{
        { "sm_bright_filter/sm_bright_filter.vs.hlsl", "sm_bright_filter.fullscreen_position_3d.vs.hlsl", "NGL_SM_BRIGHT_FULLSCREEN_POSITION_3D" },
        { "sm_bright_filter/sm_bright_filter.vs.hlsl", "sm_bright_filter.texcoord_offset.vs.hlsl",        "NGL_SM_BRIGHT_TEXCOORD_OFFSET" },
        { "sm_bright_filter/sm_bright_filter.vs.hlsl", "sm_bright_filter.fullscreen_position_2d.vs.hlsl", "NGL_SM_BRIGHT_FULLSCREEN_POSITION_2D" },
        { "sm_bright_filter/sm_bright_filter.vs.hlsl", "sm_bright_filter.ray_adjust.vs.hlsl",             "NGL_SM_BRIGHT_RAY_ADJUST" },
        { "sm_bright_filter/sm_bright_filter.vs.hlsl", "sm_bright_filter.downsample_offset.vs.hlsl",      "NGL_SM_BRIGHT_DOWNSAMPLE_OFFSET" },
        { "sm_bright_filter/sm_bright_filter.vs.hlsl", "sm_bright_filter.zoom.vs.hlsl",                   "NGL_SM_BRIGHT_ZOOM" },
    }};

    inline constexpr std::array
        <shader_configuration, (size_t)e_pixel_variant::_count>
    pixel_configurations {{
        { "sm_bright_filter/sm_bright_filter.color.ps.hlsl",      "sm_bright_filter.high_pass_luminance.ps.hlsl",              "NGL_SM_BRIGHT_HIGH_PASS_LUMINANCE" },
        { "sm_bright_filter/sm_bright_filter.color.ps.hlsl",      "sm_bright_filter.premultiply_alpha.ps.hlsl",                "NGL_SM_BRIGHT_PREMULTIPLY_ALPHA" },
        { "sm_bright_filter/sm_bright_filter.blur.ps.hlsl",       "sm_bright_filter.gaussian_blur_13.ps.hlsl",                 "NGL_SM_BRIGHT_GAUSSIAN_BLUR_13" },
        { "sm_bright_filter/sm_bright_filter.color.ps.hlsl",      "sm_bright_filter.sample_texture.ps.hlsl",                   "NGL_SM_BRIGHT_SAMPLE_TEXTURE" },
        { "sm_bright_filter/sm_bright_filter.color.ps.hlsl",      "sm_bright_filter.tone_map_luminance.ps.hlsl",               "NGL_SM_BRIGHT_TONE_MAP_LUMINANCE" },
        { "sm_bright_filter/sm_bright_filter.blur.ps.hlsl",       "sm_bright_filter.warped_gaussian_blur_x.ps.hlsl",           "NGL_SM_BRIGHT_WARPED_GAUSSIAN_BLUR_X" },
        { "sm_bright_filter/sm_bright_filter.blur.ps.hlsl",       "sm_bright_filter.warped_gaussian_blur_y.ps.hlsl",           "NGL_SM_BRIGHT_WARPED_GAUSSIAN_BLUR_Y" },
        { "sm_bright_filter/sm_bright_filter.color.ps.hlsl",      "sm_bright_filter.sample_rgb_half_alpha.ps.hlsl",            "NGL_SM_BRIGHT_SAMPLE_RGB_HALF_ALPHA" },
        { "sm_bright_filter/sm_bright_filter.color.ps.hlsl",      "sm_bright_filter.tone_map_desaturated.ps.hlsl",             "NGL_SM_BRIGHT_TONE_MAP_DESATURATED" },
        { "sm_bright_filter/sm_bright_filter.effects.ps.hlsl",    "sm_bright_filter.damage_luminance.ps.hlsl",                 "NGL_SM_BRIGHT_DAMAGE_LUMINANCE" },
        { "sm_bright_filter/sm_bright_filter.effects.ps.hlsl",    "sm_bright_filter.exposure_target_vignette.ps.hlsl",         "NGL_SM_BRIGHT_EXPOSURE_TARGET_VIGNETTE" },
        { "sm_bright_filter/sm_bright_filter.effects.ps.hlsl",    "sm_bright_filter.radial_overlay.ps.hlsl",                   "NGL_SM_BRIGHT_RADIAL_OVERLAY" },
        { "sm_bright_filter/sm_bright_filter.blur.ps.hlsl",       "sm_bright_filter.gaussian_vertical_near.ps.hlsl",           "NGL_SM_BRIGHT_GAUSSIAN_VERTICAL_NEAR" },
        { "sm_bright_filter/sm_bright_filter.blur.ps.hlsl",       "sm_bright_filter.gaussian_vertical_offset.ps.hlsl",         "NGL_SM_BRIGHT_GAUSSIAN_VERTICAL_OFFSET" },
        { "sm_bright_filter/sm_bright_filter.blur.ps.hlsl",       "sm_bright_filter.gaussian_vertical_far.ps.hlsl",            "NGL_SM_BRIGHT_GAUSSIAN_VERTICAL_FAR" },
        { "sm_bright_filter/sm_bright_filter.downsample.ps.hlsl", "sm_bright_filter.line_blur_16.ps.hlsl",                     "NGL_SM_BRIGHT_LINE_BLUR_16" },
        { "sm_bright_filter/sm_bright_filter.effects.ps.hlsl",    "sm_bright_filter.god_rays.ps.hlsl",                         "NGL_SM_BRIGHT_GOD_RAYS" },
        { "sm_bright_filter/sm_bright_filter.effects.ps.hlsl",    "sm_bright_filter.depth_gate.ps.hlsl",                       "NGL_SM_BRIGHT_DEPTH_GATE" },
        { "sm_bright_filter/sm_bright_filter.effects.ps.hlsl",    "sm_bright_filter.bloom_depth_gate.ps.hlsl",                 "NGL_SM_BRIGHT_BLOOM_DEPTH_GATE" },
        { "sm_bright_filter/sm_bright_filter.effects.ps.hlsl",    "sm_bright_filter.depth_mask.ps.hlsl",                       "NGL_SM_BRIGHT_DEPTH_MASK" },
        { "sm_bright_filter/sm_bright_filter.blur.ps.hlsl",       "sm_bright_filter.gaussian_blur_3.ps.hlsl",                  "NGL_SM_BRIGHT_GAUSSIAN_BLUR_3" },
        { "sm_bright_filter/sm_bright_filter.downsample.ps.hlsl", "sm_bright_filter.downsample_20.ps.hlsl",                    "NGL_SM_BRIGHT_DOWNSAMPLE_20" },
        { "sm_bright_filter/sm_bright_filter.downsample.ps.hlsl", "sm_bright_filter.downsample_bloom_20.ps.hlsl",              "NGL_SM_BRIGHT_DOWNSAMPLE_BLOOM_20" },
        { "sm_bright_filter/sm_bright_filter.downsample.ps.hlsl", "sm_bright_filter.downsample_alpha_20.ps.hlsl",              "NGL_SM_BRIGHT_DOWNSAMPLE_ALPHA_20" },
        { "sm_bright_filter/sm_bright_filter.blur.ps.hlsl",       "sm_bright_filter.box_blur_4.ps.hlsl",                       "NGL_SM_BRIGHT_BOX_BLUR_4" },
        { "sm_bright_filter/sm_bright_filter.blur.ps.hlsl",       "sm_bright_filter.box_blur_rgb_preserve_alpha.ps.hlsl",      "NGL_SM_BRIGHT_BOX_BLUR_RGB_PRESERVE_ALPHA" },
        { "sm_bright_filter/sm_bright_filter.downsample.ps.hlsl", "sm_bright_filter.downsample_rgb_16.ps.hlsl",                "NGL_SM_BRIGHT_DOWNSAMPLE_RGB_16" },
        { "sm_bright_filter/sm_bright_filter.color.ps.hlsl",      "sm_bright_filter.scale_color.ps.hlsl",                      "NGL_SM_BRIGHT_SCALE_COLOR" },
        { "sm_bright_filter/sm_bright_filter.color.ps.hlsl",      "sm_bright_filter.spidey_sense_bloom.ps.hlsl",               "NGL_SM_BRIGHT_SPIDEY_SENSE_BLOOM" },
        { "sm_bright_filter/sm_bright_filter.color.ps.hlsl",      "sm_bright_filter.sample_texture_partial_precision.ps.hlsl", "NGL_SM_BRIGHT_SAMPLE_TEXTURE_PARTIAL_PRECISION" },
        { "sm_bright_filter/sm_bright_filter.downsample.ps.hlsl", "sm_bright_filter.line_blur_5.ps.hlsl",                      "NGL_SM_BRIGHT_LINE_BLUR_5" },
        { "sm_bright_filter/sm_bright_filter.color.ps.hlsl",      "sm_bright_filter.gamma_correction.ps.hlsl",                 "NGL_SM_BRIGHT_GAMMA_CORRECTION" },
        { "sm_bright_filter/sm_bright_filter.downsample.ps.hlsl", "sm_bright_filter.downsample_bloom_4.ps.hlsl",               "NGL_SM_BRIGHT_DOWNSAMPLE_BLOOM_4" },
        { "sm_bright_filter/sm_bright_filter.downsample.ps.hlsl", "sm_bright_filter.downsample_alpha_4.ps.hlsl",               "NGL_SM_BRIGHT_DOWNSAMPLE_ALPHA_4" },
    }};
}}}} // treyarch::ngl::shaders::sm_bright_filter
