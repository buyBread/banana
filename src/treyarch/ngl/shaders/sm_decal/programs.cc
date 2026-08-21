#include <array>
#include <cstddef>

#include "treyarch/ngl/shaders/program_exports.hh"
#include "treyarch/ngl/d3d9/shader_program.hh"
#include "treyarch/ngl/shaders/sm_bush/programs.hh"
#include "configuration.hh"

#include "treyarch/ngl/shaders/sm_decal/programs.hh"


using namespace treyarch;
using namespace ngl::shaders;
using namespace ngl::shaders::sm_decal;

static ngl::d3d9::vertex_program vertex_program;

static std::array<ngl::d3d9::pixel_program, material_configuration_count> material_programs;

static ngl::d3d9::pixel_program facing_program;
static ngl::d3d9::pixel_program ambient_alpha_program;

bool ngl::shaders::sm_decal::initialize() {
    if (!vertex_program.create({ e_shader_program::sm_decal_vertex }))
        return false;

    if (!facing_program.create({ e_shader_program::sm_decal_facing_pixel }))
        return false;

    for (size_t index = 0; index < material_programs.size(); ++index) {
        if (!material_programs[index].create({ e_shader_program::sm_decal_material_pixel, (u16)index }))
            return false;
    }

    if (!ambient_alpha_program.create({ e_shader_program::sm_decal_ambient_alpha_pixel }))
        return false;

    IDirect3DPixelShader9* sample_texture_0 = sm_bush::get_pixel_program(
        sm_bush::e_pixel_variant::sample_texture);
    IDirect3DPixelShader9* sample_texture_1 = sm_bush::get_pixel_program(
        sm_bush::e_pixel_variant::sample_auxiliary_texture);

    if (!sample_texture_0 || !sample_texture_1)
        return false;

    auto &programs = program_exports::sm_decal::programs.get();

    programs.vertex           = vertex_program.get();
    programs.facing           = facing_program.get();
    programs.sample_texture_0 = sample_texture_0;
    programs.sample_texture_1 = sample_texture_1;
    programs.ambient_alpha    = ambient_alpha_program.get();

    programs.material                          = material_programs[0].get();
    programs.material_environment              = material_programs[1].get();
    programs.material_horizon                  = material_programs[2].get();
    programs.material_horizon_environment      = material_programs[3].get();
    programs.material_gobo                     = material_programs[4].get();
    programs.material_gobo_environment         = material_programs[5].get();
    programs.material_horizon_gobo             = material_programs[6].get();
    programs.material_horizon_gobo_environment = material_programs[7].get();

    return true;
}

IDirect3DVertexShader9* ngl::shaders::sm_decal::get_vertex_program() {
    return vertex_program.get();
}

IDirect3DPixelShader9* ngl::shaders::sm_decal::get_material_program(
    const material_configuration &configuration)
{
    for (size_t index = 0; index < material_configurations.size(); ++index) {
        if (material_configurations[index] == configuration)
            return material_programs[index].get();
    }

    return nullptr;
}

IDirect3DPixelShader9* ngl::shaders::sm_decal::get_utility_program(e_utility_variant variant) {
    switch (variant) {
        case e_utility_variant::facing:
            return facing_program.get();
        case e_utility_variant::sample_texture_0:
            return sm_bush::get_pixel_program(
                sm_bush::e_pixel_variant::sample_texture);
        case e_utility_variant::sample_texture_1:
            return sm_bush::get_pixel_program(
                sm_bush::e_pixel_variant::sample_auxiliary_texture);
        case e_utility_variant::ambient_alpha:
            return ambient_alpha_program.get();
    }

    return nullptr;
}
