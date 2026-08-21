#include "treyarch/ngl/shaders/program_exports.hh"
#include "treyarch/ngl/d3d9/shader_program.hh"

#include "treyarch/ngl/shaders/puv/programs.hh"

using namespace treyarch;
using namespace ngl::shaders;
using namespace ngl::shaders::puv;

static ngl::d3d9::vertex_program vertex_program;
static ngl::d3d9::pixel_program  pixel_program;

bool ngl::shaders::puv::initialize() {
    if (!vertex_program.create({ e_shader_program::puv_vertex }))
        return false;

    if (!pixel_program.create({ e_shader_program::puv_pixel }))
        return false;

    program_exports::puv::vertex_program.write(vertex_program.get());
    program_exports::puv::pixel_program .write(pixel_program .get());

    return true;
}
