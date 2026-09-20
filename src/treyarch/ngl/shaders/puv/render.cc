#include <bit>

#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/mesh_submission.hh"
#include "treyarch/ngl/d3d9/state_cache.hh"
#include "treyarch/ngl/scene/references.hh"
#include "treyarch/ngl/shaders/program_exports.hh"
#include "treyarch/ngl/shaders/puv/render_node.hh"
#include "treyarch/ngl/shaders/shader.hh"

using namespace treyarch;

void ngl::shaders::puv::render(render_node* value) {
    if (references::puv_shader.read() != ngl::references::all_shaders.read() ||
        !ngl::references::in_shadow_scene.read()) {

        return;
    }

    d3d9::set_render_state(D3DRS_CULLMODE, D3DCULL_CW);
    d3d9::set_render_state(D3DRS_ALPHABLENDENABLE, FALSE);

    d3d9::set_pixel_program(program_exports::puv::pixel_program.read());
    d3d9::set_vertex_program(program_exports::puv::vertex_program.read());

    d3d9::set_render_state(D3DRS_SLOPESCALEDEPTHBIAS, 0);
    d3d9::set_render_state(D3DRS_DEPTHBIAS, std::bit_cast<DWORD>(0.001f));

    const matrix4x4 &local  = value->node_data->local_to_world;
    const matrix4x4 &screen = ngl::references::current_scene.read()->world_to_screen;

    matrix4x4 local_to_screen;

    for (u32 row_index = 0; row_index < 4; ++row_index) {
        for (u32 column = 0; column < 4; ++column) {
            const vector4 &source = local[row_index];

            local_to_screen[row_index][column] = source.x * screen.x[column] +
                                                 source.y * screen.y[column] +
                                                 source.z * screen.z[column] +
                                                 (row_index == 3 ? screen.w[column] : 0.0f);
        }
    }

    d3d9::references::device.get()
        ->SetVertexShaderConstantF(0, (const f32*)&local_to_screen, 4);

    d3d9::draw_mesh_section(value->section);

    d3d9::set_render_state(D3DRS_SLOPESCALEDEPTHBIAS, 0);
    d3d9::set_render_state(D3DRS_DEPTHBIAS, 0);
}
