#include <cstdlib>
#include <cstring>

#include "treyarch/ngl/font/text.hh"
#include "treyarch/ngl/list/arena.hh"
#include "treyarch/ngl/scene/matrices.hh"
#include "treyarch/ngl/scene/references.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"

using namespace treyarch;

struct ngl_string_chunk {
    ngl_string_chunk* next;
    const char*       text;
    u32               length;
    f32               x;
    f32               y;
    f32               scale_x;
    f32               scale_y;
    u32               color;
};

struct ngl_string_node {
    ngl::render_node  base;
    const char*       text;
    ngl::font*        font_data;
    f32               x;
    f32               y;
    f32               z;
    f32               scale_x;
    f32               scale_y;
    u32               color;
    ngl_string_chunk* chunks;
};

ASSERT_SIZEOF  (ngl_string_chunk,          0x20);
ASSERT_OFFSETOF(ngl_string_chunk, next,    0x00);
ASSERT_OFFSETOF(ngl_string_chunk, text,    0x04);
ASSERT_OFFSETOF(ngl_string_chunk, length,  0x08);
ASSERT_OFFSETOF(ngl_string_chunk, x,       0x0C);
ASSERT_OFFSETOF(ngl_string_chunk, y,       0x10);
ASSERT_OFFSETOF(ngl_string_chunk, scale_x, 0x14);
ASSERT_OFFSETOF(ngl_string_chunk, scale_y, 0x18);
ASSERT_OFFSETOF(ngl_string_chunk, color,   0x1C);

ASSERT_SIZEOF  (ngl_string_node,            0x30);
ASSERT_OFFSETOF(ngl_string_node, text,       0x0C);
ASSERT_OFFSETOF(ngl_string_node, font_data,  0x10);
ASSERT_OFFSETOF(ngl_string_node, x,          0x14);
ASSERT_OFFSETOF(ngl_string_node, y,          0x18);
ASSERT_OFFSETOF(ngl_string_node, z,          0x1C);
ASSERT_OFFSETOF(ngl_string_node, scale_x,    0x20);
ASSERT_OFFSETOF(ngl_string_node, scale_y,    0x24);
ASSERT_OFFSETOF(ngl_string_node, color,      0x28);
ASSERT_OFFSETOF(ngl_string_node, chunks,     0x2C);

static util::memory_reference<void*> string_node_vtable { 0x00DB7374 };

static const ngl::glyph_info* get_glyph_info(const ngl::font* font_data,
                                                   u8         character) {

    auto index = (i32)character - font_data->header.first_glyph;

    if (index < 0)
        index = 0;
    else if (index > font_data->header.glyph_count - 1)
        index = font_data->header.glyph_count - 1;

    return &font_data->glyphs[index];
}

static u32 get_glyph_cell_width(const ngl::font* font_data, u8 character) {
    return (u32)(get_glyph_info(font_data, character)->cell_width - 1);
}

static void parse_color_token(const char** cursor, u32* color) {
    char* end;
    u32 parsed = std::strtoul(++*cursor, &end, 16);

    *color  = parsed;
    *cursor = end + 1;
}

static void parse_scale_token(const char** cursor, f32* scale) {
    char* end;
    f64 parsed = std::strtod(++*cursor, &end);

    *scale  = (f32)parsed;
    *cursor = end + 1;
}

static void parse_scale_xy_token(const char** cursor,
                                       f32*   scale_x,
                                       f32*   scale_y) {

    char* end;
    f64 parsed_x = std::strtod(++*cursor, &end);

    *scale_x = (f32)parsed_x;
    ++end;

    f64 parsed_y = std::strtod(end, &end);

    *scale_y = (f32)parsed_y;
    *cursor  = end + 1;
}

static u32 rotate_color(u32 value) {
    return (value >> 8) | (value << 24);
}

static ngl_string_chunk* build_string_chunks(      ngl::font* font_data,
                                                   f32        x,
                                                   f32        y,
                                                   f32        scale_x,
                                                   f32        scale_y,
                                                   u32        color,
                                             const char*      text) {
                                                
    f32 line_height = (f32)font_data->header.cell_height;
    f32 line_start  = x;

    f32 maximum_scale_y = scale_y;

    auto* sentinel = (ngl_string_chunk*)
        ngl::list::allocate(sizeof(ngl_string_chunk), 16);

    sentinel->next = nullptr;

    ngl_string_chunk* current = sentinel;
    
    const char* cursor = text;

    while (*cursor) {
        u8 character = (u8)*cursor++;

        switch (character) {
            case 1: {
                u32 parsed_color;

                parse_color_token(&cursor, &parsed_color);
                color = rotate_color(parsed_color);

                break;
            }
            case 2:
                parse_scale_token(&cursor, &scale_x);
                scale_y = scale_x;

                if (scale_y > maximum_scale_y)
                    maximum_scale_y = scale_y;

                break;
            case 3:
                parse_scale_xy_token(&cursor, &scale_x, &scale_y);

                if (scale_y > maximum_scale_y)
                    maximum_scale_y = scale_y;

                break;
            case 9:
                x += (f32)get_glyph_cell_width(font_data, 'a') *
                    scale_x * 4.0f + 1.0f;

                break;
            case 10:
                x  = line_start;
                y += maximum_scale_y * line_height;
                
                maximum_scale_y = scale_y;

                break;
            case 32:
                x += (f32)get_glyph_cell_width(font_data, 'a') * scale_x + 1.0f;

                break;
            default: {
                const char* chunk_start = cursor - 1;

                auto* chunk = (ngl_string_chunk*)
                    ngl::list::allocate(sizeof(ngl_string_chunk), 16);

                current->next = chunk;
                current       = chunk;

                chunk->text    = chunk_start;
                chunk->x       = x;
                chunk->y       = y;
                chunk->scale_x = scale_x;
                chunk->scale_y = scale_y;
                chunk->color   = color;

                cursor = chunk_start;

                while (*cursor != 0 && *cursor != 1 && *cursor != 2 &&
                       *cursor != 3 && *cursor != 9 && *cursor != 10 &&
                       *cursor != 32) {

                    x += (f32)get_glyph_cell_width(font_data, (u8)*cursor) * scale_x + 1.0f;

                    ++cursor;
                }

                chunk->length = (u32)(cursor - chunk_start);

                break;
            }
        }
    }

    current->next = nullptr;

    return sentinel->next;
}

