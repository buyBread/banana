#pragma once

#include "treyarch/ngl/list/render_node.hh"
#include "treyarch/ngl/texture/texture.hh"
#include "treyarch/shared/math/types/vector4.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace ngl { namespace shaders { namespace fake_peds {
    struct pedestrian {
        vector4 center;
        vector4 right;
        vector4 up;
        vector4 color;
        i32     atlas_tile;
        u8      reserved_044[0x0C];
    };

    struct render_node {
        ngl::render_node base;
        pedestrian*      pedestrians;
        u32              pedestrian_count;
        texture*         texture_data;
        u32              atlas_tile_count;
        u32              blend_mode;
    };

    void render(render_node* value);

    namespace references {
        inline util::memory_reference<void*> node_vtable { 0x00BE5314 };
    } // references

    ASSERT_SIZEOF  (pedestrian,             0x50);
    ASSERT_OFFSETOF(pedestrian, center,     0x00);
    ASSERT_OFFSETOF(pedestrian, right,      0x10);
    ASSERT_OFFSETOF(pedestrian, up,         0x20);
    ASSERT_OFFSETOF(pedestrian, color,      0x30);
    ASSERT_OFFSETOF(pedestrian, atlas_tile, 0x40);

    ASSERT_SIZEOF  (render_node,                   0x20);
    ASSERT_OFFSETOF(render_node, pedestrians,      0x0C);
    ASSERT_OFFSETOF(render_node, pedestrian_count, 0x10);
    ASSERT_OFFSETOF(render_node, texture_data,     0x14);
    ASSERT_OFFSETOF(render_node, atlas_tile_count, 0x18);
    ASSERT_OFFSETOF(render_node, blend_mode,       0x1C);
}}}} // treyarch::ngl::shaders::fake_peds
