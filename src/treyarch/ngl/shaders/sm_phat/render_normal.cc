#include "treyarch/ngl/fx/parameters.hh"
#include "treyarch/ngl/gpu/d3d9/device.hh"
#include "treyarch/ngl/gpu/d3d9/state_cache.hh"
#include "treyarch/ngl/shaders/generated_material.hh"
#include "treyarch/ngl/shaders/program_exports.hh"
#include "treyarch/ngl/shaders/sm_phat/configuration.hh"
#include "treyarch/ngl/shaders/sm_phat/render_node.hh"

using namespace treyarch;

namespace treyarch { namespace ngl { namespace shaders { namespace sm_phat {
namespace rendering {
    struct vertex_constants {
        generated_material::regular_vertex_prefix prefix;
        math::mat44                               compressed_to_local;
        generated_material::regular_vertex_suffix suffix;
    };
} // rendering

// sub_8CB2D0
void node::render_normal() {
    generated_material::material_data* material = material_data;

    if (!material->normal_texture)
        return;

    d3d9::set_render_state(D3DRS_CULLMODE, D3DCULL_CW);
    d3d9::set_vertex_program(program_exports::sm_phat::programs.get().material_vertex);

    generated_material::scene_snapshot* snapshot = scene_snapshot;
    math::rot_tran_mat43 unscaled_local_to_world =
        fx::get_unscaled_local_to_world(mesh_node);
    math::mat44 local_to_world =
        math::mat44(*(const math::mat43*)&snapshot->base_transform) * math::mat44(unscaled_local_to_world);
    math::mat44 world_to_local = math::inv(*(const math::rot_tran_mat43*)&local_to_world);

    light_table_lookup          lookup = generated_material::get_light_table_lookup(mesh_node, local_to_world);
    general_lighting_parameters lighting;

    lighting.build_local(mesh_node, section, local_to_world, world_to_local, false, &lookup, true, true);

    for (i32 index = 0; index < 4; ++index)
        lighting.material_values[index] = snapshot->material_values[index];

    generated_material::regular_vertex_context vertex_context;
    generated_material::prepare_regular_vertex_context(&vertex_context,
                                                        lighting,
                                                        snapshot,
                                                        mesh_node,
                                                        section,
                                                        local_to_world,
                                                        false);

    rendering::vertex_constants constants;
    constants.prefix              = vertex_context.prefix;
    constants.compressed_to_local = vertex_context.compressed_to_local;
    constants.suffix              = vertex_context.suffix;

    d3d9::references::dev.get()
        ->SetVertexShaderConstantF(0, (const f32*)&constants, 48);

    generated_material::transform_light_matrix(snapshot, lighting);

    u32 render_flags = mesh_node->render_flags;
    generated_material::configure_regular_pass_states(queue_class,
                                                      render_flags,
                                                      false);

    bool gobo    = lighting.projector_texture != nullptr;
    bool horizon = lookup.table               != nullptr;

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
                                            lookup.table ? lookup.table->texture : nullptr,
                                            snapshot,
                                            material,
                                            local_to_world,
                                            section);
    generated_material::restore_regular_pass_states();
}

    ASSERT_SIZEOF(rendering::vertex_constants, 0x300);
}}}} // treyarch::ngl::shaders::sm_phat
