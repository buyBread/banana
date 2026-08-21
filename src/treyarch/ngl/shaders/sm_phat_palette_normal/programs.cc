#include <array>
#include <cstddef>

#include "treyarch/ngl/shaders/program_exports.hh"
#include "treyarch/ngl/d3d9/shader_program.hh"

#include "treyarch/ngl/shaders/sm_phat_palette_normal/programs.hh"

using namespace treyarch;
using namespace ngl::shaders;
using namespace ngl::shaders::sm_phat_palette_normal;

static constexpr size_t pixel_variant_count = 3;

static ngl::d3d9::vertex_program material_vertex_program;
static ngl::d3d9::vertex_program depth_shadow_vertex_program;

static std::array<ngl::d3d9::pixel_program, pixel_variant_count> pixel_programs;

static ngl::d3d9::pixel_program sampler_passthrough_pixel_program;

bool ngl::shaders::sm_phat_palette_normal::initialize() {
    if (!material_vertex_program.create({ e_shader_program::sm_phat_palette_normal_material_vertex }))
        return false;

    for (size_t index = 0; index < pixel_programs.size(); ++index) {
        if (!pixel_programs[index].create({ e_shader_program::sm_phat_palette_normal_material_pixel, (u16)index }))
            return false;
    }

    if (!depth_shadow_vertex_program.create({ e_shader_program::sm_phat_palette_normal_depth_shadow_vertex }))
        return false;

    if (!sampler_passthrough_pixel_program.create({ e_shader_program::sm_phat_palette_normal_sampler_passthrough_pixel }))
        return false;

    program_exports::sm_phat_palette_normal::material_vertex_program
        .write(material_vertex_program.get());

    program_exports::sm_phat_palette_normal::unshadowed_pixel_program
        .write(pixel_programs[(size_t)e_pixel_variant::unshadowed].get());

    program_exports::sm_phat_palette_normal::shadow_1_pixel_program
        .write(pixel_programs[(size_t)e_pixel_variant::shadow_1].get());

    program_exports::sm_phat_palette_normal::shadow_2_pixel_program
        .write(pixel_programs[(size_t)e_pixel_variant::shadow_2].get());

    program_exports::sm_phat_palette_normal::depth_shadow_vertex_program
        .write(depth_shadow_vertex_program.get());
        
    program_exports::sm_phat_palette_normal::sampler_passthrough_pixel_program
        .write(sampler_passthrough_pixel_program.get());

    return true;
}

IDirect3DVertexShader9* ngl::shaders::sm_phat_palette_normal::get_vertex_program(e_vertex_variant variant) {
    switch (variant) {
        case e_vertex_variant::material:
            return material_vertex_program.get();
        case e_vertex_variant::depth_shadow:
            return depth_shadow_vertex_program.get();
    }

    return nullptr;
}

IDirect3DPixelShader9* ngl::shaders::sm_phat_palette_normal::get_pixel_program(e_pixel_variant variant) {
    if (variant == e_pixel_variant::sampler_passthrough) {
        return sampler_passthrough_pixel_program.get();
    }

    size_t index = (size_t)variant;

    if (index >= pixel_programs.size())
        return nullptr;

    return pixel_programs[index].get();
}
