#include "treyarch/ngl/shaders/program_exports.hh"
#include "treyarch/ngl/d3d9/shader_program.hh"

#include "treyarch/ngl/shaders/sm_roadlod/programs.hh"

using namespace treyarch;
using namespace ngl::shaders;
using namespace ngl::shaders::sm_roadlod;

static ngl::d3d9::vertex_program vertex_program;
static ngl::d3d9::pixel_program  surface_pixel_program;
static ngl::d3d9::pixel_program  sampler_passthrough_pixel_program;

bool ngl::shaders::sm_roadlod::initialize() {
    if (!vertex_program.create({ e_shader_program::sm_roadlod_vertex }))
        return false;

    if (!surface_pixel_program.create({ e_shader_program::sm_roadlod_surface_pixel }))
        return false;

    if (!sampler_passthrough_pixel_program.create({ e_shader_program::sm_roadlod_sampler_passthrough_pixel }))
        return false;

    program_exports::sm_roadlod::vertex_program                   .write(vertex_program                   .get());
    program_exports::sm_roadlod::surface_pixel_program            .write(surface_pixel_program            .get());
    program_exports::sm_roadlod::sampler_passthrough_pixel_program.write(sampler_passthrough_pixel_program.get());

    return true;
}

IDirect3DVertexShader9* ngl::shaders::sm_roadlod::get_vertex_program() {
    return vertex_program.get();
}

IDirect3DPixelShader9* ngl::shaders::sm_roadlod::get_pixel_program(e_pixel_variant variant) {
    switch (variant) {
        case e_pixel_variant::surface:
            return surface_pixel_program.get();
        case e_pixel_variant::sampler_passthrough:
            return sampler_passthrough_pixel_program.get();
    }

    return nullptr;
}
