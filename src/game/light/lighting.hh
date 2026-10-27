#pragma once

#include "game/light/light_source_data.hh"
#include "game/wds/subdivision_visitor.hh"
#include "treyarch/ngl/fx/mesh_node_data.hh"
#include "treyarch/ngl/lighting/lighting.hh"
#include "treyarch/ngl/mesh/mesh.hh"
#include "treyarch/ngl/texture/texture.hh"
#include "treyarch/math/matrix.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // the light-table scene parameter: the horizon texture and its u/v projection pairs
    struct light_table {
        u8             reserved_000[0x08];
        ngl::texture*  texture;
        u8             reserved_00c[0x08];
        i32            projection_count;
        math::vector4* projections;

        void get_projection(i32 index, i32 offset, math::vector4* destination) const;
    };

    // what the builders read from the light table
    struct light_table_lookup {
        light_table* table;
        bool         force_horizon;
        bool         fade_horizon_extra; // by the mesh's height
        i32          index;
        i32          offset;

        // inlined @ sub_8A2BA0, sub_8B8620, sub_8CB2D0, sub_8E41B0
        light_table_lookup() : table(nullptr), force_horizon(false), fade_horizon_extra(true), index(-1), offset(4) {}
    };

    struct lighting_directional_light {
        math::vector4 direction;
        math::vector4 color;
    };

    // the part of the lighting block ParamSubset effects use
    struct lighting_parameters {
        u32                        reserved_000;
        u8                         reserved_004[0x10C];
        i32                        directional_light_count;
        u8                         reserved_114[0x0C];
        lighting_directional_light directional_lights[1];
        math::vector4              horizon_projection_u;
        math::vector4              horizon_projection_v;
        math::vector4              ambient_info;
        math::vector4              fog_color;
        math::vector4              fog_control;
        light_source_data*         light_source;

        lighting_parameters();

        void set_light_source(const ngl::fx::mesh_node_data* node_data);
        void build(const ngl::fx::mesh_node_data* node_data,
                   const ngl::mesh_section*       section,
                   const light_table_lookup*      lookup);
    };

    struct lighting_ambient_defaults {
        math::vector4 values[9];

        lighting_ambient_defaults();
    };

    // the full lighting block one draw builds
    struct general_lighting_parameters : lighting_parameters {
        u8                        reserved_194[0x0C];
        i32                       light_count;
        u8                        reserved_1a4[0x0C];
        math::vector4             view_position;
        math::vector4             ambient_direction;
        math::vector4             material_values[4];
        math::vector4             light_positions[4];
        math::vector4             light_directions[4];
        math::vector4             light_colors[4];
        math::vector4             light_attenuation[4];
        math::vector4             horizon_color;
        math::vector4             horizon_axis;
        math::vector4             reserved_330;
        math::vector4             horizon_color_scale;
        math::vector4             horizon_direction;
        math::vector4             horizon_range;
        math::vector4             horizon_direction_scaled;
        math::vector4             horizon_bias;
        math::vector4             horizon_scalar;
        math::vector4             horizon_extra;
        math::vector4             ambient_colors[8];
        math::vector4             ambient_colors_with_primary[8];
        math::vector4             ambient_color;
        math::mat44               projector_matrix;
        math::vector4             post_view_position;
        math::vector4             post_direction;
        math::vector4             post_plane_0;
        math::vector4             post_plane_1;
        math::vector4             post_range;
        math::vector4             post_color;
        ngl::texture*             projector_texture;
        i32                       projector_config_0;
        i32                       projector_config_1;
        i32                       projector_index;
        i32                       special_light_index;
        f32                       horizon_scale;
        u8                        reserved_578[0x08];
        lighting_ambient_defaults ambient_defaults;

        general_lighting_parameters();

        void reset();

        // world space, for effects
        void build(const ngl::fx::mesh_node_data* node_data,
                   const ngl::mesh_section*       section,
                         bool                     flagged_only, // only lights with flag 1
                   const light_table_lookup*      lookup,
                         bool                     gather_local);

        // in the space the matrices give, for the generated materials
        void build_local(const ngl::fx::mesh_node_data* node_data,
                         const ngl::mesh_section*       section,
                         const math::mat44             &local_to_world,
                         const math::mat44             &world_to_local,
                               bool                     flagged_only,
                         const light_table_lookup*      lookup,
                               bool                     gather_local,
                               bool                     drop_dark_lights);

        void build_lights(const ngl::fx::mesh_node_data* node_data,
                          const ngl::mesh_section*       section,
                          const math::mat44             &local_to_world,
                          const math::mat44             &world_to_local,
                          const math::mat44             &scaled_world_to_local,
                                bool                     flagged_only,
                          const light_table_lookup*      lookup,
                                bool                     gather_local,
                                bool                     drop_dark_lights);

        void build_environment(const math::mat44        &local_to_world,
                               const math::mat44        &world_to_local,
                                     bool                skinned,
                                     bool                flagged_only,
                               const light_table_lookup* lookup);

        bool add_directional_light(const ngl::lighting::generated_light_data* light,
                                   const math::mat44                         &world_to_local,
                                   const math::mat44                         &local_to_world,
                                   const math::vector4                       &attenuation,
                                         f32                                  scale);

        void gather_local_lights(const ngl::fx::mesh_node_data* node_data,
                                 const math::mat44             &local_to_world,
                                 const math::mat44             &world_to_local,
                                 const math::mat44             &scaled_world_to_local,
                                 const math::vector4           &world_center,
                                       f32                      inverse_scale,
                                       bool                     flagged_only,
                                       bool                     skinned);
    };

    struct point_light_candidate {
        u32 index;
        u32 integer_distance; // the sort key
        f32 squared_distance;
        u32 reserved_00c;

        bool operator<(const point_light_candidate &other) const {
            return integer_distance < other.integer_distance;
        }
    };

    // the point-light query's visitor over the light context's R-tree
    struct lighting_visitor : subdivision_visitor {
        point_light_candidate candidates[512];
        u32                   count;
        u32                   reserved_200c;
        math::vector4         center;

        e_visit_result visit(subdivision_leaf_handle leaf_handle) override;

        i32 select_nearest();
    };

    // the point lights one mesh draw picked
    struct point_light_set {
        i32 count;
        u8  reserved_004[0x10C];

        // sub_A6C820
        point_light_set() : count(0) {}

        i32 gather(      ngl::fx::mesh_node_data* node_data,
                   const ngl::mesh_section*       section,
                   const math::vector4           &center,
                         f32                      scale);

        void query(      ngl::fx::mesh_node_data* node_data,
                   const ngl::mesh_section*       section,
                   const math::vector4           &center,
                         f32                      radius);
    };

    math::mat44* get_scaled_world_to_local(const ngl::fx::mesh_node_data* node_data, math::mat44* destination);

    i32 get_light_table_index(const math::vector4 &center, const math::vector4 &extents);

    ASSERT_SIZEOF  (light_table,                   0x1C);
    ASSERT_OFFSETOF(light_table, texture,          0x08);
    ASSERT_OFFSETOF(light_table, projection_count, 0x14);
    ASSERT_OFFSETOF(light_table, projections,      0x18);

    ASSERT_SIZEOF  (light_table_lookup,                     0x10);
    ASSERT_OFFSETOF(light_table_lookup, force_horizon,      0x04);
    ASSERT_OFFSETOF(light_table_lookup, fade_horizon_extra, 0x05);
    ASSERT_OFFSETOF(light_table_lookup, index,              0x08);
    ASSERT_OFFSETOF(light_table_lookup, offset,             0x0C);

    ASSERT_SIZEOF  (lighting_parameters,                          0x194);
    ASSERT_OFFSETOF(lighting_parameters, directional_light_count, 0x110);
    ASSERT_OFFSETOF(lighting_parameters, directional_lights,      0x120);
    ASSERT_OFFSETOF(lighting_parameters, horizon_projection_u,    0x140);
    ASSERT_OFFSETOF(lighting_parameters, horizon_projection_v,    0x150);
    ASSERT_OFFSETOF(lighting_parameters, ambient_info,            0x160);
    ASSERT_OFFSETOF(lighting_parameters, fog_color,               0x170);
    ASSERT_OFFSETOF(lighting_parameters, fog_control,             0x180);
    ASSERT_OFFSETOF(lighting_parameters, light_source,            0x190);

    ASSERT_SIZEOF  (general_lighting_parameters,                              0x610);
    ASSERT_OFFSETOF(general_lighting_parameters, light_count,                 0x1A0);
    ASSERT_OFFSETOF(general_lighting_parameters, view_position,               0x1B0);
    ASSERT_OFFSETOF(general_lighting_parameters, ambient_direction,           0x1C0);
    ASSERT_OFFSETOF(general_lighting_parameters, material_values,             0x1D0);
    ASSERT_OFFSETOF(general_lighting_parameters, light_positions,             0x210);
    ASSERT_OFFSETOF(general_lighting_parameters, light_directions,            0x250);
    ASSERT_OFFSETOF(general_lighting_parameters, light_colors,                0x290);
    ASSERT_OFFSETOF(general_lighting_parameters, light_attenuation,           0x2D0);
    ASSERT_OFFSETOF(general_lighting_parameters, horizon_color,               0x310);
    ASSERT_OFFSETOF(general_lighting_parameters, horizon_axis,                0x320);
    ASSERT_OFFSETOF(general_lighting_parameters, horizon_color_scale,         0x340);
    ASSERT_OFFSETOF(general_lighting_parameters, horizon_direction,           0x350);
    ASSERT_OFFSETOF(general_lighting_parameters, horizon_range,               0x360);
    ASSERT_OFFSETOF(general_lighting_parameters, horizon_direction_scaled,    0x370);
    ASSERT_OFFSETOF(general_lighting_parameters, horizon_bias,                0x380);
    ASSERT_OFFSETOF(general_lighting_parameters, horizon_scalar,              0x390);
    ASSERT_OFFSETOF(general_lighting_parameters, horizon_extra,               0x3A0);
    ASSERT_OFFSETOF(general_lighting_parameters, ambient_colors,              0x3B0);
    ASSERT_OFFSETOF(general_lighting_parameters, ambient_colors_with_primary, 0x430);
    ASSERT_OFFSETOF(general_lighting_parameters, ambient_color,               0x4B0);
    ASSERT_OFFSETOF(general_lighting_parameters, projector_matrix,            0x4C0);
    ASSERT_OFFSETOF(general_lighting_parameters, post_view_position,          0x500);
    ASSERT_OFFSETOF(general_lighting_parameters, post_direction,              0x510);
    ASSERT_OFFSETOF(general_lighting_parameters, post_plane_0,                0x520);
    ASSERT_OFFSETOF(general_lighting_parameters, post_plane_1,                0x530);
    ASSERT_OFFSETOF(general_lighting_parameters, post_range,                  0x540);
    ASSERT_OFFSETOF(general_lighting_parameters, post_color,                  0x550);
    ASSERT_OFFSETOF(general_lighting_parameters, projector_texture,           0x560);
    ASSERT_OFFSETOF(general_lighting_parameters, projector_config_0,          0x564);
    ASSERT_OFFSETOF(general_lighting_parameters, projector_config_1,          0x568);
    ASSERT_OFFSETOF(general_lighting_parameters, projector_index,             0x56C);
    ASSERT_OFFSETOF(general_lighting_parameters, special_light_index,         0x570);
    ASSERT_OFFSETOF(general_lighting_parameters, horizon_scale,               0x574);
    ASSERT_OFFSETOF(general_lighting_parameters, ambient_defaults,            0x580);

    ASSERT_SIZEOF(point_light_candidate, 0x10);

    ASSERT_SIZEOF  (lighting_visitor,             0x2020);
    ASSERT_OFFSETOF(lighting_visitor, candidates, 0x0008);
    ASSERT_OFFSETOF(lighting_visitor, count,      0x2008);
    ASSERT_OFFSETOF(lighting_visitor, center,     0x2010);

    ASSERT_SIZEOF(point_light_set, 0x110);
} // treyarch
