#pragma once

#include "treyarch/ngl/texture/texture.hh"
#include "treyarch/shared/container/skip_list.hh"
#include "treyarch/shared/fixed_string.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl {
    struct glyph_info {
        i32 texture_offset[2];
        i32 glyph_size[2];
        i32 glyph_origin[2];
        i32 cell_width;
    };

    struct font_header {
        i32 version;
        i32 cell_height;
        i32 ascent;
        i32 first_glyph;
        i32 glyph_count;
    };

    struct font {
        fixed_string name;
        texture*     texture_data;
        glyph_info*  glyphs;
        u32          system;
        u32          pad_014;
        u32          map_flags;
        u32          blend_mode;
        font_header  header;
        f32*         texture_coordinates;
        u32          reserved_038;
    };

    struct font_name {
        static string_hash get(const font* value) {
            return value->name.hash;
        }
    };

    struct font_directory : container::skip_list<font, font_name> {};

    void __cdecl load_font(amalga::file*       owner,
                           amalga::file_entry* entry,
                           void**              mapped_sections,
                           void*               user_data);
    void __cdecl remove_font(amalga::file*       owner,
                             amalga::file_entry* entry,
                             void**              mapped_sections,
                             void*               user_data);

    namespace references {
        inline ::util::memory_reference<font*>          system_font { 0x011160D0 };
        inline ::util::memory_reference<font_directory> fonts       { 0x01116110 };
    } // references

    ASSERT_SIZEOF  (glyph_info,                 0x1C);
    ASSERT_OFFSETOF(glyph_info, texture_offset, 0x00);
    ASSERT_OFFSETOF(glyph_info, glyph_size,     0x08);
    ASSERT_OFFSETOF(glyph_info, glyph_origin,   0x10);
    ASSERT_OFFSETOF(glyph_info, cell_width,     0x18);

    ASSERT_SIZEOF  (font_header,              0x14);
    ASSERT_OFFSETOF(font_header, version,     0x00);
    ASSERT_OFFSETOF(font_header, cell_height, 0x04);
    ASSERT_OFFSETOF(font_header, ascent,      0x08);
    ASSERT_OFFSETOF(font_header, first_glyph, 0x0C);
    ASSERT_OFFSETOF(font_header, glyph_count, 0x10);

    ASSERT_SIZEOF  (font,                     0x3C);
    ASSERT_OFFSETOF(font, name,               0x00);
    ASSERT_OFFSETOF(font, texture_data,       0x08);
    ASSERT_OFFSETOF(font, glyphs,             0x0C);
    ASSERT_OFFSETOF(font, system,             0x10);
    ASSERT_OFFSETOF(font, map_flags,          0x18);
    ASSERT_OFFSETOF(font, blend_mode,         0x1C);
    ASSERT_OFFSETOF(font, header,             0x20);
    ASSERT_OFFSETOF(font, texture_coordinates, 0x34);
    ASSERT_OFFSETOF(font, reserved_038,       0x38);

    ASSERT_SIZEOF(font_directory, 0x10);
}} // treyarch::ngl
