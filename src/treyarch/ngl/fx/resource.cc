#include <cstring>

#include "treyarch/ngl/fx/effect.hh"
#include "treyarch/ngl/fx/parameters.hh"
#include "treyarch/ngl/gpu/d3d9/shader_program_cache.hh"
#include "treyarch/ngl/ngl.hh"
#include "treyarch/ngl/resources/resources.hh"
#include "treyarch/ngl/texture/texture.hh"
#include "game/four_cc.hh"
#include "game/hash/algo.hh"

using namespace treyarch;

struct effect_variant_family {
    const char* names[4];
};

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

// inlined @ sub_9E1A90
ngl::fx::parameter* find_parameter_by_name(ngl::fx::effect* value,
                                           string_hash      name) {

    for (i32 index = 0; index < value->parameter_count; ++index) {
        ngl::fx::parameter &parameter = value->parameters[index];

        if (parameter.name.hash == name)
            return &parameter;
    }

    return nullptr;
}

// inlined @ sub_9E1A90
ngl::fx::parameter* find_parameter_by_semantic(ngl::fx::effect* value,
                                               string_hash      semantic) {

    for (i32 index = 0; index < value->parameter_count; ++index) {
        ngl::fx::parameter &parameter = value->parameters[index];

        if (parameter.semantic.hash == semantic)
            return &parameter;
    }

    return nullptr;
}

// sub_9DF2B0
void specialize_scene_matrix(ngl::fx::parameter* value) {
    using namespace ngl::fx;

    switch (*(u32*)((u8*)value->data + 0x40)) {
        case matrix_world: value->type = parameter_world; break;
        case matrix_view: value->type = parameter_view; break;
        case matrix_projection: value->type = parameter_projection; break;
        case matrix_world_view: value->type = parameter_world_view; break;
        case matrix_view_projection: value->type = parameter_view_projection; break;
        case matrix_world_view_projection: value->type = parameter_world_view_projection; break;
        case matrix_world | matrix_inverse: value->type = parameter_world_inverse; break;
        case matrix_view | matrix_inverse: value->type = parameter_view_inverse; break;
        case matrix_projection | matrix_inverse: value->type = parameter_projection_inverse; break;
        case matrix_world_view | matrix_inverse: value->type = parameter_world_view_inverse; break;
        case matrix_view_projection | matrix_inverse: value->type = parameter_view_projection_inverse; break;
        case matrix_world_view_projection | matrix_inverse: value->type = parameter_world_view_projection_inverse; break;
        case matrix_world | matrix_transpose: value->type = parameter_world_transpose; break;
        case matrix_view | matrix_transpose: value->type = parameter_view_transpose; break;
        case matrix_projection | matrix_transpose: value->type = parameter_projection_transpose; break;
        case matrix_world_view | matrix_transpose: value->type = parameter_world_view_transpose; break;
        case matrix_view_projection | matrix_transpose: value->type = parameter_view_projection_transpose; break;
        case matrix_world_view_projection | matrix_transpose: value->type = parameter_world_view_projection_transpose; break;
        case matrix_world | matrix_inverse | matrix_transpose: value->type = parameter_world_inverse_transpose; break;
        case matrix_view | matrix_inverse | matrix_transpose: value->type = parameter_view_inverse_transpose; break;
        case matrix_projection | matrix_inverse | matrix_transpose: value->type = parameter_projection_inverse_transpose; break;
        case matrix_world_view | matrix_inverse | matrix_transpose: value->type = parameter_world_view_inverse_transpose; break;
        case matrix_view_projection | matrix_inverse | matrix_transpose: value->type = parameter_view_projection_inverse_transpose; break;
        case matrix_world_view_projection | matrix_inverse | matrix_transpose: value->type = parameter_world_view_projection_inverse_transpose; break;
    }
}

