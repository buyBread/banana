#include <algorithm>
#include <cmath>
#include <utility>

#include "game/light/lighting.hh"
#include "game/light/references.hh"
#include "game/math/references.hh"
#include "game/rtree.hh"
#include "treyarch/ngl/fx/references.hh"
#include "treyarch/ngl/ngl.hh"
#include "treyarch/ngl/scene/scene.hh"
#include "vc8/algorithm.hh"
#include "banana/util/memory_reference.hh"

using namespace treyarch;

namespace treyarch { namespace references {
    util::memory_reference<math::vector4> lighting_direction_scale { 0x00E7CCE0 };
}} // treyarch::references

// the builders' row-vector transforms, every lane of the rows included
math::vector4 transform_point(const math::vector4 &value, const math::mat44 &matrix) {
    return math::vector4(value[0] * matrix[0][0] + value[1] * matrix[1][0] + value[2] * matrix[2][0] + matrix[3][0],
                         value[0] * matrix[0][1] + value[1] * matrix[1][1] + value[2] * matrix[2][1] + matrix[3][1],
                         value[0] * matrix[0][2] + value[1] * matrix[1][2] + value[2] * matrix[2][2] + matrix[3][2],
                         value[0] * matrix[0][3] + value[1] * matrix[1][3] + value[2] * matrix[2][3] + matrix[3][3]);
}

math::vector4 transform_direction(const math::vector4 &value, const math::mat44 &matrix) {
    return math::vector4(value[0] * matrix[0][0] + value[1] * matrix[1][0] + value[2] * matrix[2][0],
                         value[0] * matrix[0][1] + value[1] * matrix[1][1] + value[2] * matrix[2][1],
                         value[0] * matrix[0][2] + value[1] * matrix[1][2] + value[2] * matrix[2][2],
                         0.0f);
}

// planes go through the rows instead
math::vector4 transform_plane(const math::vector4 &value, const math::mat44 &matrix) {
    return math::vector4(value[0] * matrix[0][0] + value[1] * matrix[0][1] + value[2] * matrix[0][2],
                         value[0] * matrix[1][0] + value[1] * matrix[1][1] + value[2] * matrix[1][2],
                         value[0] * matrix[2][0] + value[1] * matrix[2][1] + value[2] * matrix[2][2],
                         value[0] * matrix[3][0] + value[1] * matrix[3][1] + value[2] * matrix[3][2] + value[3]);
}

math::vector4 multiply_lanes(const math::vector4 &a, const math::vector4 &b) {
    return math::vector4(a[0] * b[0], a[1] * b[1], a[2] * b[2], a[3] * b[3]);
}

// inlined @ sub_7C7EE0
// inlined @ sub_7DE410
void set_general_light_category(const ngl::fx::mesh_node_data* node_data) {
    // the mesh flags' top byte picks the light list, one past its index
    node_data->mesh_data[0x0B] = 0;
    *(u32*)(node_data->mesh_data + 0x08) |= ngl::lighting::light_category_general;
}

// inlined @ sub_7C7EE0
// inlined @ sub_7DE410
void select_light_context(const ngl::fx::mesh_node_data* node_data) {
    u32 parameter_id = ngl::fx::references::parameter_id_light_context.read();
    ngl::lighting::light_context* context;

    if (ngl::has_scene_parameter(node_data->parameters, parameter_id))
        context = (ngl::lighting::light_context*)ngl::get_scene_parameter(node_data->parameters, parameter_id);
    else
        context = ngl::references::build_scene.read()->light_context;

    ngl::lighting::references::send_light_context.write(context);
    context->head.local_next = &context->head;
}

// inlined @ sub_7C5720
// inlined @ sub_7C86D0
void write_ambient_and_fog(lighting_parameters* value, const light_source_data* source) {
    value->ambient_info = math::vector4(source->unk_278, source->unk_270, source->unk_274, source->unk_27c);
    value->fog_color    = source->depth_fog_color;

    f32 start = source->depth_fog_start;
    f32 end   = source->depth_fog_end;

    if (start == end)
        end += 0.00001525879997643642f;

    f32 scale = 1.0f / (end - start);

    value->fog_control = math::vector4(scale, -start * scale, source->unk_2e8, 0.0025f);
}

