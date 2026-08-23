#pragma once

#include "treyarch/ngl/texture/texture.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl {
    struct quad_vertex {
        f32 x;
        f32 y;
        f32 u;
        f32 v;
        u32 color;
    };

    struct quad {
        quad_vertex vertices[4];
        f32         z;
        u32         map_flags;
        u64         blend_mode;
        texture*    texture_data;
    };

    void init_quad(quad* value);
    void set_quad_rect(quad* value,
                       f32   left,
                       f32   top,
                       f32   right,
                       f32   bottom);
    void set_quad_color(quad* value, u32 color);
    void set_quad_z(quad* value, f32 z);
    void list_add_quad(const quad* value);

    ASSERT_SIZEOF  (quad_vertex,        0x14);
    ASSERT_OFFSETOF(quad_vertex, x,     0x00);
    ASSERT_OFFSETOF(quad_vertex, y,     0x04);
    ASSERT_OFFSETOF(quad_vertex, u,     0x08);
    ASSERT_OFFSETOF(quad_vertex, v,     0x0C);
    ASSERT_OFFSETOF(quad_vertex, color, 0x10);

    ASSERT_SIZEOF  (quad,               0x68);
    ASSERT_OFFSETOF(quad, vertices,     0x00);
    ASSERT_OFFSETOF(quad, z,            0x50);
    ASSERT_OFFSETOF(quad, map_flags,    0x54);
    ASSERT_OFFSETOF(quad, blend_mode,   0x58);
    ASSERT_OFFSETOF(quad, texture_data, 0x60);
}} // treyarch::ngl
