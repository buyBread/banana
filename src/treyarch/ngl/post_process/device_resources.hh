#pragma once

#include <d3d9.h>

#include "treyarch/ngl/d3d9/texture.hh"
#include "treyarch/ngl/d3d9/vertex_definition.hh"
#include "treyarch/ngl/texture/texture.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl { namespace post_process {
    struct surface_texture {
        ngl::d3d9::texture_resource texture;
        IDirect3DSurface9*          render_target;
        IDirect3DSurface9*          saved_color_target;
        IDirect3DSurface9*          saved_depth_target;
        u8                          owns_render_target;
        u8                          pad_02d[3];

        void release_device_resources();
        void restore_device_resources();
    };

    struct mipmap_texture_array {
        void*           vtable;
        u32             count;
        surface_texture textures[16];
        u32             width;
        u32             height;

        void release_device_resources();
        void restore_device_resources();
    };

    struct system {
        u8                    reserved_000[0x08];
        void*                 blitter;
        mipmap_texture_array* hdr_targets;
        mipmap_texture_array* ldr_targets;
        surface_texture*      single_target_0;
        surface_texture*      single_target_1;
        mipmap_texture_array* secondary_ldr_targets_0;
        mipmap_texture_array* secondary_ldr_targets_1;
        surface_texture*      single_target_2;
        u8                    reserved_028[0x3B8];

        void release_device_resources();
        void restore_device_resources();
    };

    struct device_resource_state {
        ngl::texture*            eighth_target;
        f32                      inverse_width;
        u8                       reserved_008[0x04];
        IDirect3DVertexBuffer9*  half_texel_vertex_buffer_1;
        ngl::texture*            sixty_fourth_target;
        u8                       reserved_014[0x44];
        IDirect3DVertexBuffer9*  quad_vertex_buffer_0;
        f32                      inverse_height;
        u8                       reserved_060[0x04];
        ngl::vertex_definition   quad_vertex_definition;
        ngl::vertex_definition   half_texel_vertex_definition;
        ngl::texture*            render_targets[3];
        u8                       reserved_088[0x30];
        IDirect3DVertexBuffer9*  half_texel_vertex_buffer_0;
        IDirect3DVertexBuffer9*  quad_vertex_buffer_1;
        u8                       reserved_0c0[0x3F];
        u8                       initialized;
        u8                       reserved_100[0x58];
        ngl::texture*            quarter_target;
        u8                       reserved_15c[0x04];
        post_process::system*    system;
    };

    void release_device_resources();
    void restore_device_resources();

    namespace references {
        inline util::memory_reference<device_resource_state> device_resources { 0x0102FD50 };
    } // references

    ASSERT_SIZEOF  (surface_texture,                     0x30);
    ASSERT_OFFSETOF(surface_texture, texture,            0x00);
    ASSERT_OFFSETOF(surface_texture, render_target,      0x20);
    ASSERT_OFFSETOF(surface_texture, saved_color_target, 0x24);
    ASSERT_OFFSETOF(surface_texture, saved_depth_target, 0x28);
    ASSERT_OFFSETOF(surface_texture, owns_render_target, 0x2C);

    ASSERT_SIZEOF  (mipmap_texture_array,           0x310);
    ASSERT_OFFSETOF(mipmap_texture_array, count,    0x004);
    ASSERT_OFFSETOF(mipmap_texture_array, textures, 0x008);
    ASSERT_OFFSETOF(mipmap_texture_array, width,    0x308);
    ASSERT_OFFSETOF(mipmap_texture_array, height,   0x30C);

    ASSERT_SIZEOF  (system,                          0x3E0);
    ASSERT_OFFSETOF(system, blitter,                 0x008);
    ASSERT_OFFSETOF(system, hdr_targets,             0x00C);
    ASSERT_OFFSETOF(system, ldr_targets,             0x010);
    ASSERT_OFFSETOF(system, single_target_0,         0x014);
    ASSERT_OFFSETOF(system, single_target_1,         0x018);
    ASSERT_OFFSETOF(system, secondary_ldr_targets_0, 0x01C);
    ASSERT_OFFSETOF(system, secondary_ldr_targets_1, 0x020);
    ASSERT_OFFSETOF(system, single_target_2,         0x024);

    ASSERT_SIZEOF  (device_resource_state,                               0x164);
    ASSERT_OFFSETOF(device_resource_state, eighth_target,                0x000);
    ASSERT_OFFSETOF(device_resource_state, inverse_width,                0x004);
    ASSERT_OFFSETOF(device_resource_state, half_texel_vertex_buffer_1,   0x00C);
    ASSERT_OFFSETOF(device_resource_state, sixty_fourth_target,          0x010);
    ASSERT_OFFSETOF(device_resource_state, quad_vertex_buffer_0,         0x058);
    ASSERT_OFFSETOF(device_resource_state, inverse_height,               0x05C);
    ASSERT_OFFSETOF(device_resource_state, quad_vertex_definition,       0x064);
    ASSERT_OFFSETOF(device_resource_state, half_texel_vertex_definition, 0x070);
    ASSERT_OFFSETOF(device_resource_state, render_targets,               0x07C);
    ASSERT_OFFSETOF(device_resource_state, half_texel_vertex_buffer_0,   0x0B8);
    ASSERT_OFFSETOF(device_resource_state, quad_vertex_buffer_1,         0x0BC);
    ASSERT_OFFSETOF(device_resource_state, initialized,                  0x0FF);
    ASSERT_OFFSETOF(device_resource_state, quarter_target,               0x158);
    ASSERT_OFFSETOF(device_resource_state, system,                       0x160);
}}} // treyarch::ngl::post_process