// inlined @ sub_9DF500
bool belongs_to_parameter_chain_0(ngl::fx::e_parameter_type type) {
    return type == ngl::fx::parameter_unknown_20 ||
           type == ngl::fx::parameter_view_position ||
           (type >= ngl::fx::parameter_viewport_pixel_size && type <= ngl::fx::parameter_unknown_30) ||
           (type >= ngl::fx::parameter_view && type <= ngl::fx::parameter_projection_inverse_transpose) ||
           (type >= ngl::fx::parameter_view_projection && type <= ngl::fx::parameter_view_projection_inverse_transpose);
}

// inlined @ sub_9DF500
bool belongs_to_parameter_chain_1(ngl::fx::e_parameter_type type) {
    return type == ngl::fx::parameter_bone_array_world ||
           type == ngl::fx::parameter_bone_influences ||
           type == ngl::fx::parameter_unknown_19 ||
           (type >= ngl::fx::parameter_world && type <= ngl::fx::parameter_world_inverse_transpose) ||
           (type >= ngl::fx::parameter_world_view && type <= ngl::fx::parameter_world_view_inverse_transpose) ||
           (type >= ngl::fx::parameter_world_view_projection && type <= ngl::fx::parameter_last);
}

// sub_9DF500
void build_parameter_chains(ngl::fx::effect* value) {
    value->parameter_chains[0] = nullptr;
    value->parameter_chains[1] = nullptr;
    value->parameter_chains[2] = nullptr;

    for (i32 index = value->parameter_count - 1; index >= 0; --index) {
        ngl::fx::parameter &parameter = value->parameters[index];
        ngl::fx::e_parameter_type type = parameter.type;

        if (belongs_to_parameter_chain_1(type)) {
            parameter.next = value->parameter_chains[1];
            value->parameter_chains[1] = &parameter;
        } else if (belongs_to_parameter_chain_0(type)) {
            parameter.next = value->parameter_chains[0];
            value->parameter_chains[0] = &parameter;
        }
    }
}

