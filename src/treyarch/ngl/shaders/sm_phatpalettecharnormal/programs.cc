#include <array>
#include <cstddef>

#include "treyarch/ngl/shaders/program_exports.hh"
#include "treyarch/ngl/d3d9/shader_program.hh"
#include "treyarch/ngl/shaders/sm_phat_palette_normal/programs.hh"

#include "treyarch/ngl/shaders/sm_phatpalettecharnormal/programs.hh"

using namespace treyarch;
using namespace ngl::shaders;
using namespace ngl::shaders::sm_phatpalettecharnormal;
static constexpr size_t material_pixel_variant_count = 3;

static ngl::d3d9::vertex_program material_vertex_program;
static ngl::d3d9::vertex_program depth_shadow_vertex_program;

static std::array<ngl::d3d9::pixel_program, material_pixel_variant_count> material_pixel_programs;

static ngl::d3d9::pixel_program sample_auxiliary_alpha_pixel_program;

bool ngl::shaders::sm_phatpalettecharnormal::initialize() {
    if (!material_vertex_program
            .create({ e_shader_program::sm_phatpalettecharnormal_material_vertex }))
            
        return false;

    for (size_t index = 0; index < material_pixel_programs.size(); ++index) {
        if (!material_pixel_programs[index].create({
            e_shader_program::sm_phatpalettecharnormal_material_pixel,
            (u16)index }))
            return false;
    }

    if (!sample_auxiliary_alpha_pixel_program.create({
        e_shader_program::sm_phatpalettecharnormal_sample_auxiliary_alpha_pixel }))
        return false;

    if (!depth_shadow_vertex_program.create({
        e_shader_program::sm_phatpalettecharnormal_depth_shadow_vertex }))
        return false;

    IDirect3DPixelShader9* sample_texture =
        sm_phat_palette_normal::get_pixel_program(
            sm_phat_palette_normal::e_pixel_variant::sampler_passthrough);

    if (!sample_texture)
        return false;

    program_exports::sm_phatpalettecharnormal::programs.get() = {
        material_pixel_programs[(size_t)e_pixel_variant::shadow_2].get(),
        material_pixel_programs[(size_t)e_pixel_variant::shadow_1].get(),
        sample_auxiliary_alpha_pixel_program.get(),
        sample_texture,
        material_vertex_program.get(),
        material_pixel_programs[(size_t)e_pixel_variant::unshadowed].get(),
        depth_shadow_vertex_program.get(),
    };

    return true;
}

IDirect3DVertexShader9* ngl::shaders::sm_phatpalettecharnormal::get_vertex_program(e_vertex_variant variant) {
    switch (variant) {
        case e_vertex_variant::material:
            return material_vertex_program.get();
        case e_vertex_variant::depth_shadow:
            return depth_shadow_vertex_program.get();
    }

    return nullptr;
}

IDirect3DPixelShader9* ngl::shaders::sm_phatpalettecharnormal::get_pixel_program(e_pixel_variant variant) {
    size_t index = (size_t)variant;

    if (index < material_pixel_programs.size())
        return material_pixel_programs[index].get();

    switch (variant) {
        case e_pixel_variant::sample_texture:
            return sm_phat_palette_normal::get_pixel_program(
                sm_phat_palette_normal::e_pixel_variant::sampler_passthrough);
        case e_pixel_variant::sample_auxiliary_alpha:
            return sample_auxiliary_alpha_pixel_program.get();
        default:
            return nullptr;
    }
}
