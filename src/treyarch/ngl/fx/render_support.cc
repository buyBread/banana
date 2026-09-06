#include <cstdio>

#include "treyarch/ngl/fx/parameters.hh"
#include "treyarch/ngl/fx/render_support.hh"
#include "treyarch/ngl/lighting/context.hh"
#include "treyarch/ngl/mesh/mesh.hh"
#include "treyarch/ngl/ngl.hh"
#include "treyarch/ngl/scene/parameters.hh"
#include "treyarch/ngl/scene/references.hh"
#include "util/gimmie/fn.hh"

using namespace treyarch;

static util::memory_reference<u32> parameter_id_light_context { 0x01116330 };
static util::memory_reference<u32> parameter_id_light_sphere  { 0x0111633C };
static util::memory_reference<u32> parameter_id_ifl_frame     { 0x01116348 };

static util::memory_reference<ngl::lighting::light_context*> selected_light_context { 0x01118988 };

static util::memory_reference<void*> point_light_manager { 0x010FC594 };
static util::memory_reference<f32>   point_light_data    { 0x011189D0 };

static util::memory_reference<i32> ifl_frame { 0x01118800 };

static util::memory_reference<char> effect_hash_names           { 0x00FC6950 };
static util::memory_reference<u32>  effect_hash_name_index      { 0x00FC6A00 };

struct point_light_candidate {
    u32 index;
    i32 integer_distance;
    f32 squared_distance;
    u32 reserved;
};

struct point_light_visitor;

using point_light_visit_function = i32(__fastcall*)(point_light_visitor*, void*, i32);

struct point_light_visitor_vtable {
    void*                      destroy;
    point_light_visit_function visit;
};

struct point_light_visitor {
    point_light_visitor_vtable* vtable;
    u32                         reserved_004;
    point_light_candidate       candidates[512];
    u32                         count;
    u32                         reserved_200C;
    f32                         center[4];
};

i32 __fastcall visit_point_light(point_light_visitor* visitor,
                                 void*,
                                 i32                  index) {

    f32* light = &point_light_data.get() + 8 * index;
    f32 x = (f32)((f64)visitor->center[0] - (f64)light[0]);
    f32 y = (f32)((f64)visitor->center[1] - (f64)light[1]);
    f32 z = (f32)((f64)visitor->center[2] - (f64)light[2]);
    f32 squared_distance = (f32)((f64)x * (f64)x +
                                 (f64)y * (f64)y +
                                 (f64)z * (f64)z);

    // the game sorts lights using a whole-number copy of their squared distance
    if (squared_distance < 1.8446744e19f) {
        point_light_candidate &candidate = visitor->candidates[visitor->count++];

        candidate.index            = index;
        candidate.integer_distance = (i32)(i64)squared_distance;
        candidate.squared_distance = squared_distance;
    }

    return 0;
}

static point_light_visitor_vtable point_light_visitor_methods {
    nullptr,
    visit_point_light
};

// this is the same heap sort the original game uses; changing it can pick
// different lights when two of them are the same distance away
void push_point_light_heap(point_light_candidate* candidates,
                           i32                    hole_index,
                           i32                    top_index,
                           point_light_candidate  value) {

    i32 parent_index = (hole_index - 1) / 2;

    while (top_index < hole_index) {
        if (candidates[parent_index].integer_distance >= value.integer_distance)
            break;

        candidates[hole_index] = candidates[parent_index];
        hole_index = parent_index;
        parent_index = (parent_index - 1) / 2;
    }

    candidates[hole_index] = value;
}

void adjust_point_light_heap(point_light_candidate* candidates,
                                    i32                    hole_index,
                                    i32                    count,
                                    point_light_candidate  value) {

    i32 top_index = hole_index;
    i32 second_child = 2 * hole_index + 2;

    while (second_child < count) {
        if (candidates[second_child].integer_distance <
            candidates[second_child - 1].integer_distance) {

            --second_child;
        }

        candidates[hole_index] = candidates[second_child];
        hole_index = second_child;
        second_child = 2 * second_child + 2;
    }

    if (second_child == count) {
        candidates[hole_index] = candidates[count - 1];
        hole_index = count - 1;
    }

    push_point_light_heap(candidates, hole_index, top_index, value);
}

void make_point_light_heap(point_light_candidate* candidates, i32 count) {
    i32 parent_index = count / 2;

    while (parent_index > 0) {
        --parent_index;

        point_light_candidate value = candidates[parent_index];
        adjust_point_light_heap(candidates,
                                parent_index,
                                count,
                                value);
    }
}

void sort_point_light_heap(point_light_candidate* candidates, i32 count) {
    while (count > 1) {
        --count;

        point_light_candidate value = candidates[count];
        candidates[count] = candidates[0];
        adjust_point_light_heap(candidates, 0, count, value);
    }
}

