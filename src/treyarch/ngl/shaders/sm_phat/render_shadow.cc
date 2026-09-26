#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/mesh_submission.hh"
#include "treyarch/ngl/d3d9/state_cache.hh"
#include "treyarch/ngl/scene/references.hh"
#include "treyarch/ngl/shaders/program_exports.hh"
#include "treyarch/ngl/shaders/sm_phat/render_node.hh"

using namespace treyarch;

void ngl::shaders::sm_phat::render_shadow(render_node* value) {
    u32 material_mode = value->material_data->shadow_mode;

    if (material_mode != 1 && material_mode != 2)
        return;

    d3d9::set_render_state(D3DRS_CULLMODE, D3DCULL_CW);
    d3d9::set_render_state(D3DRS_ALPHABLENDENABLE, FALSE);
    d3d9::set_render_state(D3DRS_SLOPESCALEDEPTHBIAS, 0);
    d3d9::set_render_state(D3DRS_DEPTHBIAS, 0x3A83126F);

    d3d9::set_pixel_program(program_exports::shared_shadow::depth_pixel_program.read());
    d3d9::set_vertex_program(program_exports::sm_phat::programs.get().utility_vertex);

    matrix4x4 compressed_to_local = fx::get_compressed_to_local(value->node_data);

    matrix4x4 compressed_to_screen =
        compressed_to_local * value->node_data->local_to_world.affine() *
        ngl::references::current_scene.read()->world_to_screen;

    matrix4x4 constants[2] { fx::get_compressed_to_uv(), compressed_to_screen, };

    d3d9::references::device.get()
        ->SetVertexShaderConstantF(0, (const f32*)constants, 8);
    d3d9::draw_mesh_section(value->section);

    d3d9::set_render_state(D3DRS_SLOPESCALEDEPTHBIAS, 0);
    d3d9::set_render_state(D3DRS_DEPTHBIAS, 0);
}