// inlined @ sub_7B79D0
bool light_reaches_mesh(const math::vector4 &position,
                              f32            radius,
                              f32            mesh_radius,
                        const math::vector4 &world_center,
                        const math::mat44   &scaled_world_to_local,
                              f32            inverse_scale,
                        const math::vector4 &extents) {

    math::vector4 offset(position[0] - world_center[0],
                         position[1] - world_center[1],
                         position[2] - world_center[2],
                         0.0f);

    f32 reach = radius + mesh_radius;

    if (!(reach * reach >= offset[0] * offset[0] + offset[1] * offset[1] + offset[2] * offset[2]))
        return false;

    // how far the light sits outside the mesh's box, in local units
    math::vector4 local = transform_direction(offset, scaled_world_to_local);
    math::vector4 outside;

    for (i32 axis = 0; axis < 3; ++axis) {
        f32 distance = std::fabs(local[axis]);

        outside[axis] = distance - (distance <= extents[axis] ? distance : extents[axis]);
    }

    f32 local_radius = radius * inverse_scale;

    return local_radius * local_radius >= outside[0] * outside[0] + outside[1] * outside[1] + outside[2] * outside[2];
}

// inlined @ sub_7B79D0
void add_generated_light(      general_lighting_parameters*         value,
                         const ngl::lighting::generated_light_data* light,
                         const math::mat44                         &local_to_world,
                         const math::mat44                         &world_to_local,
                               f32                                  inverse_scale) {

    i32 index = value->light_count;

    f32 outer         = light->outer_radius * inverse_scale;
    f32 inner         = light->inner_radius * inverse_scale;
    f32 outer_squared = outer * outer;
    f32 radial_scale  = 1.0f / (inner * inner - outer_squared);
    f32 cone_scale    = 1.0f / (light->attenuation_inner - light->attenuation_outer);

    math::vector4 direction = transform_direction(light->direction, world_to_local);
    direction.set_w(light->direction_w);

    value->light_positions[index]   = transform_point(light->position, world_to_local);
    value->light_directions[index]  = direction;
    value->light_attenuation[index] = math::vector4(radial_scale,
                                                    -outer_squared * radial_scale,
                                                    cone_scale,
                                                    -light->attenuation_outer * cone_scale);
    value->light_colors[index]      = light->color;

    if (!value->projector_texture && light->projector_texture) {
        value->projector_texture  = light->projector_texture;
        value->projector_matrix   = math::mat44(*(const math::mat43*)&local_to_world *
                                                *(const math::mat43*)&light->projector_matrix);
        value->projector_index    = index;
        value->projector_config_0 = light->projector_config_0;
        value->projector_config_1 = light->projector_config_1;
    }
}

// inlined @ sub_7B79D0
void add_point_light(      general_lighting_parameters*     value,
                     const ngl::lighting::point_light_data* light,
                     const math::mat44                     &world_to_local,
                           f32                              local_radius) {

    i32 index = value->light_count;

    // the generated lights' falloff with no inner radius
    f32 radius_squared = local_radius * local_radius;
    f32 radial_scale   = 1.0f / (0.0f - radius_squared);

    value->light_positions[index]   = transform_point(light->position, world_to_local);
    value->light_colors[index]      = light->color;
    value->light_directions[index]  = math::vector4(0.0f, -1.0f, 0.0f, 0.0f);
    value->light_attenuation[index] = math::vector4(radial_scale, -radius_squared * radial_scale, 0.0f, 1.0f);
}

// inlined @ sub_7C7EE0
void swap_lights(general_lighting_parameters* value, i32 first, i32 second) {
    std::swap(value->light_positions[first],   value->light_positions[second]);
    std::swap(value->light_colors[first],      value->light_colors[second]);
    std::swap(value->light_directions[first],  value->light_directions[second]);
    std::swap(value->light_attenuation[first], value->light_attenuation[second]);
}

bool is_dark(const math::vector4 &color) {
    return color[0] + color[1] + color[2] < 0.01f;
}

