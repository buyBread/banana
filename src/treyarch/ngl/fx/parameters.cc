#include <cmath>
#include <cstring>

#include "treyarch/ngl/fx/lighting_parameters.hh"
#include "treyarch/ngl/fx/parameters.hh"
#include "treyarch/ngl/fx/render_support.hh"
#include "treyarch/ngl/lighting/context.hh"
#include "treyarch/ngl/mesh/mesh.hh"
#include "treyarch/ngl/ngl.hh"
#include "treyarch/ngl/scene/parameters.hh"
#include "treyarch/ngl/scene/references.hh"
#include "treyarch/ngl/texture/texture.hh"
#include "util/memory_reference.hh"

using namespace treyarch;
using namespace treyarch::ngl;
using ngl::fx::general_lighting_parameters;

static util::memory_reference<u32> parameter_id_light_source         { 0x010F7D78 };
static util::memory_reference<u32> parameter_id_scene_light_source   { 0x010F853C };
static util::memory_reference<u32> parameter_id_light_table          { 0x010F7D74 };
static util::memory_reference<u32> parameter_id_light_table_range    { 0x01116328 };
static util::memory_reference<u32> parameter_id_character_color      { 0x01116304 };
static util::memory_reference<u32> parameter_id_parameter_subset     { 0x0111630C };
static util::memory_reference<u32> parameter_id_environment_color    { 0x011162F0 };
static util::memory_reference<u32> parameter_id_decal_projection     { 0x011162FC };
static util::memory_reference<u32> parameter_id_ui_parameters        { 0x01116334 };
static util::memory_reference<u32> parameter_id_tint_color           { 0x01116320 };
static util::memory_reference<u32> parameter_id_decal_texture_matrix { 0x01116314 };
static util::memory_reference<u32> parameter_id_last                 { 0x01116318 };

static util::memory_reference<u8*>           game_state                  { 0x01111760 };
static util::memory_reference<ngl::texture*> default_texture             { 0x011187FC };
static util::memory_reference<ngl::texture*> horizon_texture             { 0x01118804 };
static util::memory_reference<ngl::texture*> framebuffer_texture         { 0x01123A1C };
static util::memory_reference<ngl::texture*> framebuffer_texture_general { 0x01123A20 };
static util::memory_reference<ngl::texture*> depth_texture               { 0x01123A28 };
static util::memory_reference<ngl::texture*> environment_texture         { 0x010FC58C };

static util::memory_reference<vector4>       shadow_distances { 0x01075F10 };
static util::memory_reference<matrix4x4>     shadow_matrix_0  { 0x01075F20 };
static util::memory_reference<matrix4x4>     shadow_matrix_2  { 0x01075F70 };
static util::memory_reference<ngl::texture*> shadow_texture_0 { 0x01075FC0 };
static util::memory_reference<ngl::texture*> shadow_texture_2 { 0x01075FC4 };

static util::memory_reference<u32> shadow_width_0  { 0x00E7AFC4 };
static util::memory_reference<u32> shadow_width_2  { 0x00E7AFC8 };
static util::memory_reference<u32> shadow_height_0 { 0x00E7AFCC };
static util::memory_reference<u32> shadow_height_2 { 0x00E7AFD0 };

static util::memory_reference<vector4> temporary_0          { 0x010F7E20 };
static util::memory_reference<f32>     shared_scalar        { 0x010F7D00 };
static util::memory_reference<vector4> constant_80          { 0x00F4AB70 };
static util::memory_reference<vector4> ui_parameters        { 0x010F7D50 };
static util::memory_reference<vector4> tint_color           { 0x00F4A9E0 };
static util::memory_reference<vector4> shadow_factor        { 0x010F8A40 };
static util::memory_reference<vector4> subset_shadow_factor { 0x010F8840 };

static util::memory_reference<f32> point_light_data    { 0x011189D0 };
static util::memory_reference<f32> bone_constant_data  { 0x01117240 };
static util::memory_reference<u32> bone_constant_count { 0x01117168 };

static util::memory_reference<vector4>                       lighting_default_value   { 0x01086F30 };
static util::memory_reference<vector4>                       lighting_zero_value      { 0x01086F50 };
static util::memory_reference<vector4>                       lighting_horizon_axis    { 0x01087690 };
static util::memory_reference<vector4>                       lighting_direction_scale { 0x00E7CCE0 };
static util::memory_reference<vector4>                       lighting_horizon_base    { 0x00E7CED0 };
static util::memory_reference<vector4>                       lighting_half            { 0x00E7CEB0 };

void write_texture(ngl::fx::parameter* entry, ngl::texture* value) {
    *(ngl::texture**)entry->data = value ? value : default_texture.read();
}

matrix4x4 get_unscaled_local_to_world(const ngl::fx::mesh_node_data* node_data) {
    matrix4x4 result = node_data->local_to_world;

    // remove the node's scale before making an inverse matrix,
    // divide each value separately because dividing the whole vector changes the result slightly
    if (node_data->node_info[0] & 2) {
        const vector3 &scales = *(const vector3*)(node_data->node_info + 0x10);

        for (u32 column = 0; column < 4; ++column) {
            result[0][column] /= scales.x;
            result[1][column] /= scales.y;
            result[2][column] /= scales.z;
        }
    }

    return result;
}

f32 get_hour_of_day() {
    u8* state = game_state.read();
    f32 seconds = (f32)*(u32*)(state + 188) + *(f32*)(state + 196);

    return seconds / 3600.0f;
}

void copy_parameter_subset(      ngl::fx::effect*         value,
                           const ngl::fx::mesh_node_data* node_data) {

    void* subset = find_scene_parameter(node_data->parameters,
                                  parameter_id_parameter_subset.read());

    if (!subset)
        return;

    // this table converts tentacle parameter ids into offsets inside ParamSubset
    // -1 means the parameter isn't part of ParamSubset
    static const i32 offsets[28] {   0,  16,  32,  64,  80,  96, 128, 144, 160, 112,
                                    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 192,
                                   208,  -1, 176, 256, 272, 288, 304, 224 };

    for (ngl::fx::parameter* entry = value->parameter_chains[1]; entry; entry = entry->next) {
        ngl::fx::e_parameter_type type = entry->type;

        if (type >= ngl::fx::parameter_tentacle_basis_x && type <= ngl::fx::parameter_tentacle_misc_vector) {
            i32 offset = offsets[type - ngl::fx::parameter_tentacle_basis_x];

            if (offset >= 0)
                std::memcpy(entry->data, (u8*)subset + offset, 16);
        }
    }
}

