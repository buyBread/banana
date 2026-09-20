#include <cmath>

#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/state_cache.hh"
#include "treyarch/ngl/fx/references.hh"
#include "treyarch/ngl/post_process/device_resources.hh"
#include "treyarch/ngl/scene/references.hh"
#include "treyarch/ngl/shaders/smsky/render_node.hh"
#include "treyarch/ngl/texture/texture.hh"
#include "treyarch/ngl/resources/references.hh"
#include "treyarch/ngl/resources/shader_resource_manager.hh"

using namespace treyarch;

void ngl::shaders::smsky::render(render_node* value) {
    d3d9::set_render_state(D3DRS_FILLMODE, D3DFILL_SOLID);

    if (ngl::references::in_shadow_scene.read())
        return;

    u32 light_source_id = fx::references::parameter_id_light_source.read();
    auto* light_source = (light_source_data*)value->node_data->parameters->values[light_source_id];
    sky_data* sky = light_source->sky;

    if (!sky || !sky->resource_0d8 || !sky->resource_0dc)
        return;

    scene* current_scene = ngl::references::current_scene.read();

    matrix4x4 camera_translation;
    camera_translation.identity();
    camera_translation.w = current_scene->view_position;

    matrix4x4 local_to_screen;

    for (u32 row_index = 0; row_index < 4; ++row_index) {
        for (u32 column = 0; column < 4; ++column) {
            const vector4 &source = camera_translation[row_index];
            const matrix4x4 &screen = current_scene->world_to_screen;

            local_to_screen[row_index][column] = source.x * screen.x[column] +
                                                 source.y * screen.y[column] +
                                                 source.z * screen.z[column] +
                                                 (row_index == 3 ? screen.w[column] : 0.0f);
        }
    }

    d3d9::set_render_state(D3DRS_CULLMODE, D3DCULL_NONE);
    d3d9::set_render_state(D3DRS_ALPHABLENDENABLE, FALSE);
    d3d9::set_render_state(D3DRS_ALPHATESTENABLE, FALSE);
    d3d9::set_render_state(D3DRS_ZWRITEENABLE, FALSE);

    IDirect3DVertexShader9** vertex_slot = references::sky_vertex_program.read();

    if (!vertex_slot) {
        vertex_slot = resources::find_vertex_program("rvb_skysphere_vs");
        references::sky_vertex_program.write(vertex_slot);
    }

    d3d9::set_vertex_program(*vertex_slot);

    matrix4x4 vertex_constants[2] { local_to_screen.transpose(),
                                    camera_translation.transpose() };

    IDirect3DDevice9* device = d3d9::references::device.get();

    device->SetVertexShaderConstantF(0, (const f32*)&vertex_constants[0], 4);
    device->SetVertexShaderConstantF(4, (const f32*)&vertex_constants[1], 4);

    IDirect3DPixelShader9** pixel_slot = references::sky_pixel_program.read();

    if (!pixel_slot) {
        pixel_slot = resources::find_pixel_program("rvb_skybox");
        references::sky_pixel_program.write(pixel_slot);
    }

    d3d9::set_pixel_program(*pixel_slot);

    texture* texture_data = fx::references::environment_texture.read();

    if (!texture_data)
        texture_data = ngl::references::black_texture.read();

    d3d9::set_texture(0, texture_data->gpu_texture.resource);
    d3d9::set_sampler_state(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    d3d9::set_sampler_state(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
    d3d9::set_sampler_state(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    d3d9::set_sampler_state(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    d3d9::set_sampler_state(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
    d3d9::set_sampler_state(0, D3DSAMP_MAXANISOTROPY, 1);

    f32 half_angle = (f32)(current_scene->field_of_view * 0.5f * 0.01745329300562541);
    
    f32 vertical_extent   = (f32)std::tan(half_angle) * current_scene->far_plane;
    f32 horizontal_extent = current_scene->aspect_ratio * vertical_extent;

    f32 camera_height = current_scene->view_to_world.w.y;

    vector4 pixel_constants[8] { vector4(current_scene->view_to_world.x_row(), 0.0f),
                                 vector4(current_scene->view_to_world.y_row(), 0.0f),
                                 vector4(current_scene->view_to_world.z_row(), 0.0f),
                                 vector4(-horizontal_extent,
                                         -vertical_extent - camera_height,
                                         current_scene->far_plane,
                                         1.0f),
                                 vector4(horizontal_extent,
                                         vertical_extent - camera_height,
                                         current_scene->far_plane,
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

    d3d9::set_render_state(D3DRS_ZWRITEENABLE, current_scene->z_write_enabled);
}