// sub_7AD310
math::mat44* treyarch::get_scaled_world_to_local(const ngl::fx::mesh_node_data* node_data, math::mat44* destination) {
    if (!(node_data->node_info[0] & 2)) {
        *destination = math::inv(*(const math::rot_tran_mat43*)&node_data->local_to_world);

        return destination;
    }

    // a scaled node's columns get its inverse scale squared
    const math::vector4 &scale = *(const math::vector4*)(node_data->node_info + 0x10);
    math::vector4        inverse_squared;

    for (i32 column = 0; column < 4; ++column) {
        f32 inverse = 1.0f / scale[column];

        inverse_squared[column] = inverse * inverse;
    }

    math::mat44 world_to_local = math::inv(*(const math::rot_tran_mat43*)&node_data->local_to_world);

    for (i32 row = 0; row < 4; ++row)
        (*destination)[row] = multiply_lanes(world_to_local[row], inverse_squared);

    return destination;
}

// sub_7ADB90
lighting_ambient_defaults::lighting_ambient_defaults() {
    for (i32 index = 0; index < 9; ++index)
        values[index] = references::lighting_default_value.get();
}

// sub_7ADC90
bool general_lighting_parameters::add_directional_light(const ngl::lighting::generated_light_data* light,
                                                        const math::mat44                         &world_to_local,
                                                        const math::mat44                         &,
                                                        const math::vector4                       &attenuation,
                                                              f32                                  scale) {

    if (light_count >= 4)
        return false;

    math::vector4 direction = transform_direction(light->direction, world_to_local);

    light_positions[light_count] = math::vector4(direction[0] * -10000.0f,
                                                 direction[1] * -10000.0f,
                                                 direction[2] * -10000.0f,
                                                 0.0f);

    if (light->flags & 4)
        special_light_index = light_count;

    direction.set_w(light->direction_w);

    light_directions[light_count]  = direction;
    light_attenuation[light_count] = attenuation;
    light_colors[light_count]      = math::vector4(light->color[0] * scale,
                                                   light->color[1] * scale,
                                                   light->color[2] * scale,
                                                   light->color[3] * scale);

    ++light_count;

    return true;
}

// sub_7B4690
i32 treyarch::get_light_table_index(const math::vector4 &center, const math::vector4 &extents) {
    return center[1] + extents[1] <= 12.0f ? 0 : 5;
}

// sub_7B79D0
void general_lighting_parameters::gather_local_lights(const ngl::fx::mesh_node_data* node_data,
                                                      const math::mat44             &local_to_world,
                                                      const math::mat44             &world_to_local,
                                                      const math::mat44             &scaled_world_to_local,
                                                      const math::vector4           &world_center,
                                                            f32                      inverse_scale,
                                                            bool                     flagged_only,
                                                            bool                     skinned) {

    if (light_count >= 4 || !node_data->mesh_data[0x0B])
        return;

    const ngl::mesh* mesh = (const ngl::mesh*)node_data->mesh_data;

    // the light source keeps a list for each filter
    i32 list        = flagged_only + 2 * skinned;
    f32 mesh_radius = node_data->scale * mesh->sphere[3];

    for (i32 index = 0; index < light_source->generated_light_counts[list]; ++index) {
        const ngl::lighting::generated_light_data* light = &light_source->generated_lights[list][index];

        if (!light_reaches_mesh(light->position,
                                light->outer_radius,
                                mesh_radius,
                                world_center,
                                scaled_world_to_local,
                                inverse_scale,
                                mesh->position_scale)) {

            continue;
        }

        add_generated_light(this, light, local_to_world, world_to_local, inverse_scale);

        if (++light_count >= 4)
            return;
    }

    const ngl::lighting::light_context* context = ngl::lighting::references::send_light_context.read();

    for (const ngl::lighting::light_node* node = context->head.next; node != &context->head; node = node->next) {
        if (node->type == ngl::lighting::light_generated) {
            const auto* light = (const ngl::lighting::generated_light_data*)node->node_data;

            if ((flagged_only && !(light->flags & 1)) || (!skinned && (light->flags & 2)))
                continue;

            if (!light_reaches_mesh(light->position,
                                    light->outer_radius,
                                    mesh_radius,
                                    world_center,
                                    scaled_world_to_local,
                                    inverse_scale,
                                    mesh->position_scale)) {

                continue;
            }

            add_generated_light(this, light, local_to_world, world_to_local, inverse_scale);
        } else if (node->type == ngl::lighting::light_point) {
            const auto* light  = (const ngl::lighting::point_light_data*)node->node_data;
            f32         radius = light->position[3];

            if (!light_reaches_mesh(light->position,
                                    radius,
                                    mesh_radius,
                                    world_center,
                                    scaled_world_to_local,
                                    inverse_scale,
                                    mesh->position_scale)) {

                continue;
            }

            ngl::lighting::point_light_data scratch;

            add_point_light(this,
                            ngl::lighting::get_point_light(&scratch, node, world_center),
                            world_to_local,
                            inverse_scale * radius);
        } else
            continue;

        if (++light_count >= 4)
            return;
    }
}

