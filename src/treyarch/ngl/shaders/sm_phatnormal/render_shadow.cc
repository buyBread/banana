#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/mesh_submission.hh"
#include "treyarch/ngl/d3d9/state_cache.hh"
#include "treyarch/ngl/d3d9/texture.hh"
#include "treyarch/ngl/scene/references.hh"
#include "treyarch/ngl/shaders/generated_material.hh"
#include "treyarch/ngl/shaders/program_exports.hh"
#include "treyarch/ngl/shaders/sm_phatnormal/render_node.hh"

using namespace treyarch;

// sub_8AF670
void ngl::shaders::sm_phatnormal::render_shadow(render_node* value) {
    generated_material::material_data* material = value->material_data;

    if (material->shadow_mode != 1 && material->shadow_mode != 2)
        return;

    d3d9::set_render_state(D3DRS_CULLMODE, D3DCULL_CW);

    if (value->queue_class && material->opacity_texture) {
        d3d9::bind_texture_resource(0,
                                    &material->opacity_texture->gpu_texture,
                                    D3DTADDRESS_WRAP,
                                    D3DTADDRESS_WRAP,
                                    D3DTEXF_LINEAR,
                                    D3DTEXF_LINEAR,
                                    D3DTEXF_LINEAR,
                                    1);
        d3d9::set_render_state(D3DRS_ALPHATESTENABLE, TRUE);
        d3d9::set_render_state(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
        d3d9::set_render_state(D3DRS_ALPHAREF, 0);
        d3d9::set_pixel_program(program_exports::sm_depth_shadow::pixel_program.read());
    } else {
        d3d9::set_render_state(D3DRS_ALPHATESTENABLE, FALSE);
        d3d9::set_pixel_program(program_exports::shared_shadow::depth_pixel_program.read());
    }

    d3d9::set_render_state(D3DRS_SLOPESCALEDEPTHBIAS, 0);
    d3d9::set_render_state(D3DRS_DEPTHBIAS, 0x3A83126F);
    d3d9::set_vertex_program(program_exports::sm_phatnormal::depth_shadow_vertex_program.read());

    generated_material::scene_snapshot snapshot;
    generated_material::prepare_scene_snapshot(&snapshot,
                                                material,
                                                value->node_data,
                                                value->section);

    matrix4x4 world_view_projection =
        value->node_data->local_to_world.affine() *
        ngl::references::current_scene.read()->world_to_screen;
    matrix4x4 compressed_to_local = fx::get_compressed_to_local(value->node_data);

    vector4 uv_scale_offset(snapshot.texture_matrix.x.x,
                            snapshot.texture_matrix.y.y,
                            snapshot.texture_matrix.w.x,
                            snapshot.texture_matrix.w.y);
    vector4 compressed(compressed_to_local.w.x,
                       compressed_to_local.w.y,
                       compressed_to_local.w.z,
                       compressed_to_local.x.x);

    IDirect3DDevice9* device = d3d9::references::device.get();
    device->SetVertexShaderConstantF(0, (const f32*)&world_view_projection, 4);
    device->SetVertexShaderConstantF(4, (const f32*)&uv_scale_offset, 1);
    device->SetVertexShaderConstantF(5, (const f32*)&compressed, 1);

    d3d9::draw_mesh_section(value->section);

    d3d9::set_render_state(D3DRS_SLOPESCALEDEPTHBIAS, 0);
    d3d9::set_render_state(D3DRS_DEPTHBIAS, 0);
}
