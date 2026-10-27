#pragma once

#include "treyarch/ngl/font/font.hh"
#include "treyarch/ngl/list/render_node.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"

namespace treyarch { namespace ngl {
    struct string_section {
        string_section* next;
        const char*     text;
        u32             length;
        f32             x;
        f32             y;
        f32             scale_x;
        f32             scale_y;
        u32             color;
    };

    struct string_node : render_node {
        const char*     text;
        ngl::font*      font;
        f32             x;
        f32             y;
        f32             z;
        f32             scale_x;
        f32             scale_y;
        u32             color;
        string_section* sections;

        void render() override;
        void get_desc(char* desc) override;
    };

    namespace references {
        inline util::memory_reference<void*> string_node_vftable { 0x00DB7374 };
    } // references

    ASSERT_SIZEOF  (string_section,          0x20);
    ASSERT_OFFSETOF(string_section, next,    0x00);
    ASSERT_OFFSETOF(string_section, text,    0x04);
    ASSERT_OFFSETOF(string_section, length,  0x08);
    ASSERT_OFFSETOF(string_section, x,       0x0C);
    ASSERT_OFFSETOF(string_section, y,       0x10);
    ASSERT_OFFSETOF(string_section, scale_x, 0x14);
    ASSERT_OFFSETOF(string_section, scale_y, 0x18);
    ASSERT_OFFSETOF(string_section, color,   0x1C);

    ASSERT_SIZEOF  (string_node,           0x30);
    ASSERT_OFFSETOF(string_node, text,     0x0C);
    ASSERT_OFFSETOF(string_node, font,     0x10);
    ASSERT_OFFSETOF(string_node, x,        0x14);
    ASSERT_OFFSETOF(string_node, y,        0x18);
    ASSERT_OFFSETOF(string_node, z,        0x1C);
    ASSERT_OFFSETOF(string_node, scale_x,  0x20);
    ASSERT_OFFSETOF(string_node, scale_y,  0x24);
    ASSERT_OFFSETOF(string_node, color,    0x28);
    ASSERT_OFFSETOF(string_node, sections, 0x2C);
}} // treyarch::ngl