// sub_7B9610
void lighting_parameters::set_light_source(const ngl::fx::mesh_node_data* node_data) {
    u32 parameter_id = ngl::fx::references::parameter_id_light_source.read();

    if (ngl::has_scene_parameter(node_data->parameters, parameter_id)) {
        light_source = (light_source_data*)ngl::get_scene_parameter(node_data->parameters, parameter_id);

        return;
    }

    light_source = (light_source_data*)ngl::get_scene_parameter(ngl::references::build_scene.read()->parameters,
                                                                ngl::fx::references::parameter_id_scene_light_source.read());
}

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

// sub_7BC640
void light_table::get_projection(i32 index, i32 offset, math::vector4* destination) const {
    i32 projection = index + offset;

    if (index == -1 || projection < 0 || projection >= projection_count) {
        destination[1] = references::lighting_zero_value.get();
        destination[0] = destination[1];

        return;
    }

    destination[0] = projections[2 * projection + 0];
    destination[1] = projections[2 * projection + 1];
}

// sub_7C5390
void general_lighting_parameters::reset() {
    const math::vector4 &zero = references::lighting_zero_value.get();
    math::vector4        ones(math::references::float4_ones.get());

    reserved_000            = 0;
    directional_light_count = 0;
    light_source            = nullptr;
    light_count             = 0;

    material_values[0] = ones;
    material_values[1] = zero;
    material_values[2] = ones;
    material_values[3] = zero;

    for (i32 index = 0; index < 4; ++index)
        light_positions[index] = light_colors[index] = light_directions[index] = light_attenuation[index] = zero;

    projector_matrix    = math::identity_mat33();
    projector_texture   = nullptr;
    projector_index     = 0;
    projector_config_0  = 4;
    projector_config_1  = 4;
    special_light_index = -1;

    ambient_defaults = lighting_ambient_defaults();
}

