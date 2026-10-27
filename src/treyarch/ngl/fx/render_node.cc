#include "treyarch/ngl/fx/parameters.hh"
#include "treyarch/ngl/fx/pass.hh"
#include "treyarch/ngl/fx/references.hh"
#include "treyarch/ngl/fx/render_node.hh"
#include "treyarch/ngl/fx/render_support.hh"
#include "treyarch/ngl/gpu/d3d9/device.hh"
#include "treyarch/ngl/gpu/d3d9/mesh_submission.hh"
#include "treyarch/ngl/scene/scene.hh"

using namespace treyarch;

// inlined @ sub_9E0420
void draw_single_pass(ngl::fx::shader_node* value) {
    ngl::scene_parameters* parameters = value->mesh_node->parameters;
    u32 parameter_id = ngl::fx::references::parameter_id_mesh_runs.read();

    if (ngl::has_scene_parameter(parameters, parameter_id)) {
        const i32* runs = (const i32*)ngl::get_scene_parameter(parameters, parameter_id);
        ngl::d3d9::draw_mesh_section_runs(value->section, runs);

        return;
    }

    // draw_section's logic, but calling draw_primitive where that one inlines it
    ngl::mesh_section* section        = value->section;
    D3DPRIMITIVETYPE   primitive_type = (D3DPRIMITIVETYPE)section->primitive_type;

    if (section->index_count)
        ngl::d3d9::draw_indexed_primitive(primitive_type,
                                          section->index_count,
                                          section->index_offset,
                                          &section->index_buffer,
                                          section->index_size != 2 ? D3DFMT_INDEX32 : D3DFMT_INDEX16,
                                          section->vertex_count,
                                          section->vertex_offset,
                                          &section->vertex_buffer,
                                          section->vertex_definition_data);
    else
        ngl::d3d9::draw_primitive(primitive_type,
                                  section->vertex_count,
                                  0,
                                  &section->vertex_buffer,
                                  section->vertex_definition_data);
}

// sub_9E0420
void ngl::fx::shader_node::render() {
    effect* selected_effect = select_effect();

    effect_data = selected_effect;
    selected_effect->name.get_text();
    prepare_effect_scene(selected_effect);
    prepare_effect_material(selected_effect, material_data);
    prepare_animated_textures(selected_effect, this);

    technique* technique_data = &selected_effect->techniques[technique_index];

    update_material_parameters(selected_effect, mesh_node, section);

    if (technique_data->pass_count == 1) {
        pass* pass_data = technique_data->passes;

        apply_pass(selected_effect, pass_data);
        draw_single_pass(this);
        finish_pass(selected_effect, pass_data);
        pass_data->active_programs = &pass_data->programs;
    } else {
        for (i32 index = 0; index < technique_data->pass_count; ++index) {
            pass* pass_data = &technique_data->passes[index];

            apply_pass(selected_effect, pass_data);
            ngl::d3d9::draw_section(section);
            finish_pass(selected_effect, pass_data);
            
            pass_data->active_programs = &pass_data->programs;
        }
    }

    ngl::d3d9::poison_bindings();
}

// sub_9DF8E0
i32 ngl::fx::shader_node::get_list_type() {
    // todo: guess them
    switch (queue_class) {
        case 2:
        case 3:
            return 1;
        case 4:
            return 2;
        case 5:
            return 3;
        case 6:
            return 4;
        default:
            return 0;
    }
}
