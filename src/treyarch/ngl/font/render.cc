#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/internal_programs.hh"
#include "treyarch/ngl/d3d9/state_cache.hh"
#include "treyarch/ngl/d3d9/texture.hh"
#include "treyarch/ngl/d3d9/vertex_formats.hh"
#include "treyarch/ngl/font/render.hh"
#include "treyarch/ngl/scene/references.hh"

using namespace treyarch;

struct string_draw_vertex {
    f32 x;
    f32 y;
    f32 z;
    u32 color;
    f32 u;
    f32 v;
};

struct string_glyph_rectangle {
    f32 origin_x;
    f32 origin_y;
    f32 width;
    f32 height;
    f32 u;
    f32 v;
    f32 width_u;
    f32 height_v;
};

ASSERT_SIZEOF(string_draw_vertex, 0x18);

void get_string_glyph_rectangle(const ngl::font*              font_data,
                                      u8                      character,
                                      f32                     scale_x,
                                      f32                     scale_y,
                                      string_glyph_rectangle* result) {

    i32 index = (i32)character - font_data->header.first_glyph;

    if (index < 0)
        index = 0;
    else if (index > font_data->header.glyph_count - 1)
        index = font_data->header.glyph_count - 1;

    const ngl::glyph_info &glyph = font_data->glyphs[index];

    result->origin_x = (f32)glyph.glyph_origin[0] * scale_x;
    result->origin_y = (f32)glyph.glyph_origin[1] * scale_y;
    result->width    = (f32)glyph.glyph_size[0] * scale_x;
    result->height   = (f32)glyph.glyph_size[1] * scale_y;

    u32 texture_index = (u8)(character - (u8)font_data->header.first_glyph) * 4;
    const f32* coordinates = font_data->texture_coordinates + texture_index;

    result->u        = coordinates[0];
    result->v        = coordinates[1];
    result->width_u  = coordinates[2] - coordinates[0];
    result->height_v = coordinates[3] - coordinates[1];
}

void ngl::string_renderer::render(node* value) {
    if (!value->text || !value->font_data->texture_data || !value->chunks)
        return;

    u32 character_count = 0;

    for (chunk* current = value->chunks; current; current = current->next)
        character_count += current->length;

    if (!character_count)
        return;

    scene* current_scene = ngl::references::current_scene.read();
    IDirect3DDevice9* device = d3d9::references::device.get();

    d3d9::set_render_state(D3DRS_FOGENABLE, FALSE);

    matrix4x4 matrix = current_scene->derived_matrix_2d0;

    d3d9::set_render_state(D3DRS_FILLMODE, D3DFILL_SOLID);
    d3d9::set_render_state(D3DRS_CULLMODE, D3DCULL_NONE);

    d3d9::internal_program_set &programs = d3d9::references::internal_programs.get();

    d3d9::set_vertex_program(programs.font_vertex_program);
    d3d9::set_pixel_program(programs.texture_modulate_pixel_program);
    device->SetVertexShaderConstantF(0, (const f32*)&matrix, 4);

    u64 blend_mode = ((u64)value->font_data->blend_mode << 32) | value->font_data->map_flags;

    d3d9::apply_blend_mode(blend_mode);
    d3d9::set_render_state(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
    d3d9::set_render_state(D3DRS_SRCBLENDALPHA, D3DBLEND_ZERO);
    d3d9::set_render_state(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);
    d3d9::set_render_state(D3DRS_BLENDOPALPHA, D3DBLENDOP_ADD);

    d3d9::bind_texture(0,
                       value->font_data->texture_data,
                       value->font_data->system,
                       3);

    f32 depth_z = value->z;

    if (current_scene->near_plane > depth_z)
        depth_z = current_scene->near_plane;
    else if (depth_z > current_scene->far_plane)
        depth_z = current_scene->far_plane;

    f32 numerator = (f32)((f64)current_scene->view_to_screen.z.z * (f64)depth_z);
        numerator = (f32)((f64)numerator + (f64)current_scene->view_to_screen.w.z);

    f32 denominator = (f32)((f64)current_scene->view_to_screen.z.w * (f64)depth_z);
        denominator = (f32)((f64)denominator + (f64)current_scene->view_to_screen.w.w);

    f32 depth = (f32)((f64)numerator / (f64)denominator);

    if (depth < 0.0f)
        depth = 0.0f;
    else if (depth > 1.0f)
        depth = 1.0f;

    for (chunk* current = value->chunks; current; current = value->chunks) {
        f32 pen_x = current->x;

        for (u32 index = 0; index < current->length; ++index) {
            u8 character = (u8)*current->text++;

            string_glyph_rectangle rectangle;
            get_string_glyph_rectangle( value->font_data,
                                        character,
                                        current->scale_x,
                                        current->scale_y,
                                       &rectangle);

            f32 left   = (f32)((f64)rectangle.origin_x + (f64)pen_x);
            f32 right  = (f32)((f64)rectangle.width    + (f64)left);
            f32 top    = (f32)((f64)rectangle.origin_y + (f64)current->y);
            f32 bottom = (f32)((f64)rectangle.height   + (f64)top);

            f32 right_u  = (f32)((f64)rectangle.u - 0.001953125 + (f64)rectangle.width_u);
            f32 bottom_v = (f32)((f64)rectangle.v - 0.001953125 + (f64)rectangle.height_v);

            string_draw_vertex vertices[4] { { left,  top,    depth, current->color, rectangle.u, rectangle.v },
                                             { right, top,    depth, current->color, right_u,     rectangle.v },
                                             { right, bottom, depth, current->color, right_u,     bottom_v    },
                                             { left,  bottom, depth, current->color, rectangle.u, bottom_v    } };

            d3d9::binding_cache &bindings = d3d9::references::bindings.get();
            bindings.stream_source      = nullptr;
            bindings.vertex_declaration = nullptr;

            device->SetVertexDeclaration(d3d9::references::quad_position_color_uv_format.get().declaration);
            device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertices, sizeof(string_draw_vertex));

            i32 glyph_index = (i32)character - value->font_data->header.first_glyph;

            if (glyph_index < 0)
                glyph_index = 0;
            else if (glyph_index > value->font_data->header.glyph_count - 1)
                glyph_index = value->font_data->header.glyph_count - 1;

            u32 cell_width = (u32)(value->font_data->glyphs[glyph_index].cell_width - 1);
            pen_x = (f32)((f64)cell_width * (f64)current->scale_x + 1.0 + (f64)pen_x);
        }

        value->chunks = current->next;
    }
}
