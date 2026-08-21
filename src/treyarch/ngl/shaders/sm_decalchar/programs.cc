#include "treyarch/ngl/shaders/program_exports.hh"
#include "treyarch/ngl/d3d9/shader_program.hh"

#include "treyarch/ngl/shaders/sm_decalchar/programs.hh"

using namespace treyarch;
using namespace ngl::shaders;
using namespace ngl::shaders::sm_decalchar;

static ngl::d3d9::vertex_program material_vertex_program;
static ngl::d3d9::vertex_program depth_shadow_vertex_program;
static ngl::d3d9::pixel_program  pixel_program;

bool ngl::shaders::sm_decalchar::initialize() {
    if (!material_vertex_program.create({ e_shader_program::sm_decalchar_material_vertex }))
        return false;

    if (!depth_shadow_vertex_program.create({ e_shader_program::sm_decalchar_depth_shadow_vertex }))
        return false;

    if (!pixel_program.create({ e_shader_program::sm_decalchar_pixel }))
        return false;

    program_exports::sm_decalchar::material_vertex_program    .write(material_vertex_program    .get());
    program_exports::sm_decalchar::depth_shadow_vertex_program.write(depth_shadow_vertex_program.get());
    program_exports::sm_decalchar::pixel_program              .write(pixel_program              .get());

    return true;
}

IDirect3DVertexShader9* ngl::shaders::sm_decalchar::get_vertex_program(e_vertex_variant variant) {
    switch (variant) {
        case e_vertex_variant::material:
            return material_vertex_program.get();
        case e_vertex_variant::depth_shadow:
            return depth_shadow_vertex_program.get();
    }

    return nullptr;
}

IDirect3DPixelShader9* ngl::shaders::sm_decalchar::get_pixel_program() {
    return pixel_program.get();
}