struct subset_lighting_parameters {
    i32      directional_light_count;
    vector4  directional_light_directions[8];
    vector4  directional_light_colors[8];
    vector4  horizon_projection_u;
    vector4  horizon_projection_v;
    vector4  ambient_info;
    vector4  fog_color;
    vector4  fog_control;
    texture* horizon_texture;
};

struct generated_light_data {
    u32       flags;
    u8        reserved_004[0x1C];
    vector4   direction;
    u8        reserved_030[0x10];
    vector4   position;
    matrix4x4 projector_matrix;
    vector4   color;
    texture*  projector_texture;
    f32       activity;
    f32       inner_radius;
    f32       outer_radius;
    u8        reserved_0B0[0x10];
    f32       attenuation_inner;
    f32       attenuation_outer;
    u8        reserved_0C8[0x0C];
    f32       direction_w;
    u8        reserved_0D8[0x04];
    i32       projector_config_0;
    i32       projector_config_1;
    u8        reserved_0E4[0x0C];
};

struct context_point_light_data {
    vector4 position;
    vector4 color;
};

static_assert(sizeof(generated_light_data) == 0xF0);
static_assert(sizeof(context_point_light_data) == 0x20);

void initialize_general_lighting(general_lighting_parameters* value) {
    std::memset(value, 0, sizeof(*value));

    const vector4 &zero = lighting_zero_value.get();

    vector4 ones { 1.0f, 1.0f, 1.0f, 1.0f };

    value->reserved_1d0 = ones;
    value->reserved_1e0 = zero;
    value->reserved_1f0 = ones;
    value->reserved_200 = zero;

    for (u32 index = 0; index < 4; ++index) {
        value->light_directions[index] = zero;
        value->light_colors[index] = zero;
        value->light_attenuation[index] = zero;
    }

    value->projector_matrix.identity();

    value->projector_texture = nullptr;
    value->projector_config_0 = 4;
    value->projector_config_1 = 4;
    value->projector_index = 0;
    value->special_light_index = -1;

    for (u32 index = 0; index < 9; ++index)
        value->ambient_defaults[index] = lighting_default_value.get();
}

u8* get_light_source(const ngl::fx::mesh_node_data* node_data) {
    u8* source = (u8*)find_scene_parameter(node_data->parameters,
                                    parameter_id_light_source.read());

    if (source)
        return source;

    ngl::scene* current_scene = ngl::references::current_scene.read();

    return (u8*)get_scene_parameter(current_scene->parameters,
                              parameter_id_scene_light_source.read());
}

void add_general_directional_light(      general_lighting_parameters* value,
                                   const u8*                          source,
                                         bool                         primary) {

    i32 &count = value->light_count;

    if (count >= 4)
        return;

    const f32* direction = (const f32*)(source + (primary ? 0x50 : 0x20));
    const f32  sign      = primary ? 1.0f : -1.0f;

    value->light_positions[count] = vector4(
        sign * direction[0] * 10000.0f,
        sign * direction[1] * 10000.0f,
        sign * direction[2] * 10000.0f,
        0.0f);

    value->light_directions[count] = vector4(
        primary ? -direction[0] : direction[0],
        primary ? -direction[1] : direction[1],
        primary ? -direction[2] : direction[2],
        primary ? 0.0f : *(const f32*)(source + 0xD4));

    value->light_colors[count] = *(const vector4*)(source + (primary ? 0x40 : 0x90));

    value->light_attenuation[count] = vector4(1.0f, 1.0f, 1.0f, 1.0f);

    if (!primary && (*(const u32*)source & 4))
        value->special_light_index = count;

    ++count;
}

void write_general_primary_block(      general_lighting_parameters* value,
                                 const u8*                          source,
                                 const u8*                          light_table,
                                       i32                          table_index,
                                       i32                          table_offset) {

    const vector4 &zero          = lighting_zero_value.get();
    const vector4 &default_value = lighting_default_value.get();

    const f32* direction = (const f32*)(source + 0x50);

    value->horizon_range = zero;
    value->horizon_color_scale = zero;
    value->horizon_color = zero;
    value->horizon_direction = default_value;
    value->horizon_axis = default_value;
    value->horizon_direction_scaled = default_value;
    value->horizon_bias = zero;
    value->horizon_scalar[0] = 0.0f;
    value->horizon_extra = zero;

    if (!light_table) {
        add_general_directional_light(value, source, true);

        return;
    }

    f32 scalar = *(const f32*)(source + 0xC8);
    const vector4 &axis = lighting_horizon_axis.get();
    f32 projection = direction[0] * axis.x +
                     direction[1] * axis.y +
                     direction[2] * axis.z;
    f32 residual = 1.0f - projection;
    f32 positive = scalar + residual;
    f32 negative = residual - scalar;

    value->horizon_range = vector4(negative, positive, 0.0f, 0.0f);

    f32 horizon_temporary[4] { positive, negative, 0.0f, 0.0f };

    const f32* horizon_base = (const f32*)&lighting_horizon_base.get();

    for (u32 index = 0; index < 4; ++index)
        value->horizon_bias[index] = horizon_base[index] - horizon_temporary[index];

    value->horizon_direction = vector4(direction[0], direction[1], direction[2], 0.0f);

    const f32* direction_scale = (const f32*)&lighting_direction_scale.get();
    value->horizon_direction_scaled = vector4(
        direction[0] * direction_scale[0],
        direction[1] * direction_scale[1],
        direction[2] * direction_scale[2],
        0.0f);

    f32 denominator = scalar + scalar;
    value->horizon_color_scale = vector4(
        *(const f32*)(source + 0x40) / denominator,
        *(const f32*)(source + 0x44) / denominator,
        *(const f32*)(source + 0x48) / denominator,
        1.0f / denominator);

    value->horizon_color = *(const vector4*)(source + 0x80);
    value->horizon_extra = *(const vector4*)(source + 0xB0);
    value->horizon_axis = vector4(0.0f, 1.0f, 0.0f, 0.0f);
    value->horizon_scale = 1.0f;

    if (table_offset == 4) {
        const f32* half = (const f32*)&lighting_half.get();

        for (u32 index = 0; index < 4; ++index)
            value->horizon_color[index] *= half[index];

        value->horizon_scale = 0.5f;
    }

    i32 projection_index = table_index + table_offset;

    if (table_index == -1 ||
        projection_index < 0 ||
        projection_index >= *(const i32*)(light_table + 0x14)) {

        value->horizon_projection_u = lighting_zero_value.get();
        value->horizon_projection_v = lighting_zero_value.get();
    } else {
        const vector4* projections = *(const vector4**)(light_table + 0x18);

        value->horizon_projection_u = projections[projection_index * 2 + 0];
        value->horizon_projection_v = projections[projection_index * 2 + 1];
    }

    add_general_directional_light(value, source, true);
}

