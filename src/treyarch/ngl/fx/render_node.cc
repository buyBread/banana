#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/mesh_submission.hh"
#include "treyarch/ngl/fx/parameters.hh"
#include "treyarch/ngl/fx/pass.hh"
#include "treyarch/ngl/fx/render_node.hh"
#include "treyarch/ngl/fx/render_support.hh"
#include "treyarch/ngl/scene/references.hh"

using namespace treyarch;

static util::memory_reference<u32> parameter_id_mesh_runs     { 0x011171E0 };

static void draw_single_pass(ngl::fx::render_node* value) {
    ngl::scene_parameters* parameters = value->node_data->parameters;
    u32 parameter_id = parameter_id_mesh_runs.read();

    if (ngl::fx::has_scene_parameter(parameters, parameter_id)) {
        const i32* runs = (const i32*)ngl::fx::get_scene_parameter(parameters, parameter_id);
        ngl::d3d9::draw_mesh_section_runs(value->section, runs);
    } else
        ngl::d3d9::draw_mesh_section_individual(value->section);
}

void ngl::fx::render(render_node* value) {
    effect* effect_data = select_effect(value);

    value->effect_data = effect_data;
    record_hash_name(effect_data->name);
    prepare_effect_scene(effect_data);
    prepare_effect_material(effect_data, value->material_data);
    prepare_animated_textures(effect_data,
                              value->material_data,
                              value->node_data);

    technique* technique_data = &effect_data->techniques[value->technique_index];

    update_material_parameters(effect_data,
                               value->node_data,
                               value->section,
                               ngl::references::current_scene.read()->depth_bias_enabled != 0);

    if (technique_data->pass_count == 1) {
        pass* pass_data = technique_data->passes;

        apply_pass(effect_data, pass_data);
        draw_single_pass(value);
        finish_pass(effect_data, pass_data);
        pass_data->active_programs = &pass_data->programs;
    } else {
        for (i32 index = 0; index < technique_data->pass_count; ++index) {
            pass* pass_data = &technique_data->passes[index];

            apply_pass(effect_data, pass_data);
            ngl::d3d9::draw_mesh_section(value->section);
            finish_pass(effect_data, pass_data);
            
            pass_data->active_programs = &pass_data->programs;
        }
    }

    ngl::d3d9::poison_bindings();
}
