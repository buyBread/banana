#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/mesh_submission.hh"
#include "treyarch/ngl/d3d9/state_cache.hh"
#include "treyarch/ngl/fx/batch_renderer.hh"
#include "treyarch/ngl/fx/parameters.hh"
#include "treyarch/ngl/fx/pass.hh"
#include "treyarch/ngl/fx/render_node.hh"
#include "treyarch/ngl/fx/render_support.hh"
#include "treyarch/ngl/scene/references.hh"

using namespace treyarch;

static util::memory_reference<u32> parameter_id_mesh_runs { 0x011171E0 };

static ngl::fx::effect* queue_effect_runtime(ngl::fx::effect_runtime* runtime,
                                             ngl::fx::effect*         head) {

    if (runtime->queued)
        return head;

    ngl::fx::effect* previous = nullptr;
    ngl::fx::effect* current  = head;

    while (current) {
        ngl::fx::effect_runtime* current_runtime = current->runtime;

        if (runtime->priority >= current_runtime->priority)
            break;

        previous = current;
        current  = current_runtime->next;
    }

    runtime->next = current;

    if (previous)
        previous->runtime->next = runtime->owner;
    else
        head = runtime->owner;

    runtime->queued = 1;

    for (i32 index = 0; index < runtime->owner->technique_count; ++index) {
        runtime->technique_batches[index].head  = nullptr;
        runtime->technique_batches[index].count = 0;
    }

    return head;
}

static void draw_single_pass_batch_node(ngl::fx::render_node* value) {
    if (value->section->index_count) {
        ngl::d3d9::draw_mesh_section(value->section);
        
        return;
    }

    ngl::scene_parameters* parameters = value->node_data->parameters;
    u32 parameter_id = parameter_id_mesh_runs.read();

    if (ngl::fx::has_scene_parameter(parameters, parameter_id)) {
        auto* runs = (const i32*)ngl::fx::get_scene_parameter(parameters, parameter_id);

        ngl::d3d9::draw_mesh_section_runs(value->section, runs);

        return;
    }

    ngl::d3d9::draw_mesh_section(value->section);
}

static void begin_depth_only_technique(ngl::fx::pass*      pass_data,
                                       IDirect3DSurface9** saved_color_target) {

    ngl::scene* current_scene = ngl::references::current_scene.read();

    if (!current_scene->depth_bias_enabled || pass_data->states.alpha_test_enabled)
        return;

    IDirect3DDevice9* device = ngl::d3d9::references::device.get();

    device->GetRenderTarget(0, saved_color_target);

    ngl::d3d9::set_render_state(D3DRS_COLORWRITEENABLE, 0);
}

static void finish_depth_only_technique(ngl::fx::pass*     pass_data,
                                        IDirect3DSurface9* saved_color_target) {

    ngl::scene* current_scene = ngl::references::current_scene.read();

    if (!current_scene->depth_bias_enabled || pass_data->states.alpha_test_enabled)
        return;

    IDirect3DDevice9* device = ngl::d3d9::references::device.get();

    if (saved_color_target) {
        ngl::d3d9::set_render_state(D3DRS_COLORWRITEENABLE, 0x0F);
        device->SetRenderTarget(0, saved_color_target);
        saved_color_target->Release();
    } else
        ngl::d3d9::set_render_state(D3DRS_COLORWRITEENABLE, 0);
}

static i32 render_technique_batch(ngl::fx::effect*          effect_data,
                                  ngl::fx::technique*       technique_data,
                                  ngl::fx::technique_batch* batch) {

    ngl::fx::record_hash_name(technique_data->name);

    ngl::fx::pass* first_pass = technique_data->passes;
    IDirect3DSurface9* saved_color_target = nullptr;

    begin_depth_only_technique(first_pass, &saved_color_target);

    ngl::fx::pass* final_pass = first_pass;

    i32 rendered_count = 0;

    for (ngl::fx::render_node* node = batch->head; node; node = (ngl::fx::render_node*)node->base.next) {
        ngl::fx::update_material_parameters(effect_data,
                                            node->node_data,
                                            node->section,
                                            ngl::references::current_scene.read()->depth_bias_enabled != 0);
        ngl::fx::prepare_effect_material(effect_data, node->material_data);
        ngl::fx::prepare_animated_textures(effect_data,
                                           node->material_data,
                                           node->node_data);

        if (technique_data->pass_count <= 1) {
            ngl::fx::apply_pass(effect_data, first_pass);
            draw_single_pass_batch_node(node);
            
            final_pass = first_pass;
        } else {
            for (i32 index = 0; index < technique_data->pass_count; ++index) {
                ngl::fx::pass* pass_data = &technique_data->passes[index];

                ngl::fx::apply_pass(effect_data, pass_data);
                ngl::d3d9::draw_mesh_section(node->section);
                ngl::fx::finish_pass(effect_data, pass_data);

                pass_data->active_programs = &pass_data->programs;

                final_pass = pass_data;
            }
        }

        ++rendered_count;
    }

    ngl::fx::finish_pass(effect_data, final_pass);
    final_pass->active_programs = &final_pass->programs;

    finish_depth_only_technique(first_pass, saved_color_target);

    return rendered_count;
}

static i32 render_effect_batches(ngl::fx::effect_runtime* runtime) {
    ngl::fx::effect* effect_data = runtime->owner;

    ngl::fx::record_hash_name(effect_data->name);
    ngl::fx::prepare_effect_scene(effect_data);

    i32 rendered_count = 0;

    for (i32 index = 0; index < effect_data->technique_count; ++index) {
        ngl::fx::technique_batch &batch = runtime->technique_batches[index];

        if (!batch.head)
            continue;

        rendered_count += render_technique_batch(effect_data,
                                                 &effect_data->techniques[index],
                                                 &batch);
    }

    ngl::d3d9::poison_bindings();

    return rendered_count;
}

i32 ngl::fx::render_batch(ngl::render_node* head) {
    effect* queued_effects = nullptr;

    for (ngl::render_node* base = head; base;) {
        ngl::render_node* next = base->next;

        auto* node            = (render_node*)base;
        auto* selected_effect = select_effect(node);
        auto* runtime         = selected_effect->runtime;

        if (!runtime->queued)
            queued_effects = queue_effect_runtime(runtime, queued_effects);

        technique_batch &batch = runtime->technique_batches[node->technique_index];

        node->base.next = (ngl::render_node*)batch.head;
        ++batch.count;
        batch.head = node;

        base = next;
    }

    i32 rendered_count = 0;

    for (effect* effect_data = queued_effects; effect_data;) {
        auto* runtime = effect_data->runtime;

        effect* next = runtime->next;

        rendered_count += render_effect_batches(runtime);

        runtime->next   = nullptr;
        runtime->queued = 0;
        effect_data     = next;
    }

    return rendered_count;
}
