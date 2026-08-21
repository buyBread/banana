#include "treyarch/ngl/shaders/program_exports.hh"
#include "treyarch/ngl/d3d9/shader_program.hh"

#include "treyarch/ngl/shaders/smsky/programs.hh"

using namespace treyarch;
using namespace ngl::shaders;
using namespace ngl::shaders::smsky;

static ngl::d3d9::vertex_program vertex_program;

static ngl::d3d9::pixel_program daylight_pixel_program;
static ngl::d3d9::pixel_program night_pixel_program;
static ngl::d3d9::pixel_program combined_pixel_program;

bool ngl::shaders::smsky::initialize() {
    if (!vertex_program.create({ e_shader_program::smsky_vertex }))
        return false;

    if (!daylight_pixel_program.create({ e_shader_program::smsky_daylight_pixel }))
        return false;

    if (!night_pixel_program.create({ e_shader_program::smsky_night_pixel }))
        return false;

    if (!combined_pixel_program.create({ e_shader_program::smsky_combined_pixel }))
        return false;

    program_exports::smsky::vertex_program.write(vertex_program.get());

    program_exports::smsky::daylight_pixel_program.write(daylight_pixel_program.get());
    program_exports::smsky::night_pixel_program   .write(night_pixel_program   .get());
    program_exports::smsky::combined_pixel_program.write(combined_pixel_program.get());

    return true;
}

IDirect3DVertexShader9* ngl::shaders::smsky::get_vertex_program() {
    return vertex_program.get();
}

IDirect3DPixelShader9* ngl::shaders::smsky::get_pixel_program(e_pixel_variant variant) {
    switch (variant) {
        case e_pixel_variant::daylight:
            return daylight_pixel_program.get();
        case e_pixel_variant::night:
            return night_pixel_program.get();
        case e_pixel_variant::combined:
            return combined_pixel_program.get();
    }

    return nullptr;
}