// sub_7C5720
void general_lighting_parameters::build_environment(const math::mat44        &local_to_world,
                                                    const math::mat44        &world_to_local,
                                                          bool                skinned,
                                                          bool,
                                                    const light_table_lookup* lookup) {

    const ngl::scene*        build_scene = ngl::references::build_scene.read();
    const light_source_data* source      = light_source;
    const sky_data*          sky         = source->sky;

    const math::vector4 &zero = references::lighting_zero_value.get();
    math::vector4        ones(math::references::float4_ones.get());
    math::vector4        contribution = zero;

    const auto &view = build_scene->view_to_world.get_w();

    view_position     = transform_point(math::vector4(view[0], view[1], view[2], 1.0f), world_to_local);
    ambient_direction = transform_direction(source->ambient_direction, world_to_local);

    // a plane: the normal goes to local space, the distance follows local_to_world's translation
    math::vector4 ambient = source->world_to_ambient;

    ambient_color = transform_direction(ambient, world_to_local);
    ambient_color.set_w(ambient[0] * local_to_world[3][0] +
                        ambient[1] * local_to_world[3][1] +
                        ambient[2] * local_to_world[3][2] +
                        ambient[3]);

    post_view_position = math::vector4(view_position[0], view_position[1], view_position[2], 0.0f);

    math::vector4 normal         = transform_direction(source->fog_normal, world_to_local);
    f32           inverse_length = 1.0f / std::sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);

    post_direction = math::vector4(inverse_length * normal[0],
                                   inverse_length * normal[1],
                                   inverse_length * normal[2],
                                   0.0f);

    post_plane_0 = transform_point(source->fog_position_0, world_to_local);
    post_plane_0.set_w(1.0f);

    post_plane_1 = transform_point(source->fog_position_1, world_to_local);
    post_plane_1.set_w(1.0f);

    // fogs per meter become fogs per local unit
    math::vector4 fog_axis = transform_direction(math::vector4(source->fogs_per_meter, 0.0f, 0.0f, 0.0f), world_to_local);

    post_range = math::vector4(std::sqrt(fog_axis[0] * fog_axis[0] + fog_axis[1] * fog_axis[1] + fog_axis[2] * fog_axis[2]),
                               source->unk_31c,
                               0.0f,
                               0.0f);
    post_color = math::vector4(source->color_260.x, source->color_260.y, source->color_260.z, 0.0f);

    write_ambient_and_fog(this, source);

    if (sky && (lookup->table || lookup->force_horizon)) {
        math::vector4        sky_direction = sky->direction_050;
        const math::vector4 &axis          = references::lighting_horizon_axis.get();
        f32                  half_width    = sky->unk_0c8;

        f32 facing = sky_direction[0] * axis[0] + sky_direction[1] * axis[1] + sky_direction[2] * axis[2];

        math::vector4 width(half_width, half_width, half_width, half_width);
        math::vector4 away  = ones - math::vector4(facing, facing, facing, facing);
        math::vector4 below = away - width;
        math::vector4 above = width + away;

        horizon_scalar.set_x(0.0f);

        horizon_range = math::vector4(below[0], above[1], 0.0f, 0.0f);
        horizon_bias  = math::vector4(math::references::float4_twos.get()) - math::vector4(above[0], below[1], 0.0f, 0.0f);

        horizon_direction        = transform_direction(sky_direction, world_to_local);
        horizon_direction_scaled = transform_direction(multiply_lanes(references::lighting_direction_scale.get(), sky_direction),
                                                       world_to_local);

        f32 denominator = half_width + half_width;

        horizon_color_scale = math::vector4(sky->color_040.x / denominator,
                                            sky->color_040.y / denominator,
                                            sky->color_040.z / denominator,
                                            1.0f / denominator);

        horizon_color = sky->unk_080;
        horizon_extra = sky->unk_0b0;
        horizon_axis  = world_to_local[1];
        horizon_scale = 1.0f;

        if (lookup->offset == 4) {
            horizon_color = multiply_lanes(horizon_color, math::vector4(math::references::float4_half.get()));
            horizon_scale = 0.5f;
        }

        // the extra term fades out over the first 20 meters of height
        if (lookup->fade_horizon_extra) {
            f32 height = local_to_world[3][1] / 20.0f;
            f32 fade   = 1.0f;

            if (height < 0.0f)
                fade = 0.0f;
            else if (height <= 1.0f)
                fade = height;

            f32 remaining = 1.0f - fade;

            horizon_extra = math::vector4(horizon_extra[0] * remaining,
                                          horizon_extra[1] * remaining,
                                          horizon_extra[2] * remaining,
                                          horizon_extra[3] * remaining);
        }

        if (lookup->table) {
            math::vector4 projections[2];

            lookup->table->get_projection(lookup->index, lookup->offset, projections);

            horizon_projection_u = transform_plane(projections[0], local_to_world);
            horizon_projection_v = transform_plane(projections[1], local_to_world);
        }
    } else {
        const math::vector4 &default_value = references::lighting_default_value.get();

        horizon_range            = zero;
        horizon_color_scale      = zero;
        horizon_color            = zero;
        horizon_direction        = math::vector4(default_value[0], default_value[1], default_value[2], 0.0f);
        horizon_axis             = default_value;
        horizon_direction_scaled = default_value;
        horizon_bias             = zero;
        horizon_extra            = zero;

        horizon_scalar.set_x(0.0f);
    }

    // the sky is the primary light, far away along its direction
    if (sky) {
        math::vector4 sky_direction = sky->direction_050;
        math::vector4 direction     = transform_direction(sky_direction, world_to_local);

        light_positions[light_count]   = math::vector4(direction[0] * 10000.0f,
                                                       direction[1] * 10000.0f,
                                                       direction[2] * 10000.0f,
                                                       0.0f);
        light_directions[light_count]  = transform_direction(-sky_direction, world_to_local);
        light_attenuation[light_count] = math::vector4(0.0f, 1.0f, 0.0f, 1.0f);
        light_colors[light_count]      = sky->color_040;

        ++light_count;

        contribution = multiply_lanes(sky->unk_080, math::vector4(math::references::float4_half.get()));
    }

    for (i32 index = 0; index < 8; ++index)
        ambient_colors[index] = multiply_lanes(source->ambient_color_lo[index], ones);

    for (i32 index = 0; index < 8; ++index)
        ambient_colors_with_primary[index] = contribution + multiply_lanes(source->ambient_color_hi[index], ones);

    for (i32 index = 0; index < source->directional_light_count; ++index) {
        const ngl::lighting::generated_light_data* light = &source->directional_lights[index];

        if ((!skinned && (light->flags & 2)) || light->activity == 0.0f)
            continue;

        if (!add_directional_light(light, world_to_local, local_to_world, math::vector4(0.0f, 1.0f, 0.0f, 1.0f), 1.0f))
            break;
    }
}

