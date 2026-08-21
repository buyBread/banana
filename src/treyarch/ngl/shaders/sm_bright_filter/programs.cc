#include <array>
#include <cstddef>

#include "treyarch/ngl/shaders/program_exports.hh"
#include "treyarch/ngl/d3d9/shader_program.hh"

#include "treyarch/ngl/shaders/sm_bright_filter/programs.hh"

using namespace treyarch;
using namespace ngl::shaders;
using namespace ngl::shaders::sm_bright_filter;

static std::array<ngl::d3d9::vertex_program, vertex_variant_count> vertex_programs;
static std::array<ngl::d3d9::pixel_program,  pixel_variant_count>  pixel_programs;

bool ngl::shaders::sm_bright_filter::initialize() {
    for (size_t index = 0; index < vertex_programs.size(); ++index) {
        if (!vertex_programs[index].create({ e_shader_program::sm_bright_filter_vertex, (u16)index }))
            return false;
    }

    for (size_t index = 0; index < pixel_programs.size(); ++index) {
        if (!pixel_programs[index].create({ e_shader_program::sm_bright_filter_pixel, (u16)index }))
            return false;
    }

    auto &programs = program_exports::sm_bright_filter::programs.get();

    programs.scale_color                      = get_pixel_program (e_pixel_variant ::scale_color);
    programs.downsample_bloom_20              = get_pixel_program (e_pixel_variant ::downsample_bloom_20);
    programs.warped_gaussian_blur_x           = get_pixel_program (e_pixel_variant ::warped_gaussian_blur_x);
    programs.radial_overlay                   = get_pixel_program (e_pixel_variant ::radial_overlay);
    programs.damage_luminance                 = get_pixel_program (e_pixel_variant ::damage_luminance);
    programs.sample_texture_partial_precision = get_pixel_program (e_pixel_variant ::sample_texture_partial_precision);
    programs.fullscreen_position_3d_vertex    = get_vertex_program(e_vertex_variant::fullscreen_position_3d);
    programs.sample_texture_0                 = get_pixel_program (e_pixel_variant ::sample_texture);
    programs.gamma_correction                 = get_pixel_program (e_pixel_variant ::gamma_correction);
    programs.gaussian_blur_13_0               = get_pixel_program (e_pixel_variant ::gaussian_blur_13);
    programs.line_blur_16                     = get_pixel_program (e_pixel_variant ::line_blur_16);
    programs.gaussian_vertical_offset         = get_pixel_program (e_pixel_variant ::gaussian_vertical_offset);
    programs.high_pass_luminance              = get_pixel_program (e_pixel_variant ::high_pass_luminance);
    programs.downsample_20                    = get_pixel_program (e_pixel_variant ::downsample_20);
    programs.box_blur_4_0                     = get_pixel_program (e_pixel_variant ::box_blur_4);
    programs.downsample_bloom_4               = get_pixel_program (e_pixel_variant ::downsample_bloom_4);
    programs.god_rays                         = get_pixel_program (e_pixel_variant ::god_rays);
    programs.downsample_rgb_16                = get_pixel_program (e_pixel_variant ::downsample_rgb_16);
    programs.tone_map_desaturated             = get_pixel_program (e_pixel_variant ::tone_map_desaturated);
    programs.tone_map_luminance               = get_pixel_program (e_pixel_variant ::tone_map_luminance);
    programs.sample_rgb_half_alpha_0          = get_pixel_program (e_pixel_variant ::sample_rgb_half_alpha);
    programs.sample_texture_1                 = get_pixel_program (e_pixel_variant ::sample_texture);
    programs.downsample_offset_vertex         = get_vertex_program(e_vertex_variant::downsample_offset);
    programs.exposure_target_vignette         = get_pixel_program (e_pixel_variant ::exposure_target_vignette);
    programs.texcoord_offset_vertex           = get_vertex_program(e_vertex_variant::texcoord_offset);
    programs.ray_adjust_vertex                = get_vertex_program(e_vertex_variant::ray_adjust);
    programs.sample_texture_2                 = get_pixel_program (e_pixel_variant ::sample_texture);
    programs.premultiply_alpha                = get_pixel_program (e_pixel_variant ::premultiply_alpha);
    programs.warped_gaussian_blur_y           = get_pixel_program (e_pixel_variant ::warped_gaussian_blur_y);
    programs.box_blur_4_1                     = get_pixel_program (e_pixel_variant ::box_blur_4);
    programs.depth_gate                       = get_pixel_program (e_pixel_variant ::depth_gate);
    programs.line_blur_5                      = get_pixel_program (e_pixel_variant ::line_blur_5);
    programs.zoom_vertex                      = get_vertex_program(e_vertex_variant::zoom);
    programs.sample_rgb_half_alpha_1          = get_pixel_program (e_pixel_variant ::sample_rgb_half_alpha);
    programs.box_blur_4_2                     = get_pixel_program (e_pixel_variant ::box_blur_4);
    programs.sample_texture_3                 = get_pixel_program (e_pixel_variant ::sample_texture);
    programs.gaussian_blur_3                  = get_pixel_program (e_pixel_variant ::gaussian_blur_3);
    programs.bloom_depth_gate                 = get_pixel_program (e_pixel_variant ::bloom_depth_gate);
    programs.downsample_alpha_20              = get_pixel_program (e_pixel_variant ::downsample_alpha_20);
    programs.gaussian_blur_13_1               = get_pixel_program (e_pixel_variant ::gaussian_blur_13);
    programs.fullscreen_position_2d_vertex    = get_vertex_program(e_vertex_variant::fullscreen_position_2d);
    programs.box_blur_rgb_preserve_alpha      = get_pixel_program (e_pixel_variant ::box_blur_rgb_preserve_alpha);
    programs.gaussian_vertical_far            = get_pixel_program (e_pixel_variant ::gaussian_vertical_far);
    programs.gaussian_vertical_near           = get_pixel_program (e_pixel_variant ::gaussian_vertical_near);
    programs.spidey_sense_bloom               = get_pixel_program (e_pixel_variant ::spidey_sense_bloom);
    programs.downsample_alpha_4               = get_pixel_program (e_pixel_variant ::downsample_alpha_4);
    programs.depth_mask                       = get_pixel_program (e_pixel_variant ::depth_mask);

    return true;
}

IDirect3DVertexShader9* ngl::shaders::sm_bright_filter::get_vertex_program(e_vertex_variant variant) {
    size_t index = (size_t)variant;

    return index < vertex_programs.size() ? vertex_programs[index].get() : nullptr;
}

IDirect3DPixelShader9* ngl::shaders::sm_bright_filter::get_pixel_program(e_pixel_variant variant) {
    size_t index = (size_t)variant;

    return index < pixel_programs.size() ? pixel_programs[index].get() : nullptr;
}
