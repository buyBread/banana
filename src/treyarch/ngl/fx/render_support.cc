#include <cstdio>

#include "treyarch/ngl/fx/parameters.hh"
#include "treyarch/ngl/fx/render_support.hh"
#include "treyarch/ngl/lighting/context.hh"
#include "treyarch/ngl/mesh/mesh.hh"
#include "treyarch/ngl/ngl.hh"
#include "treyarch/ngl/scene/references.hh"

using namespace treyarch;

static util::memory_reference<u32> parameter_id_light_context { 0x01116330 };
static util::memory_reference<u32> parameter_id_light_sphere  { 0x0111633C };
static util::memory_reference<u32> parameter_id_ifl_frame     { 0x01116348 };

static util::memory_reference<ngl::lighting::light_context*> selected_light_context { 0x01118988 };

static util::memory_reference<u32> point_light_gather_enabled { 0x010FC594 };

static util::memory_reference<i32> ifl_frame { 0x01118800 };

static util::memory_reference<char> effect_hash_names           { 0x00FC6950 };
static util::memory_reference<u32>  effect_hash_name_index      { 0x00FC6A00 };

bool ngl::fx::has_scene_parameter(const scene_parameters* parameters, u32 id) {
    const u32* words = (const u32*)parameters;

    return (words[id >> 5] & (1u << (id & 31))) != 0;
}

void* ngl::fx::get_scene_parameter(const scene_parameters* parameters, u32 id) {
    return *(void**)((u8*)parameters + 8 + 4 * id);
}

static void gather_point_lights(      ngl::fx::mesh_node_data* node_data,
                                const ngl::mesh_section*       section) {

    f32 x = section->sphere[0];
    f32 y = section->sphere[1];
    f32 z = section->sphere[2];

    const f32* matrix = (const f32*)&node_data->local_to_world;

    f32 sphere[4] { matrix[0] * x + matrix[4] * y + matrix[ 8] * z + matrix[12],
                    matrix[1] * x + matrix[5] * y + matrix[ 9] * z + matrix[13],
                    matrix[2] * x + matrix[6] * y + matrix[10] * z + matrix[14],
                    matrix[3] * x + matrix[7] * y + matrix[11] * z + matrix[15] };

    f32 scale = (*(u32*)node_data->node_info & 2) ? node_data->scale : 1.0f;

    node_data->mesh_data[0x0B] = 0;
    *(u32*)(node_data->mesh_data + 0x08) |= 0x01000000;

    ngl::scene_parameters* parameters = node_data->parameters;
    
    ngl::lighting::light_context* context;

    if (ngl::fx::has_scene_parameter(parameters, parameter_id_light_context.read()))
        context = (ngl::lighting::light_context*)ngl::fx::get_scene_parameter
            (parameters, parameter_id_light_context.read());
    else
        context = ngl::references::current_scene.read()->light_context;

    selected_light_context.write(context);
    context->head.local_next = &context->head;

    if (!point_light_gather_enabled.read())
        return;

    if (ngl::fx::has_scene_parameter(parameters, parameter_id_light_sphere.read())) {
        const f32* adjustment = (const f32*)ngl::fx::get_scene_parameter
            (parameters, parameter_id_light_sphere.read());

        sphere[0] += adjustment[0];
        sphere[1] += adjustment[1];
        sphere[2] += adjustment[2];
        sphere[3] += adjustment[3];
    } else
        sphere[3] = *(f32*)(node_data->mesh_data + 0x2C);

    sphere[3] *= scale;
}

ngl::fx::effect* ngl::fx::select_effect(render_node* value) {
    gather_point_lights(value->node_data, value->section);

    effect* base = value->effect_data;
    effect_runtime* runtime = base->runtime;
    u8 count = value->node_data->point_light_count;

    if (!count && runtime->zero_point_lights)
        return runtime->zero_point_lights;

    if (count <= 2 && runtime->two_point_lights)
        return runtime->two_point_lights;

    if (count <= 4 && runtime->four_point_lights)
        return runtime->four_point_lights;

    return base;
}

void ngl::fx::record_hash_name(const fixed_string &value) {
    if (value.text)
        return;

    u32 index = effect_hash_name_index.read();
    char* names = &effect_hash_names.get();

    std::sprintf(names + 11 * index,
                 "0x%08X",
                 value.hash.source_hash_code);

    effect_hash_name_index.write((index + 1) & 0x0F);
}

void ngl::fx::prepare_effect_scene(effect* value) {
    u32 frame_epoch = ngl::references::frame_epoch.read();

    if (value->current_frame_epoch != frame_epoch) {
        value->current_frame_epoch = frame_epoch;
        value->current_scene       = nullptr;
        value->current_state_02c   = 0;
        value->current_material    = nullptr;
    }

    scene* current_scene = ngl::references::current_scene.read();

    if (value->current_scene != current_scene) {
        update_scene_parameters(value);
        value->current_scene = current_scene;
    }
}

void ngl::fx::prepare_effect_material(effect* value, material* material_data) {
    if (value->current_material == material_data)
        return;

    if (material_data->bound_effect != value)
        bind_material(material_data, value);

    copy_material_parameters(material_data);
    value->current_material = material_data;
}

void ngl::fx::prepare_animated_textures(effect*         effect_data,
                                        material*       material_data,
                                        mesh_node_data* node_data) {

    parameter* sentinel = (parameter*)&references::parameter_chain_sentinel.get();

    if (material_data->animated_texture_parameter_chain == sentinel)
        return;

    if (!effect_data->animated_texture_parameter_chain)
        build_animated_texture_parameter_chain(effect_data);

    if (!material_data->animated_texture_parameter_chain)
        build_animated_texture_parameter_chain(effect_data, material_data);

    scene_parameters* parameters = node_data->parameters;
    u32 parameter_id = parameter_id_ifl_frame.read();

    if (has_scene_parameter(parameters, parameter_id))
        ifl_frame.write((i32)get_scene_parameter(parameters, parameter_id));
    else
        ifl_frame.write(ngl::references::current_scene.read()->ifl_frame);
}