// sub_7C7EE0
void general_lighting_parameters::build_lights(const ngl::fx::mesh_node_data* node_data,
                                               const ngl::mesh_section*       section,
                                               const math::mat44             &local_to_world,
                                               const math::mat44             &world_to_local,
                                               const math::mat44             &scaled_world_to_local,
                                                     bool                     flagged_only,
                                               const light_table_lookup*      lookup,
                                                     bool                     gather_local,
                                                     bool                     drop_dark_lights) {

    const ngl::mesh* mesh = (const ngl::mesh*)node_data->mesh_data;

    math::vector4 world_center  = transform_point(section->sphere, *(const math::mat44*)&node_data->local_to_world);
    bool          skinned       = mesh->skeleton != nullptr;
    f32           inverse_scale = (node_data->node_info[0] & 2) ? 1.0f / node_data->scale : 1.0f;

    set_general_light_category(node_data);
    set_light_source(node_data);
    select_light_context(node_data);

    build_environment(local_to_world, world_to_local, skinned, flagged_only, lookup);

    if (!gather_local)
        return;

    gather_local_lights(node_data,
                        local_to_world,
                        world_to_local,
                        scaled_world_to_local,
                        world_center,
                        inverse_scale,
                        flagged_only,
                        skinned);

    // the projector's light goes to slot 1, the special light to slot 0
    if (projector_texture && projector_index != 1) {
        swap_lights(this, projector_index, 1);

        light_count = std::max(light_count, 2);

        if (special_light_index == 1)
            special_light_index = projector_index;

        projector_index = 1;
    }

    if (special_light_index != -1) {
        swap_lights(this, special_light_index, 0);

        special_light_index = 0;
    }

    if (!drop_dark_lights)
        return;

    // dark lights move behind the lit ones and fall off the end
    for (i32 index = 0; index < light_count; ++index) {
        if (!is_dark(light_colors[index]))
            continue;

        if (index == projector_index)
            projector_index = -1;

        i32 next = index + 1;

        while (next < light_count && is_dark(light_colors[next]))
            ++next;

        if (next < light_count)
            swap_lights(this, index, next);
        else
            light_count = index;
    }
}

// sub_7C8400
void general_lighting_parameters::build_local(const ngl::fx::mesh_node_data* node_data,
                                              const ngl::mesh_section*       section,
                                              const math::mat44             &local_to_world,
                                              const math::mat44             &world_to_local,
                                                    bool                     flagged_only,
                                              const light_table_lookup*      lookup,
                                                    bool                     gather_local,
                                                    bool                     drop_dark_lights) {

    math::mat44 scaled_world_to_local;

    build_lights(node_data,
                 section,
                 local_to_world,
                 world_to_local,
                 *get_scaled_world_to_local(node_data, &scaled_world_to_local),
                 flagged_only,
                 lookup,
                 gather_local,
                 drop_dark_lights);
}

