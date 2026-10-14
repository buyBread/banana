#pragma once

#include <d3d9.h>

#include "speedtree/rt.hh"
#include "treyarch/game/light/light_source_data.hh"
#include "treyarch/game/wds/region.hh"
#include "treyarch/ngl/d3d9/vertex_definition.hh"
#include "treyarch/ngl/fx/effect.hh"
#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/math/types/matrix4x4.hh"
#include "treyarch/shared/math/types/vector3.hh"
#include "treyarch/shared/math/types/vector4.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    namespace mash {
        class mash_info_struct;
    } // mash

    namespace ngl {
        struct texture;
    } // ngl

#pragma pack(push, 2)
    // one placed tree in the level's speedtree_collection
    struct speed_tree_color_source {
        i16 position[3]; // quarter units
        u16 unk_06;
        u32 color;

        // sub_5FD110
        u32 get_color() const {
            return color;
        }

        void construct_mashed_class() {}
        void destruct_mashed_class() {}
        void unmash(mash::mash_info_struct*, void*, mash::buffer_type) {}
    };
#pragma pack(pop)

    struct speed_tree_instance {
        vector3                  position;
        u8                       reserved_00c[0x10];
        f32                      radius;
        f32                      rotation; // degrees
        f32                      scale;
        i32                      wind_matrix_offset;
        u8                       tree[0xE4]; // CSpeedTreeRT
        speed_tree_color_source* color_source;
        speed_tree_instance*     next;

        bool in_view(const matrix4x4 &basis) const;
    };

    // the node payload; the milestone asserts on Params.region.InstanceCount
    struct speed_tree_region {
        region*              world_region;
        speed_tree_instance* instances;
        i32                  instance_count;
    };

    struct speed_tree_render_params {
        speed_tree_region  region;
        light_source_data* light;
        bool               translucent;
        u8                 pad_011[3];
        f32*               fades;
        region_horizon*    horizon;
        u32                unk_1c;
        vector4            horizon_projection_u;
        vector4            horizon_projection_v;
    };

    struct speed_tree_billboards_only_params {
        i32                region;
        light_source_data* light;
    };

    struct speed_tree_lighting {
        vector4 light_direction;
        vector4 diffuse;
        vector4 ambient;
        vector4 unk_30;
        vector4 depth_fog_color;
        vector4 depth_fog_control;
        f32     diffuse_minimum;
        f32     diffuse_maximum;
        f32     diffuse_scale;
        f32     exposure;

        void build(const light_source_data* light);
        void shade(const u8* color, vector4 (&result)[2]) const;
    };

    // named after the HLSL globals they resolve
    struct speed_tree_parameters {
        ngl::fx::parameter* opacity;
        ngl::fx::parameter* model_view_projection;
        ngl::fx::parameter* local_to_world;
        ngl::fx::parameter* model_view;
        ngl::fx::parameter* dof;
        ngl::fx::parameter* self_shadow_map;
        ngl::fx::parameter* branch_diffuse_map;
        ngl::fx::parameter* branch_normal_map;
        ngl::fx::parameter* billboard_diffuse_map;
        ngl::fx::parameter* billboard_normal_map;
        ngl::fx::parameter* tree_position;
        ngl::fx::parameter* tree_scale;
        ngl::fx::parameter* billboard_vertex_table;
        ngl::fx::parameter* billboard_fade;
        ngl::fx::parameter* special_azimuth;
        ngl::fx::parameter* texcoords_360;
        ngl::fx::parameter* constants_360;
        ngl::fx::parameter* image_count_360;
        ngl::fx::parameter* billboard_light_adjust;
        ngl::fx::parameter* tree_alpha_test_value;
        ngl::fx::parameter* composite_leaf_map;
        ngl::fx::parameter* leaf_unit_square;
        ngl::fx::parameter* leaf_angles;
        ngl::fx::parameter* leaf_cards_active;
        ngl::fx::parameter* leaf_angle_scalars;
        ngl::fx::parameter* camera_angles;
        ngl::fx::parameter* wind_matrices;
        ngl::fx::parameter* wind_matrix_offset;
        ngl::fx::parameter* self_shadow_motion;
        ngl::fx::parameter* camera_position;
        ngl::fx::parameter* light_direction;
        ngl::fx::parameter* diffuse;
        ngl::fx::parameter* ambient;
        ngl::fx::parameter* exposure;
        ngl::fx::parameter* depth_fog_color;
        ngl::fx::parameter* depth_fog_control;
        ngl::fx::parameter* horizon_projection_u;
        ngl::fx::parameter* horizon_projection_v;
        ngl::fx::parameter* horizon_map;
    };

    struct speed_tree_leaf_lod {
        u32 unk_00;
        u32 vertex_buffer;
        i32 card_count;
        u8  reserved_00c[0x3C];
    };

    class speed_tree_wrapper {

    public:
        ngl::fx::effect*         effect;
        u8                       reserved_004[0x04];
        speed_tree_region        regions[16];
        u8                       reserved_0c8[0x108];
        ngl::texture*            textures[24];
        speed_tree_parameters    parameters;
        u8                       reserved_2cc[0x10];
        i32                      branch_vertex_count;
        u8                       reserved_2e0[0x54];
        i32                      frond_vertex_count;
        u8                       reserved_338[0x48];
        speed_tree_leaf_lod*     leaf_lods;
        u8                       reserved_384[0x08];
        i32                      image_count_360;
        u8                       reserved_390[0xC0];
        IDirect3DVertexBuffer9*  region_vertex_buffers[800];
        i32                      region_vertex_counts[800];
        IDirect3DVertexBuffer9*  billboard_vertex_buffer;
        IDirect3DIndexBuffer9*   billboard_index_buffer;
        matrix4x4                texcoords_360[4];
        u8                       region_colors[800][4];
        IDirect3DVertexBuffer9*  branch_vertex_buffer;
        IDirect3DIndexBuffer9**  branch_index_buffers;
        u16*                     branch_strip_lengths;
        IDirect3DVertexBuffer9*  frond_vertex_buffer;
        IDirect3DIndexBuffer9**  frond_index_buffers;
        u16*                     frond_strip_lengths;
        IDirect3DVertexBuffer9** leaf_vertex_buffers;
        u8                       reserved_2af4[0x0C];
        speed_tree_lighting      lighting;

        static matrix4x4 axis_basis();
        static bool      sphere_in_view(const matrix4x4 &basis,
                                        const vector3   &position,
                                              f32        radius,
                                              vector3   &transformed);

        void find_technique(u32                 &guard,
                            u32                  bit,
                            ngl::fx::technique* &technique,
                            const char*          name);

        void upload_wind_matrices();
        void upload_lighting(light_source_data* light);
        void upload_color(const u8* color);
        void upload_transforms(const matrix4x4 &basis);
        f32  upload_billboard_images();

        void submit_nodes();
        void submit_billboards_only(bool all_regions);

        void render_billboards_only(const speed_tree_billboards_only_params* params);
        void render_billboards(const speed_tree_render_params* params);
        void render_branches(const speed_tree_render_params* params);
        void render_fronds(const speed_tree_render_params* params);
        void render_leafs(const speed_tree_render_params* params);
    };

    namespace references {
        inline util::memory_reference<speedtree::wind> speed_tree_wind { 0x010876D8 };

        inline util::memory_reference<f32> speed_tree_sky_color_scale { 0x00E7CCD0 };

        inline util::memory_reference<ngl::vertex_definition> speed_tree_branch_format           { 0x01086D90 };
        inline util::memory_reference<ngl::vertex_definition> speed_tree_region_billboard_format { 0x01086D9C };
        inline util::memory_reference<ngl::vertex_definition> speed_tree_leaf_format             { 0x01086E00 };
        inline util::memory_reference<ngl::vertex_definition> speed_tree_frond_format            { 0x01086E0C };
        inline util::memory_reference<ngl::vertex_definition> speed_tree_billboard_format        { 0x01086E70 };

        inline util::memory_reference<ngl::fx::technique*> speed_tree_billboards_only_technique    { 0x01087F48 };
        inline util::memory_reference<ngl::fx::technique*> speed_tree_billboards_technique         { 0x01087F50 };
        inline util::memory_reference<ngl::fx::technique*> speed_tree_branches_depth_technique     { 0x01087F58 };
        inline util::memory_reference<ngl::fx::technique*> speed_tree_branches_technique           { 0x01087F5C };
        inline util::memory_reference<ngl::fx::technique*> speed_tree_fronds_depth_technique       { 0x01087F64 };
        inline util::memory_reference<ngl::fx::technique*> speed_tree_fronds_technique             { 0x01087F68 };
        inline util::memory_reference<ngl::fx::technique*> speed_tree_leaves_translucent_technique { 0x01087F70 };
        inline util::memory_reference<ngl::fx::technique*> speed_tree_leaves_depth_technique       { 0x01087F74 };
        inline util::memory_reference<ngl::fx::technique*> speed_tree_leaves_technique             { 0x01087F78 };

        // the function-local static guards for the lookups above
        inline util::memory_reference<u32> speed_tree_billboards_only_guard { 0x01087F4C };
        inline util::memory_reference<u32> speed_tree_billboards_guard      { 0x01087F54 };
        inline util::memory_reference<u32> speed_tree_branches_guard        { 0x01087F60 };
        inline util::memory_reference<u32> speed_tree_fronds_guard          { 0x01087F6C };
        inline util::memory_reference<u32> speed_tree_leaves_guard          { 0x01087F7C };

        inline util::memory_reference<vector4> speed_tree_view_direction { 0x01087820 };

        inline util::memory_reference<f32> speed_tree_view_heading { 0x01086E18 };
        inline util::memory_reference<f32> speed_tree_view_pitch   { 0x01086E1C };

        inline util::memory_reference<void*> speed_tree_global_environment { 0x01086E8C };

        // leaves in these regions fade with view depth
        inline util::memory_reference<region*> speed_tree_fading_regions      { 0x01087D20 };
        inline util::memory_reference<u32>     speed_tree_fading_region_count { 0x01087D5C };

        inline util::memory_reference<vector4> speed_tree_fade_offset       { 0x01087F90 };
        inline util::memory_reference<u32>     speed_tree_fade_offset_guard { 0x01087FA0 };
    } // references

    ASSERT_SIZEOF  (speed_tree_color_source,         0x0C);
    ASSERT_OFFSETOF(speed_tree_color_source, unk_06, 0x06);
    ASSERT_OFFSETOF(speed_tree_color_source, color,  0x08);

    ASSERT_OFFSETOF(speed_tree_instance, radius,             0x01C);
    ASSERT_OFFSETOF(speed_tree_instance, rotation,           0x020);
    ASSERT_OFFSETOF(speed_tree_instance, scale,              0x024);
    ASSERT_OFFSETOF(speed_tree_instance, wind_matrix_offset, 0x028);
    ASSERT_OFFSETOF(speed_tree_instance, tree,               0x02C);
    ASSERT_OFFSETOF(speed_tree_instance, color_source,       0x110);
    ASSERT_OFFSETOF(speed_tree_instance, next,               0x114);

    ASSERT_SIZEOF  (speed_tree_region,                 0x0C);
    ASSERT_OFFSETOF(speed_tree_region, world_region,   0x00);
    ASSERT_OFFSETOF(speed_tree_region, instances,      0x04);
    ASSERT_OFFSETOF(speed_tree_region, instance_count, 0x08);

    ASSERT_SIZEOF  (speed_tree_render_params,                       0x40);
    ASSERT_OFFSETOF(speed_tree_render_params, light,                0x0C);
    ASSERT_OFFSETOF(speed_tree_render_params, translucent,          0x10);
    ASSERT_OFFSETOF(speed_tree_render_params, fades,                0x14);
    ASSERT_OFFSETOF(speed_tree_render_params, horizon,              0x18);
    ASSERT_OFFSETOF(speed_tree_render_params, horizon_projection_u, 0x20);
    ASSERT_OFFSETOF(speed_tree_render_params, horizon_projection_v, 0x30);

    ASSERT_SIZEOF  (speed_tree_billboards_only_params,        0x08);
    ASSERT_OFFSETOF(speed_tree_billboards_only_params, light, 0x04);

    ASSERT_SIZEOF  (speed_tree_lighting,                    0x70);
    ASSERT_OFFSETOF(speed_tree_lighting, diffuse,           0x10);
    ASSERT_OFFSETOF(speed_tree_lighting, ambient,           0x20);
    ASSERT_OFFSETOF(speed_tree_lighting, depth_fog_color,   0x40);
    ASSERT_OFFSETOF(speed_tree_lighting, depth_fog_control, 0x50);
    ASSERT_OFFSETOF(speed_tree_lighting, diffuse_minimum,   0x60);
    ASSERT_OFFSETOF(speed_tree_lighting, exposure,          0x6C);

    ASSERT_SIZEOF  (speed_tree_parameters,                0x9C);
    ASSERT_OFFSETOF(speed_tree_parameters, wind_matrices, 0x68);
    ASSERT_OFFSETOF(speed_tree_parameters, horizon_map,   0x98);

    ASSERT_SIZEOF  (speed_tree_leaf_lod,                0x48);
    ASSERT_OFFSETOF(speed_tree_leaf_lod, vertex_buffer, 0x04);
    ASSERT_OFFSETOF(speed_tree_leaf_lod, card_count,    0x08);

    ASSERT_OFFSETOF(speed_tree_wrapper, regions,                 0x0008);
    ASSERT_OFFSETOF(speed_tree_wrapper, textures,                0x01D0);
    ASSERT_OFFSETOF(speed_tree_wrapper, parameters,              0x0230);
    ASSERT_OFFSETOF(speed_tree_wrapper, branch_vertex_count,     0x02DC);
    ASSERT_OFFSETOF(speed_tree_wrapper, frond_vertex_count,      0x0334);
    ASSERT_OFFSETOF(speed_tree_wrapper, leaf_lods,               0x0380);
    ASSERT_OFFSETOF(speed_tree_wrapper, image_count_360,         0x038C);
    ASSERT_OFFSETOF(speed_tree_wrapper, region_vertex_buffers,   0x0450);
    ASSERT_OFFSETOF(speed_tree_wrapper, region_vertex_counts,    0x10D0);
    ASSERT_OFFSETOF(speed_tree_wrapper, billboard_vertex_buffer, 0x1D50);
    ASSERT_OFFSETOF(speed_tree_wrapper, texcoords_360,           0x1D58);
    ASSERT_OFFSETOF(speed_tree_wrapper, region_colors,           0x1E58);
    ASSERT_OFFSETOF(speed_tree_wrapper, branch_vertex_buffer,    0x2AD8);
    ASSERT_OFFSETOF(speed_tree_wrapper, frond_strip_lengths,     0x2AEC);
    ASSERT_OFFSETOF(speed_tree_wrapper, leaf_vertex_buffers,     0x2AF0);
    ASSERT_OFFSETOF(speed_tree_wrapper, lighting,                0x2B00);
} // treyarch
