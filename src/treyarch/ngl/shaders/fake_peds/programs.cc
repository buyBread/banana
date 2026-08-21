#include "treyarch/ngl/shaders/program_exports.hh"
#include "treyarch/ngl/d3d9/shader_program.hh"

#include "treyarch/ngl/shaders/fake_peds/programs.hh"

using namespace treyarch;
using namespace ngl::shaders;
using namespace ngl::shaders::fake_peds;

static ngl::d3d9::vertex_program vertex_program;
static ngl::d3d9::pixel_program  pixel_program;

bool ngl::shaders::fake_peds::initialize() {
    if (!vertex_program.create({ e_shader_program::fake_peds_vertex }))
        return false;

    if (!pixel_program.create({ e_shader_program::fake_peds_pixel }))
        return false;

    program_exports::fake_peds::vertex_program.write(vertex_program.get());
    program_exports::fake_peds::pixel_program .write(pixel_program .get());

    return true;
}

IDirect3DVertexShader9* ngl::shaders::fake_peds::get_vertex_program() {
    return vertex_program.get();
}

IDirect3DPixelShader9* ngl::shaders::fake_peds::get_pixel_program() {
    return pixel_program.get();
}