void get_world_sphere_center(      f32*                     destination,
                             const ngl::fx::mesh_node_data* node_data,
                             const ngl::mesh_section*       section) {

    const f32* matrix = (const f32*)&node_data->local_to_world;
    f32 x = section->sphere[0];
    f32 y = section->sphere[1];
    f32 z = section->sphere[2];

    destination[0] = matrix[0] * x + matrix[4] * y + matrix[ 8] * z + matrix[12];
    destination[1] = matrix[1] * x + matrix[5] * y + matrix[ 9] * z + matrix[13];
    destination[2] = matrix[2] * x + matrix[6] * y + matrix[10] * z + matrix[14];
    destination[3] = matrix[3] * x + matrix[7] * y + matrix[11] * z + matrix[15];
}

f32 get_inverse_node_transform(      matrix4x4                &destination,
                               const ngl::fx::mesh_node_data*  node_data) {

    destination = node_data->local_to_world.inverse_orthonormal();

    if (!(node_data->node_info[0] & 2))
        return 1.0f;

    const f32* scales = (const f32*)(node_data->node_info + 0x10);
    f32 inverse_scale_squared[4];

    for (u32 index = 0; index < 4; ++index) {
        f32 inverse_scale = 1.0f / scales[index];
        inverse_scale_squared[index] = inverse_scale * inverse_scale;
    }

    for (u32 row = 0; row < 4; ++row)
        for (u32 column = 0; column < 4; ++column)
            destination[row][column] *= inverse_scale_squared[column];

    return 1.0f / node_data->scale;
}

bool local_light_intersects_mesh(const f32*                     position,
                                       f32                      outer_radius,
                                 const f32*                     world_center,
                                 const matrix4x4               &inverse_transform,
                                       f32                      inverse_scale,
                                 const ngl::fx::mesh_node_data* node_data) {

    f32 delta_x = position[0] - world_center[0];
    f32 delta_y = position[1] - world_center[1];
    f32 delta_z = position[2] - world_center[2];
    f32 mesh_radius = node_data->scale * *(f32*)(node_data->mesh_data + 0x2C);
    f32 combined_radius = outer_radius + mesh_radius;

    if (combined_radius * combined_radius <
        delta_x * delta_x + delta_y * delta_y + delta_z * delta_z) {

        return false;
    }

    f32 local_x = inverse_transform[0][0] * delta_x +
                  inverse_transform[1][0] * delta_y +
                  inverse_transform[2][0] * delta_z;
    f32 local_y = inverse_transform[0][1] * delta_x +
                  inverse_transform[1][1] * delta_y +
                  inverse_transform[2][1] * delta_z;
    f32 local_z = inverse_transform[0][2] * delta_x +
                  inverse_transform[1][2] * delta_y +
                  inverse_transform[2][2] * delta_z;

    const f32* extents = (const f32*)(node_data->mesh_data + 0x30);
    f32 absolute_x = std::fabs(local_x);
    f32 absolute_y = std::fabs(local_y);
    f32 absolute_z = std::fabs(local_z);
    f32 outside_x = absolute_x - (absolute_x <= extents[0] ? absolute_x : extents[0]);
    f32 outside_y = absolute_y - (absolute_y <= extents[1] ? absolute_y : extents[1]);
    f32 outside_z = absolute_z - (absolute_z <= extents[2] ? absolute_z : extents[2]);
    f32 local_radius = inverse_scale * outer_radius;

    return local_radius * local_radius >=
           outside_x * outside_x + outside_y * outside_y + outside_z * outside_z;
}

void add_general_generated_light(      general_lighting_parameters* value,
                                 const generated_light_data*        light,
                                       f32                          inverse_scale) {

    i32 &count = value->light_count;

    value->light_positions[count] = vector4(light->position.x, light->position.y, light->position.z, 1.0f);

    value->light_directions[count] = vector4(light->direction.x, light->direction.y, light->direction.z, light->direction_w);

    value->light_colors[count] = light->color;

    f32 inner_radius = light->inner_radius * inverse_scale;
    f32 outer_radius = light->outer_radius * inverse_scale;
    f32 inner_squared = inner_radius * inner_radius;
    f32 outer_squared = outer_radius * outer_radius;
    f32 radial_scale    = 1.0f / (inner_squared - outer_squared);
    f32 secondary_scale = 1.0f / (light->attenuation_inner - light->attenuation_outer);

    value->light_attenuation[count] = vector4(
        radial_scale,
        -outer_squared * radial_scale,
        secondary_scale,
        -light->attenuation_outer * secondary_scale);

    texture* &projector_texture = value->projector_texture;

    if (!projector_texture && light->projector_texture) {
        projector_texture = light->projector_texture;
        value->projector_matrix = light->projector_matrix;
        value->projector_config_0 = light->projector_config_0;
        value->projector_config_1 = light->projector_config_1;
        value->projector_index = count;
    }

    ++count;
}

void add_general_point_light(      general_lighting_parameters* value,
                             const context_point_light_data*    light,
                                   f32                          local_radius) {

    i32 &count = value->light_count;

    value->light_positions[count] = vector4(light->position.x, light->position.y, light->position.z, 1.0f);
    value->light_directions[count] = vector4(0.0f, -1.0f, 0.0f, 0.0f);
    value->light_colors[count] = light->color;

    f32 radius_squared = local_radius * local_radius;
    value->light_attenuation[count] = vector4(-1.0f / radius_squared, 1.0f, 0.0f, 1.0f);

    ++count;
}

void reorder_general_lights(general_lighting_parameters* value) {
    i32 &count = value->light_count;
    i32 &projector_index = value->projector_index;
    i32 &special_index = value->special_light_index;

    // force the projector into slot 1 and the special light into slot 0.
    // these are copies, so the same light can end up in more than one slot.
    if (value->projector_texture && projector_index != 1) {
        value->light_positions[1] = value->light_positions[projector_index];
        value->light_directions[1] = value->light_directions[projector_index];
        value->light_colors[1] = value->light_colors[projector_index];
        value->light_attenuation[1] = value->light_attenuation[projector_index];

        if (count < 2)
            count = 2;

        if (special_index == 1)
            special_index = projector_index;

        projector_index = 1;
    }

    if (special_index != -1) {
        value->light_positions[0] = value->light_positions[special_index];
        value->light_directions[0] = value->light_directions[special_index];
        value->light_colors[0] = value->light_colors[special_index];
        value->light_attenuation[0] = value->light_attenuation[special_index];
        special_index = 0;
    }
}

