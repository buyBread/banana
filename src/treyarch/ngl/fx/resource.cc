#include <cstdio>
#include <cstring>

#include "treyarch/ngl/d3d9/shader_program_cache.hh"
#include "treyarch/ngl/fx/effect.hh"
#include "treyarch/ngl/ngl.hh"
#include "treyarch/ngl/resources/resolver.hh"
#include "treyarch/ngl/texture/texture.hh"
#include "treyarch/shared/four_cc.hh"
#include "treyarch/shared/hash/algo.hh"
#include "treyarch/shared/memory/game_heap.hh"

using namespace treyarch;

struct effect_variant_family {
    const char* names[4];
};

struct effect_priority_prefix {
    const char* prefix;
          i32   priority;
};

static const effect_variant_family effect_variant_families[] {
    {{ "building_quad",                  "building_quad_0ptlights",                  "building_quad_2ptlights",                  "building_quad_4ptlights"                  }},
    {{ "building_walls",                 "building_walls_0ptlights",                 "building_walls_2ptlights",                 "building_walls_4ptlights"                 }},
    {{ "building_walls_dirty_diff",      "building_walls_dirty_diff_0ptlights",      "building_walls_dirty_diff_2ptlights",      "building_walls_dirty_diff_4ptlights"      }},
    {{ "building_walls_dirty_norm",      "building_walls_dirty_norm_0ptlights",      "building_walls_dirty_norm_2ptlights",      "building_walls_dirty_norm_4ptlights"      }},
    {{ "building_walls_dirty_diff_norm", "building_walls_dirty_diff_norm_0ptlights", "building_walls_dirty_diff_norm_2ptlights", "building_walls_dirty_diff_norm_4ptlights" }},
    {{ "road",                           "road_0ptlights",                           "road_2ptlights",                           "road_4ptlights"                           }},
    {{ "road_flat",                      "road_flat_0ptlights",                      "road_flat_2ptlights",                      "road_flat_4ptlights"                      }},
    {{ "road_decal",                     "road_decal_0ptlights",                     "road_decal_2ptlights",                     "road_decal_4ptlights"                     }},
    {{ "road_flat_decal",                "road_flat_decal_0ptlights",                "road_flat_decal_2ptlights",                "road_flat_decal_4ptlights"                }},
    {{ "sidewalk",                       "sidewalk_0ptlights",                       "sidewalk_2ptlights",                       "sidewalk_4ptlights"                       }},
    {{ "sidewalk_flat",                  "sidewalk_flat_0ptlights",                  "sidewalk_flat_2ptlights",                  "sidewalk_flat_4ptlights"                  }},
    {{ "sidewalk_decal",                 "sidewalk_decal_0ptlights",                 "sidewalk_decal_2ptlights",                 "sidewalk_decal_4ptlights"                 }},
    {{ "sidewalk_flat_decal",            "sidewalk_flat_decal_0ptlights",            "sidewalk_flat_decal_2ptlights",            "sidewalk_flat_decal_4ptlights"            }},
    {{ "terrain_shared",                 "terrain_shared_0ptlights",                 "terrain_shared_2ptlights",                 "terrain_shared_4ptlights"                 }},
    {{ "rvb_symbiote",                   "rvb_symbiote_0ptlights",                   "rvb_symbiote_2ptlights",                   "rvb_symbiote_4ptlights"                   }}
};

static const effect_priority_prefix effect_priority_prefixes[] {
    { "building_walls",      100 },
    { "building_quad",        90 },
    { "storefront",           80 },
    { "buildinglod_roof",     70 },
    { "buildinglod_shared",   70 },
    { "jumbotron",            20 },
    { "carpaint",             20 },
    { "shell_Skin",           20 },
    { "shell_",               20 },
    { "spidey_",              20 },
    { "subsurface",           20 },
    { "tentacle",             20 },
    { "terrain",              10 },
    { "sidewalk",              0 },
    { "road",                  0 }
};

