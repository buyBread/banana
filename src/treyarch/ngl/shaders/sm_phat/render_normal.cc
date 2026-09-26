#include <bit>
#include <cstring>

#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/mesh_submission.hh"
#include "treyarch/ngl/d3d9/state_cache.hh"
#include "treyarch/ngl/fx/lighting_parameters.hh"
#include "treyarch/ngl/fx/parameters.hh"
#include "treyarch/ngl/scene/references.hh"
#include "treyarch/ngl/shaders/generated_material.hh"
#include "treyarch/ngl/shaders/program_exports.hh"
#include "treyarch/ngl/shaders/sm_phat/configuration.hh"
#include "treyarch/ngl/shaders/sm_phat/render_node.hh"

using namespace treyarch;

namespace treyarch { namespace ngl { namespace shaders { namespace sm_phat {
namespace rendering {
    struct vertex_constants {
        vector4   ambient_color_lo[8];
        vector4   ambient_color_hi[8];
        matrix4x4 local_to_gobo;
        matrix4x4 light_to_screen[2];
        matrix4x4 compressed_to_uv;
        matrix4x4 compressed_to_screen;
        matrix4x4 compressed_to_local;
        vector4   local_to_ambient;
        vector4   fog_eye_to_local;
        vector4   fog_normal;
        vector4   fog_position_0;
        vector4   fog_position_1;
        vector4   fogs_per_meter;
        vector4   horizon_map_matrix_u;
        vector4   horizon_map_matrix_v;
    };
} // rendering

void render_normal(render_node* value) {
    generated_material::material_data* material = value->material_data;

    if (!material->normal_texture)
        return;

    d3d9::set_render_state(D3DRS_CULLMODE, D3DCULL_CW);
    d3d9::set_vertex_program(program_exports::sm_phat::programs.get().material_vertex);

    generated_material::scene_snapshot* snapshot = value->scene_snapshot;
    matrix4x4 unscaled_local_to_world =
        fx::get_unscaled_local_to_world(value->node_data);
    matrix4x4 local_to_world =
        snapshot->base_transform.affine() * unscaled_local_to_world.affine();

    fx::general_lighting_parameters lighting;
    fx::build_general_lighting(&lighting, value->node_data, value->section);

    fx::apply_material_values(&lighting, snapshot->material_values);
    fx::transform_lighting_to_local(&lighting, local_to_world);

    rendering::vertex_constants constants;
    std::memset(&constants, 0, sizeof(constants));

    std::memcpy(constants.ambient_color_lo,
                lighting.ambient_colors,
                sizeof(constants.ambient_color_lo));
    std::memcpy(constants.ambient_color_hi,
                lighting.ambient_colors_with_primary,
                sizeof(constants.ambient_color_hi));

    matrix4x4 compressed_to_local  = fx::get_compressed_to_local(value->node_data);
    matrix4x4 compressed_to_screen =
        compressed_to_local * local_to_world * ngl::references::current_scene.read()->world_to_screen;

    matrix4x4 texture_transform = fx::get_compressed_to_uv() * snapshot->texture_matrix.affine();

    const vector4 gobo_scale(0.5f, 1.0f, 0.5f, 1.0f);

    for (u32 row = 0; row < 4; ++row) {
        for (u32 column = 0; column < 4; ++column) {
            constants.local_to_gobo[row][column] = lighting.projector_matrix[row][column] * gobo_scale[column];
        }
    }

    u32 shadow_count = 0;

    constants.compressed_to_uv     = texture_transform;
    constants.compressed_to_screen = compressed_to_screen;
    constants.compressed_to_local  = compressed_to_local;
    constants.local_to_ambient     = lighting.ambient_color;
    constants.fog_eye_to_local     = lighting.post_view_position;
    constants.fog_normal           = lighting.post_direction;
    constants.fog_position_0       = lighting.post_plane_0;
    constants.fog_position_1       = lighting.post_plane_1;
    constants.fogs_per_meter       = lighting.post_range;
    constants.horizon_map_matrix_u = lighting.horizon_projection_u;
    constants.horizon_map_matrix_v = lighting.horizon_projection_v;

    d3d9::references::device.get()
        ->SetVertexShaderConstantF(0, (const f32*)&constants, 48);

    generated_material::transform_light_matrix(snapshot, lighting);

    d3d9::set_render_state(D3DRS_ALPHATESTENABLE, FALSE);

    if (value->queue_class) {
        d3d9::set_render_state(D3DRS_ALPHABLENDENABLE, TRUE);
        d3d9::set_render_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        d3d9::set_render_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        d3d9::set_render_state(D3DRS_BLENDFACTOR, 0);
        d3d9::set_render_state(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    } else
        d3d9::set_render_state(D3DRS_ALPHABLENDENABLE, FALSE);

    u32 render_flags = value->node_data->render_flags;

    if ((render_flags & 0xF) == 5) {
        d3d9::set_render_state(D3DRS_ALPHABLENDENABLE, TRUE);
        d3d9::set_render_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        d3d9::set_render_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        d3d9::set_render_state(D3DRS_BLENDFACTOR, 0);
        d3d9::set_render_state(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        d3d9::set_render_state(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
        d3d9::set_render_state(D3DRS_SRCBLENDALPHA, D3DBLEND_ZERO);
        d3d9::set_render_state(D3DRS_DESTBLENDALPHA, D3DBLEND_DESTALPHA);
        d3d9::set_render_state(D3DRS_BLENDOPALPHA, D3DBLENDOP_ADD);
    }

    f32 depth_bias = (f32)((render_flags >> 4) & 0xF) * std::bit_cast<f32>(0x35800008u);
    d3d9::set_render_state(D3DRS_SLOPESCALEDEPTHBIAS, 0);
    d3d9::set_render_state(D3DRS_DEPTHBIAS, std::bit_cast<DWORD>(depth_bias));

    bool gobo    = lighting.projector_texture != nullptr;
    bool horizon = lighting.horizon_texture   != nullptr;

    if (horizon && !gobo && shadow_count == 0)
        horizon = false;

    size_t material_index = select_material_configuration(material, snapshot, lighting);
    size_t lighting_index = select_lighting_configuration(horizon, lighting.light_count, gobo, shadow_count);

    const auto &pipeline =
        program_exports::sm_phat::pixel_pipelines.get()[material_index][lighting_index];

    d3d9::set_pixel_program(*pipeline.pixel_program_output);
    generated_material::configure_samplers(snapshot, material);
    generated_material::bind_textures(lighting, snapshot, material);
    generated_material::upload_pixel_constants(pipeline, lighting, snapshot, material, local_to_world);

    d3d9::draw_mesh_section(value->section);

    d3d9::set_render_state(D3DRS_SLOPESCALEDEPTHBIAS, 0);
    d3d9::set_render_state(D3DRS_DEPTHBIAS, 0);
    d3d9::set_render_state(D3DRS_ALPHABLENDENABLE, FALSE);
}

    ASSERT_SIZEOF(rendering::vertex_constants, 0x300);
}}}} // treyarch::ngl::shaders::sm_phat
