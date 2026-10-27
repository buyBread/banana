#include <cstdio>
#include <cstddef>

#include "treyarch/ngl/ngl.hh"
#include "treyarch/ngl/mesh/mesh.hh"
#include "treyarch/ngl/scene/scene.hh"
#include "treyarch/ngl/texture/texture.hh"
#include "treyarch/ngl/fx/parameters.hh"
#include "treyarch/ngl/fx/references.hh"
#include "treyarch/ngl/fx/render_support.hh"
#include "game/light/references.hh"
#include "game/rtree.hh"
#include "treyarch/ngl/lighting/lighting.hh"

using namespace treyarch;

struct point_light_candidate {
    u32 index;
    u32 integer_distance;
    f32 squared_distance;
    u32 reserved;
};

struct lighting_visitor : subdivision_visitor {
    point_light_candidate candidates[512];
    u32                   count;
    u32                   reserved_200C;
    math::vector4         center;

    e_visit_result visit(subdivision_leaf_handle leaf_handle) override;
};

ASSERT_SIZEOF(point_light_candidate, 0x10);

ASSERT_SIZEOF  (lighting_visitor,             0x2020);
ASSERT_OFFSETOF(lighting_visitor, candidates, 0x0008);
ASSERT_OFFSETOF(lighting_visitor, count,      0x2008);
ASSERT_OFFSETOF(lighting_visitor, center,     0x2010);

// sub_7B9680
e_visit_result lighting_visitor::visit(subdivision_leaf_handle leaf_handle) {
    u32 index = (u32)leaf_handle;

    const auto &light = (&ngl::lighting::references::point_lights.get())[index];
    f32 x = (f32)((f64)center[0] - (f64)light.position[0]);
    f32 y = (f32)((f64)center[1] - (f64)light.position[1]);
    f32 z = (f32)((f64)center[2] - (f64)light.position[2]);
    f32 squared_distance = (f32)((f64)x * (f64)x +
                                 (f64)y * (f64)y +
                                 (f64)z * (f64)z);

    // the game sorts lights using a whole-number copy of their squared distance
    if (squared_distance < 1.8446744e19f) {
        point_light_candidate &candidate = candidates[count++];

        candidate.index            = index;
        candidate.integer_distance = (u32)(i64)squared_distance;
        candidate.squared_distance = squared_distance;
    }

    return continue_visiting;
}

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