void gather_general_local_lights(      general_lighting_parameters*  value,
                                 const ngl::fx::mesh_node_data*      node_data,
                                 const ngl::mesh_section*            section,
                                 const ngl::lighting::light_context* context,
                                       bool                          include_disabled) {

    if (value->light_count >= 4)
        return;

    f32 world_center[4];
    matrix4x4 inverse_transform;
    get_world_sphere_center(world_center, node_data, section);
    f32 inverse_scale = get_inverse_node_transform(inverse_transform, node_data);

    u8* source = value->light_source;
    u32 source_index = include_disabled ? 2 : 0;
    generated_light_data* generated_lights =
        *(generated_light_data**)(source + 0x2F4 + 4 * source_index);
    i32 generated_light_count = *(i32*)(source + 0x304 + 4 * source_index);

    for (i32 index = 0; index < generated_light_count; ++index) {
        generated_light_data* light = generated_lights + index;

        if (!local_light_intersects_mesh((const f32*)&light->position,
                                         light->outer_radius,
                                         world_center,
                                         inverse_transform,
                                         inverse_scale,
                                         node_data)) {

            continue;
        }

        add_general_generated_light(value, light, inverse_scale);

        if (value->light_count >= 4)
            return;
    }

    const ngl::lighting::light_node* node = context->head.next;

    while (node != &context->head) {
        if (node->type == ngl::lighting::light_generated) {
            auto* light = (generated_light_data*)node->node_data;

            if ((!include_disabled && (light->flags & 2)) ||
                !local_light_intersects_mesh((const f32*)&light->position,
                                             light->outer_radius,
                                             world_center,
                                             inverse_transform,
                                             inverse_scale,
                                             node_data)) {

                node = node->next;

                continue;
            }

            add_general_generated_light(value, light, inverse_scale);
        } else if (node->type == ngl::lighting::light_point) {
            auto* light = (context_point_light_data*)node->node_data;

            if (!local_light_intersects_mesh((const f32*)&light->position,
                                             light->position.w,
                                             world_center,
                                             inverse_transform,
                                             inverse_scale,
                                             node_data)) {

                node = node->next;

                continue;
            }

            add_general_point_light(value,
                                    light,
                                    inverse_scale * light->position.w);
        }

        if (value->light_count >= 4)
            return;

        node = node->next;
    }
}

void build_general_lighting(      general_lighting_parameters* value,
                            const ngl::fx::mesh_node_data*     node_data,
                            const ngl::mesh_section*           section) {

    initialize_general_lighting(value);

    ngl::lighting::light_context* context = ngl::fx::prepare_light_context(node_data);

    u8* source = get_light_source(node_data);
    
    ngl::scene* current_scene = ngl::references::current_scene.read();

    value->light_source = source;

    value->view_position = vector4(current_scene->view_to_world.w.x,
                                   current_scene->view_to_world.w.y,
                                   current_scene->view_to_world.w.z,
                                   1.0f);

    value->ambient_direction = vector4(*(f32*)(source + 0x220), *(f32*)(source + 0x224), *(f32*)(source + 0x228), 0.0f);

    value->ambient_color = *(const vector4*)(source + 0x210);
    value->post_view_position = vector4(current_scene->view_to_world.w.x,
                                        current_scene->view_to_world.w.y,
                                        current_scene->view_to_world.w.z,
                                        0.0f);

    const f32* normal = (const f32*)(source + 0x230);
    f32 inverse_length = 1.0f / std::sqrt(normal[0] * normal[0] +
                                         normal[1] * normal[1] +
                                         normal[2] * normal[2]);

    value->post_direction = vector4(
        normal[0] * inverse_length,
        normal[1] * inverse_length,
        normal[2] * inverse_length,
        0.0f);

    value->post_plane_0 = vector4(*(f32*)(source + 0x240),
                                  *(f32*)(source + 0x244),
                                  *(f32*)(source + 0x248),
                                  1.0f);

    value->post_plane_1 = vector4(*(f32*)(source + 0x250),
                                  *(f32*)(source + 0x254),
                                  *(f32*)(source + 0x258),
                                  1.0f);

    value->post_range = vector4(std::fabs(*(f32*)(source + 0x318)),
                                *(f32*)(source + 0x31C),
                                0.0f,
                                0.0f);

    value->post_color = vector4(*(f32*)(source + 0x260),
                                *(f32*)(source + 0x264),
                                *(f32*)(source + 0x268),
                                0.0f);

    value->ambient_info = vector4(*(f32*)(source + 0x278),
                                  *(f32*)(source + 0x270),
                                  *(f32*)(source + 0x274),
                                  *(f32*)(source + 0x27C));

    value->fog_color = *(const vector4*)(source + 0x2D0);

    f32 fog_near = *(f32*)(source + 0x2E0);
    f32 fog_far  = *(f32*)(source + 0x2E4);

    if (fog_near == fog_far)
        fog_far += 0.00001525879997643642f;

    f32 fog_scale = 1.0f / (fog_far - fog_near);
    // this exact value is passed to the shader
    u32 fog_magic_bits = 0x3B23D70A;
    f32 fog_magic;
    std::memcpy(&fog_magic, &fog_magic_bits, sizeof(fog_magic));

    value->fog_control = vector4(fog_scale, -fog_near * fog_scale, *(f32*)(source + 0x2E8), fog_magic);

    u8* table = (u8*)find_scene_parameter(node_data->parameters, parameter_id_light_table.read());

    value->horizon_texture = table ? *(texture**)(table + 0x08) : nullptr;

    i32 table_index  = 0;
    i32 table_offset = 4;

    u8* range = (u8*)find_scene_parameter(node_data->parameters, parameter_id_light_table_range.read());

    if (range) {
        table_index  = *(i32*)(range + 0x48);
        table_offset = *(i32*)(range + 0x4C);
    }

    u8* primary = *(u8**)(source + 0x32C);

    vector4 contribution = lighting_zero_value.get();

    if (primary) {
        write_general_primary_block(value,
                                    primary,
                                    table,
                                    table_index,
                                    table_offset);

        const f32* half = (const f32*)&lighting_half.get();

        for (u32 index = 0; index < 4; ++index)
            contribution[index] = *(f32*)(primary + 0x80 + 4 * index) * half[index];
    }

    for (u32 vector_index = 0; vector_index < 8; ++vector_index) {
        for (u32 component = 0; component < 4; ++component) {
            value->ambient_colors[vector_index][component] =
                *(f32*)(source + 0x00 + 0x10 * vector_index + 4 * component);

            value->ambient_colors_with_primary[vector_index][component] =
                *(f32*)(source + 0x80 + 0x10 * vector_index + 4 * component) + contribution[component];
        }
    }

    i32 directional_count  = *(i32*)(source + 0x314);
    u8* directional_lights = *(u8**)(source + 0x2F0);

    bool include_disabled = *(u32*)(node_data->mesh_data + 0x14) != 0;

    for (i32 index = 0; index < directional_count; ++index) {
        u8* light = directional_lights + 0xF0 * index;

        if ((!include_disabled && (*(u32*)light & 2)) ||
            *(f32*)(light + 0xA4) == 0.0f) {

            continue;
        }

        add_general_directional_light(value, light, false);

        if (value->light_count >= 4)
            break;
    }

    gather_general_local_lights(value,
                                node_data,
                                section,
                                context,
                                include_disabled);
    reorder_general_lights(value);

    if (*(u32*)node_data->node_info & 2) {
        f32 inverse_scale_squared = 1.0f / (node_data->scale * node_data->scale);

        for (u32 index = 0; index < 4; ++index)
            value->light_attenuation[index][0] *= inverse_scale_squared;
    }
}

