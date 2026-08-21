#include "treyarch/ngl/shaders/program_exports.hh"
#include "treyarch/ngl/d3d9/shader_program.hh"

#include "treyarch/ngl/shaders/sm_retrofit/programs.hh"

using namespace treyarch;
using namespace ngl::shaders;
using namespace ngl::shaders::sm_retrofit;

static ngl::d3d9::vertex_program vertex_program;
static ngl::d3d9::pixel_program  pixel_program;

bool ngl::shaders::sm_retrofit::initialize() {
    if (!vertex_program.create({ e_shader_program::sm_retrofit_vertex }))
        return false;

    if (!pixel_program.create({ e_shader_program::sm_retrofit_pixel }))
        return false;

    program_exports::sm_retrofit::vertex_program.write(vertex_program.get());
    program_exports::sm_retrofit::pixel_program .write(pixel_program .get());

    return true;
}
