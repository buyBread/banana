#include "treyarch/ngl/shaders/program_exports.hh"
#include "treyarch/ngl/shaders/program_helpers.hh"

#include "treyarch/ngl/shaders/sm_depth_shadow/programs.hh"

using namespace treyarch;
using namespace ngl::shaders;
using namespace ngl::shaders::sm_depth_shadow;

static ngl::d3d9::vertex_program vertex_program;
static ngl::d3d9::pixel_program  pixel_program;

bool ngl::shaders::sm_depth_shadow::initialize() {
    if (!create_program_pair(vertex_program,
                             e_shader_program::sm_depth_shadow_vertex,
                             pixel_program,
                             e_shader_program::sm_depth_shadow_pixel))
        return false;

    program_exports::sm_depth_shadow::vertex_program.write(vertex_program.get());
    program_exports::sm_depth_shadow::pixel_program .write(pixel_program .get());

    return true;
}