ngl::fx::parameter* find_parameter_by_name(ngl::fx::effect* value,
                                                  string_hash      name) {

    for (i32 index = 0; index < value->parameter_count; ++index) {
        ngl::fx::parameter &parameter = value->parameters[index];

        if (parameter.name.hash == name)
            return &parameter;
    }

    return nullptr;
}

ngl::fx::parameter* find_parameter_by_semantic(ngl::fx::effect* value,
                                               string_hash      semantic) {

    for (i32 index = 0; index < value->parameter_count; ++index) {
        ngl::fx::parameter &parameter = value->parameters[index];

        if (parameter.semantic.hash == semantic)
            return &parameter;
    }

    return nullptr;
}

bool is_direct_binding_type(ngl::fx::e_parameter_type type) {
    if ((type >= ngl::fx::parameter_horizon_projection_u && type <= ngl::fx::parameter_horizon_info) ||
        (type >= ngl::fx::parameter_light_info && type <= ngl::fx::parameter_view_projection_shadow_2) ||
        (type >= ngl::fx::parameter_temporary_0 && type <= ngl::fx::parameter_number_directional_lights) ||
        (type >= ngl::fx::parameter_ibl_parameters && type <= ngl::fx::parameter_tentacle_width_frequency_offset_x) ||
        (type >= ngl::fx::parameter_environment_color && type <= ngl::fx::parameter_tentacle_basis_color_1) ||
        (type >= ngl::fx::parameter_tentacle_width_frequency_offset_y && type <= ngl::fx::parameter_last)) {

        return true;
    }

    switch (type) {
        case ngl::fx::parameter_bool:
        case ngl::fx::parameter_int:
        case ngl::fx::parameter_float:
        case ngl::fx::parameter_vector:
        case ngl::fx::parameter_matrix:
        case ngl::fx::parameter_scene_matrix:
        case ngl::fx::parameter_bone_influences:
        case ngl::fx::parameter_unknown_20:
        case ngl::fx::parameter_view_position:
        case ngl::fx::parameter_frame:
        case ngl::fx::parameter_time:
        case ngl::fx::parameter_unknown_26:
        case ngl::fx::parameter_unknown_27:
        case ngl::fx::parameter_post_info:
        case ngl::fx::parameter_number_point_lights:
        case ngl::fx::parameter_unknown_88:
            return true;

        default:
            return false;
    }
}

// array data starts after a 16-byte header, including vector, matrix and lighting arrays
bool is_array_binding_type(ngl::fx::e_parameter_type type) {
    switch (type) {
        case ngl::fx::parameter_bool_array:
        case ngl::fx::parameter_int_array:
        case ngl::fx::parameter_float_array:
        case ngl::fx::parameter_vector_array:
        case ngl::fx::parameter_matrix_array:
        case ngl::fx::parameter_directional_light_directions:
        case ngl::fx::parameter_directional_light_colors:
        case ngl::fx::parameter_point_light_positions_ranges:
        case ngl::fx::parameter_point_light_colors:
        case ngl::fx::parameter_unknown_89:
        case ngl::fx::parameter_unknown_90:
        case ngl::fx::parameter_unknown_91:
        case ngl::fx::parameter_decal_texture_matrix:
            return true;

        default:
            return false;
    }
}

bool is_forced_default_texture_type(ngl::fx::e_parameter_type type) {
    switch (type) {
        case ngl::fx::parameter_horizon_texture:
        case ngl::fx::parameter_shadow_texture:
        case ngl::fx::parameter_shadow_texture_1:
        case ngl::fx::parameter_shadow_texture_2:
        case ngl::fx::parameter_depth_texture:
        case ngl::fx::parameter_framebuffer_texture:
        case ngl::fx::parameter_environment_map:
        case ngl::fx::parameter_unknown_106:
            return true;

        default:
            return false;
    }
}

