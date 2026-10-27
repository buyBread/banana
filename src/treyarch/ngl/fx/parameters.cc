#include <cmath>
#include <cstring>

#include "treyarch/ngl/ngl.hh"
#include "treyarch/ngl/gpu/d3d9/framebuffer.hh"
#include "treyarch/ngl/fx/parameters.hh"
#include "treyarch/ngl/fx/references.hh"
#include "treyarch/ngl/fx/render_support.hh"
#include "treyarch/ngl/lighting/lighting.hh"
#include "treyarch/ngl/mesh/mesh.hh"
#include "treyarch/ngl/scene/scene.hh"
#include "treyarch/ngl/texture/texture.hh"
#include "treyarch/ngl/shaders/generated_material.hh"
#include "game/light/lighting.hh"
#include "game/mission/mission_manager.hh"
#include "game/shadow/shadow.hh"
#include "banana/util/memory_reference.hh"

using namespace treyarch;
using namespace treyarch::ngl;

/*
    todo: mooooooooooooooooooooooooooooooooooove
          where
          somewhere
          someday
          all will be moved
          isn't that great?
          * Acknowledging this fills you with Determination.
*/

namespace treyarch { namespace ngl { namespace fx { namespace references {
    util::memory_reference<math::vector4> temporary_0          { 0x010F7E20 };
    util::memory_reference<f32>           shared_scalar        { 0x010F7D00 };
    util::memory_reference<math::vector4> constant_80          { 0x00F4AB70 };
    util::memory_reference<math::vector4> tint_color           { 0x00F4A9E0 };
    util::memory_reference<math::vector4> shadow_factor        { 0x010F8A40 };
    util::memory_reference<math::vector4> subset_shadow_factor { 0x010F8840 };
}}}} // treyarch::ngl::fx::references

// sub_9DF680
void ngl::fx::set_float(parameter* entry, f32 value) {
    *(f32*)entry->data = value;
}

// sub_9DF6A0
void ngl::fx::set_int(parameter* entry, i32 value) {
    *(i32*)entry->data = value;
}

// sub_9DF6B0
void ngl::fx::set_vector(parameter* entry, const math::vector4 &value) {
    *(math::vector4*)entry->data = value;
}

// sub_9DF6D0
void ngl::fx::set_matrix(parameter* entry, const math::mat44 &value) {
    *(math::mat44*)entry->data = value;
}

// sub_9DF6F0
void ngl::fx::set_texture(parameter* entry, texture* value) {
    *(texture**)entry->data = value ? value : ngl::references::default_tex.read();
}