// sub_7DD9F0
i32 select_nearest_point_lights(lighting_visitor* visitor) {
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

// sub_7DDA40
void query_point_lights(      ngl::fx::mesh_node_data*  node_data,
                        const math::vector4            &sphere,
                              f32                       radius) {

    ngl::lighting::light_context* context =
        ngl::lighting::references::send_light_context.read();

    if (context->head.next == &context->head)
        return;

    lighting_visitor visitor;
    visitor.count         = 0;
    visitor.reserved_200C = 0;

    visitor.center = sphere;

    rtree* tree = *(rtree**)context->platform_state_040;
    query_sphere(tree, *(const vector3d*)&sphere, radius, &visitor);

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

    if (has_scene_parameter(node_data->parameters, references::parameter_id_light_context.read()))
        context = (lighting::light_context*)get_scene_parameter(
            node_data->parameters,
            references::parameter_id_light_context.read());
    else
        context = ngl::references::build_scene.read()->light_context;

    lighting::references::send_light_context.write(context);
    context->head.local_next = &context->head;

    return context;
}

// sub_884C70
void gather_point_lights(      ngl::fx::mesh_node_data* node_data,
                         const ngl::mesh_section*       section) {

    const math::vector4   &local_sphere = section->sphere;
    const math::dir3      &x            = node_data->local_to_world.get_x();
    const math::dir3      &y            = node_data->local_to_world.get_y();
    const math::dir3      &z            = node_data->local_to_world.get_z();
    const math::position3 &w            = node_data->local_to_world.get_w();

    // every lane of the rows, the unused fourth one included
    math::vector4 sphere((f32)((f64)x[0] * (f64)local_sphere[0] +
                               (f64)y[0] * (f64)local_sphere[1] +
                               (f64)z[0] * (f64)local_sphere[2] +
                               (f64)w[0]),
                         (f32)((f64)x[1] * (f64)local_sphere[0] +
                               (f64)y[1] * (f64)local_sphere[1] +
                               (f64)z[1] * (f64)local_sphere[2] +
                               (f64)w[1]),
                         (f32)((f64)x[2] * (f64)local_sphere[0] +
                               (f64)y[2] * (f64)local_sphere[1] +
                               (f64)z[2] * (f64)local_sphere[2] +
                               (f64)w[2]),
                         (f32)((f64)x[3] * (f64)local_sphere[0] +
                               (f64)y[3] * (f64)local_sphere[1] +
                               (f64)z[3] * (f64)local_sphere[2] +
                               (f64)w[3]));

    f32 scale = (*(u32*)node_data->node_info & 2) ? node_data->scale : 1.0f;

    ngl::scene_parameters* parameters = node_data->parameters;
    ngl::fx::prepare_light_context(node_data);

    if (!treyarch::references::light_manager.read())
        return;

    f32 radius;

    if (ngl::has_scene_parameter(parameters, ngl::fx::references::parameter_id_light_sphere.read())) {
        const math::vector4 &adjustment = *(const math::vector4*)ngl::get_scene_parameter
            (parameters, ngl::fx::references::parameter_id_light_sphere.read());

        sphere += adjustment;
        radius = adjustment.get_w();
    } else
        radius = *(f32*)(node_data->mesh_data + 0x2C);

    query_point_lights(node_data, sphere, radius * scale);
}

// sub_9DF800
ngl::fx::effect* ngl::fx::shader_node::select_effect() {
    gather_point_lights(mesh_node, section);

    effect* base = effect_data;
    effect_runtime* runtime = base->runtime;
    u8 count = mesh_node->point_light_count;

    if (!count && runtime->zero_point_lights)
        return runtime->zero_point_lights;

    if (count <= 2 && runtime->two_point_lights)
        return runtime->two_point_lights;

    if (count <= 4 && runtime->four_point_lights)
        return runtime->four_point_lights;

    return base;
}

// inlined @ sub_9E0420, sub_9E0A50
void ngl::fx::prepare_effect_scene(effect* value) {
    u32 frame_epoch = ngl::references::frame.read();

    if (value->current_frame_epoch != frame_epoch) {
        value->current_frame_epoch = frame_epoch;
        value->current_scene       = nullptr;
        value->current_state_02c   = 0;
        value->current_material    = nullptr;
    }

    scene* build_scene = ngl::references::build_scene.read();

    if (value->current_scene != build_scene) {
        update_scene_parameters(value);
        value->current_scene = build_scene;
    }
}

// inlined @ sub_9E0420, sub_9E0A50
void ngl::fx::prepare_effect_material(effect* value, material* material_data) {
    if (value->current_material == material_data)
        return;

    if (material_data->bound_effect != value)
        bind_material(material_data, value);

    copy_material_parameters(material_data);
    value->current_material = material_data;
}

// inlined @ sub_9E0420, sub_9E0A50
void ngl::fx::prepare_animated_textures(effect* effect_data, shader_node* node) {
    material*  material_data = node->material_data;
    parameter* sentinel      = (parameter*)&references::parameter_chain_sentinel.get();

    if (material_data->animated_texture_parameter_chain == sentinel)
        return;

    if (!effect_data->animated_texture_parameter_chain)
        build_animated_texture_parameter_chain(effect_data);

    if (!material_data->animated_texture_parameter_chain)
        build_animated_texture_parameter_chain(material_data);

    node->set_ifl_frame();
}