void get_subset_lighting(      subset_lighting_parameters* destination,
                         const ngl::fx::mesh_node_data*    node_data) {

    std::memset(destination, 0, sizeof(*destination));

    u8* source = (u8*)find_scene_parameter(node_data->parameters,
                                    parameter_id_light_source.read());

    if (!source) {
        ngl::scene* current_scene = ngl::references::current_scene.read();

        source = (u8*)get_scene_parameter(current_scene->parameters,
                                    parameter_id_scene_light_source.read());
    }

    destination->ambient_info.x = *(f32*)(source + 0x278);
    destination->ambient_info.y = *(f32*)(source + 0x270);
    destination->ambient_info.z = *(f32*)(source + 0x274);
    destination->ambient_info.w = *(f32*)(source + 0x27C);
    destination->fog_color    = *(vector4*)(source + 0x2D0);

    f32 fog_near = *(f32*)(source + 0x2E0);
    f32 fog_far  = *(f32*)(source + 0x2E4);

    if (fog_near == fog_far)
        fog_far += 0.00001525879997643642f;

    destination->fog_control.x = 1.0f / (fog_far - fog_near);
    destination->fog_control.y = -fog_near * destination->fog_control.x;
    destination->fog_control.z = *(f32*)(source + 0x2E8);
    *(u32*)&destination->fog_control.w = 992204554;

    u8* directional_light = *(u8**)(source + 0x32C);

    if (directional_light) {
        destination->directional_light_directions[0] = *(vector4*)(directional_light + 0x50);
        destination->directional_light_colors[0] = *(vector4*)(directional_light + 0x40);
        destination->directional_light_colors[0].w = 1.0f;
        destination->directional_light_count = 1;
    }

    u8* table = (u8*)find_scene_parameter(node_data->parameters, parameter_id_light_table.read());

    if (!table)
        return;

    destination->horizon_texture = *(texture**)(table + 0x08);

    i32 index = 0;
    i32 offset = 4;
    u8* range = (u8*)find_scene_parameter(node_data->parameters,
                                   parameter_id_light_table_range.read());

    if (range) {
        index  = *(i32*)(range + 0x48);
        offset = *(i32*)(range + 0x4C);
    }

    i32 table_index = index + offset;

    if (index == -1 || table_index < 0 || table_index >= *(i32*)(table + 0x14)) {
        static util::memory_reference<vector4> default_horizon_projection { 0x01086F50 };
        
        destination->horizon_projection_u = default_horizon_projection.get();
        destination->horizon_projection_v = default_horizon_projection.get();
        
        return;
    }

    vector4* projections = *(vector4**)(table + 0x18);

    destination->horizon_projection_u = projections[table_index * 2 + 0];
    destination->horizon_projection_v = projections[table_index * 2 + 1];
}

void write_specialized_matrix(      ngl::fx::parameter*      entry,
                              const ngl::fx::mesh_node_data* node_data,
                                    bool                     subset_effect) {
    using namespace ngl::fx;

    matrix4x4 &destination = *(matrix4x4*)entry->data;
    matrix4x4 local = node_data->local_to_world.affine();
    scene* current_scene = ngl::references::current_scene.read();

    switch (entry->type) {
        case parameter_world:
            destination = local.transpose();
            break;
        case parameter_world_inverse:
            destination = get_unscaled_local_to_world(node_data).inverse_orthonormal().transpose();
            break;
        case parameter_world_transpose:
            destination = local;
            break;
        case parameter_world_inverse_transpose:
            destination = get_unscaled_local_to_world(node_data).inverse_orthonormal();
            break;
        case parameter_world_view:
            destination = (local * current_scene->world_to_view).affine().transpose();
            break;
        case parameter_world_view_inverse:
            destination = (local * current_scene->world_to_view).inverse_orthonormal().transpose();
            break;
        case parameter_world_view_transpose:
            destination = (local * current_scene->world_to_view).affine();
            break;
        case parameter_world_view_inverse_transpose:
            destination = (local * current_scene->world_to_view).inverse_orthonormal();
            break;
        case parameter_world_view_projection:
            destination = (local * current_scene->world_to_screen).transpose();
            break;
        case parameter_world_view_projection_transpose:
            destination = local * current_scene->world_to_screen;
            break;
        case parameter_world_view_projection_inverse:
        case parameter_world_view_projection_inverse_transpose:
            // general effects get identity here, but subset effects leave the old value alone
            if (!subset_effect)
                destination.identity();
            break;
    }
}

void write_bone_matrices(const ngl::fx::mesh_node_data* node_data,
                         const ngl::mesh_section*       section) {
    vector4* destination = (vector4*)&bone_constant_data.get();

    // shaders only receive three rows per bone,
    // meshes without bones still get two copies of the node transform
    if (!section->bone_count) {
        matrix4x4 matrix = node_data->local_to_world.affine().transpose();
        std::memcpy(destination, &matrix, sizeof(vector4) * 3);
        std::memcpy(destination + 3, &matrix, sizeof(vector4) * 3);
        bone_constant_count.write(2);
        return;
    }

    u32 flags = *(u32*)node_data->node_info;
    const matrix4x4* bone_matrices = *(matrix4x4**)(node_data->node_info + 8);
    const u8* bind_matrices = nullptr;

    if (flags & (4 | 8))
        bind_matrices = *(u8**)(*(u8**)(node_data->mesh_data + 0x14) + 0x0C);

    for (i32 index = 0; index < section->bone_count; ++index) {
        u16 bone_index = section->bone_indices[index];
        const matrix4x4 &bone = bone_matrices[bone_index];
        matrix4x4 matrix;

        if (flags & (4 | 8)) {
            // each bone's bind data is 0x90 bytes and its matrix starts at +0x40
            const matrix4x4 &bind = *(const matrix4x4*)(bind_matrices + 0x90 * bone_index + 0x40);
            matrix = bind.affine() * bone.affine();

            // flag 4 wins when both flags are set
            if (!(flags & 4))
                matrix = matrix.affine() * node_data->local_to_world.affine();
        } else if (flags & 0x10)
            matrix = bone;
        else
            matrix = node_data->local_to_world;

        matrix = matrix.affine().transpose();
        std::memcpy(destination + 3 * index, &matrix, sizeof(vector4) * 3);
    }

    bone_constant_count.write(section->bone_count);
}

