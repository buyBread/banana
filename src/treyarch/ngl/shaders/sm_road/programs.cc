#include <array>
#include <cstddef>

#include "treyarch/ngl/shaders/program_exports.hh"
#include "treyarch/ngl/d3d9/shader_program.hh"
#include "treyarch/ngl/shaders/sm_roadlod/programs.hh"

#include "treyarch/ngl/shaders/sm_road/programs.hh"

using namespace treyarch;
using namespace ngl::shaders;
using namespace ngl::shaders::sm_road;

static constexpr size_t material_pixel_variant_count = 3;

static ngl::d3d9::vertex_program vertex_program;

static std::array<ngl::d3d9::pixel_program, material_pixel_variant_count> material_pixel_programs;

static ngl::d3d9::pixel_program sample_auxiliary_alpha_pixel_program;

bool ngl::shaders::sm_road::initialize() {
    if (!vertex_program.create({ e_shader_program::sm_road_vertex }))
        return false;

    for (size_t index = 0; index < material_pixel_programs.size(); ++index) {
        if (!material_pixel_programs[index].create({ e_shader_program::sm_road_material_pixel, (u16)index }))
            
            return false;
    }

    if (!sample_auxiliary_alpha_pixel_program.create({ e_shader_program::sm_road_sample_auxiliary_alpha_pixel }))
        return false;

    IDirect3DPixelShader9* sample_texture = sm_roadlod::get_pixel_program(sm_roadlod::e_pixel_variant::sampler_passthrough);

    if (!sample_texture)
        return false;

    program_exports::sm_road::programs.get() = {
        sample_texture,
        material_pixel_programs[(size_t)e_pixel_variant::unshadowed].get(),
        sample_auxiliary_alpha_pixel_program.get(),
        vertex_program.get(),
        material_pixel_programs[(size_t)e_pixel_variant::shadow_2].get(),
        material_pixel_programs[(size_t)e_pixel_variant::shadow_1].get(),
    };

    return true;
}

IDirect3DVertexShader9* ngl::shaders::sm_road::get_vertex_program() {
    return vertex_program.get();
}

IDirect3DPixelShader9* ngl::shaders::sm_road::get_pixel_program(e_pixel_variant variant) {
    size_t index = (size_t)variant;

    if (index < material_pixel_programs.size())
        return material_pixel_programs[index].get();

    switch (variant) {
        case e_pixel_variant::sample_texture:
            return sm_roadlod::get_pixel_program(
                sm_roadlod::e_pixel_variant::sampler_passthrough);
        case e_pixel_variant::sample_auxiliary_alpha:
            return sample_auxiliary_alpha_pixel_program.get();
        default:
            return nullptr;
    }
}
