#include <bit>

#include "retail.hh"
#include "treyarch/ngl/gpu/d3d9/device.hh"
#include "treyarch/ngl/gpu/d3d9/mesh_submission.hh"
#include "treyarch/ngl/gpu/d3d9/state_cache.hh"
#include "treyarch/ngl/scene/scene.hh"
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

math::mat44 get_pcuv_local_to_screen(const ngl::fx::mesh_node_data* node_data,
                                     const ngl::scene*              build_scene) {

    const math::mat43 &local  = node_data->local_to_world;
    const math::mat44 &screen = build_scene->world_to_screen;

    math::mat44 result;

    for (u32 row_index = 0; row_index < 4; ++row_index) {
        for (u32 column = 0; column < 4; ++column) {
            const math::float4 &source = row_index == 3 ?
                local.get_w().val34() : local.get_m33()[row_index].val34();

            result[row_index][column] = source.get_y() * screen[1][column] +
                                        source.get_x() * screen[0][column] +
                                        source.get_z() * screen[2][column] +
                                        (row_index == 3 ? screen[3][column] : 0.0f);
        }
    }

    return result;
}

// sub_8B8600
void ngl::shaders::pcuv::node::render() {
    if (references::pcuv_shader.read() == ::ngl::references::all_shaders.read() &&
        !ngl::references::in_shadow_scene.read()) {

        render_normal();
    }
}

// sub_8A26A0
void ngl::shaders::pcuv::node::get_sort_info(sort_info &info) {
    retail::sub_8A26A0((u32*)this, (i32)&info);
}

// sub_8A8FD0
void ngl::shaders::pcuv::node::render_normal() {
    scene* build_scene = ngl::references::build_scene.read();

    IDirect3DDevice9* device = d3d9::references::dev.get();

    math::mat44 local_to_screen = get_pcuv_local_to_screen(mesh_node,
                                                           build_scene);

    d3d9::set_render_state(D3DRS_CULLMODE, D3DCULL_NONE);

    u32 blend_low  = material->blend_mode_low;
    u32 blend_high = material->blend_mode_high;

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

    math::vector4 tint_color(1.0f, 1.0f, 1.0f, 1.0f);

    void* tint_parameter = find_scene_parameter(mesh_node->parameters,
                                                fx::references::parameter_id_tint_color.read());

    if (tint_parameter)
        tint_color = *(const math::vector4*)tint_parameter;

    device->SetVertexShaderConstantF(4, (const f32*)&tint_color, 1);
    device->SetVertexShaderConstantF(0, (const f32*)&local_to_screen, 4);

    f32 emissive = 0.0f;

    u32 emissive_parameter_id = fx::references::parameter_id_emissive.read();

    if (has_scene_parameter(mesh_node->parameters, emissive_parameter_id))
        emissive = std::bit_cast<f32>(mesh_node->parameters->values[emissive_parameter_id]);

    d3d9::set_pixel_program(program_exports::pcuv::pixel_program.read());

    math::vector4 emissive_constant(emissive, 0.0f, 0.0f, 0.0f);
    device->SetPixelShaderConstantF(0, (const f32*)&emissive_constant, 1);

    texture* texture_data = material->texture_data;
    d3d9::set_texture(0, texture_data->gpu_texture.resource);

    d3d9::set_sampler_filters(0, D3DTEXF_LINEAR, D3DTEXF_LINEAR, D3DTEXF_POINT, 1);

    u32 map_flags = material->map_flags;

    u32 address_u = (map_flags & 0x80) ? D3DTADDRESS_MIRROR :
                    (map_flags & 0x10) ? D3DTADDRESS_CLAMP : D3DTADDRESS_WRAP;
    u32 address_v = (map_flags & 0x100) ? D3DTADDRESS_MIRROR :
                    (map_flags & 0x20) ? D3DTADDRESS_CLAMP : D3DTADDRESS_WRAP;

    d3d9::set_sampler_address(0, address_u, address_v);

    d3d9::draw_section(section);

    d3d9::set_render_state(D3DRS_COLORWRITEENABLE, build_scene->framebuffer_write_mask);
    d3d9::set_render_state(D3DRS_ZWRITEENABLE, build_scene->z_write_enabled);
}