void resolve_texture_parameter(ngl::fx::parameter &parameter) {
    ngl::texture* texture = *(ngl::texture**)parameter.data;

    if (!texture)
        texture = ngl::references::default_texture.read();

    *(ngl::texture**)parameter.data = texture;
}

void initialize_binding(ngl::fx::effect           &value,
                               ngl::fx::function_binding &binding) {

    ngl::fx::parameter &parameter = value.parameters[binding.parameter_index];

    ngl::fx::e_parameter_type type = parameter.type;

    if (is_direct_binding_type(type)) {
        binding.source_class  = ngl::fx::binding_parameters;
        binding.source_offset = (u32)parameter.data - (u32)value.parameter_data;

        return;
    }

    if (is_array_binding_type(type)) {
        binding.source_class  = ngl::fx::binding_parameters;
        binding.source_offset = (u32)parameter.data - (u32)value.parameter_data + 0x10;

        return;
    }

    if (type == ngl::fx::parameter_sampler) {
        i32 texture_parameter_index = *(i32*)parameter.data;

        if (texture_parameter_index != -1) {
            ngl::fx::parameter &texture_parameter = value.parameters[texture_parameter_index];

            if (is_forced_default_texture_type(texture_parameter.type))
                *(ngl::texture**)texture_parameter.data = ngl::references::default_texture.read();

            resolve_texture_parameter(texture_parameter);

            binding.source_class  = ngl::fx::binding_parameters;
            binding.source_offset = (u32)texture_parameter.data - (u32)value.parameter_data;

            return;
        }
    } else if (type == ngl::fx::parameter_bone_array_world) {
        binding.source_class  = ngl::fx::binding_bones;
        binding.source_offset = 0;

        return;
    } else if (type == ngl::fx::parameter_view_direction) {
        binding.source_class  = ngl::fx::binding_scene;
        binding.source_offset = offsetof(ngl::scene, view_direction);

        return;
    }

    binding.source_class  = binding.destination_type == ngl::fx::binding_sampler ? ngl::fx::binding_fallback_texture : ngl::fx::binding_global_constants;
    binding.source_offset = 0;
}

void initialize_function_bindings(ngl::fx::effect   &value,
                                         ngl::fx::function &function) {

    for (i32 index = 0; index < function.binding_count; ++index)
        initialize_binding(value, function.bindings[index]);
}

void create_vertex_program(ngl::fx::effect   &value,
                                  ngl::fx::function &function) {

    if (!function.microcode)
        return;

    ngl::d3d9::shader_program_cache::create_vertex_program
        (function.microcode, (IDirect3DVertexShader9**)&function.shader);

    initialize_function_bindings(value, function);
}

void create_pixel_program(ngl::fx::effect   &value,
                                 ngl::fx::function &function) {

    if (!function.microcode)
        return;

    ngl::d3d9::shader_program_cache::create_pixel_program
        (function.microcode, (IDirect3DPixelShader9**)&function.shader);

    initialize_function_bindings(value, function);
}

void initialize_pass(ngl::fx::effect &value,
                            ngl::fx::pass   &pass) {

    pass.active_programs = &pass.programs;

    if (pass.states.alpha_test_enabled)
        pass.programs.alpha_test_enabled = 1;

    create_vertex_program(value, pass.programs.vertex_programs[0]);
    create_pixel_program (value, pass.programs.pixel_programs [0]);
    create_vertex_program(value, pass.programs.vertex_programs[1]);
    create_pixel_program (value, pass.programs.pixel_programs [1]);
}

// the value at +0x40 says which scene matrix to use and whether to invert or transpose it
enum e_matrix_components : u32 {
    matrix_world = 0x01,
    matrix_view = 0x02,
    matrix_projection = 0x03,
    matrix_world_view = 0x09,
    matrix_view_projection = 0x0E,
    matrix_world_view_projection = 0x39,
    matrix_inverse = 0x40,
    matrix_transpose = 0x80
};

