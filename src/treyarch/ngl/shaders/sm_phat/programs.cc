#include <array>
#include <cstddef>

#include "treyarch/ngl/shaders/program_exports.hh"
#include "treyarch/ngl/d3d9/shader_program.hh"
#include "treyarch/ngl/shaders/sm_babyphat/programs.hh"
#include "configuration.hh"

#include "treyarch/ngl/shaders/sm_phat/programs.hh"

using namespace treyarch;
using namespace ngl::shaders;
using namespace ngl::shaders::sm_phat;

using material_program_set = std::array<ngl::d3d9::pixel_program, lighting_configuration_count>;

static ngl::d3d9::vertex_program material_vertex_program;

static std::array<material_program_set, material_configuration_count> material_pixel_programs;
static std::array<ngl::d3d9::pixel_program, debug_variant_count> debug_pixel_programs;

static bool create_material_programs() {
    auto &pipelines = program_exports::sm_phat::pixel_pipelines.get();

    for (size_t material_index = 0; material_index < material_configurations.size(); ++material_index) {
        for (size_t lighting_index = 0; lighting_index < lighting_configurations.size(); ++lighting_index) {
            ngl::d3d9::pixel_program &program = material_pixel_programs[material_index][lighting_index];

            shader_key key {
                e_shader_program::sm_phat_material_pixel,
                material_permutation(material_index, lighting_index),
            };

            if (!program.create(key))
                return false;

            IDirect3DPixelShader9** output = pipelines[material_index][lighting_index].pixel_program_output;

            if (!output)
                return false;

            *output = program.get();
        }
    }

    return true;
}

static bool create_debug_programs() {
    for (size_t index = 0; index < debug_pixel_programs.size(); ++index) {
        if (!debug_pixel_programs[index].create({ e_shader_program::sm_phat_debug_pixel, (u16)index }))
            return false;
    }

    return true;
}

bool ngl::shaders::sm_phat::initialize() {
    if (!material_vertex_program.create({ e_shader_program::sm_phat_material_vertex }))
        return false;

    if (!create_debug_programs())
        return false;

    if (!create_material_programs())
        return false;

    program_exports::sm_phat::program_exports &programs = program_exports::sm_phat::programs.get();

    IDirect3DVertexShader9* utility_vertex     = sm_babyphat::get_vertex_program
        (sm_babyphat::e_vertex_variant::depth_shadow);
    IDirect3DPixelShader9*  special_unshadowed = sm_babyphat::get_pixel_program
        (sm_babyphat::e_pixel_variant::unshadowed);
    IDirect3DPixelShader9*  special_shadow_1   = sm_babyphat::get_pixel_program
        (sm_babyphat::e_pixel_variant::shadow_1);
    IDirect3DPixelShader9*  special_shadow_2   = sm_babyphat::get_pixel_program
        (sm_babyphat::e_pixel_variant::shadow_2);

    if (!utility_vertex || !special_unshadowed || !special_shadow_1 || !special_shadow_2)
        return false;

    programs.material_vertex = material_vertex_program.get();
    programs.utility_vertex  = utility_vertex;

    programs.debug_texture_coordinates_0 = debug_pixel_programs[0].get();
    programs.debug_facing                = debug_pixel_programs[1].get();
    programs.debug_texture_coordinates_1 = debug_pixel_programs[2].get();
    programs.debug_gobo                  = debug_pixel_programs[3].get();
    programs.debug_normal                = debug_pixel_programs[4].get();
    programs.debug_environment           = debug_pixel_programs[5].get();
    programs.debug_shadow_0              = debug_pixel_programs[6].get();
    programs.debug_shadow_1              = debug_pixel_programs[7].get();

    programs.special_unshadowed = special_unshadowed;
    programs.special_shadow_1   = special_shadow_1;
    programs.special_shadow_2   = special_shadow_2;

    return true;
}

IDirect3DVertexShader9* ngl::shaders::sm_phat::get_vertex_program(e_vertex_variant variant) {
    switch (variant) {
        case e_vertex_variant::material:
            return material_vertex_program.get();
        case e_vertex_variant::utility:
            return sm_babyphat::get_vertex_program(
                sm_babyphat::e_vertex_variant::depth_shadow);
    }

    return nullptr;
}

IDirect3DPixelShader9* ngl::shaders::sm_phat::get_pixel_program(const material_configuration &material, const lighting_configuration &lighting) {
    size_t material_index = material_configurations.size();
    size_t lighting_index = lighting_configurations.size();

    for (size_t index = 0; index < material_configurations.size(); ++index) {
        if (material_configurations[index] == material) {
            material_index = index;
            
            break;
        }
    }

    for (size_t index = 0; index < lighting_configurations.size(); ++index) {
        if (lighting_configurations[index] == lighting) {
            lighting_index = index;
            
            break;
        }
    }

    if (material_index == material_configurations.size() || lighting_index == lighting_configurations.size())
        return nullptr;

    return material_pixel_programs[material_index][lighting_index].get();
}

IDirect3DPixelShader9* ngl::shaders::sm_phat::get_debug_program(e_debug_variant variant) {
    size_t index = (size_t)variant;

    if (index >= debug_pixel_programs.size())
        return nullptr;

    return debug_pixel_programs[index].get();
}
