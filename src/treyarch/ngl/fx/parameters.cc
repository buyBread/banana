#include <cmath>
#include <cstring>

#include "treyarch/ngl/fx/parameters.hh"
#include "treyarch/ngl/lighting/context.hh"
#include "treyarch/ngl/mesh/mesh.hh"
#include "treyarch/ngl/ngl.hh"
#include "treyarch/ngl/scene/references.hh"
#include "treyarch/ngl/texture/texture.hh"
#include "util/memory_reference.hh"

/*
    todo: ~two weeks of mangling this before studies start to get it into a semi-working state...
          meaning no comments, only sporadic common sense and a lot of  ๋࣭ ⭑magic₊⊹ (in most of these files tbh)
          ^^^^^fix allat
*/

using namespace treyarch;
using namespace treyarch::ngl;

static util::memory_reference<u32> parameter_id_light_source         { 0x010F7D78 };
static util::memory_reference<u32> parameter_id_scene_light_source   { 0x010F853C };
static util::memory_reference<u32> parameter_id_light_table          { 0x010F7D74 };
static util::memory_reference<u32> parameter_id_light_context        { 0x01116330 };
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
static util::memory_reference<ngl::lighting::light_context*> selected_light_context   { 0x01118988 };

static bool has_parameter(const ngl::scene_parameters* parameters, u32 id) {
    const u32* words = (const u32*)parameters;

    return (words[id >> 5] & (1u << (id & 31))) != 0;
}

static void* get_parameter(const ngl::scene_parameters* parameters, u32 id) {
    return *(void**)((u8*)parameters + 8 + 4 * id);
}

static void* find_parameter(const ngl::scene_parameters* parameters, u32 id) {
    return has_parameter(parameters, id) ? get_parameter(parameters, id) : nullptr;
}

static void write_vector(f32* destination, f32 x, f32 y, f32 z, f32 w) {
    destination[0] = x;
    destination[1] = y;
    destination[2] = z;
    destination[3] = w;
}

static void identity_matrix(f32* destination);

static void write_texture(ngl::fx::parameter* entry, ngl::texture* value) {
    *(ngl::texture**)entry->data = value ? value : default_texture.read();
}

static void multiply_matrix(f32* destination, const f32* left, const f32* right) {
    f32 product[16];

    for (u32 row = 0; row < 4; ++row)
        for (u32 column = 0; column < 4; ++column)
            product[row * 4 + column] = left[row * 4 + 0] * right[0 * 4 + column] +
                                        left[row * 4 + 1] * right[1 * 4 + column] +
                                        left[row * 4 + 2] * right[2 * 4 + column] +
                                        left[row * 4 + 3] * right[3 * 4 + column];

    std::memcpy(destination, product, sizeof(product));
}

static void inverse_orthonormal_matrix(f32* destination, const f32* source) {
    destination[ 0] = source[ 0];
    destination[ 1] = source[ 4];
    destination[ 2] = source[ 8];
    destination[ 3] = 0.0f;
    destination[ 4] = source[ 1];
    destination[ 5] = source[ 5];
    destination[ 6] = source[ 9];
    destination[ 7] = 0.0f;
    destination[ 8] = source[ 2];
    destination[ 9] = source[ 6];
    destination[10] = source[10];
    destination[11] = 0.0f;

    destination[12] = -(source[12] * destination[0] +
                        source[13] * destination[4] +
                        source[14] * destination[8]);
    destination[13] = -(source[12] * destination[1] +
                        source[13] * destination[5] +
                        source[14] * destination[9]);
    destination[14] = -(source[12] * destination[2] +
                        source[13] * destination[6] +
                        source[14] * destination[10]);
    destination[15] = 1.0f;
}

static void get_local_to_world(f32* destination, const ngl::fx::mesh_node_data* node_data) {
    std::memcpy(destination, &node_data->local_to_world, sizeof(matrix4x4));

    if (!(node_data->node_info[0] & 2))
        return;

    const f32* scales = (const f32*)(node_data->node_info + 0x10);

    for (u32 column = 0; column < 4; ++column) {
        destination[0 * 4 + column] /= scales[0];
        destination[1 * 4 + column] /= scales[1];
        destination[2 * 4 + column] /= scales[2];
    }
}

static f32 get_hour_of_day() {
    u8* state = game_state.read();
    f32 seconds = (f32)*(u32*)(state + 188) + *(f32*)(state + 196);

    return seconds / 3600.0f;
}

