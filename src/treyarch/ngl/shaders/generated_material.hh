#pragma once

#include "treyarch/ngl/material/material.hh"
#include "treyarch/ngl/texture/texture.hh"
#include "treyarch/ngl/fx/lighting_parameters.hh"
#include "treyarch/ngl/shaders/program_exports.hh"
#include "treyarch/shared/math/types/matrix4x4.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace ngl { namespace fx {
    struct mesh_node_data;
}}}

namespace treyarch { namespace ngl { namespace shaders { namespace generated_material {
    struct scene_initializer {
        void* reserved_000;
        void* value;
    };

    struct material_data {
        ngl::material      base;
        scene_initializer* scene_initializer_data;
        i32                configuration_index;
        u32                reserved_01c;
        u32                mode;
        u32                shadow_mode;
        texture*           diffuse_texture;
        texture*           opacity_texture;
        texture*           specularity_texture;
        texture*           specular_exponent_texture;
        texture*           environment_texture;
        texture*           emissiveness_texture;
        vector4            material_values[3];
        f32                animation_scale_u;
        f32                animation_scale_v;
        texture*           normal_texture;
        texture*           horizon_texture;
        f32                texture_scale_u;
        f32                texture_scale_v;
        f32                texture_offset_u;
        f32                texture_offset_v;
        f32                texture_scroll_u;
        f32                texture_scroll_v;
        f32                texture_wrap_u;
        f32                texture_wrap_v;
    };

    struct scene_snapshot {
        matrix4x4          base_transform;
        matrix4x4          local_to_world;
        matrix4x4          light_matrix;
        matrix4x4          texture_matrix;
        vector4            material_values[4];
        vector4            material_vector_140;
        vector4            material_vector_150;
        vector4            material_vector_160;
        f32                animation_time;
        u32                random_seed;
        u32                unknown_178;
        f32                scalar_17c;
        f32                scalar_180;
        texture*           diffuse_texture;
        u8                 reserved_188[0x0C];
        u32                reserved_194;
        u32                reserved_198;
        u32                texture_filter;
        u32                reserved_1a0;
        scene_initializer* scene_initializer_data;
        u8                 reserved_1a8[0x08];
    };

    struct regular_vertex_prefix {
        vector4   ambient_color_lo[8];
        vector4   ambient_color_hi[8];
        matrix4x4 local_to_gobo;
        matrix4x4 light_to_screen[2];
        matrix4x4 compressed_to_uv;
        matrix4x4 compressed_to_screen;
    };

    struct regular_vertex_suffix {
        vector4 local_to_ambient;
        vector4 fog_eye_to_local;
        vector4 fog_normal;
        vector4 fog_position_0;
        vector4 fog_position_1;
        vector4 fogs_per_meter;
        vector4 horizon_map_matrix_u;
        vector4 horizon_map_matrix_v;
    };

    struct regular_vertex_context {
        regular_vertex_prefix prefix;
        regular_vertex_suffix suffix;
        matrix4x4             compressed_to_local;
        u32                   shadow_count;
    };

    bool texture_is_usable(const texture* value);

    void prepare_scene_snapshot(      scene_snapshot*     snapshot,
                                const material_data*      material,
                                const fx::mesh_node_data* node_data,
                                const mesh_section*       section);

    void prepare_regular_vertex_context(      regular_vertex_context*          context,
                                        const fx::general_lighting_parameters &lighting,
                                        const scene_snapshot*                  snapshot,
                                        const fx::mesh_node_data*              node_data,
                                        const mesh_section*                    section,
                                        const matrix4x4                       &local_to_world,
                                              bool                             receive_shadows);

    void configure_regular_pass_states(bool queue_class,
                                       u32  render_flags,
                                       bool use_packed_alpha);
    void restore_regular_pass_states();

    void transform_light_matrix(      scene_snapshot*                  snapshot,
                                const fx::general_lighting_parameters &lighting);

    void configure_samplers(const scene_snapshot* snapshot,
                            const material_data*  material);

    void bind_textures(const fx::general_lighting_parameters &lighting,
                       const scene_snapshot*                  snapshot,
                       const material_data*                   material);

    void upload_pixel_constants(const program_exports::pixel_pipeline_descriptor &pipeline,
                                const fx::general_lighting_parameters            &lighting,
                                const scene_snapshot*                             snapshot,
                                const material_data*                              material,
                                const matrix4x4                                  &local_to_world);

    void draw_pixel_pipeline(const program_exports::pixel_pipeline_descriptor &pipeline,
                             const fx::general_lighting_parameters            &lighting,
                             const scene_snapshot*                             snapshot,
                             const material_data*                              material,
                             const matrix4x4                                  &local_to_world,
                                   mesh_section*                               section);

    namespace references {
        inline util::memory_reference<vector4>        default_material_vector { 0x010F7D30 };
        inline util::memory_reference<vector4>        default_normal_vector   { 0x010F7D50 };
        inline util::memory_reference<vector4>        facing_vector           { 0x00F4A980 };
        inline util::memory_reference<f32>            animation_time          { 0x01086F20 };
        inline util::memory_reference<scene_snapshot> default_scene_snapshot  { 0x010873B0 };
    } // references

    ASSERT_SIZEOF  (material_data,                            0xA0);
    ASSERT_OFFSETOF(material_data, scene_initializer_data,    0x14);
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
    ASSERT_OFFSETOF(material_data, texture_scale_u,           0x80);
    ASSERT_OFFSETOF(material_data, texture_wrap_v,            0x9C);

    ASSERT_SIZEOF  (scene_snapshot,                  0x1B0);
    ASSERT_OFFSETOF(scene_snapshot, light_matrix,    0x080);
    ASSERT_OFFSETOF(scene_snapshot, texture_matrix,  0x0C0);
    ASSERT_OFFSETOF(scene_snapshot, material_values, 0x100);
    ASSERT_OFFSETOF(scene_snapshot, animation_time,  0x170);
    ASSERT_OFFSETOF(scene_snapshot, diffuse_texture, 0x184);
    ASSERT_OFFSETOF(scene_snapshot, texture_filter,  0x19C);
    ASSERT_OFFSETOF(scene_snapshot, scene_initializer_data, 0x1A4);

    ASSERT_SIZEOF(regular_vertex_prefix, 0x240);
    ASSERT_SIZEOF(regular_vertex_suffix, 0x080);
}}}} // treyarch::ngl::shaders::generated_material