ngl::fx::e_parameter_type specialized_view_direction_type(u32 component_mask) {
    switch (component_mask) {
        case matrix_world: return ngl::fx::parameter_world;
        case matrix_view: return ngl::fx::parameter_view;
        case matrix_projection: return ngl::fx::parameter_projection;
        case matrix_world_view: return ngl::fx::parameter_world_view;
        case matrix_view_projection: return ngl::fx::parameter_view_projection;
        case matrix_world_view_projection: return ngl::fx::parameter_world_view_projection;
        case matrix_world | matrix_inverse: return ngl::fx::parameter_world_inverse;
        case matrix_view | matrix_inverse: return ngl::fx::parameter_view_inverse;
        case matrix_projection | matrix_inverse: return ngl::fx::parameter_projection_inverse;
        case matrix_world_view | matrix_inverse: return ngl::fx::parameter_world_view_inverse;
        case matrix_view_projection | matrix_inverse: return ngl::fx::parameter_view_projection_inverse;
        case matrix_world_view_projection | matrix_inverse: return ngl::fx::parameter_world_view_projection_inverse;
        case matrix_world | matrix_transpose: return ngl::fx::parameter_world_transpose;
        case matrix_view | matrix_transpose: return ngl::fx::parameter_view_transpose;
        case matrix_projection | matrix_transpose: return ngl::fx::parameter_projection_transpose;
        case matrix_world_view | matrix_transpose: return ngl::fx::parameter_world_view_transpose;
        case matrix_view_projection | matrix_transpose: return ngl::fx::parameter_view_projection_transpose;
        case matrix_world_view_projection | matrix_transpose: return ngl::fx::parameter_world_view_projection_transpose;
        case matrix_world | matrix_inverse | matrix_transpose: return ngl::fx::parameter_world_inverse_transpose;
        case matrix_view | matrix_inverse | matrix_transpose: return ngl::fx::parameter_view_inverse_transpose;
        case matrix_projection | matrix_inverse | matrix_transpose: return ngl::fx::parameter_projection_inverse_transpose;
        case matrix_world_view | matrix_inverse | matrix_transpose: return ngl::fx::parameter_world_view_inverse_transpose;
        case matrix_view_projection | matrix_inverse | matrix_transpose: return ngl::fx::parameter_view_projection_inverse_transpose;
        case matrix_world_view_projection | matrix_inverse | matrix_transpose: return ngl::fx::parameter_world_view_projection_inverse_transpose;
        default:   return ngl::fx::parameter_scene_matrix;
    }
}

void specialize_parameter_types(ngl::fx::effect& value) {
    for (i32 index = 0; index < value.parameter_count; ++index) {
        ngl::fx::parameter &parameter = value.parameters[index];

        if (parameter.type != ngl::fx::parameter_scene_matrix)
            continue;

        u32 component_mask = *(u32*)((u8*)parameter.data + 0x40);

        parameter.type = specialized_view_direction_type(component_mask);
    }
}

bool belongs_to_parameter_chain_0(ngl::fx::e_parameter_type type) {
    return type == ngl::fx::parameter_unknown_20 ||
           type == ngl::fx::parameter_view_position ||
           (type >= ngl::fx::parameter_viewport_pixel_size && type <= ngl::fx::parameter_unknown_30) ||
           (type >= ngl::fx::parameter_view && type <= ngl::fx::parameter_projection_inverse_transpose) ||
           (type >= ngl::fx::parameter_view_projection && type <= ngl::fx::parameter_view_projection_inverse_transpose);
}

