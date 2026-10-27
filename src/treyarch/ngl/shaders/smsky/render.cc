#include <cmath>

#include "game/post_process/post_process.hh"
#include "game/shader_resource_manager.hh"
#include "treyarch/ngl/fx/references.hh"
#include "treyarch/ngl/gpu/d3d9/device.hh"
#include "treyarch/ngl/gpu/d3d9/state_cache.hh"
#include "treyarch/ngl/scene/scene.hh"
#include "treyarch/ngl/shaders/smsky/render_node.hh"
#include "treyarch/ngl/texture/texture.hh"

using namespace treyarch;

// sub_8B2B30
void ngl::shaders::smsky::node::render() {
    d3d9::set_render_state(D3DRS_FILLMODE, D3DFILL_SOLID);

    if (!ngl::references::in_shadow_scene.read())
        render_normal();
}

// sub_8ADB30
void ngl::shaders::smsky::node::render_normal() {
    u32 light_source_id = fx::references::parameter_id_light_source.read();
    auto* light_source = (light_source_data*)mesh_node->parameters->values[light_source_id];
    sky_data* sky = light_source->sky;

    if (!sky || !sky->resource_0d8 || !sky->resource_0dc)
        return;

    scene* build_scene = ngl::references::build_scene.read();

    math::mat44 camera_translation = math::identity_mat33();
    camera_translation.set_w(build_scene->view_position);

    math::mat44 local_to_screen;

    for (u32 row_index = 0; row_index < 4; ++row_index) {
        for (u32 column = 0; column < 4; ++column) {
            const math::vector4 &source = camera_translation[row_index];
            const math::mat44   &screen = build_scene->world_to_screen;

            local_to_screen[row_index][column] = source[0] * screen[0][column] +
                                                 source[1] * screen[1][column] +
                                                 source[2] * screen[2][column] +
                                                 (row_index == 3 ? screen[3][column] : 0.0f);
        }
    }

    d3d9::set_render_state(D3DRS_CULLMODE, D3DCULL_NONE);
    d3d9::set_render_state(D3DRS_ALPHABLENDENABLE, FALSE);
    d3d9::set_render_state(D3DRS_ALPHATESTENABLE, FALSE);
    d3d9::set_render_state(D3DRS_ZWRITEENABLE, FALSE);

    IDirect3DVertexShader9** vertex_slot = references::sky_vertex_program.read();

    if (!vertex_slot) {
        vertex_slot = treyarch::references::shader_resource_manager.read()->find_vertex_program("rvb_skysphere_vs");
        references::sky_vertex_program.write(vertex_slot);
    }

    d3d9::set_vertex_program(*vertex_slot);

    math::mat44 vertex_constants[2] { ~local_to_screen,
                                      ~camera_translation };

    IDirect3DDevice9* device = d3d9::references::dev.get();

    device->SetVertexShaderConstantF(0, (const f32*)&vertex_constants[0], 4);
    device->SetVertexShaderConstantF(4, (const f32*)&vertex_constants[1], 4);

    IDirect3DPixelShader9** pixel_slot = references::sky_pixel_program.read();

    if (!pixel_slot) {
        pixel_slot = treyarch::references::shader_resource_manager.read()->find_pixel_program("rvb_skybox");
        references::sky_pixel_program.write(pixel_slot);
    }

    d3d9::set_pixel_program(*pixel_slot);

    texture* texture_data = fx::references::environment_texture.read();

    if (!texture_data)
        texture_data = ngl::references::black_texture.read();

    d3d9::set_texture(0, texture_data->gpu_texture.resource);
    d3d9::set_sampler_address(0, D3DTADDRESS_CLAMP, D3DTADDRESS_CLAMP);
    d3d9::set_sampler_filters(0, D3DTEXF_LINEAR, D3DTEXF_LINEAR, D3DTEXF_POINT, 1);

    f32 half_angle = (f32)(build_scene->field_of_view * 0.5f * 0.01745329300562541);
    
    f32 vertical_extent   = (f32)std::tan((f64)half_angle) * build_scene->far_plane;
    f32 horizontal_extent = build_scene->aspect_ratio * vertical_extent;

    f32 camera_height = build_scene->view_to_world.get_w()[1];

    math::vector4 pixel_constants[8] { build_scene->view_to_world.get_x(),
                                       build_scene->view_to_world.get_y(),
                                       build_scene->view_to_world.get_z(),
                                       math::vector4(-horizontal_extent,
                                                     -vertical_extent - camera_height,
                                                     build_scene->far_plane,
                                                     1.0f),
                                       math::vector4(horizontal_extent,
                                                     vertical_extent - camera_height,
                                                     build_scene->far_plane,
                                                     1.0f),
                                 light_source->color_2d0,
                                 sky->color_0e0,
                                 sky->color_110 };

    device->SetPixelShaderConstantF(0, (const f32*)&pixel_constants[0], 3);
    device->SetPixelShaderConstantF(3, (const f32*)&pixel_constants[3], 2);
    device->SetPixelShaderConstantF(5, (const f32*)&pixel_constants[5], 1);
    device->SetPixelShaderConstantF(6, (const f32*)&pixel_constants[6], 1);
    device->SetPixelShaderConstantF(7, (const f32*)&pixel_constants[7], 1);

    post_process::references::device_resources.get().system->blitter->draw_fullscreen();

    d3d9::set_render_state(D3DRS_ZWRITEENABLE, build_scene->z_write_enabled);
}