// sub_7C8450
void general_lighting_parameters::build(const ngl::fx::mesh_node_data* node_data,
                                        const ngl::mesh_section*       section,
                                              bool                     flagged_only,
                                        const light_table_lookup*      lookup,
                                              bool                     gather_local) {

    math::mat44 world_to_local = math::identity_mat33();
    math::mat44 local_to_world = math::identity_mat33();
    math::mat44 scaled_world_to_local;

    build_lights(node_data,
                 section,
                 local_to_world,
                 world_to_local,
                 *get_scaled_world_to_local(node_data, &scaled_world_to_local),
                 flagged_only,
                 lookup,
                 gather_local,
                 false);

    if (node_data->node_info[0] & 2) {
        f32 inverse_scale_squared = 1.0f / (node_data->scale * node_data->scale);

        for (i32 index = 0; index < 4; ++index)
            light_attenuation[index][0] *= inverse_scale_squared;
    }
}

// sub_7C86B0
lighting_parameters::lighting_parameters() : reserved_000(0), directional_light_count(0), light_source(nullptr) {}

// sub_7C86D0
void lighting_parameters::build(const ngl::fx::mesh_node_data* node_data,
                                const ngl::mesh_section*,
                                const light_table_lookup*      lookup) {

    set_light_source(node_data);

    const sky_data* sky = light_source->sky;

    write_ambient_and_fog(this, light_source);

    if (sky) {
        lighting_directional_light &light = directional_lights[directional_light_count];

        light.direction = sky->direction_050;
        light.color     = math::vector4(sky->color_040.x, sky->color_040.y, sky->color_040.z, 1.0f);

        ++directional_light_count;
    }

    if (lookup->table) {
        math::vector4 projections[2];

        lookup->table->get_projection(lookup->index, lookup->offset, projections);

        horizon_projection_u = projections[0];
        horizon_projection_v = projections[1];
    }
}

// sub_7D6300
general_lighting_parameters::general_lighting_parameters() {
    reset();
}

// sub_7DD9F0
i32 lighting_visitor::select_nearest() {
    u32 nearest = count > 8 ? 8 : count;

    vc8::partial_sort(candidates, candidates + nearest, candidates + count);

    return (i32)nearest;
}

// sub_7DDA40
void point_light_set::query(      ngl::fx::mesh_node_data* node_data,
                            const ngl::mesh_section*,
                            const math::vector4           &center,
                                  f32                      radius) {

    const ngl::lighting::light_context* context = ngl::lighting::references::send_light_context.read();

    if (!node_data->mesh_data[0x0B] || context->head.next == &context->head)
        return;

    lighting_visitor visitor;
    visitor.count         = 0;
    visitor.reserved_200c = 0;
    visitor.center        = center;

    query_sphere(*(rtree* const*)context->platform_state_040, *(const vector3d*)&center, radius, &visitor);

    i32 nearest = visitor.select_nearest();

    for (i32 index = 0; index < nearest; ++index)
        node_data->point_light_indices[index] = (u8)visitor.candidates[index].index;

    count                        = nearest;
    node_data->point_light_count = (u8)nearest;
}

// sub_7DE410
i32 point_light_set::gather(      ngl::fx::mesh_node_data* node_data,
                            const ngl::mesh_section*       section,
                            const math::vector4           &center,
                                  f32                      scale) {

    set_general_light_category(node_data);

    math::vector4 sphere = center;

    select_light_context(node_data);

    if (!references::light_manager.read())
        return count;

    u32 parameter_id = ngl::fx::references::parameter_id_light_sphere.read();
    f32 radius;

    if (ngl::has_scene_parameter(node_data->parameters, parameter_id)) {
        const math::vector4 &adjustment = *(const math::vector4*)ngl::get_scene_parameter(node_data->parameters, parameter_id);

        sphere += adjustment;
        radius  = adjustment[3];
    } else
        radius = ((const ngl::mesh*)node_data->mesh_data)->sphere[3];

    query(node_data, section, sphere, radius * scale);

    return count;
}