// inlined @ sub_9E0770
bool is_direct_binding_type(ngl::fx::e_parameter_type type) {
    if ((type >= ngl::fx::parameter_horizon_projection_u              && type <= ngl::fx::parameter_horizon_info)                      ||
        (type >= ngl::fx::parameter_light_info                        && type <= ngl::fx::parameter_view_projection_shadow_2)          ||
        (type >= ngl::fx::parameter_temporary_0                       && type <= ngl::fx::parameter_number_directional_lights)         ||
        (type >= ngl::fx::parameter_ibl_parameters                    && type <= ngl::fx::parameter_tentacle_width_frequency_offset_x) ||
        (type >= ngl::fx::parameter_environment_color                 && type <= ngl::fx::parameter_tentacle_basis_color_1)            ||
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

// inlined @ sub_9E0770
bool is_array_binding_type(ngl::fx::e_parameter_type type) { // array data starts after a 16-byte header
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

// inlined @ sub_9E0770
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

// sub_9E0770
void initialize_function_bindings(ngl::fx::effect*   value,
                                  ngl::fx::function* function) {

    for (i32 index = 0; index < function->binding_count; ++index) {
        ngl::fx::function_binding &binding   = function->bindings[index];
        ngl::fx::parameter        &parameter = value->parameters[binding.parameter_index];
        ngl::fx::e_parameter_type  type      = parameter.type;

        if (is_direct_binding_type(type)) {
            binding.source_class  = ngl::fx::binding_parameters;
            binding.source_offset = (u32)parameter.data - (u32)value->parameter_data;
        } else if (is_array_binding_type(type)) {
            binding.source_class  = ngl::fx::binding_parameters;
            binding.source_offset = (u32)parameter.data - (u32)value->parameter_data + 0x10;
        } else if (type == ngl::fx::parameter_sampler) {
            i32 texture_parameter_index = *(i32*)parameter.data;

            // samplers without a texture always use the fallback, whatever the destination
            if (texture_parameter_index == -1) {
                binding.source_class  = ngl::fx::binding_fallback_texture;
                binding.source_offset = 0;

                continue;
            }

            ngl::fx::parameter &texture_parameter = value->parameters[texture_parameter_index];

            if (is_forced_default_texture_type(texture_parameter.type))
                *(ngl::texture**)texture_parameter.data = ngl::references::default_tex.read();

            ngl::fx::set_texture(&texture_parameter, *(ngl::texture**)texture_parameter.data);

            binding.source_class  = ngl::fx::binding_parameters;
            binding.source_offset = (u32)texture_parameter.data - (u32)value->parameter_data;
        } else if (type == ngl::fx::parameter_bone_array_world) {
            binding.source_class  = ngl::fx::binding_bones;
            binding.source_offset = 0;
        } else if (type == ngl::fx::parameter_view_direction) {
            binding.source_class  = ngl::fx::binding_scene;
            binding.source_offset = offsetof(ngl::scene, view_direction);
        } else {
            binding.source_class  = binding.destination_type == ngl::fx::binding_sampler ?
                                        ngl::fx::binding_fallback_texture : ngl::fx::binding_global_constants;
            binding.source_offset = 0;
        }
    }
}

// sub_9E0990
void initialize_programs(      ngl::fx::program_set* value,
                         const ngl::fx::pass*        pass_data,
                               ngl::fx::effect*      effect_data) {

    if (pass_data->states.alpha_test_enabled)
        value->alpha_test_enabled = 1;

    if (value->vertex_programs[0].microcode) {
        ngl::d3d9::shader_program_cache::create_vertex_program(value->vertex_programs[0].microcode,
                                                               (IDirect3DVertexShader9**)&value->vertex_programs[0].shader);
        initialize_function_bindings(effect_data, &value->vertex_programs[0]);
    }

    if (value->pixel_programs[0].microcode) {
        ngl::d3d9::shader_program_cache::create_pixel_program(value->pixel_programs[0].microcode,
                                                              (IDirect3DPixelShader9**)&value->pixel_programs[0].shader);
        initialize_function_bindings(effect_data, &value->pixel_programs[0]);
    }

    if (value->vertex_programs[1].microcode) {
        ngl::d3d9::shader_program_cache::create_vertex_program(value->vertex_programs[1].microcode,
                                                               (IDirect3DVertexShader9**)&value->vertex_programs[1].shader);
        initialize_function_bindings(effect_data, &value->vertex_programs[1]);
    }

    if (value->pixel_programs[1].microcode) {
        ngl::d3d9::shader_program_cache::create_pixel_program(value->pixel_programs[1].microcode,
                                                              (IDirect3DPixelShader9**)&value->pixel_programs[1].shader);
        initialize_function_bindings(effect_data, &value->pixel_programs[1]);
    }
}

// sub_9E12E0
void ngl::fx::remove(apk::file*       owner,
                     apk::file_entry*,
                     void**           mapped_sections,
                     void*) {

    i32     image_section = owner->get_section_index(string_hash(four_cc('I', 'M', 'G')));
    effect* value         = (effect*)mapped_sections[image_section];

    references::effects.get().del(value);

    if (value->runtime) {
        delete value->runtime;
        value->runtime = nullptr;
    }
}

// inlined @ sub_9E12E0
ngl::fx::effect_runtime::~effect_runtime() {
    owner             = nullptr;
    zero_point_lights = nullptr;
    two_point_lights  = nullptr;
    four_point_lights = nullptr;

    if (technique_batches) {
        delete[] technique_batches;
        technique_batches = nullptr;
    }

    next   = nullptr;
    queued = 0;
}

// sub_9E13A0
ngl::fx::effect_runtime::effect_runtime(effect* value) : owner(value) {
    if (value->technique_count)
        technique_batches = new technique_batch[value->technique_count];
    else
        technique_batches = nullptr;

    next   = nullptr;
    queued = 0;

    // retail hashes these names once into a function-local static
    static const effect_variant_family families[] {
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

    zero_point_lights = nullptr;
    two_point_lights  = nullptr;
    four_point_lights = nullptr;

    for (const effect_variant_family &family : families) {
        if (value->name.hash != string_hash(hash::djb2(family.names[0])))
            continue;

        effect** variants[3] { &zero_point_lights, &two_point_lights, &four_point_lights };

        for (i32 index = 0; index < 3; ++index) {
            fixed_string variant_name = make_fixed_string(family.names[1 + index]);

            *variants[index] = (effect*)ngl::resources::get_resource(&variant_name, four_cc('F', 'X', '\0'));
        }

        break;
    }

    // the first matching name prefix sets the priority
    priority = 50;

    const char*            name     = value->name.get_text();
    const priority_prefix* prefixes = &references::priority_prefixes.get();

    for (i32 index = 0; index < 15; ++index) {
        if (!_strnicmp(name, prefixes[index].prefix, std::strlen(prefixes[index].prefix))) {
            priority = prefixes[index].priority;

            break;
        }
    }
}

// sub_9E1A90
void ngl::fx::load(apk::file*        owner,
                   apk::file_entry*,
                   void**            mapped_sections,
                   void*) {

    references::parameter_texture_fallback_pointer.write(&references::parameter_texture_fallback.get());
    references::parameter_texture_fallback        .write(ngl::references::default_tex.read());

    i32     image_section = owner->get_section_index(string_hash(four_cc('I', 'M', 'G')));
    effect* value         = (effect*)mapped_sections[image_section];

    for (i32 technique_index = 0; technique_index < value->technique_count; ++technique_index) {
        technique &technique_data = value->techniques[technique_index];

        for (i32 pass_index = 0; pass_index < technique_data.pass_count; ++pass_index) {
            pass &pass_data = technique_data.passes[pass_index];

            pass_data.active_programs = &pass_data.programs;
            initialize_programs(&pass_data.programs, &pass_data, value);
        }
    }

    for (i32 index = 0; index < value->parameter_count; ++index) {
        if (value->parameters[index].type == parameter_scene_matrix)
            specialize_scene_matrix(&value->parameters[index]);
    }

    build_parameter_chains(value);

    static const fixed_string transparency_name = make_fixed_string("SW_UsesTransparency");

    parameter* transparency_parameter = find_parameter_by_name(value, transparency_name.hash);
    bool       uses_transparency      = transparency_parameter ? *(u32*)transparency_parameter->data != 0 : false;

    value->flags &= ~0x10u;

    if (value->technique_count > 1 &&
        value->techniques[0].name.hash == value->techniques[value->technique_count / 2].name.hash) {

        value->flags |= 0x10u;
    }

    for (i32 technique_index = 0; technique_index < value->technique_count; ++technique_index) {
        technique &technique_data = value->techniques[technique_index];

        technique_data.flags = uses_transparency;

        for (i32 pass_index = 0; pass_index < technique_data.pass_count; ++pass_index) {
            if (!technique_data.passes[pass_index].states.alpha_blend_enabled)
                continue;

            technique_data.flags |= 1;

            break;
        }
    }

    for (i32 index = 0; index < value->parameter_count; ++index) {
        if (value->parameters[index].type != parameter_framebuffer_texture)
            continue;

        for (i32 technique_index = 0; technique_index < value->technique_count; ++technique_index)
            value->techniques[technique_index].flags |= 0x20;

        break;
    }

    static const fixed_string parameter_subset_name = make_fixed_string("ParamSubset");

    if (find_parameter_by_name(value, parameter_subset_name.hash)) {
        value->flags |= 0x40;

        static const fixed_string decompressed_position_scale = make_fixed_string("DECOMP_POS_SCALE");

        if (find_parameter_by_semantic(value, decompressed_position_scale.hash))
            value->flags |= 0x80;

        static const fixed_string bone_array_world = make_fixed_string("BoneArrayWorld");

        if (find_parameter_by_semantic(value, bone_array_world.hash))
            value->flags |= 0x100;
    }

    value->runtime = new effect_runtime(value);

    if (!ngl::references::resource_callback_function.read())
        references::effects.get().add(value);
}
