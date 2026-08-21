#include "treyarch/ngl/shaders/program_exports.hh"
#include "treyarch/ngl/d3d9/shader_program.hh"

#include "treyarch/ngl/shaders/sm_buildinglod/programs.hh"

using namespace treyarch;
using namespace ngl::shaders;
using namespace ngl::shaders::sm_buildinglod;

static ngl::d3d9::vertex_program vertex_program;
static ngl::d3d9::pixel_program  adaptive_alpha_pixel_program;
static ngl::d3d9::pixel_program  opaque_pixel_program;

bool ngl::shaders::sm_buildinglod::initialize() {
    if (!vertex_program.create({ e_shader_program::sm_buildinglod_vertex }))
        return false;

    if (!adaptive_alpha_pixel_program.create({ e_shader_program::sm_buildinglod_adaptive_alpha_pixel }))
        return false;

    if (!opaque_pixel_program.create({ e_shader_program::sm_buildinglod_opaque_pixel }))
        return false;

    program_exports::sm_buildinglod::vertex_program              .write(vertex_program              .get());
    program_exports::sm_buildinglod::adaptive_alpha_pixel_program.write(adaptive_alpha_pixel_program.get());
    program_exports::sm_buildinglod::opaque_pixel_program        .write(opaque_pixel_program        .get());

    return true;
}

IDirect3DVertexShader9* ngl::shaders::sm_buildinglod::get_vertex_program() {
    return vertex_program.get();
}

IDirect3DPixelShader9* ngl::shaders::sm_buildinglod::get_pixel_program(e_pixel_variant variant) {
    switch (variant) {
        case e_pixel_variant::adaptive_alpha:
            return adaptive_alpha_pixel_program.get();
        case e_pixel_variant::opaque:
            return opaque_pixel_program.get();
    }

    return nullptr;
}
