#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/mesh_submission.hh"
#include "treyarch/ngl/d3d9/state_cache.hh"
#include "treyarch/ngl/scene/parameters.hh"
#include "treyarch/ngl/scene/references.hh"
#include "treyarch/ngl/shaders/shader.hh"
#include "treyarch/ngl/shaders/program_exports.hh"
#include "treyarch/ngl/shaders/pcuv/render_node.hh"
#include "treyarch/ngl/fx/references.hh"

using namespace treyarch;

void set_pcuv_blend(u32 source,
                    u32 destination,
                    u32 alpha_source,
                    u32 alpha_destination,
                    u32 operation) {

    ngl::d3d9::set_render_state(D3DRS_ALPHABLENDENABLE, TRUE);
    ngl::d3d9::set_render_state(D3DRS_SRCBLEND, source);
    ngl::d3d9::set_render_state(D3DRS_DESTBLEND, destination);
    ngl::d3d9::set_render_state(D3DRS_BLENDFACTOR, 0);
    ngl::d3d9::set_render_state(D3DRS_BLENDOP, operation);
    ngl::d3d9::set_render_state(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
    ngl::d3d9::set_render_state(D3DRS_SRCBLENDALPHA, alpha_source);
    ngl::d3d9::set_render_state(D3DRS_DESTBLENDALPHA, alpha_destination);
    ngl::d3d9::set_render_state(D3DRS_BLENDOPALPHA, D3DBLENDOP_ADD);
}

void set_pcuv_basic_blend() {
    ngl::d3d9::set_render_state(D3DRS_ALPHABLENDENABLE, TRUE);
    ngl::d3d9::set_render_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    ngl::d3d9::set_render_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    ngl::d3d9::set_render_state(D3DRS_BLENDFACTOR, 0);
    ngl::d3d9::set_render_state(D3DRS_BLENDOP, D3DBLENDOP_ADD);
}

matrix4x4 get_pcuv_local_to_screen(const ngl::fx::mesh_node_data* node_data,
                                   const ngl::scene*              current_scene) {

    const matrix4x4 &local  = node_data->local_to_world;
    const matrix4x4 &screen = current_scene->world_to_screen;

    matrix4x4 result;

    for (u32 row_index = 0; row_index < 4; ++row_index) {
        for (u32 column = 0; column < 4; ++column) {
            const vector4 &source = local[row_index];

            result[row_index][column] = source.y * screen.y[column] +
                                        source.x * screen.x[column] +
                                        source.z * screen.z[column] +
                                        (row_index == 3 ? screen.w[column] : 0.0f);
        }
    }

    return result;
}

void ngl::shaders::pcuv::render(render_node* value) {
    if (references::pcuv_shader.read() != ::ngl::references::all_shaders.read() ||
        ngl::references::in_shadow_scene.read()) {

        return;
    }

    scene* current_scene = ngl::references::current_scene.read();

    IDirect3DDevice9* device = d3d9::references::device.get();

    matrix4x4 local_to_screen = get_pcuv_local_to_screen(value->node_data,
                                                         current_scene);

    d3d9::set_render_state(D3DRS_CULLMODE, D3DCULL_NONE);

    u32 blend_low  = value->material_data->blend_mode_low;
    u32 blend_high = value->material_data->blend_mode_high;

    if ((blend_low == 2 && blend_high == 0) ||
        (blend_low == 0x06C10000 && blend_high == 5)) {

        set_pcuv_basic_blend();
        d3d9::set_render_state(D3DRS_COLORWRITEENABLE, 7);
        d3d9::set_render_state(D3DRS_ZWRITEENABLE, FALSE);
    } else if ((blend_low == 4 && blend_high == 0) ||
               (blend_low == 0x02C30000 && blend_high == 5)) {

        set_pcuv_blend(D3DBLEND_SRCALPHA,
                       D3DBLEND_ONE,
                       D3DBLEND_ZERO,
                       D3DBLEND_ONE,
                       D3DBLENDOP_REVSUBTRACT);
        d3d9::set_render_state(D3DRS_ZWRITEENABLE, FALSE);
    } else if ((blend_low == 3 && blend_high == 0) ||
               (blend_low == 0x02C10000 && blend_high == 5)) {

        set_pcuv_blend(D3DBLEND_SRCALPHA,
                       D3DBLEND_ONE,
                       D3DBLEND_ZERO,
                       D3DBLEND_ONE,
                       D3DBLENDOP_ADD);
        d3d9::set_render_state(D3DRS_ZWRITEENABLE, FALSE);
    } else if (blend_low == 0x08C10000 && blend_high == 7) {
        set_pcuv_blend(D3DBLEND_DESTALPHA,
                       D3DBLEND_ONE,
                       D3DBLEND_ZERO,
                       D3DBLEND_ZERO,
                       D3DBLENDOP_ADD);
        d3d9::set_render_state(D3DRS_ZWRITEENABLE, FALSE);
    } else {
        set_pcuv_blend(D3DBLEND_ZERO,
                       D3DBLEND_ONE,
                       D3DBLEND_ZERO,
                       D3DBLEND_ONE,
                       D3DBLENDOP_ADD);
        d3d9::set_render_state(D3DRS_ZWRITEENABLE, TRUE);
        d3d9::set_render_state(D3DRS_ALPHABLENDENABLE, FALSE);
    }

    if (blend_low & 0x00400000) {
        d3d9::set_render_state(D3DRS_ALPHATESTENABLE, TRUE);
        d3d9::set_render_state(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
        d3d9::set_render_state(D3DRS_ALPHAREF, (u8)blend_low);
    } else
        d3d9::set_render_state(D3DRS_ALPHATESTENABLE, FALSE);

    d3d9::set_vertex_program(program_exports::pcuv::vertex_program.read());

    vector4 tint_color { 1.0f, 1.0f, 1.0f, 1.0f };

    void* tint_parameter = find_scene_parameter(value->node_data->parameters,
                                                fx::references::parameter_id_tint_color.read());

    if (tint_parameter)
        tint_color = *(const vector4*)tint_parameter;

    device->SetVertexShaderConstantF(4, (const f32*)&tint_color, 1);
    device->SetVertexShaderConstantF(0, (const f32*)&local_to_screen, 4);

    f32 emissive = 0.0f;

    void* emissive_parameter = find_scene_parameter(value->node_data->parameters,
                                                    fx::references::parameter_id_emissive.read());

    if (emissive_parameter)
        emissive = *(const f32*)emissive_parameter;

    d3d9::set_pixel_program(program_exports::pcuv::pixel_program.read());

    vector4 emissive_constant { emissive, 0.0f, 0.0f, 0.0f };
    device->SetPixelShaderConstantF(0, (const f32*)&emissive_constant, 1);

    texture* texture_data = value->material_data->texture_data;
    d3d9::set_texture(0, texture_data->gpu_texture.resource);

    d3d9::set_sampler_state(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    d3d9::set_sampler_state(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    d3d9::set_sampler_state(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
    d3d9::set_sampler_state(0, D3DSAMP_MAXANISOTROPY, 1);

    u32 map_flags = value->material_data->map_flags;

    u32 address_u = (map_flags & 0x80) ? D3DTADDRESS_MIRROR :
                    (map_flags & 0x10) ? D3DTADDRESS_CLAMP : D3DTADDRESS_WRAP;
    u32 address_v = (map_flags & 0x100) ? D3DTADDRESS_MIRROR :
                    (map_flags & 0x20) ? D3DTADDRESS_CLAMP : D3DTADDRESS_WRAP;

    d3d9::set_sampler_state(0, D3DSAMP_ADDRESSU, address_u);
    d3d9::set_sampler_state(0, D3DSAMP_ADDRESSV, address_v);

    d3d9::draw_mesh_section(value->section);

    d3d9::set_render_state(D3DRS_COLORWRITEENABLE, current_scene->framebuffer_write_mask);
    d3d9::set_render_state(D3DRS_ZWRITEENABLE, current_scene->z_write_enabled);
}
