#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/state_cache.hh"
#include "treyarch/ngl/fx/lighting_parameters.hh"
#include "treyarch/ngl/fx/parameters.hh"
#include "treyarch/ngl/shaders/generated_material.hh"
#include "treyarch/ngl/shaders/program_exports.hh"
#include "treyarch/ngl/shaders/sm_phat/configuration.hh"
#include "treyarch/ngl/shaders/sm_phat/render_node.hh"

using namespace treyarch;

namespace treyarch { namespace ngl { namespace shaders { namespace sm_phat {
namespace rendering {
    struct vertex_constants {
        generated_material::regular_vertex_prefix prefix;
        matrix4x4                                 compressed_to_local;
        generated_material::regular_vertex_suffix suffix;
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

    generated_material::regular_vertex_context vertex_context;
    generated_material::prepare_regular_vertex_context(&vertex_context,
                                                        lighting,
                                                        snapshot,
                                                        value->node_data,
                                                        value->section,
                                                        local_to_world,
                                                        false);

    rendering::vertex_constants constants;
    constants.prefix              = vertex_context.prefix;
    constants.compressed_to_local = vertex_context.compressed_to_local;
    constants.suffix              = vertex_context.suffix;

    d3d9::references::device.get()
        ->SetVertexShaderConstantF(0, (const f32*)&constants, 48);

    generated_material::transform_light_matrix(snapshot, lighting);

    u32 render_flags = value->node_data->render_flags;
    generated_material::configure_regular_pass_states(value->queue_class,
                                                      render_flags,
                                                      false);

    bool gobo    = lighting.projector_texture != nullptr;
    bool horizon = lighting.horizon_texture   != nullptr;

    if (horizon && !gobo && vertex_context.shadow_count == 0)
        horizon = false;

    size_t material_index = resolve_material_configuration(material, snapshot, lighting);
    size_t lighting_index = select_lighting_configuration(horizon,
                                                          lighting.light_count,
                                                          gobo,
                                                          vertex_context.shadow_count);

    const auto &pipeline =
        program_exports::sm_phat::pixel_pipelines.get()[material_index][lighting_index];

    generated_material::draw_pixel_pipeline(pipeline,
                                            lighting,
                                            snapshot,
                                            material,
                                            local_to_world,
                                            value->section);
    generated_material::restore_regular_pass_states();
}

    ASSERT_SIZEOF(rendering::vertex_constants, 0x300);
}}}} // treyarch::ngl::shaders::sm_phat