void write_point_light_positions(      f32*                     destination,
                                 const ngl::fx::mesh_node_data* node_data) {

    f32* lights = &point_light_data.get();
    // skip the 16-byte array header
    destination += 4;

    for (u32 index = 0; index < node_data->point_light_count; ++index) {
        u32 light_index = node_data->point_light_indices[index];
        f32* light = lights + 8 * light_index;

        *(vector4*)(destination + 4 * index) = vector4(light[0], light[1], light[2], 1.0f / light[3]);
    }
}

void write_point_light_colors(      f32* destination,
                              const ngl::fx::mesh_node_data* node_data) {

    f32* lights = &point_light_data.get();
    u32 index = 0;
    // skip the 16-byte array header
    destination += 4;

    for (; index < node_data->point_light_count; ++index) {
        u32 light_index = node_data->point_light_indices[index];
        f32* light = lights + 8 * light_index;

        *(vector4*)(destination + 4 * index) = vector4(light[4], light[5], light[6], 0.0f);
    }

    // clear unused colors, but leave unused positions and ranges alone
    for (; index < 8; ++index)
        *(vector4*)(destination + 4 * index) = vector4(0.0f, 0.0f, 0.0f, 0.0f);
}

void write_shadow_size(f32* destination, u32 width, u32 height) {
    *(vector4*)(destination) = vector4((f32)width, (f32)height, 1.0f / (f32)width, 1.0f / (f32)height);
}

void write_default_environment_color(f32* destination) {
    // default color starts at (10, 10, 20) / 255 and is squared below
    *(vector4*)(destination) = vector4(0.03921569f, 0.03921569f, 0.07843138f, 1.0f);

    for (u32 index = 0; index < 4; ++index)
        destination[index] *= destination[index];
}

bool is_depth_bias_parameter(ngl::fx::e_parameter_type type) {
    return type == ngl::fx::parameter_bone_array_world ||
          (type >= ngl::fx::parameter_world && type <= ngl::fx::parameter_world_inverse_transpose) ||
          (type >= ngl::fx::parameter_world_view && type <= ngl::fx::parameter_world_view_inverse_transpose) ||
           type == ngl::fx::parameter_world_view_projection ||
           type == ngl::fx::parameter_world_view_projection_transpose ||
           type == ngl::fx::parameter_mesh_map_distance ||
           type == ngl::fx::parameter_ui_parameters ||
           type == ngl::fx::parameter_decompressed_position_scale ||
           type == ngl::fx::parameter_decompressed_position_bias;
}

void ngl::fx::update_scene_parameters(effect* value) {
    scene* current_scene = ngl::references::current_scene.read();

    for (parameter* entry = value->parameter_chains[0]; entry; entry = entry->next) {
        f32* destination = (f32*)entry->data;
        vector4* vectors = (vector4*)entry->data;

        switch (entry->type) {
            case parameter_view_position:
                vectors[0] = vector4(current_scene->view_position.get_xyz(), 1.0f);

                break;
            case parameter_viewport_pixel_size:
                vectors[0] = vector4((f32)current_scene->color_target->gpu_texture.width,
                                     (f32)current_scene->color_target->gpu_texture.height, 0.0f, 0.0f);

                break;
            case parameter_frame:
                *(u32*)destination = ngl::references::frame_epoch.read();

                break;
            case parameter_time:
                destination[0] = current_scene->current_animation_time;

                break;
            case parameter_view: {
                *(matrix4x4*)destination = current_scene->world_to_view.affine().transpose();

                break;
            }
            case parameter_view_inverse: {
                *(matrix4x4*)destination = current_scene->view_to_world.affine().transpose();

                break;
            }
            case parameter_view_transpose:
                *(matrix4x4*)destination = current_scene->world_to_view.affine();

                break;
            case parameter_view_inverse_transpose:
                *(matrix4x4*)destination = current_scene->view_to_world.affine();

                break;
            case parameter_projection:
                *(matrix4x4*)destination = current_scene->view_to_screen.transpose();

                break;
            case parameter_projection_inverse:
                ((matrix4x4*)destination)->identity();

                break;
            case parameter_projection_transpose:
                *(matrix4x4*)(destination) = current_scene->view_to_screen;

                break;
            case parameter_projection_inverse_transpose:
                ((matrix4x4*)destination)->identity();

                break;
            case parameter_view_projection:
                *(matrix4x4*)destination = current_scene->world_to_screen.transpose();

                break;
            case parameter_view_projection_inverse:
                ((matrix4x4*)destination)->identity();

                break;
            case parameter_view_projection_transpose:
                *(matrix4x4*)(destination) = current_scene->world_to_screen;

                break;
            case parameter_view_projection_inverse_transpose:
                ((matrix4x4*)destination)->identity();

                break;
        }
    }
}

