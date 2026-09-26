#include "treyarch/ngl/fx/lighting_parameters.hh"
#include "treyarch/ngl/shaders/generated_material.hh"
#include "treyarch/ngl/shaders/sm_phat/configuration.hh"

using namespace treyarch;

size_t ngl::shaders::sm_phat::select_material_configuration(      generated_material::material_data*   material,
                                                            const generated_material::scene_snapshot*  snapshot,
                                                            const fx::general_lighting_parameters     &lighting) {

    const vector4   &default_value = generated_material::references::default_material_vector.get();
    const matrix4x4 &matrix        = snapshot->light_matrix;

    bool diffuse = generated_material::texture_is_usable(snapshot->diffuse_texture);

    if (diffuse) {
        diffuse = false;

        for (u32 row = 0; row < 3 && !diffuse; ++row) {
            for (u32 column = 0; column < 3; ++column) {
                if (matrix[row][column] != default_value[column]) {
                    diffuse = true;

                    break;
                }
            }
        }
    }

    bool opacity           = generated_material::texture_is_usable(material->opacity_texture)           && lighting.reserved_1f0.x != 0.0f;
    bool specularity       = generated_material::texture_is_usable(material->specularity_texture)       && lighting.reserved_1f0.y != 0.0f;
    bool specular_exponent = generated_material::texture_is_usable(material->specular_exponent_texture) && lighting.reserved_1d0.w != 0.0f;
    bool emissiveness      = generated_material::texture_is_usable(material->emissiveness_texture)      && lighting.reserved_1d0.y != 0.0f;

    const vector4 &normal_value = material->normal_texture->gpu_texture.format == D3DFMT_DXT5 ?
        snapshot->material_vector_160 : generated_material::references::default_normal_vector.get();

    bool normal = generated_material::texture_is_usable(material->normal_texture) && normal_value.x != 0.0f;

    e_environment_mapping environment_mapping = e_environment_mapping::none;

    if (lighting.reserved_1f0.z != 0.0f || lighting.reserved_200.z != 0.0f) {
        environment_mapping =
            generated_material::texture_is_usable(material->environment_texture) && lighting.reserved_1f0.z != 0.0f ?
                e_environment_mapping::mapped : e_environment_mapping::uniform;
    }

    material_configuration required { diffuse,
                                      opacity,
                                      specularity,
                                      specular_exponent,
                                      emissiveness,
                                      normal,
                                      environment_mapping, };

    size_t selected      = material_configurations.size() - 1;
    i32    selected_cost = 9999;

    for (size_t index = 0; index < material_configurations.size(); ++index) {
        const material_configuration &candidate = material_configurations[index];

        if (candidate.diffuse < required.diffuse                     ||
            candidate.opacity < required.opacity                     ||
            candidate.specularity < required.specularity             ||
            candidate.specular_exponent < required.specular_exponent ||
            candidate.emissiveness < required.emissiveness           ||
            candidate.normal < required.normal                       ||
            candidate.environment_mapping < required.environment_mapping)

            continue;

        i32 cost = ((i32)candidate.diffuse - (i32)required.diffuse) +
                   ((i32)candidate.opacity - (i32)required.opacity) +
                   ((i32)candidate.specularity - (i32)required.specularity) +
                   ((i32)candidate.specular_exponent - (i32)required.specular_exponent) +
                   ((i32)candidate.emissiveness - (i32)required.emissiveness) +
                   ((i32)candidate.normal - (i32)required.normal) +
                   ((i32)candidate.environment_mapping - (i32)required.environment_mapping);

        if (cost < selected_cost) {
            selected      = index;
            selected_cost = cost;
        }
    }

    material->configuration_index = (i32)selected + 1;

    return selected;
}

size_t ngl::shaders::sm_phat::select_lighting_configuration(bool horizon,
                                                            i32  light_count,
                                                            bool gobo,
                                                            u32  shadow_count) {

    e_light_count selected_light_count;

    if (horizon)
        selected_light_count = light_count == 0 ? e_light_count::none : e_light_count::four;
    else
        selected_light_count = light_count <= 2 ? e_light_count::two : e_light_count::four;

    lighting_configuration required { horizon,
                                      selected_light_count,
                                      gobo,
                                      (e_shadow_count)(u8)shadow_count, };

    for (size_t index = 0; index < lighting_configurations.size(); ++index) {
        if (lighting_configurations[index] == required)
            return index;
    }

    return 0;
}