void ngl::get_string_dimensions(      font* font_data,
                                const char* text,
                                      u32*  width,
                                      u32*  height,
                                      f32   scale_x,
                                      f32   scale_y) {
    if (text && *text && font_data && font_data->texture_data) {

        f32 maximum_width    = 0.0f;
        f32 current_width    = 0.0f;
        f32 completed_height = 0.0f;
        f32 maximum_scale_y  = scale_y;
        u8  last_character   = 0;

        const char*  cursor           = text;

        while (*cursor) {
            u8 character = (u8)*cursor++;

            switch (character) {
                case 1: {
                    u32 ignored_color;

                    parse_color_token(&cursor, &ignored_color);

                    break;
                }
                case 2:
                    parse_scale_token(&cursor, &scale_x);
                    scale_y = scale_x;

                    if (scale_y > maximum_scale_y)
                        maximum_scale_y = scale_y;

                    break;
                case 3:
                    parse_scale_xy_token(&cursor, &scale_x, &scale_y);

                    if (scale_y > maximum_scale_y)
                        maximum_scale_y = scale_y;

                    break;
                case 9:
                    current_width +=
                        (f32)get_glyph_cell_width(font_data, ' ') * scale_x * 4.0f + 1.0f;

                    last_character = ' ';

                    break;
                case 10:
                    if (last_character) {
                        const glyph_info* glyph = get_glyph_info(font_data, last_character);

                        current_width +=
                            (f32)(glyph->glyph_size[0] + glyph->glyph_origin[0]) -
                            (f32)(get_glyph_cell_width(font_data, last_character) + 1) *
                            scale_x;
                    }

                    if (current_width > maximum_width)
                        maximum_width = current_width;

                    current_width     = 0.0f;
                    completed_height += (f32)font_data->header.cell_height * maximum_scale_y;
                    maximum_scale_y   = scale_y;

                    last_character = 0;
                    
                    break;
                case 32:
                    current_width += (f32)get_glyph_cell_width(font_data, 'a') * scale_x + 1.0f;

                    last_character = 'a';

                    break;
                default:
                    current_width += (f32)get_glyph_cell_width(font_data, character) * scale_x + 1.0f;
                    
                    last_character = character;
                    
                    break;
            }
        }

        if (last_character) {
            const glyph_info* glyph = get_glyph_info(font_data, last_character);
            
            current_width +=
                (f32)(glyph->glyph_size[0] + glyph->glyph_origin[0]) -
                (f32)(get_glyph_cell_width(font_data, last_character) + 1) *
                scale_x;
        }

        if (width) {
            if (current_width < maximum_width)
                current_width = maximum_width;

            *width = (u32)current_width;
        }

        if (height)
            *height = (u32)((f32)font_data->header.cell_height * maximum_scale_y + completed_height);
    } else {
        if (width)
            *width = 0;

        if (height)
            *height = 0;
    }
}

void ngl::list_add_string(      font* font_data,
                          const char* text,
                                f32   x,
                                f32   y,
                                f32   z,
                                u32   color,
                                f32   scale_x,
                                f32   scale_y) {

    scene* current_scene = references::current_scene.read();

    validate_matrices(current_scene);

    if (text && *text && font_data && font_data->texture_data) {
        auto* node = (ngl_string_node*)
            list::allocate(sizeof(ngl_string_node), 16);

        if (node) {
            node->base.vtable = (void*)&string_node_vtable.get();

            u32 text_length = (u32)std::strlen(text);
            char* stored_text = (char*)list::allocate(text_length + 1, 16);

            node->text = stored_text;
            std::memcpy(stored_text, text, text_length + 1);

            node->font_data              = font_data;
            node->x                      = x;
            node->y                      = y;
            node->z                      = z;
            node->scale_x                = scale_x;
            node->scale_y                = scale_y;
            node->color                  = color;
            node->base.sort_key.floating = z;
            node->chunks                 = build_string_chunks(font_data, x, y, scale_x, scale_y, color, stored_text);
            node->base.next              = current_scene->translucent_render_list;
            
            ++current_scene->translucent_render_list_count;
            current_scene->translucent_render_list = &node->base;
        }
    }
}
