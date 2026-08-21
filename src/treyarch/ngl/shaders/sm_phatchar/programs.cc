#include <array>
#include <cstddef>

#include "treyarch/ngl/shaders/program_exports.hh"
#include "treyarch/ngl/d3d9/shader_program.hh"
#include "treyarch/ngl/shaders/sm_phatcharnormal/programs.hh"

#include "treyarch/ngl/shaders/sm_phatchar/programs.hh"

using namespace treyarch;
using namespace ngl::shaders;
using namespace ngl::shaders::sm_phatchar;

static constexpr size_t material_pixel_variant_count = 3;

static std::array<ngl::d3d9::pixel_program, material_pixel_variant_count> material_pixel_programs;

bool ngl::shaders::sm_phatchar::initialize() {
    for (size_t index = 0; index < material_pixel_programs.size(); ++index) {
        if (!material_pixel_programs[index].create({ e_shader_program::sm_phatchar_material_pixel, (u16)index }))
            
            return false;
    }

    IDirect3DVertexShader9* material_vertex =
        sm_phatcharnormal::get_vertex_program(sm_phatcharnormal::e_vertex_variant::material);
    IDirect3DVertexShader9* depth_shadow_vertex =
        sm_phatcharnormal::get_vertex_program(sm_phatcharnormal::e_vertex_variant::depth_shadow);
    IDirect3DPixelShader9* sample_texture =
        sm_phatcharnormal::get_pixel_program(sm_phatcharnormal::e_pixel_variant::sample_texture);
    IDirect3DPixelShader9* sample_green =
        sm_phatcharnormal::get_pixel_program(sm_phatcharnormal::e_pixel_variant::sample_green);

    if (!material_vertex || !depth_shadow_vertex || !sample_texture || !sample_green)
        return false;

    program_exports::sm_phatchar::programs.get() = {
        material_pixel_programs[(size_t)e_pixel_variant::unshadowed].get(),
        sample_green,
        material_pixel_programs[(size_t)e_pixel_variant::shadow_2].get(),
        material_pixel_programs[(size_t)e_pixel_variant::shadow_1].get(),
        depth_shadow_vertex,
        sample_texture,
        material_vertex,
    };

    return true;
}

IDirect3DVertexShader9* ngl::shaders::sm_phatchar::get_vertex_program(e_vertex_variant variant) {
    switch (variant) {
        case e_vertex_variant::material:
            return sm_phatcharnormal::get_vertex_program(
                sm_phatcharnormal::e_vertex_variant::material);
        case e_vertex_variant::depth_shadow:
            return sm_phatcharnormal::get_vertex_program(
                sm_phatcharnormal::e_vertex_variant::depth_shadow);
    }

    return nullptr;
}

IDirect3DPixelShader9* ngl::shaders::sm_phatchar::get_pixel_program(e_pixel_variant variant) {
    size_t index = (size_t)variant;

    if (index < material_pixel_programs.size())
        return material_pixel_programs[index].get();

    switch (variant) {
        case e_pixel_variant::sample_texture:
            return sm_phatcharnormal::get_pixel_program(
                sm_phatcharnormal::e_pixel_variant::sample_texture);
        case e_pixel_variant::sample_green:
            return sm_phatcharnormal::get_pixel_program(
                sm_phatcharnormal::e_pixel_variant::sample_green);
        default:
            return nullptr;
    }
}
