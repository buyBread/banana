#include <array>
#include <cstddef>

#include "treyarch/ngl/shaders/program_exports.hh"
#include "treyarch/ngl/d3d9/shader_program.hh"

#include "treyarch/ngl/shaders/sm_phatspiderman/programs.hh"

using namespace treyarch;
using namespace ngl::shaders;
using namespace ngl::shaders::sm_phatspiderman;

static constexpr size_t material_pixel_variant_count = 3;

static ngl::d3d9::vertex_program material_vertex_program;
static ngl::d3d9::vertex_program depth_shadow_vertex_program;

static std::array<ngl::d3d9::pixel_program, material_pixel_variant_count> material_pixel_programs;

static ngl::d3d9::pixel_program sample_texture_pixel_program;
static ngl::d3d9::pixel_program sample_green_pixel_program;

bool ngl::shaders::sm_phatspiderman::initialize() {
    if (!material_vertex_program.create({ e_shader_program::sm_phatspiderman_material_vertex }))
        return false;

    if (!depth_shadow_vertex_program.create({ e_shader_program::sm_phatspiderman_depth_shadow_vertex }))
        return false;

    for (size_t index = 0; index < material_pixel_programs.size(); ++index) {
        if (!material_pixel_programs[index].create({ e_shader_program::sm_phatspiderman_material_pixel, (u16)index }))
            return false;
    }

    if (!sample_texture_pixel_program.create({ e_shader_program::sm_phatspiderman_sample_texture_pixel }))
        return false;

    if (!sample_green_pixel_program.create({ e_shader_program::sm_phatspiderman_sample_green_pixel }))
        return false;

    program_exports::sm_phatspiderman::programs.get() = {
        sample_green_pixel_program.get(),
        material_pixel_programs[(size_t)e_pixel_variant::shadow_2].get(),
        sample_texture_pixel_program.get(),
        material_vertex_program.get(),
        depth_shadow_vertex_program.get(),
        material_pixel_programs[(size_t)e_pixel_variant::unshadowed].get(),
        material_pixel_programs[(size_t)e_pixel_variant::shadow_1].get(),
    };

    return true;
}

IDirect3DVertexShader9* ngl::shaders::sm_phatspiderman::get_vertex_program(e_vertex_variant variant) {
    switch (variant) {
        case e_vertex_variant::material:
            return material_vertex_program.get();
        case e_vertex_variant::depth_shadow:
            return depth_shadow_vertex_program.get();
    }

    return nullptr;
}

IDirect3DPixelShader9* ngl::shaders::sm_phatspiderman::get_pixel_program(e_pixel_variant variant) {
    size_t index = (size_t)variant;

    if (index < material_pixel_programs.size())
        return material_pixel_programs[index].get();

    switch (variant) {
        case e_pixel_variant::sample_texture:
            return sample_texture_pixel_program.get();
        case e_pixel_variant::sample_green:
            return sample_green_pixel_program.get();
        default:
            return nullptr;
    }
}
