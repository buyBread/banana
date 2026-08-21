#include "treyarch/ngl/shaders/program_exports.hh"
#include "treyarch/ngl/d3d9/shader_program.hh"

#include "treyarch/ngl/shaders/road_lights/programs.hh"

using namespace treyarch;
using namespace ngl::shaders;
using namespace ngl::shaders::road_lights;

static ngl::d3d9::vertex_program vertex_program;
static ngl::d3d9::pixel_program  pixel_program;

bool ngl::shaders::road_lights::initialize() {
    if (!vertex_program.create({ e_shader_program::road_lights_vertex }))
        return false;

    if (!pixel_program.create({ e_shader_program::road_lights_pixel }))
        return false;

    program_exports::road_lights::vertex_program.write(vertex_program.get());
    program_exports::road_lights::pixel_program .write(pixel_program .get());

    return true;
}

IDirect3DVertexShader9* ngl::shaders::road_lights::get_vertex_program() {
    return vertex_program.get();
}

IDirect3DPixelShader9* ngl::shaders::road_lights::get_pixel_program() {
    return pixel_program.get();
}