bool belongs_to_parameter_chain_1(ngl::fx::e_parameter_type type) {
    return type == ngl::fx::parameter_bone_array_world ||
           type == ngl::fx::parameter_bone_influences ||
           type == ngl::fx::parameter_unknown_19 ||
           (type >= ngl::fx::parameter_world && type <= ngl::fx::parameter_world_inverse_transpose) ||
           (type >= ngl::fx::parameter_world_view && type <= ngl::fx::parameter_world_view_inverse_transpose) ||
           (type >= ngl::fx::parameter_world_view_projection && type <= ngl::fx::parameter_last);
}

void build_parameter_chains(ngl::fx::effect &value) {
    value.parameter_chains[0] = nullptr;
    value.parameter_chains[1] = nullptr;
    value.parameter_chains[2] = nullptr;

    for (i32 index = value.parameter_count - 1; index >= 0; --index) {
        ngl::fx::parameter &parameter = value.parameters[index];
        ngl::fx::e_parameter_type type = parameter.type;

        if (belongs_to_parameter_chain_1(type)) {
            parameter.next = value.parameter_chains[1];
            value.parameter_chains[1] = &parameter;
        } else if (belongs_to_parameter_chain_0(type)) {
            parameter.next = value.parameter_chains[0];
            value.parameter_chains[0] = &parameter;
        }
    }
}

void classify_effect(ngl::fx::effect &value) {
    fixed_string transparency_name = make_fixed_string("SW_UsesTransparency");
    
    ngl::fx::parameter* transparency_parameter =
        find_parameter_by_name(&value, transparency_name.hash);

    bool uses_transparency = transparency_parameter ?
        *(u32*)transparency_parameter->data != 0 : false;

    value.flags &= ~0x10u;

    if (value.technique_count > 1 &&
        value.techniques[0].name.hash == value.techniques[value.technique_count / 2].name.hash) {

        value.flags |= 0x10u;
    }

    for (i32 technique_index = 0; technique_index < value.technique_count; ++technique_index) {
        ngl::fx::technique &technique = value.techniques[technique_index];

        technique.flags = uses_transparency;

        for (i32 pass_index = 0; pass_index < technique.pass_count; ++pass_index) {
            if (!technique.passes[pass_index].states.alpha_blend_enabled)
                continue;

            technique.flags |= 1;
            break;
        }
    }

    bool uses_framebuffer_texture = false;

    for (i32 parameter_index = 0; parameter_index < value.parameter_count; ++parameter_index) {
        if (value.parameters[parameter_index].type != ngl::fx::parameter_framebuffer_texture)
            continue;

        uses_framebuffer_texture = true;

        break;
    }

    if (uses_framebuffer_texture) {
        for (i32 technique_index = 0; technique_index < value.technique_count; ++technique_index)
            value.techniques[technique_index].flags |= 0x20;
    }

    fixed_string parameter_subset_name = make_fixed_string("ParamSubset");

    if (!find_parameter_by_name(&value, parameter_subset_name.hash))
        return;

    value.flags |= 0x40;

    fixed_string decomposed_position_scale = make_fixed_string("DECOMP_POS_SCALE");

    if (find_parameter_by_semantic(&value, decomposed_position_scale.hash))
        value.flags |= 0x80;

    fixed_string bone_array_world = make_fixed_string("BoneArrayWorld");

    if (find_parameter_by_semantic(&value, bone_array_world.hash))
        value.flags |= 0x100;
}

ngl::fx::effect* resolve_effect(const char* name) {
    fixed_string lookup = make_fixed_string(name);

    return (ngl::fx::effect*)ngl::resources::resolve(&lookup, four_cc('F', 'X', '\0'));
}

void resolve_light_variants(ngl::fx::effect_runtime &runtime) {
    for (const effect_variant_family &family : effect_variant_families) {
        if (runtime.owner->name.hash != string_hash(hash::djb2(family.names[0])))
            continue;

        runtime.zero_point_lights = resolve_effect(family.names[1]);
        runtime.two_point_lights  = resolve_effect(family.names[2]);
        runtime.four_point_lights = resolve_effect(family.names[3]);

        return;
    }
}

