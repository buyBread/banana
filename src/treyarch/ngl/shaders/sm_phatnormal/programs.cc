#include "treyarch/ngl/shaders/program_exports.hh"
#include "treyarch/ngl/d3d9/shader_program.hh"
#include "treyarch/ngl/shaders/sm_babyphatnormal/programs.hh"

#include "treyarch/ngl/shaders/sm_phatnormal/programs.hh"

using namespace treyarch;
using namespace ngl::shaders;
using namespace ngl::shaders::sm_phatnormal;

static ngl::d3d9::vertex_program material_vertex_program;
static ngl::d3d9::vertex_program depth_shadow_vertex_program;

bool ngl::shaders::sm_phatnormal::initialize() {
    if (!material_vertex_program.create({ e_shader_program::sm_phatnormal_material_vertex }))
        return false;

    if (!depth_shadow_vertex_program.create({ e_shader_program::sm_phatnormal_depth_shadow_vertex }))
        return false;

    IDirect3DPixelShader9* unshadowed = sm_babyphatnormal::get_pixel_program(
        sm_babyphatnormal::e_pixel_variant::unshadowed);
    IDirect3DPixelShader9* shadow_1 = sm_babyphatnormal::get_pixel_program(
        sm_babyphatnormal::e_pixel_variant::shadow_1);
    IDirect3DPixelShader9* shadow_2 = sm_babyphatnormal::get_pixel_program(
        sm_babyphatnormal::e_pixel_variant::shadow_2);

    if (!unshadowed || !shadow_1 || !shadow_2)
        return false;

    program_exports::sm_phatnormal::unshadowed_pixel_program   .write(unshadowed);
    program_exports::sm_phatnormal::material_vertex_program    .write(material_vertex_program.get());
    program_exports::sm_phatnormal::shadow_1_pixel_program     .write(shadow_1);
    program_exports::sm_phatnormal::shadow_2_pixel_program     .write(shadow_2);
    program_exports::sm_phatnormal::depth_shadow_vertex_program.write(depth_shadow_vertex_program.get());

    return true;
}

IDirect3DVertexShader9* ngl::shaders::sm_phatnormal::get_vertex_program(e_vertex_variant variant) {
    switch (variant) {
        case e_vertex_variant::material:
            return material_vertex_program.get();
        case e_vertex_variant::depth_shadow:
            return depth_shadow_vertex_program.get();
    }

    return nullptr;
}

IDirect3DPixelShader9* ngl::shaders::sm_phatnormal::get_pixel_program(e_pixel_variant variant) {
    switch (variant) {
        case e_pixel_variant::unshadowed:
            return sm_babyphatnormal::get_pixel_program(
                sm_babyphatnormal::e_pixel_variant::unshadowed);
        case e_pixel_variant::shadow_1:
            return sm_babyphatnormal::get_pixel_program(
                sm_babyphatnormal::e_pixel_variant::shadow_1);
        case e_pixel_variant::shadow_2:
            return sm_babyphatnormal::get_pixel_program(
                sm_babyphatnormal::e_pixel_variant::shadow_2);
    }

    return nullptr;
}
