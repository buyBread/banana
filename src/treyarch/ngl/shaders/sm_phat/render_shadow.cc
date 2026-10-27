#include "treyarch/ngl/gpu/d3d9/device.hh"
#include "treyarch/ngl/gpu/d3d9/mesh_submission.hh"
#include "treyarch/ngl/gpu/d3d9/state_cache.hh"
#include "treyarch/ngl/scene/scene.hh"
#include "treyarch/ngl/shaders/program_exports.hh"
#include "treyarch/ngl/shaders/sm_phat/render_node.hh"

using namespace treyarch;

// sub_885020
void ngl::shaders::sm_phat::node::render_shadow() {
    u32 material_mode = material_data->shadow_mode;

    if (material_mode != 1 && material_mode != 2)
        return;

    d3d9::set_render_state(D3DRS_CULLMODE, D3DCULL_CW);
    d3d9::set_render_state(D3DRS_ALPHABLENDENABLE, FALSE);
    d3d9::set_render_state(D3DRS_SLOPESCALEDEPTHBIAS, 0);
    d3d9::set_render_state(D3DRS_DEPTHBIAS, 0x3A83126F);

    d3d9::set_pixel_program(program_exports::shared_shadow::depth_pixel_program.read());
    d3d9::set_vertex_program(program_exports::sm_phat::programs.get().utility_vertex);

    math::mat44 compressed_to_local = generated_material::get_compressed_to_local(mesh_node);

    math::mat44 compressed_to_screen =
        compressed_to_local * math::mat44(mesh_node->local_to_world) *
        ngl::references::build_scene.read()->world_to_screen;

    math::mat44 constants[2] { generated_material::get_compressed_to_uv(), compressed_to_screen, };

    d3d9::references::dev.get()
        ->SetVertexShaderConstantF(0, (const f32*)constants, 8);
    d3d9::draw_section(section);

    d3d9::set_render_state(D3DRS_SLOPESCALEDEPTHBIAS, 0);
    d3d9::set_render_state(D3DRS_DEPTHBIAS, 0);
}
