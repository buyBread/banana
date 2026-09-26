#pragma once

#include "treyarch/ngl/material/material.hh"
#include "treyarch/ngl/texture/texture.hh"
#include "treyarch/ngl/fx/lighting_parameters.hh"
#include "treyarch/ngl/shaders/program_exports.hh"
#include "treyarch/shared/math/types/matrix4x4.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace ngl { namespace shaders { namespace generated_material {
    struct material_data {
        ngl::material base;
        u32           reserved_014;
        i32           configuration_index;
        u32           reserved_01c;
        u32           mode;
        u32           shadow_mode;
        texture*      diffuse_texture;
        texture*      opacity_texture;
        texture*      specularity_texture;
        texture*      specular_exponent_texture;
        texture*      environment_texture;
        texture*      emissiveness_texture;
        vector4       material_values[3];
        f32           animation_scale_u;
        f32           animation_scale_v;
        texture*      normal_texture;
        texture*      horizon_texture;
        u8            reserved_080[0x20];
    };

    struct scene_snapshot {
        matrix4x4 base_transform;
        matrix4x4 local_to_world;
        matrix4x4 light_matrix;
        matrix4x4 texture_matrix;
        vector4   material_values[4];
        vector4   material_vector_140;
        vector4   material_vector_150;
        vector4   material_vector_160;
        f32       scalar_170;
        u32       random_seed;
        f32       scalar_178;
        f32       scalar_17c;
        f32       scalar_180;
        texture*  diffuse_texture;
        u8        reserved_188[0x14];
        u32       texture_filter;
        u8        reserved_1a0[0x10];
    };

    bool texture_is_usable(const texture* value);
    
    void transform_light_matrix(      scene_snapshot*                  snapshot,
                                const fx::general_lighting_parameters& lighting);

    void configure_samplers(const scene_snapshot* snapshot,
                            const material_data*  material);

    void bind_textures(const fx::general_lighting_parameters& lighting,
                       const scene_snapshot*                  snapshot,
                       const material_data*                   material);

    void upload_pixel_constants(const program_exports::pixel_pipeline_descriptor& pipeline,
                                const fx::general_lighting_parameters&            lighting,
                                const scene_snapshot*                             snapshot,
                                const material_data*                              material,
                                const matrix4x4&                                  local_to_world);

    namespace references {
        inline util::memory_reference<vector4> default_material_vector { 0x010F7D30 };
        inline util::memory_reference<vector4> default_normal_vector   { 0x010F7D50 };
        inline util::memory_reference<vector4> facing_vector           { 0x00F4A980 };
    } // references

    ASSERT_SIZEOF  (material_data,                            0xA0);
    ASSERT_OFFSETOF(material_data, configuration_index,       0x18);
    ASSERT_OFFSETOF(material_data, mode,                      0x20);
    ASSERT_OFFSETOF(material_data, shadow_mode,               0x24);
    ASSERT_OFFSETOF(material_data, opacity_texture,           0x2C);
    ASSERT_OFFSETOF(material_data, specularity_texture,       0x30);
    ASSERT_OFFSETOF(material_data, specular_exponent_texture, 0x34);
    ASSERT_OFFSETOF(material_data, environment_texture,       0x38);
    ASSERT_OFFSETOF(material_data, emissiveness_texture,      0x3C);
    ASSERT_OFFSETOF(material_data, normal_texture,            0x78);
    ASSERT_OFFSETOF(material_data, horizon_texture,           0x7C);

    ASSERT_SIZEOF  (scene_snapshot,                  0x1B0);
    ASSERT_OFFSETOF(scene_snapshot, light_matrix,    0x080);
    ASSERT_OFFSETOF(scene_snapshot, texture_matrix,  0x0C0);
    ASSERT_OFFSETOF(scene_snapshot, material_values, 0x100);
    ASSERT_OFFSETOF(scene_snapshot, diffuse_texture, 0x184);
    ASSERT_OFFSETOF(scene_snapshot, texture_filter,  0x19C);
}}}} // treyarch::ngl::shaders::generated_material
