#include "treyarch/ngl/shaders/program_exports.hh"
#include "treyarch/ngl/d3d9/shader_program.hh"

#include "treyarch/ngl/shaders/sm_bush/programs.hh"

using namespace treyarch;
using namespace ngl::shaders;
using namespace ngl::shaders::sm_bush;

static ngl::d3d9::vertex_program material_vertex_program;

static ngl::d3d9::pixel_program material_pixel_program;
static ngl::d3d9::pixel_program shadowed_material_pixel_program;
static ngl::d3d9::pixel_program constant_red_pixel_program;
static ngl::d3d9::pixel_program sample_texture_pixel_program;
static ngl::d3d9::pixel_program sample_auxiliary_texture_pixel_program;

bool ngl::shaders::sm_bush::initialize() {
    if (!material_vertex_program.create({ e_shader_program::sm_bush_material_vertex }))
        
        return false;

    if (!material_pixel_program.create({ e_shader_program::sm_bush_material_pixel }))
        
        return false;

    if (!shadowed_material_pixel_program.create({ e_shader_program::sm_bush_shadowed_material_pixel }))
        
        return false;

    if (!constant_red_pixel_program.create({ e_shader_program::sm_bush_constant_red_pixel }))
        
        return false;

    if (!sample_texture_pixel_program.create({ e_shader_program::sm_bush_sample_texture_pixel }))
        
        return false;

    if (!sample_auxiliary_texture_pixel_program.create({ e_shader_program::sm_bush_sample_auxiliary_texture_pixel }))
        
        return false;

    program_exports::sm_bush::material_pixel_program
        .write(material_pixel_program.get());

    program_exports::sm_bush::constant_red_pixel_program
        .write(constant_red_pixel_program.get());

    program_exports::sm_bush::shadowed_material_pixel_program
        .write(shadowed_material_pixel_program.get());

    program_exports::sm_bush::sample_texture_pixel_program
        .write(sample_texture_pixel_program.get());

    program_exports::sm_bush::sample_auxiliary_texture_pixel_program
        .write(sample_auxiliary_texture_pixel_program.get());
        
    program_exports::sm_bush::material_vertex_program
        .write(material_vertex_program.get());

    return true;
}

IDirect3DVertexShader9* ngl::shaders::sm_bush::get_vertex_program() {
    return material_vertex_program.get();
}

IDirect3DPixelShader9* ngl::shaders::sm_bush::get_pixel_program(e_pixel_variant variant) {
    switch (variant) {
        case e_pixel_variant::material:
            return material_pixel_program.get();
        case e_pixel_variant::shadowed_material:
            return shadowed_material_pixel_program.get();
        case e_pixel_variant::constant_red:
            return constant_red_pixel_program.get();
        case e_pixel_variant::sample_texture:
            return sample_texture_pixel_program.get();
        case e_pixel_variant::sample_auxiliary_texture:
            return sample_auxiliary_texture_pixel_program.get();
    }

    return nullptr;
}