void ngl::fx::update_material_parameters(effect*         value,
                                         mesh_node_data* node_data,
                                         mesh_section*   section,
                                         bool            depth_bias_enabled) {

    bool subset_effect = (value->flags & 0x40) != 0;
    subset_lighting_parameters lighting;
    general_lighting_parameters general_lighting;
    texture* active_horizon_texture = nullptr;

    if (!subset_effect) {
        build_general_lighting(&general_lighting, node_data, section);
        active_horizon_texture = general_lighting.horizon_texture;
    } else if (!depth_bias_enabled) {
        get_subset_lighting(&lighting, node_data);
        active_horizon_texture = lighting.horizon_texture;
    }

    copy_parameter_subset(value, node_data);

    for (parameter* entry = value->parameter_chains[1]; entry; entry = entry->next) {
        f32* destination = (f32*)entry->data;
        vector4* vectors = (vector4*)entry->data;
        e_parameter_type type = entry->type;

        if (subset_effect && depth_bias_enabled && !is_depth_bias_parameter(type))
            continue;

        if ((type >= parameter_world && type <= parameter_world_inverse_transpose) ||
            (type >= parameter_world_view && type <= parameter_world_view_inverse_transpose) ||
            (type >= parameter_world_view_projection && type <= parameter_world_view_projection_inverse_transpose)) {

            write_specialized_matrix(entry, node_data, subset_effect);

            continue;
        }

        switch (type) {
            case parameter_bone_array_world:
                write_bone_matrices(node_data, section);

                break;
            case parameter_bone_influences:
                if (!subset_effect)
                    destination[0] = section && section->bone_count > 0 ? 4.0f : 1.0f;

                break;
            case parameter_horizon_projection_u:
                vectors[0] = *(const vector4*)(subset_effect ?
                                (const void*)&lighting.horizon_projection_u : (const void*)(f32*)&general_lighting.horizon_projection_u);

                break;
            case parameter_horizon_projection_v:
                vectors[0] = *(const vector4*)(subset_effect ?
                                (const void*)&lighting.horizon_projection_v : (const void*)(f32*)&general_lighting.horizon_projection_v);

                break;
            case parameter_lightmap_color: {
                f32 hour = get_hour_of_day();
                f32 night = hour < 3.0f || hour > 23.9f ? 1.0f : 0.0f;
                vectors[0] = vector4(0.60000002f * night, 0.5f * night, 0.40000001f * night, 1.0f);

                break;
            }
            case parameter_window_color: {
                f32 hour = get_hour_of_day();
                f32 night = hour < 3.0f || hour > 23.9f ? 1.0f : 0.0f;
                vectors[0] = vector4(1.3f, 1.2f, 0.8f, night);

                break;
            }
            case parameter_fog_color:
                vectors[0] = subset_effect ? lighting.fog_color : general_lighting.fog_color;

                break;
            case parameter_fog_control:
                vectors[0] = *(const vector4*)(subset_effect ?
                                (const void*)&lighting.fog_control : (const void*)(f32*)&general_lighting.fog_control);

                break;
            case parameter_ambient_info:
                if (!subset_effect) {
                    vectors[0] = general_lighting.ambient_color;
                    vectors[1] = general_lighting.ambient_direction;
                    std::memcpy(destination + 4 * 2, (f32*)&general_lighting.ambient_colors_with_primary[0], sizeof(vector4) * 8);
                    std::memcpy(destination + 4 * 10, (f32*)&general_lighting.ambient_colors[0], sizeof(vector4) * 8);
                    std::memcpy(destination + 4 * 18, (f32*)&general_lighting.ambient_defaults[0], sizeof(vector4) * 9);
                }

                break;
            case parameter_horizon_info:
                if (!subset_effect) {
                    vectors[0] = general_lighting.horizon_projection_u;
                    vectors[1] = general_lighting.horizon_projection_v;
                    vectors[2] = general_lighting.light_colors[0];
                    vectors[3] = general_lighting.light_directions[0];
                    vectors[4] = general_lighting.horizon_range;
                    vectors[5] = general_lighting.horizon_color;
                    vectors[6] = general_lighting.horizon_direction_scaled;
                    vectors[7] = general_lighting.horizon_bias;

                    f32 scalar = general_lighting.horizon_scalar[0];
                    vectors[8] = vector4(scalar, scalar, scalar, scalar);

                    std::memcpy(destination + 4 * 9, (f32*)&general_lighting.horizon_extra, sizeof(f32) * 3);
                    destination[4 * 9 + 3] = 0.0f;

                    std::memcpy(destination + 4 * 10, (f32*)&general_lighting.horizon_axis, sizeof(f32) * 3);
                    destination[4 * 10 + 3] = 0.0f;
                }

                break;
            case parameter_horizon_texture: {
                write_texture(entry,
                              active_horizon_texture ? active_horizon_texture : horizon_texture.read());

                break;
            }
            case parameter_light_info:
                if (!subset_effect) {
                    std::memcpy(destination + 4 * 0, (f32*)&general_lighting.light_positions[0], sizeof(vector4) * 4);
                    std::memcpy(destination + 4 * 4, (f32*)&general_lighting.light_directions[0], sizeof(vector4) * 4);
                    std::memcpy(destination + 4 * 8, (f32*)&general_lighting.light_colors[0], sizeof(vector4) * 4);
                    std::memcpy(destination + 4 * 12, (f32*)&general_lighting.light_attenuation[0], sizeof(vector4) * 4);
                }

                break;
            case parameter_shadow_distances:
                vectors[0] = shadow_distances.get();

                break;
            case parameter_shadow_buffer_size:
            case parameter_shadow_buffer_size_1:
                write_shadow_size(destination, shadow_width_0.read(), shadow_height_0.read());

                break;
            case parameter_shadow_buffer_size_2:
                write_shadow_size(destination, shadow_width_2.read(), shadow_height_2.read());

                break;
            case parameter_view_projection_shadow:
            case parameter_view_projection_shadow_1:
                *(matrix4x4*)destination = shadow_matrix_0.get().transpose();

                break;
            case parameter_view_projection_shadow_2:
                *(matrix4x4*)destination = shadow_matrix_2.get().transpose();

                break;
            case parameter_shadow_texture:
            case parameter_shadow_texture_1:
                write_texture(entry, shadow_texture_0.read());

                break;
            case parameter_shadow_texture_2:
                write_texture(entry, shadow_texture_2.read());

                break;
            case parameter_post_info:
                if (!subset_effect) {
                    const f32* primary_direction = (f32*)&general_lighting.post_direction;
                    const f32* first_plane = (f32*)&general_lighting.post_plane_0;
                    const f32* second_plane = (f32*)&general_lighting.post_plane_1;

                    vectors[0] = *(const vector4*)(primary_direction);

                    vectors[1] = vector4(primary_direction[0] * first_plane[0] +
                                 primary_direction[1] * first_plane[1] +
                                 primary_direction[2] * first_plane[2], primary_direction[0] * second_plane[0] +
                                 primary_direction[1] * second_plane[1] +
                                 primary_direction[2] * second_plane[2], 0.0f, 0.0f);

                    vectors[2] = general_lighting.post_range;
                    vectors[3] = general_lighting.post_color;
                    vectors[4] = general_lighting.fog_color;
                    vectors[5] = general_lighting.fog_control;

                    u8* light_source = (u8*)find_scene_parameter(node_data->parameters,
                                                           parameter_id_light_source.read());

                    if (light_source) {
                        f32 scalar_value = *(f32*)(light_source + 0x200);
                        vectors[6] = vector4(scalar_value, scalar_value, scalar_value, scalar_value);
                    } else
                        vectors[6] = vector4(-2.0f, -2.0f, -2.0f, -2.0f);

                    vectors[7] = general_lighting.post_view_position;
                    vectors[8] = *(const vector4*)(first_plane);
                    vectors[9] = *(const vector4*)(second_plane);
                }

                break;
            case parameter_depth_texture:
                write_texture(entry, depth_texture.read());

                break;
            case parameter_temporary_0:
                for (u32 index = 0; index < 4; ++index)
                    destination[index] = temporary_0.get()[index] * 2.0f;

                break;
            case parameter_shadow_factor:
                vectors[0] = *(const vector4*)(subset_effect ? &subset_shadow_factor.get() : &shadow_factor.get());

                break;
            case parameter_shared_scalar:
                vectors[0] = vector4(
                    shared_scalar.read(),
                    shared_scalar.read(),
                    shared_scalar.read(),
                    shared_scalar.read());

                break;
            case parameter_constant_80:
                vectors[0] = constant_80.get();

                break;
            case parameter_mesh_map_distance:
                vectors[0] = vector4(50.0f, 0.0199999996f, 0.0f, 0.0f);

                break;
            case parameter_number_directional_lights:
                destination[0] = subset_effect ?
                    (f32)lighting.directional_light_count : (f32)general_lighting.directional_light_count;

                break;
            case parameter_directional_light_directions:
                if (subset_effect) {
                    for (i32 index = 0; index < lighting.directional_light_count; ++index) {
                        const vector4 &direction =
                            lighting.directional_light_directions[index];

                        vectors[1 + index] = vector4(direction.x, direction.y, direction.z, 0.0f);
                    }
                } else {
                    for (i32 index = 0; index < general_lighting.directional_light_count; ++index) {

                        const f32* direction = ((f32*)&general_lighting.directional_light_direction + 8 * index);

                        vectors[1 + index] = vector4(direction[0], direction[1], direction[2], 0.0f);
                    }
                }

                break;
            case parameter_directional_light_colors:
                if (subset_effect) {
                    for (i32 index = 0; index < lighting.directional_light_count; ++index) {
                        const vector4 &color = lighting.directional_light_colors[index];

                        vectors[1 + index] = vector4(color.x, color.y, color.z, 0.0f);
                    }
                } else {
                    for (i32 index = 0; index < general_lighting.directional_light_count; ++index) {

                        const f32* color = ((f32*)&general_lighting.directional_light_color + 8 * index);

                        vectors[1 + index] = vector4(color[0], color[1], color[2], 0.0f);
                    }
                }

                break;
            case parameter_number_point_lights:
                destination[0] = (f32)node_data->point_light_count;

                break;
            case parameter_point_light_positions_ranges:
                write_point_light_positions(destination, node_data);

                break;
            case parameter_point_light_colors:
                write_point_light_colors(destination, node_data);

                break;
            case parameter_ibl_parameters:
                vectors[0] = *(const vector4*)(subset_effect ?
                                (const void*)&lighting.ambient_info : (const void*)(f32*)&general_lighting.ambient_info);

                break;
            case parameter_character_highlight: {
                void* color = find_scene_parameter(node_data->parameters,
                                             parameter_id_character_color.read());

                if (color)
                    vectors[0] = *(const vector4*)(color);
                else
                    vectors[0] = vector4(1.0f, 1.0f, 1.0f, 1.0f);

                break;
            }
            case parameter_framebuffer_texture:
                write_texture(entry,
                              subset_effect ? framebuffer_texture.read()
                                            : framebuffer_texture_general.read());
                                            
                break;
            case parameter_environment_map:
                if (!subset_effect)
                    write_texture(entry, environment_texture.read());

                break;
            case parameter_environment_color: {
                f32* color = (f32*)find_scene_parameter(node_data->parameters,
                                                  parameter_id_environment_color.read());

                if (color) {
                    destination[0] = color[0] * color[0];
                    destination[1] = color[1] * color[1];
                    destination[2] = color[2] * color[2];
                    destination[3] = color[3];
                } else
                    write_default_environment_color(destination);

                break;
            }
            case parameter_decal_projection: {
                matrix4x4* matrix = (matrix4x4*)find_scene_parameter(node_data->parameters,
                                                   parameter_id_decal_projection.read());

                if (matrix)
                    *(matrix4x4*)destination = matrix->transpose();
                else
                    ((matrix4x4*)destination)->identity();

                break;
            }
            case parameter_viewport_to_world: {
                scene* current_scene = ngl::references::current_scene.read();
                *(matrix4x4*)destination = (node_data->local_to_world.affine() *
                                           current_scene->derived_matrix_250).transpose();
                break;
            }
            case parameter_world_to_viewport: {
                scene* current_scene = ngl::references::current_scene.read();
                // world_to_viewport also includes the viewport matrix,
                // leaving it out makes projected decals move and resize with the camera
                *(matrix4x4*)destination = (node_data->local_to_world.affine() *
                                           current_scene->world_to_view        *
                                           current_scene->projection           *
                                           current_scene->view).transpose();
                break;
            }
            case parameter_ui_parameters: {
                void* parameters = find_scene_parameter(node_data->parameters,
                                                  parameter_id_ui_parameters.read());
                vectors[0] = *(const vector4*)(parameters ? parameters : &ui_parameters.get());

                break;
            }
            case parameter_tint_color: {
                void* color = find_scene_parameter(node_data->parameters,
                                             parameter_id_tint_color.read());
                vectors[0] = *(const vector4*)(color ? color : &tint_color.get());

                break;
            }
            case parameter_decal_texture_matrix: {
                const vector4* rows = (const vector4*)find_scene_parameter(
                    node_data->parameters, parameter_id_decal_texture_matrix.read());
                if (rows)
                    std::memcpy(vectors + 1, rows, sizeof(vector4) * 2);
                else {
                    vectors[1] = vector4(1.0f, 0.0f, 0.0f, 0.0f);
                    vectors[2] = vector4(0.0f, 1.0f, 0.0f, 0.0f);
                }

                break;
            }
            case parameter_decompressed_position_scale:
                std::memcpy(destination, node_data->mesh_data + 0x30, sizeof(f32) * 3);
                destination[3] = 0.0f;

                break;
            case parameter_decompressed_position_bias:
                std::memcpy(destination, node_data->mesh_data + 0x20, sizeof(f32) * 3);
                destination[3] = 1.0f;

                break;
            case parameter_last: {
                void* data = find_scene_parameter(node_data->parameters,
                                            parameter_id_last.read());
                if (data)
                    vectors[0] = *(const vector4*)(data);

                break;
            }
        }
    }

}