i32 get_effect_priority(const ngl::fx::effect &value) {
    char hash_name[11];

    const char* name = value.name.text;

    if (!name) {
        std::sprintf(hash_name, "0x%08X", value.name.hash.source_hash_code);
        
        name = hash_name;
    }

    for (const effect_priority_prefix &entry : effect_priority_prefixes) {
        size_t length = std::strlen(entry.prefix);

        if (!_strnicmp(name, entry.prefix, length))
            return entry.priority;
    }

    return 50;
}

ngl::fx::effect_runtime* create_effect_runtime(ngl::fx::effect &value) {
    auto* runtime = (ngl::fx::effect_runtime*)
        memory::game_heap::allocate_small_block(sizeof(ngl::fx::effect_runtime));

    runtime->owner             = &value;
    runtime->zero_point_lights = nullptr;
    runtime->two_point_lights  = nullptr;
    runtime->four_point_lights = nullptr;

    if (value.technique_count) {
        runtime->technique_batches = (ngl::fx::technique_batch*)
            memory::game_heap::allocate_small_block(sizeof(ngl::fx::technique_batch) * value.technique_count);

        for (i32 index = 0; index < value.technique_count; ++index) {
            runtime->technique_batches[index].head  = nullptr;
            runtime->technique_batches[index].count = 0;
        }
    } else
        runtime->technique_batches = nullptr;

    runtime->next     = nullptr;
    runtime->priority = get_effect_priority(value);
    runtime->queued   = 0;

    resolve_light_variants(*runtime);

    return runtime;
}

void destroy_effect_runtime(ngl::fx::effect &value) {
    ngl::fx::effect_runtime* runtime = value.runtime;

    if (!runtime)
        return;

    runtime->owner             = nullptr;
    runtime->zero_point_lights = nullptr;
    runtime->two_point_lights  = nullptr;
    runtime->four_point_lights = nullptr;

    if (runtime->technique_batches) {
        memory::game_heap::free_small_block(runtime->technique_batches);
        runtime->technique_batches = nullptr;
    }

    runtime->next   = nullptr;
    runtime->queued = 0;

    memory::game_heap::free_small_block(runtime);

    value.runtime = nullptr;
}

ngl::fx::effect* ngl::fx::find(string_hash name) {
    return references::effects.get().find(name);
}

void ngl::fx::initialize_directory() {
    references::effects.get().initialize();
}

void __cdecl ngl::fx::load(amalga::file*       owner,
                           amalga::file_entry* entry,
                           void**              mapped_sections,
                           void*               user_data) {

    (void)entry;
    (void)user_data;

    references::parameter_texture_fallback        .write(ngl::references::default_texture.read());
    references::parameter_texture_fallback_pointer.write(&references::parameter_texture_fallback.get());

    i32 image_section = owner->find_section_index(string_hash(four_cc('I', 'M', 'G')));
    effect* value = (effect*)mapped_sections[image_section];

    for (i32 technique_index = 0; technique_index < value->technique_count; ++technique_index) {
        technique &technique = value->techniques[technique_index];

        for (i32 pass_index = 0; pass_index < technique.pass_count; ++pass_index)
            initialize_pass(*value, technique.passes[pass_index]);
    }

    specialize_parameter_types(*value);
    build_parameter_chains(*value);
    classify_effect(*value);

    value->runtime = create_effect_runtime(*value);

    if (!ngl::references::resource_callback.read())
        references::effects.get().insert(value);
}

void __cdecl ngl::fx::remove(amalga::file*       owner,
                             amalga::file_entry* entry,
                             void**              mapped_sections,
                             void*               user_data) {

    (void)entry;
    (void)user_data;

    i32 image_section = owner->find_section_index(string_hash(four_cc('I', 'M', 'G')));
    effect* value = (effect*)mapped_sections[image_section];

    references::effects.get().erase(value);
    destroy_effect_runtime(*value);
}
