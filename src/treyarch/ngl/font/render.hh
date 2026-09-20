#pragma once

#include "treyarch/ngl/font/font.hh"
#include "treyarch/ngl/list/render_node.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace ngl { namespace string_renderer {
    struct chunk {
        chunk*      next;
        const char* text;
        u32         length;
        f32         x;
        f32         y;
        f32         scale_x;
        f32         scale_y;
        u32         color;
    };

    struct node {
        render_node base;
        const char* text;
        font*       font_data;
        f32         x;
        f32         y;
        f32         z;
        f32         scale_x;
        f32         scale_y;
        u32         color;
        chunk*      chunks;
    };

    void render(node* value);

    namespace references {
        inline util::memory_reference<void*> node_vtable { 0x00DB7374 };
    } // references

    ASSERT_SIZEOF  (chunk,          0x20);
    ASSERT_OFFSETOF(chunk, next,    0x00);
    ASSERT_OFFSETOF(chunk, text,    0x04);
    ASSERT_OFFSETOF(chunk, length,  0x08);
    ASSERT_OFFSETOF(chunk, x,       0x0C);
    ASSERT_OFFSETOF(chunk, y,       0x10);
    ASSERT_OFFSETOF(chunk, scale_x, 0x14);
    ASSERT_OFFSETOF(chunk, scale_y, 0x18);
    ASSERT_OFFSETOF(chunk, color,   0x1C);

    ASSERT_SIZEOF  (node,             0x30);
    ASSERT_OFFSETOF(node, text,       0x0C);
    ASSERT_OFFSETOF(node, font_data,  0x10);
    ASSERT_OFFSETOF(node, x,          0x14);
    ASSERT_OFFSETOF(node, y,          0x18);
    ASSERT_OFFSETOF(node, z,          0x1C);
    ASSERT_OFFSETOF(node, scale_x,    0x20);
    ASSERT_OFFSETOF(node, scale_y,    0x24);
    ASSERT_OFFSETOF(node, color,      0x28);
    ASSERT_OFFSETOF(node, chunks,     0x2C);
}}} // treyarch::ngl::string_renderer