i32 select_nearest_point_lights(point_light_visitor* visitor) {
    i32 selected_count = visitor->count > 8 ? 8 : visitor->count;

    if (selected_count > 1)
        make_point_light_heap(visitor->candidates, selected_count);

    for (u32 index = selected_count; index < visitor->count; ++index) {
        if (visitor->candidates[index].integer_distance >=
            visitor->candidates[0].integer_distance) {

            continue;
        }

        point_light_candidate value = visitor->candidates[index];
        visitor->candidates[index] = visitor->candidates[0];
        adjust_point_light_heap(visitor->candidates,
                                0,
                                selected_count,
                                value);
    }

    sort_point_light_heap(visitor->candidates, selected_count);

    return selected_count;
}

void query_point_lights(     ngl::fx::mesh_node_data* node_data,
                               const f32*                    sphere,
                                     f32                     radius) {

    ngl::lighting::light_context* context = selected_light_context.read();

    if (context->head.next == &context->head)
        return;

    point_light_visitor visitor;
    visitor.vtable        = &point_light_visitor_methods;
    visitor.reserved_004  = 0;
    visitor.count         = 0;
    visitor.reserved_200C = 0;

    for (u32 index = 0; index < 4; ++index)
        visitor.center[index] = sphere[index];

    using spatial_query_function =
        i32(__thiscall*)(void*, f32*, f32, point_light_visitor*, i32);

    void* spatial_index = *(void**)context->platform_state_040;

    util::gimmie::fn<spatial_query_function>
        (0x0095C270)(spatial_index,
                     (f32*)sphere,
                     radius,
                     &visitor,
                     (i32)0xDEADBEEF);

    i32 count = select_nearest_point_lights(&visitor);

    for (i32 index = 0; index < count; ++index)
        node_data->point_light_indices[index] = (u8)visitor.candidates[index].index;

    node_data->point_light_count = (u8)count;
}

ngl::lighting::light_context* ngl::fx::prepare_light_context(
    const mesh_node_data* node_data) {

    // the game sets this byte to 1 before setting up the mesh's lights
    node_data->mesh_data[0x0B] = 0;
    *(u32*)(node_data->mesh_data + 0x08) |= 0x01000000;

    lighting::light_context* context;

    if (has_scene_parameter(node_data->parameters, parameter_id_light_context.read()))
        context = (lighting::light_context*)get_scene_parameter(
            node_data->parameters,
            parameter_id_light_context.read());
    else
        context = ngl::references::current_scene.read()->light_context;

    selected_light_context.write(context);
    context->head.local_next = &context->head;

    return context;
}

void gather_point_lights(      ngl::fx::mesh_node_data* node_data,
                         const ngl::mesh_section*       section) {

    f32 x = section->sphere[0];
    f32 y = section->sphere[1];
    f32 z = section->sphere[2];

    const f32* matrix = (const f32*)&node_data->local_to_world;

    f32 sphere[4] { (f32)((f64)matrix[0] * (f64)x +
                          (f64)matrix[4] * (f64)y +
                          (f64)matrix[8] * (f64)z +
                          (f64)matrix[12]),
                    (f32)((f64)matrix[1] * (f64)x +
                          (f64)matrix[5] * (f64)y +
                          (f64)matrix[9] * (f64)z +
                          (f64)matrix[13]),
                    (f32)((f64)matrix[2]  * (f64)x +
                          (f64)matrix[6]  * (f64)y +
                          (f64)matrix[10] * (f64)z +
                          (f64)matrix[14]),
                    (f32)((f64)matrix[3]  * (f64)x +
                          (f64)matrix[7]  * (f64)y +
                          (f64)matrix[11] * (f64)z +
                          (f64)matrix[15]) };

    f32 scale = (*(u32*)node_data->node_info & 2) ? node_data->scale : 1.0f;

    ngl::scene_parameters* parameters = node_data->parameters;
    ngl::fx::prepare_light_context(node_data);

    if (!point_light_manager.read())
        return;

    f32 radius;

    if (ngl::has_scene_parameter(parameters, parameter_id_light_sphere.read())) {
        const f32* adjustment = (const f32*)ngl::get_scene_parameter
            (parameters, parameter_id_light_sphere.read());

        sphere[0] += adjustment[0];
        sphere[1] += adjustment[1];
        sphere[2] += adjustment[2];
        sphere[3] += adjustment[3];
        radius = adjustment[3];
    } else
        radius = *(f32*)(node_data->mesh_data + 0x2C);

    query_point_lights(node_data, sphere, radius * scale);
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

    // room for sixteen names in the form "0x12345678"
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
        build_animated_texture_parameter_chain(material_data);

    scene_parameters* parameters = node_data->parameters;
    u32 parameter_id = parameter_id_ifl_frame.read();

    if (has_scene_parameter(parameters, parameter_id))
        ifl_frame.write((i32)get_scene_parameter(parameters, parameter_id));
    else
        ifl_frame.write(ngl::references::current_scene.read()->ifl_frame);
}