static void copy_parameter_subset(      ngl::fx::effect*         value,
                                  const ngl::fx::mesh_node_data* node_data) {

    void* subset = find_parameter(node_data->parameters,
                                  parameter_id_parameter_subset.read());

    if (!subset)
        return;

    static const i32 offsets[28] {   0,  16,  32,  64,  80,  96, 128, 144, 160, 112,
                                    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 192,
                                   208,  -1, 176, 256, 272, 288, 304, 224 };

    for (ngl::fx::parameter* entry = value->parameter_chains[1]; entry; entry = entry->next) {
        u32 type = (u32)entry->type;

        if (type >= 94 && type <= 121) {
            i32 offset = offsets[type - 94];

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

struct general_lighting_parameters {
    u8       data[0x610];
    texture* horizon_texture;
};

static f32* general_vector(general_lighting_parameters* value, u32 offset) {
    return (f32*)(value->data + offset);
}

static i32 &general_integer(general_lighting_parameters* value, u32 offset) {
    return *(i32*)(value->data + offset);
}

static void copy_general_vector(      general_lighting_parameters* value,
                                      u32                          offset,
                                const void*                        source) {

    std::memcpy(general_vector(value, offset), source, sizeof(vector4));
}

static void initialize_general_lighting(general_lighting_parameters* value) {
    std::memset(value, 0, sizeof(*value));

    const vector4 &zero = lighting_zero_value.get();

    vector4 ones { 1.0f, 1.0f, 1.0f, 1.0f };

    copy_general_vector(value, 0x1D0, &ones);
    copy_general_vector(value, 0x1E0, &zero);
    copy_general_vector(value, 0x1F0, &ones);
    copy_general_vector(value, 0x200, &zero);

    for (u32 index = 0; index < 4; ++index) {
        copy_general_vector(value, 0x250 + 0x10 * index, &zero);
        copy_general_vector(value, 0x290 + 0x10 * index, &zero);
        copy_general_vector(value, 0x2D0 + 0x10 * index, &zero);
    }

    identity_matrix(general_vector(value, 0x4C0));

    general_integer(value, 0x560) = 0;
    general_integer(value, 0x564) = 4;
    general_integer(value, 0x568) = 4;
    general_integer(value, 0x56C) = 0;
    general_integer(value, 0x570) = -1;

    for (u32 index = 0; index < 9; ++index)
        copy_general_vector(value,
                            0x580 + 0x10 * index,
                            &lighting_default_value.get());
}

static u8* get_light_source(const ngl::fx::mesh_node_data* node_data) {
    u8* source = (u8*)find_parameter(node_data->parameters,
                                    parameter_id_light_source.read());

    if (source)
        return source;

    ngl::scene* current_scene = ngl::references::current_scene.read();

    return (u8*)get_parameter(current_scene->parameters,
                              parameter_id_scene_light_source.read());
}

static void add_general_directional_light(      general_lighting_parameters* value,
                                          const u8*                          source,
                                                bool                         primary) {

    i32 &count = general_integer(value, 0x1A0);

    if (count >= 4)
        return;

    const f32* direction = (const f32*)(source + (primary ? 0x50 : 0x20));
    const f32  sign      = primary ? 1.0f : -1.0f;

    write_vector(general_vector(value, 0x210 + 0x10 * count),
                 sign * direction[0] * 10000.0f,
                 sign * direction[1] * 10000.0f,
                 sign * direction[2] * 10000.0f,
                 0.0f);

    write_vector(general_vector(value, 0x250 + 0x10 * count),
                 primary ? -direction[0] : direction[0],
                 primary ? -direction[1] : direction[1],
                 primary ? -direction[2] : direction[2],
                 primary ? 0.0f : *(const f32*)(source + 0xD4));

    std::memcpy(general_vector(value, 0x290 + 0x10 * count),
                source + (primary ? 0x40 : 0x90),
                sizeof(vector4));

    write_vector(general_vector(value, 0x2D0 + 0x10 * count),
                 1.0f, 1.0f, 1.0f, 1.0f);

    if (!primary && (*(const u32*)source & 4))
        general_integer(value, 0x570) = count;

    ++count;
}

static void write_general_primary_block(      general_lighting_parameters* value,
                                        const u8*                          source,
                                        const u8*                          light_table,
                                              i32                          table_index,
                                              i32                          table_offset) {

    const vector4 &zero          = lighting_zero_value.get();
    const vector4 &default_value = lighting_default_value.get();

    const f32* direction = (const f32*)(source + 0x50);

    copy_general_vector(value, 0x360, &zero);
    copy_general_vector(value, 0x340, &zero);
    copy_general_vector(value, 0x310, &zero);
    copy_general_vector(value, 0x350, &default_value);
    copy_general_vector(value, 0x320, &default_value);
    copy_general_vector(value, 0x370, &default_value);
    copy_general_vector(value, 0x380, &zero);
    general_vector(value, 0x390)[0] = 0.0f;
    copy_general_vector(value, 0x3A0, &zero);

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

    write_vector(general_vector(value, 0x360), negative, positive, 0.0f, 0.0f);

    f32 horizon_temporary[4] { positive, negative, 0.0f, 0.0f };

    const f32* horizon_base = (const f32*)&lighting_horizon_base.get();

    for (u32 index = 0; index < 4; ++index)
        general_vector(value, 0x380)[index] = horizon_base[index] - horizon_temporary[index];

    write_vector(general_vector(value, 0x350), direction[0], direction[1], direction[2], 0.0f);

    const f32* direction_scale = (const f32*)&lighting_direction_scale.get();
    write_vector(general_vector(value, 0x370),
                 direction[0] * direction_scale[0],
                 direction[1] * direction_scale[1],
                 direction[2] * direction_scale[2],
                 0.0f);

    f32 denominator = scalar + scalar;
    write_vector(general_vector(value, 0x340),
                 *(const f32*)(source + 0x40) / denominator,
                 *(const f32*)(source + 0x44) / denominator,
                 *(const f32*)(source + 0x48) / denominator,
                 1.0f / denominator);

    std::memcpy(general_vector(value, 0x310), source + 0x80, sizeof(vector4));
    std::memcpy(general_vector(value, 0x3A0), source + 0xB0, sizeof(vector4));
    write_vector(general_vector(value, 0x320), 0.0f, 1.0f, 0.0f, 0.0f);
    general_vector(value, 0x574)[0] = 1.0f;

    if (table_offset == 4) {
        const f32* half = (const f32*)&lighting_half.get();

        for (u32 index = 0; index < 4; ++index)
            general_vector(value, 0x310)[index] *= half[index];

        general_vector(value, 0x574)[0] = 0.5f;
    }

    i32 projection_index = table_index + table_offset;

    if (table_index == -1 ||
        projection_index < 0 ||
        projection_index >= *(const i32*)(light_table + 0x14)) {

        copy_general_vector(value, 0x140, &lighting_zero_value.get());
        copy_general_vector(value, 0x150, &lighting_zero_value.get());
    } else {
        const vector4* projections = *(const vector4**)(light_table + 0x18);

        copy_general_vector(value, 0x140, &projections[projection_index * 2 + 0]);
        copy_general_vector(value, 0x150, &projections[projection_index * 2 + 1]);
    }

    add_general_directional_light(value, source, true);
}

static void build_general_lighting(      general_lighting_parameters* value,
                                   const ngl::fx::mesh_node_data*     node_data) {

    initialize_general_lighting(value);

    node_data->mesh_data[0x0B] = 0;
    *(u32*)(node_data->mesh_data + 0x08) |= 0x01000000;

    ngl::lighting::light_context* context;

    if (has_parameter(node_data->parameters, parameter_id_light_context.read()))
        context = (ngl::lighting::light_context*)get_parameter(
            node_data->parameters,
            parameter_id_light_context.read());
    else
        context = ngl::references::current_scene.read()->light_context;

    selected_light_context.write(context);
    context->head.local_next = &context->head;

    u8* source = get_light_source(node_data);
    
    ngl::scene* current_scene = ngl::references::current_scene.read();

    *(u8**)(value->data + 0x190) = source;

    write_vector(general_vector(value, 0x1B0),
                 ((f32*)current_scene)[64],
                 ((f32*)current_scene)[65],
                 ((f32*)current_scene)[66],
                 1.0f);

    write_vector(general_vector(value, 0x1C0),
                 *(f32*)(source + 0x220),
                 *(f32*)(source + 0x224),
                 *(f32*)(source + 0x228),
                 0.0f);

    std::memcpy(general_vector(value, 0x4B0), source + 0x210, sizeof(vector4));
    write_vector(general_vector(value, 0x500),
                 ((f32*)current_scene)[64],
                 ((f32*)current_scene)[65],
                 ((f32*)current_scene)[66],
                 0.0f);

    const f32* normal = (const f32*)(source + 0x230);
    f32 inverse_length = 1.0f / std::sqrt(normal[0] * normal[0] +
                                         normal[1] * normal[1] +
                                         normal[2] * normal[2]);

    write_vector(general_vector(value, 0x510),
                 normal[0] * inverse_length,
                 normal[1] * inverse_length,
                 normal[2] * inverse_length,
                 0.0f);

    write_vector(general_vector(value, 0x520),
                 *(f32*)(source + 0x240),
                 *(f32*)(source + 0x244),
                 *(f32*)(source + 0x248),
                 1.0f);

    write_vector(general_vector(value, 0x530),
                 *(f32*)(source + 0x250),
                 *(f32*)(source + 0x254),
                 *(f32*)(source + 0x258),
                 1.0f);

    write_vector(general_vector(value, 0x540),
                 std::fabs(*(f32*)(source + 0x318)),
                 *(f32*)(source + 0x31C),
                 0.0f,
                 0.0f);

    write_vector(general_vector(value, 0x550),
                 *(f32*)(source + 0x260),
                 *(f32*)(source + 0x264),
                 *(f32*)(source + 0x268),
                 0.0f);

    write_vector(general_vector(value, 0x160),
                 *(f32*)(source + 0x278),
                 *(f32*)(source + 0x270),
                 *(f32*)(source + 0x274),
                 *(f32*)(source + 0x27C));

    std::memcpy(general_vector(value, 0x170), source + 0x2D0, sizeof(vector4));

    f32 fog_near = *(f32*)(source + 0x2E0);
    f32 fog_far  = *(f32*)(source + 0x2E4);

    if (fog_near == fog_far)
        fog_far += 0.00001525879997643642f;

    f32 fog_scale = 1.0f / (fog_far - fog_near);
    u32 fog_magic_bits = 992204554;
    f32 fog_magic;
    std::memcpy(&fog_magic, &fog_magic_bits, sizeof(fog_magic));

    write_vector(general_vector(value, 0x180),
                 fog_scale,
                 -fog_near * fog_scale,
                 *(f32*)(source + 0x2E8),
                 fog_magic);

    u8* table = (u8*)find_parameter(node_data->parameters, parameter_id_light_table.read());

    value->horizon_texture = table ? *(texture**)(table + 0x08) : nullptr;

    i32 table_index  = 0;
    i32 table_offset = 4;

    u8* range = (u8*)find_parameter(node_data->parameters,
                                   parameter_id_light_table_range.read());

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
            ((f32*)&contribution)[index] = *(f32*)(primary + 0x80 + 4 * index) * half[index];
    }

    for (u32 vector_index = 0; vector_index < 8; ++vector_index) {
        for (u32 component = 0; component < 4; ++component) {
            general_vector(value, 0x3B0 + 0x10 * vector_index)[component] =
                *(f32*)(source + 0x00 + 0x10 * vector_index + 4 * component);

            general_vector(value, 0x430 + 0x10 * vector_index)[component] =
                *(f32*)(source + 0x80 + 0x10 * vector_index + 4 * component) + ((f32*)&contribution)[component];
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

        if (general_integer(value, 0x1A0) >= 4)
            break;
    }

    if (*(u32*)node_data->node_info & 2) {
        f32 inverse_scale_squared = 1.0f / (node_data->scale * node_data->scale);

        for (u32 index = 0; index < 4; ++index)
            general_vector(value, 0x2D0 + 0x10 * index)[0] *= inverse_scale_squared;
    }
}

static void get_subset_lighting(      subset_lighting_parameters* destination,
                                const ngl::fx::mesh_node_data*    node_data) {

    std::memset(destination, 0, sizeof(*destination)); // non-trivial

    u8* source = (u8*)find_parameter(node_data->parameters,
                                    parameter_id_light_source.read());

    if (!source) {
        ngl::scene* current_scene = ngl::references::current_scene.read();

        source = (u8*)get_parameter(current_scene->parameters,
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

    u8* table = (u8*)find_parameter(node_data->parameters,
                                   parameter_id_light_table.read());

    if (!table)
        return;

    destination->horizon_texture = *(texture**)(table + 0x08);

    i32 index = 0;
    i32 offset = 4;
    u8* range = (u8*)find_parameter(node_data->parameters,
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

static void transpose_affine_3x4(f32* destination, const f32* source) {
    destination[ 0] = source[ 0];
    destination[ 1] = source[ 4];
    destination[ 2] = source[ 8];
    destination[ 3] = source[12];
    destination[ 4] = source[ 1];
    destination[ 5] = source[ 5];
    destination[ 6] = source[ 9];
    destination[ 7] = source[13];
    destination[ 8] = source[ 2];
    destination[ 9] = source[ 6];
    destination[10] = source[10];
    destination[11] = source[14];
}

static void transpose_matrix(f32* destination, const f32* source) {
    for (u32 row = 0; row < 4; ++row)
        for (u32 column = 0; column < 4; ++column)
            destination[row * 4 + column] = source[column * 4 + row];
}

static void expand_affine_3x4(f32* destination, const f32* source) {
    std::memcpy(destination, source, sizeof(f32) * 12);

    destination[12] = 0.0f;
    destination[13] = 0.0f;
    destination[14] = 0.0f;
    destination[15] = 1.0f;
}

static void copy_affine_matrix(f32* destination, const f32* source) {
    destination[ 0] = source[ 0];
    destination[ 1] = source[ 1];
    destination[ 2] = source[ 2];
    destination[ 3] = 0.0f;
    destination[ 4] = source[ 4];
    destination[ 5] = source[ 5];
    destination[ 6] = source[ 6];
    destination[ 7] = 0.0f;
    destination[ 8] = source[ 8];
    destination[ 9] = source[ 9];
    destination[10] = source[10];
    destination[11] = 0.0f;
    destination[12] = source[12];
    destination[13] = source[13];
    destination[14] = source[14];
    destination[15] = 1.0f;
}

static void multiply_affine_matrices(f32* destination, const f32* left, const f32* right) {

    f32 canonical_left[16];
    f32 canonical_right[16];

    copy_affine_matrix(canonical_left, left);
    copy_affine_matrix(canonical_right, right);
    multiply_matrix(destination, canonical_left, canonical_right);
}

static void identity_matrix(f32* destination) {
    std::memset(destination, 0, sizeof(f32) * 16);

    destination[ 0] = 1.0f;
    destination[ 5] = 1.0f;
    destination[10] = 1.0f;
    destination[15] = 1.0f;
}

static void write_specialized_matrix(      ngl::fx::parameter*      entry,
                                     const ngl::fx::mesh_node_data* node_data,
                                           bool                     subset_effect) {

    u32 type = (u32)entry->type;
    f32 local[16];
    f32 matrix[16];
    f32 temporary[16];
    f32 affine[12];

    ngl::scene* current_scene = ngl::references::current_scene.read();

    copy_affine_matrix(local, (const f32*)&node_data->local_to_world);

    switch (type) {
        case 31:
            transpose_affine_3x4(affine, local);
            expand_affine_3x4((f32*)entry->data, affine);

            break;
        case 32:
            get_local_to_world(local, node_data);
            inverse_orthonormal_matrix(matrix, local);
            transpose_affine_3x4(affine, matrix);
            expand_affine_3x4((f32*)entry->data, affine);

            break;
        case 33:
            copy_affine_matrix((f32*)entry->data, local);

            break;
        case 34:
            get_local_to_world(local, node_data);
            inverse_orthonormal_matrix(matrix, local);
            copy_affine_matrix((f32*)entry->data, matrix);

            break;
        case 43:
            multiply_matrix(matrix, local, (const f32*)&current_scene->world_to_view);
            transpose_affine_3x4(affine, matrix);
            expand_affine_3x4((f32*)entry->data, affine);

            break;
        case 44:
            multiply_matrix(temporary, local, (const f32*)&current_scene->world_to_view);
            inverse_orthonormal_matrix(matrix, temporary);
            transpose_affine_3x4(affine, matrix);
            expand_affine_3x4((f32*)entry->data, affine);

            break;
        case 45:
            multiply_matrix(matrix, local, (const f32*)&current_scene->world_to_view);
            copy_affine_matrix((f32*)entry->data, matrix);

            break;
        case 46:
            multiply_matrix(temporary, local, (const f32*)&current_scene->world_to_view);
            inverse_orthonormal_matrix(matrix, temporary);
            copy_affine_matrix((f32*)entry->data, matrix);

            break;
        case 51:
            multiply_matrix(matrix, local, (const f32*)&current_scene->world_to_screen);
            transpose_matrix((f32*)entry->data, matrix);

            break;
        case 52:
            if (!subset_effect)
                identity_matrix((f32*)entry->data);

            break;
        case 53:
            multiply_matrix(matrix, local, (const f32*)&current_scene->world_to_screen);
            std::memcpy(entry->data, matrix, sizeof(matrix));
            
            break;
        case 54:
            if (!subset_effect)
                identity_matrix((f32*)entry->data);

            break;
    }
}

static void write_bone_matrices(const ngl::fx::mesh_node_data* node_data,
                                const ngl::mesh_section*       section) {

    f32* destination = &bone_constant_data.get();
    f32 matrix[16];
    f32 temporary[16];
    f32 affine[12];

    if (!section->bone_count) {
        transpose_affine_3x4(affine, (const f32*)&node_data->local_to_world);
        std::memcpy(destination + 0, affine, sizeof(affine));
        std::memcpy(destination + 12, affine, sizeof(affine));
        bone_constant_count.write(2);
        
        return;
    }

    u32 flags = *(u32*)node_data->node_info;

    f32* bone_matrices = *(f32**)(node_data->node_info + 8);
    f32* bind_matrices = nullptr;

    if (flags & (4 | 8))
        bind_matrices = *(f32**)(*(u8**)(node_data->mesh_data + 0x14) + 0x0C);

    for (i32 index = 0; index < section->bone_count; ++index) {
        u16 bone_index = section->bone_indices[index];
        
        const f32* bone_matrix = bone_matrices + 16 * bone_index;

        if (flags & 4) {
            const f32* bind_matrix = bind_matrices + 36 * bone_index + 16;

            multiply_affine_matrices(matrix, bind_matrix, bone_matrix);
        } else if (flags & 8) {
            const f32* bind_matrix = bind_matrices + 36 * bone_index + 16;

            multiply_affine_matrices(temporary, bind_matrix, bone_matrix);
            multiply_affine_matrices(matrix,
                                     temporary,
                                     (const f32*)&node_data->local_to_world);
        } else if (flags & 0x10)
            std::memcpy(matrix, bone_matrix, sizeof(matrix));
        else
            std::memcpy(matrix, &node_data->local_to_world, sizeof(matrix));

        transpose_affine_3x4(destination + 12 * index, matrix);
    }

    bone_constant_count.write(section->bone_count);
}

static void write_point_light_positions(      f32*                     destination,
                                        const ngl::fx::mesh_node_data* node_data) {

    f32* lights = &point_light_data.get();
    destination += 4;

    for (u32 index = 0; index < node_data->point_light_count; ++index) {
        u32 light_index = node_data->point_light_indices[index];
        f32* light = lights + 8 * light_index;

        write_vector(destination + 4 * index,
                     light[0], light[1], light[2], 1.0f / light[3]);
    }
}

static void write_point_light_colors(f32* destination,
                                     const ngl::fx::mesh_node_data* node_data) {

    f32* lights = &point_light_data.get();
    u32 index = 0;
    destination += 4;

    for (; index < node_data->point_light_count; ++index) {
        u32 light_index = node_data->point_light_indices[index];
        f32* light = lights + 8 * light_index;

        write_vector(destination + 4 * index, light[4], light[5], light[6], 0.0f);
    }

    for (; index < 8; ++index)
        write_vector(destination + 4 * index, 0.0f, 0.0f, 0.0f, 0.0f);
}

static void write_shadow_size(f32* destination, u32 width, u32 height) {
    write_vector(destination,
                 (f32)width,
                 (f32)height,
                 1.0f / (f32)width,
                 1.0f / (f32)height);
}

static void write_default_environment_color(f32* destination) {
    write_vector(destination,
                 0.03921569f,
                 0.03921569f,
                 0.07843138f,
                 1.0f);

    for (u32 index = 0; index < 4; ++index)
        destination[index] *= destination[index];
}

static bool is_depth_bias_parameter(u32 type) {
    return type == 16                ||
          (type >= 31 && type <= 34) ||
          (type >= 43 && type <= 46) ||
           type == 51                ||
           type == 53                ||
           type == 81                ||
           type == 111               ||
           type == 122               ||
           type == 123;
}

void ngl::fx::update_scene_parameters(effect* value) {
    scene* current_scene = ngl::references::current_scene.read();

    for (parameter* entry = value->parameter_chains[0]; entry; entry = entry->next) {
        f32* destination = (f32*)entry->data;

        switch ((u32)entry->type) {
            case 21:
                destination[0] = current_scene->view_position.x;
                destination[1] = current_scene->view_position.y;
                destination[2] = current_scene->view_position.z;
                destination[3] = 1.0f;

                break;
            case 23:
                destination[0] = (f32)current_scene->color_target->gpu_texture.width;
                destination[1] = (f32)current_scene->color_target->gpu_texture.height;
                destination[2] = 0.0f;
                destination[3] = 0.0f;

                break;
            case 24:
                *(u32*)destination = ngl::references::frame_epoch.read();

                break;
            case 25:
                destination[0] = current_scene->current_animation_time;

                break;
            case 35: {
                f32 affine[12];
                transpose_affine_3x4(affine, (const f32*)&current_scene->world_to_view);
                expand_affine_3x4(destination, affine);

                break;
            }
            case 36: {
                f32 affine[12];
                transpose_affine_3x4(affine, (const f32*)&current_scene->view_to_world);
                expand_affine_3x4(destination, affine);

                break;
            }
            case 37:
                copy_affine_matrix(destination, (const f32*)&current_scene->world_to_view);

                break;
            case 38:
                copy_affine_matrix(destination, (const f32*)&current_scene->view_to_world);

                break;
            case 39:
                transpose_matrix(destination, (const f32*)&current_scene->view_to_screen);

                break;
            case 40:
                identity_matrix(destination);

                break;
            case 41:
                std::memcpy(destination, &current_scene->view_to_screen, sizeof(matrix4x4));

                break;
            case 42:
                identity_matrix(destination);

                break;
            case 47:
                transpose_matrix(destination, (const f32*)&current_scene->world_to_screen);

                break;
            case 48:
                identity_matrix(destination);

                break;
            case 49:
                std::memcpy(destination, &current_scene->world_to_screen, sizeof(matrix4x4));

                break;
            case 50:
                identity_matrix(destination);

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
        build_general_lighting(&general_lighting, node_data);
        active_horizon_texture = general_lighting.horizon_texture;
    } else if (!depth_bias_enabled) {
        get_subset_lighting(&lighting, node_data);
        active_horizon_texture = lighting.horizon_texture;
    }

    copy_parameter_subset(value, node_data);

    for (parameter* entry = value->parameter_chains[1]; entry; entry = entry->next) {
        f32* destination = (f32*)entry->data;
        u32 type = (u32)entry->type;

        if (subset_effect && depth_bias_enabled && !is_depth_bias_parameter(type))
            continue;

        if ((type >= 31 && type <= 34) ||
            (type >= 43 && type <= 46) ||
            (type >= 51 && type <= 54)) {

            write_specialized_matrix(entry, node_data, subset_effect);

            continue;
        }

        switch (type) {
            case 16:
                write_bone_matrices(node_data, section);

                break;
            case 17:
                if (!subset_effect)
                    destination[0] = section && section->bone_count > 0 ? 4.0f : 1.0f;

                break;
            case 55:
                std::memcpy(destination,
                            subset_effect ?
                                (const void*)&lighting.horizon_projection_u : (const void*)general_vector(&general_lighting, 0x140),
                            sizeof(vector4));

                break;
            case 56:
                std::memcpy(destination,
                            subset_effect ?
                                (const void*)&lighting.horizon_projection_v : (const void*)general_vector(&general_lighting, 0x150),
                            sizeof(vector4));

                break;
            case 57: {
                f32 hour = get_hour_of_day();
                f32 night = hour < 3.0f || hour > 23.9f ? 1.0f : 0.0f;
                write_vector(destination,
                             0.60000002f * night,
                             0.5f * night,
                             0.40000001f * night,
                             1.0f);

                break;
            }
            case 58: {
                f32 hour = get_hour_of_day();
                f32 night = hour < 3.0f || hour > 23.9f ? 1.0f : 0.0f;
                write_vector(destination, 1.3f, 1.2f, 0.8f, night);

                break;
            }
            case 59:
                std::memcpy(destination,
                            subset_effect
                                ? (const void*)&lighting.fog_color : (const void*)general_vector(&general_lighting, 0x170),
                            sizeof(vector4));

                break;
            case 60:
                std::memcpy(destination,
                            subset_effect ?
                                (const void*)&lighting.fog_control : (const void*)general_vector(&general_lighting, 0x180),
                            sizeof(vector4));

                break;
            case 61:
                if (!subset_effect) {
                    std::memcpy(destination + 4 * 0,
                                general_vector(&general_lighting, 0x4B0),
                                sizeof(vector4));
                    std::memcpy(destination + 4 * 1,
                                general_vector(&general_lighting, 0x1C0),
                                sizeof(vector4));
                    std::memcpy(destination + 4 * 2,
                                general_vector(&general_lighting, 0x430),
                                sizeof(vector4) * 8);
                    std::memcpy(destination + 4 * 10,
                                general_vector(&general_lighting, 0x3B0),
                                sizeof(vector4) * 8);
                    std::memcpy(destination + 4 * 18,
                                general_vector(&general_lighting, 0x580),
                                sizeof(vector4) * 9);
                }

                break;
            case 62:
                if (!subset_effect) {
                    std::memcpy(destination + 4 * 0,
                                general_vector(&general_lighting, 0x140),
                                sizeof(vector4));
                    std::memcpy(destination + 4 * 1,
                                general_vector(&general_lighting, 0x150),
                                sizeof(vector4));
                    std::memcpy(destination + 4 * 2,
                                general_vector(&general_lighting, 0x290),
                                sizeof(vector4));
                    std::memcpy(destination + 4 * 3,
                                general_vector(&general_lighting, 0x250),
                                sizeof(vector4));
                    std::memcpy(destination + 4 * 4,
                                general_vector(&general_lighting, 0x360),
                                sizeof(vector4));
                    std::memcpy(destination + 4 * 5,
                                general_vector(&general_lighting, 0x310),
                                sizeof(vector4));
                    std::memcpy(destination + 4 * 6,
                                general_vector(&general_lighting, 0x370),
                                sizeof(vector4));
                    std::memcpy(destination + 4 * 7,
                                general_vector(&general_lighting, 0x380),
                                sizeof(vector4));

                    f32 scalar = general_vector(&general_lighting, 0x390)[0];
                    write_vector(destination + 4 * 8,
                                 scalar, scalar, scalar, scalar);

                    std::memcpy(destination + 4 * 9,
                                general_vector(&general_lighting, 0x3A0),
                                sizeof(f32) * 3);
                    destination[4 * 9 + 3] = 0.0f;

                    std::memcpy(destination + 4 * 10,
                                general_vector(&general_lighting, 0x320),
                                sizeof(f32) * 3);
                    destination[4 * 10 + 3] = 0.0f;
                }

                break;
            case 63: {
                write_texture(entry,
                              active_horizon_texture ? active_horizon_texture : horizon_texture.read());

                break;
            }
            case 64:
                if (!subset_effect) {
                    std::memcpy(destination + 4 * 0,
                                general_vector(&general_lighting, 0x210),
                                sizeof(vector4) * 4);
                    std::memcpy(destination + 4 * 4,
                                general_vector(&general_lighting, 0x250),
                                sizeof(vector4) * 4);
                    std::memcpy(destination + 4 * 8,
                                general_vector(&general_lighting, 0x290),
                                sizeof(vector4) * 4);
                    std::memcpy(destination + 4 * 12,
                                general_vector(&general_lighting, 0x2D0),
                                sizeof(vector4) * 4);
                }

                break;
            case 65:
                std::memcpy(destination, &shadow_distances.get(), sizeof(vector4));

                break;
            case 66:
            case 67:
                write_shadow_size(destination, shadow_width_0.read(), shadow_height_0.read());

                break;
            case 68:
                write_shadow_size(destination, shadow_width_2.read(), shadow_height_2.read());

                break;
            case 69:
            case 70:
                transpose_matrix(destination, (const f32*)&shadow_matrix_0.get());

                break;
            case 71:
                transpose_matrix(destination, (const f32*)&shadow_matrix_2.get());

                break;
            case 72:
            case 73:
                write_texture(entry, shadow_texture_0.read());

                break;
            case 74:
                write_texture(entry, shadow_texture_2.read());

                break;
            case 75:
                if (!subset_effect) {
                    const f32* primary_direction = general_vector(&general_lighting, 0x510);
                    const f32* first_plane = general_vector(&general_lighting, 0x520);
                    const f32* second_plane = general_vector(&general_lighting, 0x530);

                    std::memcpy(destination + 4 * 0,
                                primary_direction,
                                sizeof(vector4));

                    write_vector(destination + 4 * 1,
                                 primary_direction[0] * first_plane[0] +
                                 primary_direction[1] * first_plane[1] +
                                 primary_direction[2] * first_plane[2],
                                 primary_direction[0] * second_plane[0] +
                                 primary_direction[1] * second_plane[1] +
                                 primary_direction[2] * second_plane[2],
                                 0.0f,
                                 0.0f);

                    std::memcpy(destination + 4 * 2,
                                general_vector(&general_lighting, 0x540),
                                sizeof(vector4));
                    std::memcpy(destination + 4 * 3,
                                general_vector(&general_lighting, 0x550),
                                sizeof(vector4));
                    std::memcpy(destination + 4 * 4,
                                general_vector(&general_lighting, 0x170),
                                sizeof(vector4));
                    std::memcpy(destination + 4 * 5,
                                general_vector(&general_lighting, 0x180),
                                sizeof(vector4));

                    u8* light_source = (u8*)find_parameter(node_data->parameters,
                                                           parameter_id_light_source.read());

                    if (light_source) {
                        f32 scalar_value = *(f32*)(light_source + 0x200);
                        write_vector(destination + 4 * 6,
                                     scalar_value,
                                     scalar_value,
                                     scalar_value,
                                     scalar_value);
                    } else
                        write_vector(destination + 4 * 6,
                                     -2.0f, -2.0f, -2.0f, -2.0f);

                    std::memcpy(destination + 4 * 7,
                                general_vector(&general_lighting, 0x500),
                                sizeof(vector4));
                    std::memcpy(destination + 4 * 8,
                                first_plane,
                                sizeof(vector4));
                    std::memcpy(destination + 4 * 9,
                                second_plane,
                                sizeof(vector4));
                }

                break;
            case 76:
                write_texture(entry, depth_texture.read());

                break;
            case 77:
                for (u32 index = 0; index < 4; ++index)
                    destination[index] = ((f32*)&temporary_0.get())[index] * 2.0f;

                break;
            case 78:
                std::memcpy(destination,
                            subset_effect ? &subset_shadow_factor.get() : &shadow_factor.get(),
                            sizeof(vector4));

                break;
            case 79:
                write_vector(destination,
                             shared_scalar.read(),
                             shared_scalar.read(),
                             shared_scalar.read(),
                             shared_scalar.read());

                break;
            case 80:
                std::memcpy(destination, &constant_80.get(), sizeof(vector4));

                break;
            case 81:
                write_vector(destination, 50.0f, 0.0199999996f, 0.0f, 0.0f);

                break;
            case 82:
                destination[0] = subset_effect ?
                    (f32)lighting.directional_light_count : (f32)general_integer(&general_lighting, 0x110);

                break;
            case 83:
                if (subset_effect) {
                    for (i32 index = 0; index < lighting.directional_light_count; ++index) {
                        const vector4 &direction =
                            lighting.directional_light_directions[index];

                        write_vector(destination + 4 + 4 * index,
                                     direction.x,
                                     direction.y,
                                     direction.z,
                                     0.0f);
                    }
                } else {
                    for (i32 index = 0; index < general_integer(&general_lighting, 0x110); ++index) {

                        const f32* direction = general_vector(&general_lighting, 0x120 + 0x20 * index);

                        write_vector(destination + 4 + 4 * index,
                                     direction[0],
                                     direction[1],
                                     direction[2],
                                     0.0f);
                    }
                }

                break;
            case 84:
                if (subset_effect) {
                    for (i32 index = 0; index < lighting.directional_light_count; ++index) {
                        const vector4 &color = lighting.directional_light_colors[index];

                        write_vector(destination + 4 + 4 * index,
                                     color.x,
                                     color.y,
                                     color.z,
                                     0.0f);
                    }
                } else {
                    for (i32 index = 0; index < general_integer(&general_lighting, 0x110); ++index) {

                        const f32* color = general_vector(&general_lighting, 0x130 + 0x20 * index);

                        write_vector(destination + 4 + 4 * index,
                                     color[0],
                                     color[1],
                                     color[2],
                                     0.0f);
                    }
                }

                break;
            case 85:
                destination[0] = (f32)node_data->point_light_count;

                break;
            case 86:
                write_point_light_positions(destination, node_data);

                break;
            case 87:
                write_point_light_colors(destination, node_data);

                break;
            case 92:
                std::memcpy(destination,
                            subset_effect ?
                                (const void*)&lighting.ambient_info : (const void*)general_vector(&general_lighting, 0x160),
                            sizeof(vector4));

                break;
            case 93: {
                void* color = find_parameter(node_data->parameters,
                                             parameter_id_character_color.read());

                if (color)
                    std::memcpy(destination, color, sizeof(vector4));
                else
                    write_vector(destination, 1.0f, 1.0f, 1.0f, 1.0f);

                break;
            }
            case 104:
                write_texture(entry,
                              subset_effect ? framebuffer_texture.read()
                                            : framebuffer_texture_general.read());
                                            
                break;
            case 105:
                if (!subset_effect)
                    write_texture(entry, environment_texture.read());

                break;
            case 107: {
                f32* color = (f32*)find_parameter(node_data->parameters,
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
            case 108: {
                f32* matrix = (f32*)find_parameter(node_data->parameters,
                                                   parameter_id_decal_projection.read());

                if (matrix)
                    transpose_matrix(destination, matrix);
                else
                    identity_matrix(destination);

                break;
            }
            case 109: {
                f32 local_to_world[16];
                f32 matrix[16];

                scene* current_scene = ngl::references::current_scene.read();

                copy_affine_matrix(local_to_world,
                                   (const f32*)&node_data->local_to_world);
                multiply_matrix(matrix,
                                local_to_world,
                                (const f32*)&current_scene->derived_matrix_250);
                transpose_matrix(destination, matrix);

                break;
            }
            case 110: {
                f32 local_to_world[16];
                f32 world_to_view[16];
                f32 world_to_projection[16];
                f32 world_to_viewport[16];
                
                scene* current_scene = ngl::references::current_scene.read();

                copy_affine_matrix(local_to_world,
                                   (const f32*)&node_data->local_to_world);
                multiply_matrix(world_to_view,
                                local_to_world,
                                (const f32*)&current_scene->world_to_view);
                multiply_matrix(world_to_projection,
                                world_to_view,
                                (const f32*)&current_scene->projection);
                multiply_matrix(world_to_viewport,
                                world_to_projection,
                                (const f32*)&current_scene->view);
                transpose_matrix(destination, world_to_viewport);

                break;
            }
            case 111: {
                void* parameters = find_parameter(node_data->parameters,
                                                  parameter_id_ui_parameters.read());
                std::memcpy(destination,
                            parameters ? parameters : &ui_parameters.get(),
                            sizeof(vector4));

                break;
            }
            case 112: {
                void* color = find_parameter(node_data->parameters,
                                             parameter_id_tint_color.read());
                std::memcpy(destination,
                            color ? color : &tint_color.get(),
                            sizeof(vector4));

                break;
            }
            case 115: {
                f32* matrix = (f32*)find_parameter(node_data->parameters,
                                                   parameter_id_decal_texture_matrix.read());
                if (matrix)
                    std::memcpy(destination + 4, matrix, sizeof(f32) * 8);
                else {
                    write_vector(destination + 4,
                                 1.0f, 0.0f, 0.0f, 0.0f);
                    write_vector(destination + 8,
                                 0.0f, 1.0f, 0.0f, 0.0f);
                }

                break;
            }
            case 122:
                std::memcpy(destination, node_data->mesh_data + 0x30, sizeof(f32) * 3);
                destination[3] = 0.0f;

                break;
            case 123:
                std::memcpy(destination, node_data->mesh_data + 0x20, sizeof(f32) * 3);
                destination[3] = 1.0f;

                break;
            case 124: {
                void* data = find_parameter(node_data->parameters,
                                            parameter_id_last.read());
                if (data)
                    std::memcpy(destination, data, sizeof(vector4));

                break;
            }
        }
    }

}