// sub_8A2910
void copy_parameter_subset(      ngl::fx::effect*         value,
                           const ngl::fx::mesh_node_data* node_data) {

    void* subset = find_scene_parameter(node_data->parameters,
                                        fx::references::parameter_id_parameter_subset.read());

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

// sub_9DE390
void write_bone_matrices(const ngl::fx::mesh_node_data* node_data,
                         const ngl::mesh_section*       section) {
    math::vector4* destination = &fx::references::bone_constants.get();

    // shaders only receive three rows per bone,
    // meshes without bones still get two copies of the node transform
    if (!section->bone_count) {
        math::mat44 matrix = ~math::mat44(node_data->local_to_world);
        std::memcpy(destination, &matrix, sizeof(math::vector4) * 3);
        std::memcpy(destination + 3, &matrix, sizeof(math::vector4) * 3);
        fx::references::bone_constant_count.write(2);
        return;
    }

    u32 flags = *(u32*)node_data->node_info;
    const math::mat43* bone_matrices = *(math::mat43**)(node_data->node_info + 8);
    const u8* bind_matrices = nullptr;

    if (flags & (4 | 8))
        bind_matrices = *(u8**)(*(u8**)(node_data->mesh_data + 0x14) + 0x0C);

    for (i32 index = 0; index < section->bone_count; ++index) {
        u16 bone_index = section->bone_indices[index];
        const math::mat43 &bone = bone_matrices[bone_index];
        math::mat44 matrix;

        if (flags & (4 | 8)) {
            // each bone's bind data is 0x90 bytes and its matrix starts at +0x40
            const math::mat43 &bind = *(const math::mat43*)(bind_matrices + 0x90 * bone_index + 0x40);
            matrix = math::mat44(bind) * math::mat44(bone);

            // flag 4 wins when both flags are set
            if (!(flags & 4))
                matrix = matrix * math::mat44(node_data->local_to_world);
        } else if (flags & 0x10)
            matrix = bone;
        else
            matrix = node_data->local_to_world;

        matrix = ~matrix;
        std::memcpy(destination + 3 * index, &matrix, sizeof(math::vector4) * 3);
    }

    fx::references::bone_constant_count.write(section->bone_count);
}

// inlined @ sub_8B8620
// inlined @ sub_8A2BA0
void write_shadow_size(f32* destination, u32 width, u32 height) {
    *(math::vector4*)(destination) = math::vector4((f32)width, (f32)height, 1.0f / (f32)width, 1.0f / (f32)height);
}

// inlined @ sub_8A6D50
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

// sub_9DFF60
void ngl::fx::update_scene_parameters(effect* value) {
    scene* build_scene = ngl::references::build_scene.read();

    for (parameter* entry = value->parameter_chains[0]; entry; entry = entry->next) {
        f32* destination = (f32*)entry->data;
        math::vector4* vectors = (math::vector4*)entry->data;

        switch (entry->type) {
            case parameter_view_position:
                vectors[0] = math::vector4(build_scene->view_position);

                break;
            case parameter_viewport_pixel_size:
                vectors[0] = math::vector4((f32)build_scene->color_target->gpu_texture.width,
                                           (f32)build_scene->color_target->gpu_texture.height, 0.0f, 0.0f);

                break;
            case parameter_frame:
                *(u32*)destination = ngl::references::frame.read();

                break;
            case parameter_time:
                destination[0] = build_scene->current_animation_time;

                break;
            case parameter_view: {
                *(math::mat44*)destination = ~math::mat44(build_scene->world_to_view);

                break;
            }
            case parameter_view_inverse: {
                *(math::mat44*)destination = ~math::mat44(build_scene->view_to_world);

                break;
            }
            case parameter_view_transpose:
                *(math::mat44*)destination = build_scene->world_to_view;

                break;
            case parameter_view_inverse_transpose:
                *(math::mat44*)destination = build_scene->view_to_world;

                break;
            case parameter_projection:
                *(math::mat44*)destination = ~build_scene->view_to_screen;

                break;
            case parameter_projection_inverse:
                *(math::mat44*)destination = math::identity_mat33();

                break;
            case parameter_projection_transpose:
                *(math::mat44*)(destination) = build_scene->view_to_screen;

                break;
            case parameter_projection_inverse_transpose:
                *(math::mat44*)destination = math::identity_mat33();

                break;
            case parameter_view_projection:
                *(math::mat44*)destination = ~build_scene->world_to_screen;

                break;
            case parameter_view_projection_inverse:
                *(math::mat44*)destination = math::identity_mat33();

                break;
            case parameter_view_projection_transpose:
                *(math::mat44*)(destination) = build_scene->world_to_screen;

                break;
            case parameter_view_projection_inverse_transpose:
                *(math::mat44*)destination = math::identity_mat33();

                break;
        }
    }
}

// inlined @ sub_8B8620
// inlined @ sub_8A2BA0
light_table_lookup get_light_table_lookup(const ngl::fx::mesh_node_data* node_data) {
    light_table_lookup lookup;

    u32 range_id = fx::references::parameter_id_light_table_range.read();

    if (has_scene_parameter(node_data->parameters, range_id)) {
        const u8* range = (const u8*)get_scene_parameter(node_data->parameters, range_id);

        lookup.index  = *(const i32*)(range + 0x48);
        lookup.offset = *(const i32*)(range + 0x4C);
    } else {
        lookup.index  = 0;
        lookup.offset = 4;
    }

    lookup.table         = (light_table*)find_scene_parameter(node_data->parameters,
                                                              fx::references::parameter_id_light_table.read());
    lookup.force_horizon = false;

    return lookup;
}

// inlined @ sub_8B8620
// inlined @ sub_8A2BA0
// inlined @ sub_8A6D50
void write_material_parameter(      ngl::fx::parameter*          entry,
                              const ngl::fx::mesh_node_data*     node_data,
                              const ngl::mesh_section*           section,
                                    bool                         subset_effect,
                              const lighting_parameters*         lighting,
                              const general_lighting_parameters* general_lighting,
                              const light_table_lookup*          lookup) {
    using namespace ngl::fx;

    f32* destination = (f32*)entry->data;
    math::vector4* vectors = (math::vector4*)entry->data;
    switch (entry->type) {
        case parameter_bone_array_world:
            write_bone_matrices(node_data, section);

            break;
        case parameter_bone_influences:
            if (!subset_effect)
                destination[0] = section && section->bone_count > 0 ? 4.0f : 1.0f;

            break;
        case parameter_world:
            *(math::mat44*)destination = ~math::mat44(node_data->local_to_world);

            break;
        case parameter_world_inverse:
            *(math::mat44*)destination = ~math::mat44(get_unscaled_world_to_local(node_data));

            break;
        case parameter_world_transpose:
            *(math::mat44*)destination = node_data->local_to_world;

            break;
        case parameter_world_inverse_transpose:
            *(math::mat44*)destination = get_unscaled_world_to_local(node_data);

            break;
        case parameter_world_view:
            *(math::mat44*)destination = ~(math::mat44(node_data->local_to_world) * ngl::references::build_scene.read()->world_to_view);

            break;
        case parameter_world_view_inverse: {
            math::mat44 local_to_view = math::mat44(node_data->local_to_world) * ngl::references::build_scene.read()->world_to_view;

            *(math::mat44*)destination = ~math::mat44(math::inv(*(const math::rot_tran_mat43*)&local_to_view));

            break;
        }
        case parameter_world_view_transpose:
            *(math::mat44*)destination = math::mat44(node_data->local_to_world) * ngl::references::build_scene.read()->world_to_view;

            break;
        case parameter_world_view_inverse_transpose: {
            math::mat44 local_to_view = math::mat44(node_data->local_to_world) * ngl::references::build_scene.read()->world_to_view;

            *(math::mat44*)destination = math::inv(*(const math::rot_tran_mat43*)&local_to_view);

            break;
        }
        case parameter_world_view_projection:
            *(math::mat44*)destination = ~(math::mat44(node_data->local_to_world) * ngl::references::build_scene.read()->world_to_screen);

            break;
        case parameter_world_view_projection_inverse:
            // subset effects leave these alone
            if (!subset_effect)
                *(math::mat44*)destination = math::identity_mat33();

            break;
        case parameter_world_view_projection_transpose:
            *(math::mat44*)destination = math::mat44(node_data->local_to_world) * ngl::references::build_scene.read()->world_to_screen;

            break;
        case parameter_world_view_projection_inverse_transpose:
            if (!subset_effect)
                *(math::mat44*)destination = math::identity_mat33();

            break;
        case parameter_horizon_projection_u:
            vectors[0] = lighting->horizon_projection_u;

            break;
        case parameter_horizon_projection_v:
            vectors[0] = lighting->horizon_projection_v;

            break;
        case parameter_lightmap_color: {
            f64 hour = (f64)mission_manager::inst()->game_time_get_full_time() / 3600.0;
            f32 night = hour < 3.0 || hour > 23.9 ? 1.0f : 0.0f;
            vectors[0] = math::vector4(0.60000002f * night, 0.5f * night, 0.40000001f * night, 1.0f);

            break;
        }
        case parameter_window_color: {
            f64 hour = (f64)mission_manager::inst()->game_time_get_full_time() / 3600.0;
            f32 night = hour < 3.0 || hour > 23.9 ? 1.0f : 0.0f;
            vectors[0] = math::vector4(1.3f, 1.2f, 0.8f, night);

            break;
        }
        case parameter_fog_color:
            vectors[0] = lighting->fog_color;

            break;
        case parameter_fog_control:
            vectors[0] = lighting->fog_control;

            break;
        case parameter_ambient_info:
            if (!subset_effect) {
                vectors[0] = general_lighting->ambient_color;
                vectors[1] = general_lighting->ambient_direction;
                std::memcpy(destination + 4 * 2, (f32*)&general_lighting->ambient_colors_with_primary[0], sizeof(math::vector4) * 8);
                std::memcpy(destination + 4 * 10, (f32*)&general_lighting->ambient_colors[0], sizeof(math::vector4) * 8);
                std::memcpy(destination + 4 * 18, (f32*)&general_lighting->ambient_defaults.values[0], sizeof(math::vector4) * 9);
            }

            break;
        case parameter_horizon_info:
            if (!subset_effect) {
                vectors[0] = general_lighting->horizon_projection_u;
                vectors[1] = general_lighting->horizon_projection_v;
                vectors[2] = general_lighting->light_colors[0];
                vectors[3] = general_lighting->light_directions[0];
                vectors[4] = general_lighting->horizon_range;
                vectors[5] = general_lighting->horizon_color;
                vectors[6] = general_lighting->horizon_direction_scaled;
                vectors[7] = general_lighting->horizon_bias;

                f32 scalar = general_lighting->horizon_scalar[0];
                vectors[8] = math::vector4(scalar, scalar, scalar, scalar);

                std::memcpy(destination + 4 * 9, (f32*)&general_lighting->horizon_extra, sizeof(f32) * 3);
                destination[4 * 9 + 3] = 0.0f;

                std::memcpy(destination + 4 * 10, (f32*)&general_lighting->horizon_axis, sizeof(f32) * 3);
                destination[4 * 10 + 3] = 0.0f;
            }

            break;
        case parameter_horizon_texture:
            fx::set_texture(entry, lookup->table ? lookup->table->texture : ngl::references::white_tex.read());

            break;
        case parameter_light_info:
            if (!subset_effect) {
                std::memcpy(destination + 4 * 0, (f32*)&general_lighting->light_positions[0], sizeof(math::vector4) * 4);
                std::memcpy(destination + 4 * 4, (f32*)&general_lighting->light_directions[0], sizeof(math::vector4) * 4);
                std::memcpy(destination + 4 * 8, (f32*)&general_lighting->light_colors[0], sizeof(math::vector4) * 4);
                std::memcpy(destination + 4 * 12, (f32*)&general_lighting->light_attenuation[0], sizeof(math::vector4) * 4);
            }

            break;
        case parameter_shadow_distances:
            vectors[0] = shadow::references::shadow_distances.get();

            break;
        case parameter_shadow_buffer_size:
        case parameter_shadow_buffer_size_1:
            write_shadow_size(destination,
                              shadow::references::dimensions.get().widths[0],
                              shadow::references::dimensions.get().heights[0]);

            break;
        case parameter_shadow_buffer_size_2:
            write_shadow_size(destination,
                              shadow::references::dimensions.get().widths[1],
                              shadow::references::dimensions.get().heights[1]);

            break;
        case parameter_view_projection_shadow:
        case parameter_view_projection_shadow_1:
            *(math::mat44*)destination = ~shadow::references::matrix_0.get();

            break;
        case parameter_view_projection_shadow_2:
            *(math::mat44*)destination = ~shadow::references::matrix_1.get();

            break;
        case parameter_shadow_texture:
        case parameter_shadow_texture_1:
            fx::set_texture(entry,
                            shadow::references::device_resources.get().depth_targets[0]);

            break;
        case parameter_shadow_texture_2:
            fx::set_texture(entry,
                            shadow::references::device_resources.get().depth_targets[1]);

            break;
        case parameter_post_info:
            if (!subset_effect) {
                const f32* primary_direction = (f32*)&general_lighting->post_direction;
                const f32* first_plane = (f32*)&general_lighting->post_plane_0;
                const f32* second_plane = (f32*)&general_lighting->post_plane_1;

                vectors[0] = *(const math::vector4*)(primary_direction);

                vectors[1] = math::vector4(primary_direction[0] * first_plane[0] +
                             primary_direction[1] * first_plane[1] +
                             primary_direction[2] * first_plane[2], primary_direction[0] * second_plane[0] +
                             primary_direction[1] * second_plane[1] +
                             primary_direction[2] * second_plane[2], 0.0f, 0.0f);

                vectors[2] = general_lighting->post_range;
                vectors[3] = general_lighting->post_color;
                vectors[4] = general_lighting->fog_color;
                vectors[5] = general_lighting->fog_control;

                const auto* light_source = (const light_source_data*)find_scene_parameter(node_data->parameters,
                                                                                         fx::references::parameter_id_light_source.read());

                if (light_source) {
                    f32 exposure = light_source->exposure;
                    vectors[6] = math::vector4(exposure, exposure, exposure, exposure);
                } else
                    vectors[6] = math::vector4(-2.0f, -2.0f, -2.0f, -2.0f);

                vectors[7] = general_lighting->post_view_position;
                vectors[8] = *(const math::vector4*)(first_plane);
                vectors[9] = *(const math::vector4*)(second_plane);
            }

            break;
        case parameter_depth_texture:
            fx::set_texture(entry, ngl::d3d9::references::framebuffers.get().active_depth_buffer);

            break;
        case parameter_temporary_0:
            for (u32 index = 0; index < 4; ++index)
                destination[index] = fx::references::temporary_0.get()[index] * 2.0f;

            break;
        case parameter_shadow_factor:
            vectors[0] = *(const math::vector4*)(subset_effect ? &fx::references::subset_shadow_factor.get() : &fx::references::shadow_factor.get());

            break;
        case parameter_shared_scalar:
            vectors[0] = math::vector4(
                fx::references::shared_scalar.read(),
                fx::references::shared_scalar.read(),
                fx::references::shared_scalar.read(),
                fx::references::shared_scalar.read());

            break;
        case parameter_constant_80:
            vectors[0] = fx::references::constant_80.get();

            break;
        case parameter_mesh_map_distance:
            vectors[0] = math::vector4(50.0f, 0.0199999996f, 0.0f, 0.0f);

            break;
        case parameter_number_directional_lights:
            destination[0] = (f32)lighting->directional_light_count;

            break;
        case parameter_directional_light_directions:
            for (i32 index = 0; index < lighting->directional_light_count; ++index) {
                const math::vector4 &direction = lighting->directional_lights[index].direction;

                vectors[1 + index] = math::vector4(direction[0], direction[1], direction[2], 0.0f);
            }

            break;
        case parameter_directional_light_colors:
            for (i32 index = 0; index < lighting->directional_light_count; ++index) {
                const math::vector4 &color = lighting->directional_lights[index].color;

                vectors[1 + index] = math::vector4(color[0], color[1], color[2], 0.0f);
            }

            break;
        case parameter_number_point_lights:
            destination[0] = (f32)node_data->point_light_count;

            break;
        case parameter_point_light_positions_ranges: {
            const ngl::lighting::point_light_data* lights = &ngl::lighting::references::point_lights.get();

            // past the 16-byte array header
            for (u32 index = 0; index < node_data->point_light_count; ++index) {
                const ngl::lighting::point_light_data &light = lights[node_data->point_light_indices[index]];

                vectors[1 + index] = math::vector4(light.position[0],
                                                   light.position[1],
                                                   light.position[2],
                                                   1.0f / light.position[3]);
            }

            break;
        }
        case parameter_point_light_colors: {
            const ngl::lighting::point_light_data* lights = &ngl::lighting::references::point_lights.get();
            u32                                    index  = 0;

            for (; index < node_data->point_light_count; ++index) {
                const ngl::lighting::point_light_data &light = lights[node_data->point_light_indices[index]];

                vectors[1 + index] = math::vector4(light.color[0], light.color[1], light.color[2], 0.0f);
            }

            // unused colors are cleared, unused positions and ranges are left alone
            for (; index < 8; ++index)
                vectors[1 + index] = math::vector4(0.0f, 0.0f, 0.0f, 0.0f);

            break;
        }
        case parameter_ibl_parameters:
            vectors[0] = lighting->ambient_info;

            break;
        case parameter_character_highlight: {
            void* color = find_scene_parameter(node_data->parameters,
                                               fx::references::parameter_id_character_color.read());

            if (color)
                vectors[0] = *(const math::vector4*)(color);
            else
                vectors[0] = math::vector4(1.0f, 1.0f, 1.0f, 1.0f);

            break;
        }
        case parameter_framebuffer_texture:
            fx::set_texture(entry, subset_effect ?
                                ngl::d3d9::references::framebuffers.get().back_buffer :
                                ngl::d3d9::references::framebuffers.get().secondary_hdr_buffer);
                                        
            break;
        case parameter_environment_map:
            if (!subset_effect)
                fx::set_texture(entry, ngl::fx::references::environment_texture.read());

            break;
        case parameter_environment_color: {
            f32* color = (f32*)find_scene_parameter(node_data->parameters,
                                                    fx::references::parameter_id_environment_color.read());

            if (color) {
                destination[0] = color[0] * color[0];
                destination[1] = color[1] * color[1];
                destination[2] = color[2] * color[2];
                destination[3] = color[3];
            } else {
                // (10, 10, 20) / 255, squared below
                vectors[0] = math::vector4(0.03921569f, 0.03921569f, 0.07843138f, 1.0f);

                for (u32 index = 0; index < 4; ++index)
                    destination[index] *= destination[index];
            }

            break;
        }
        case parameter_decal_projection: {
            math::mat44* matrix = (math::mat44*)find_scene_parameter(node_data->parameters,
                                                                     fx::references::parameter_id_decal_projection.read());

            if (matrix)
                *(math::mat44*)destination = ~*matrix;
            else
                *(math::mat44*)destination = math::identity_mat33();

            break;
        }
        case parameter_viewport_to_world: {
            scene* build_scene = ngl::references::build_scene.read();
            *(math::mat44*)destination = ~(math::mat44(node_data->local_to_world) *
                                           build_scene->derived_matrix_250);
            break;
        }
        case parameter_world_to_viewport: {
            scene* build_scene = ngl::references::build_scene.read();
            // world_to_viewport also includes the viewport matrix,
            // leaving it out makes projected decals move and resize with the camera
            *(math::mat44*)destination = ~(math::mat44(node_data->local_to_world) *
                                           build_scene->world_to_view    *
                                           build_scene->projection       *
                                           build_scene->view);
            break;
        }
        case parameter_ui_parameters: {
            void* parameters = find_scene_parameter(node_data->parameters,
                                                    fx::references::parameter_id_ui_parameters.read());
            vectors[0] = *(const math::vector4*)(parameters ? parameters : &shaders::generated_material::references::default_normal_vector.get());

            break;
        }
        case parameter_tint_color: {
            void* color = find_scene_parameter(node_data->parameters,
                                               fx::references::parameter_id_tint_color.read());
            vectors[0] = *(const math::vector4*)(color ? color : &fx::references::tint_color.get());

            break;
        }
        case parameter_decal_texture_matrix: {
            const auto* rows = (const math::vector4*)find_scene_parameter(node_data->parameters,
                                                                          fx::references::parameter_id_decal_texture_matrix.read());
            if (rows)
                std::memcpy(vectors + 1, rows, sizeof(math::vector4) * 2);
            else {
                vectors[1] = math::vector4(1.0f, 0.0f, 0.0f, 0.0f);
                vectors[2] = math::vector4(0.0f, 1.0f, 0.0f, 0.0f);
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
                                              fx::references::parameter_id_last.read());
            if (data)
                vectors[0] = *(const math::vector4*)(data);

            break;
        }
    }
}

// sub_8B8620
void ngl::fx::update_general_material_parameters(effect*         value,
                                                 mesh_node_data* node_data,
                                                 mesh_section*   section) {

    light_table_lookup          lookup = get_light_table_lookup(node_data);
    general_lighting_parameters lighting;

    // depth-bias scenes only get the default lighting
    if (!ngl::references::build_scene.read()->depth_bias_enabled)
        lighting.build(node_data, section, false, &lookup, true);

    copy_parameter_subset(value, node_data);

    for (parameter* entry = value->parameter_chains[1]; entry; entry = entry->next)
        write_material_parameter(entry, node_data, section, false, &lighting, &lighting, &lookup);
}

// sub_8A2BA0
void ngl::fx::update_subset_material_parameters(effect*         value,
                                                mesh_node_data* node_data,
                                                mesh_section*   section) {

    light_table_lookup  lookup = get_light_table_lookup(node_data);
    lighting_parameters lighting;

    lighting.build(node_data, section, &lookup);

    copy_parameter_subset(value, node_data);

    for (parameter* entry = value->parameter_chains[1]; entry; entry = entry->next)
        write_material_parameter(entry, node_data, section, true, &lighting, nullptr, &lookup);
}

// sub_8A6D50
void ngl::fx::update_subset_depth_bias_parameters(effect*         value,
                                                  mesh_node_data* node_data,
                                                  mesh_section*   section) {

    copy_parameter_subset(value, node_data);

    for (parameter* entry = value->parameter_chains[1]; entry; entry = entry->next) {
        if (is_depth_bias_parameter(entry->type))
            write_material_parameter(entry, node_data, section, true, nullptr, nullptr, nullptr);
    }
}

// inlined @ sub_9E0420
// inlined @ sub_9E0A50
void ngl::fx::update_material_parameters(effect*         value,
                                         mesh_node_data* node_data,
                                         mesh_section*   section) {

    if (!(value->flags & 0x40))
        update_general_material_parameters(value, node_data, section);
    else if (!ngl::references::build_scene.read()->depth_bias_enabled)
        update_subset_material_parameters(value, node_data, section);
    else
        update_subset_depth_bias_parameters(value, node_data, section);
}
