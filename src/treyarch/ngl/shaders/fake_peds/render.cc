#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/mesh_submission.hh"
#include "treyarch/ngl/d3d9/scratch_mesh.hh"
#include "treyarch/ngl/d3d9/state_cache.hh"
#include "treyarch/ngl/d3d9/texture.hh"
#include "treyarch/ngl/d3d9/vertex_formats.hh"
#include "treyarch/ngl/scene/references.hh"
#include "treyarch/ngl/shaders/fake_peds/render_node.hh"
#include "treyarch/ngl/shaders/program_exports.hh"
#include "treyarch/shared/color.hh"

using namespace treyarch;

struct fake_pedestrian_vertex {
    f32 x;
    f32 y;
    f32 z;
    u32 color;
    f32 u;
    f32 v;
};

ASSERT_SIZEOF(fake_pedestrian_vertex, 0x18);

void ngl::shaders::fake_peds::render(render_node* value) {
    if (ngl::references::in_shadow_scene.read() || !value->texture_data || !value->pedestrian_count)
        return;

    IDirect3DDevice9* device = d3d9::references::device.get();

    DWORD previous_cull_mode;
    device->GetRenderState(D3DRS_CULLMODE, &previous_cull_mode);

    d3d9::set_render_state(D3DRS_CULLMODE, D3DCULL_NONE);

    if (value->blend_mode <= 1) {
        d3d9::set_render_state(D3DRS_ALPHABLENDENABLE, TRUE);
        d3d9::set_render_state(D3DRS_SRCBLEND,
                               value->blend_mode == 0 ? D3DBLEND_SRCALPHA : D3DBLEND_ONE);
        d3d9::set_render_state(D3DRS_DESTBLEND,
                               value->blend_mode == 0 ? D3DBLEND_INVSRCALPHA : D3DBLEND_ZERO);
        d3d9::set_render_state(D3DRS_BLENDFACTOR, 0);
        d3d9::set_render_state(D3DRS_BLENDOP, D3DBLENDOP_ADD);

        if (value->blend_mode == 1) {
            d3d9::set_render_state(D3DRS_ALPHATESTENABLE, TRUE);
            d3d9::set_render_state(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
            d3d9::set_render_state(D3DRS_ALPHAREF, 128);
        }
    }

    d3d9::set_render_state(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
    d3d9::set_render_state(D3DRS_SRCBLENDALPHA, D3DBLEND_ZERO);
    d3d9::set_render_state(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);
    d3d9::set_render_state(D3DRS_BLENDOPALPHA, D3DBLENDOP_ADD);

    d3d9::set_vertex_program(program_exports::fake_peds::vertex_program.read());
    d3d9::set_pixel_program(program_exports::fake_peds::pixel_program.read());

    scene* current_scene = ngl::references::current_scene.read();

    device->SetVertexShaderConstantF(0, (const f32*)&current_scene->world_to_screen, 4);

    d3d9::bind_texture(0, value->texture_data, 1, 3);

    mesh_section* section = d3d9::allocate_scratch_mesh_section( D3DPT_TRIANGLELIST,
                                                                 6 * value->pedestrian_count,
                                                                 4 * value->pedestrian_count,
                                                                &d3d9::references::quad_position_color_uv_format.get());

    u16* indices = d3d9::lock_scratch_mesh_indices(section);
    auto* vertices = (fake_pedestrian_vertex*)d3d9::lock_scratch_mesh_vertices(section);

    f32 tile_width = 1.0f / (f32)(i32)value->atlas_tile_count;

    for (u32 index = 0; index < value->pedestrian_count; ++index) {
        const pedestrian &ped = value->pedestrians[index];

        u32 color = pack_color(ped.color);
        f32 tile_left  = (f32)ped.atlas_tile * tile_width;
        f32 tile_right = (f32)(ped.atlas_tile + 1) * tile_width;

        fake_pedestrian_vertex* quad = vertices + index * 4;

        quad[0] = { ped.center.x - ped.right.x + ped.up.x,
                    ped.center.y - ped.right.y + ped.up.y,
                    ped.center.z - ped.right.z + ped.up.z,
                    color, tile_left, 0.0f };

        quad[1] = { ped.center.x - ped.right.x - ped.up.x,
                    ped.center.y - ped.right.y - ped.up.y,
                    ped.center.z - ped.right.z - ped.up.z,
                    color, tile_left, 1.0f };

        quad[2] = { ped.center.x + ped.right.x - ped.up.x,
                    ped.center.y + ped.right.y - ped.up.y,
                    ped.center.z + ped.right.z - ped.up.z,
                    color, tile_right, 1.0f };

        quad[3] = { ped.center.x + ped.right.x + ped.up.x,
                    ped.center.y + ped.right.y + ped.up.y,
                    ped.center.z + ped.right.z + ped.up.z,
                    color, tile_right, 0.0f };

        u16 first_vertex = (u16)(4 * index);
        u16* quad_indices = indices + index * 6;

        quad_indices[0] = first_vertex;
        quad_indices[1] = first_vertex + 1;
        quad_indices[2] = first_vertex + 2;
        quad_indices[3] = first_vertex + 2;
        quad_indices[4] = first_vertex + 3;
        quad_indices[5] = first_vertex;
    }

    d3d9::unlock_scratch_mesh_indices(section);
    d3d9::unlock_scratch_mesh_vertices(section);
    d3d9::draw_mesh_section(section);

    d3d9::set_render_state(D3DRS_CULLMODE, previous_cull_mode);
}
