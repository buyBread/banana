#include "treyarch/ngl/font/font.hh"
#include "treyarch/ngl/ngl.hh"
#include "treyarch/shared/four_cc.hh"

using namespace treyarch;

void __cdecl ngl::load_font(amalga::file*       owner,
                            amalga::file_entry* entry,
                            void**              mapped_sections,
                            void*               user_data) {

    (void)entry;
    (void)user_data;

    i32 image_section = owner->find_section_index(string_hash(four_cc('I', 'M', 'G')));

    font* value = (font*)mapped_sections[image_section];

    value->system     = 1;
    value->map_flags  = 0x06C10000;
    value->blend_mode = 5;

    f32 inverse_width  = (f32)(1.0 / (f64)value->texture_data->gpu_texture.width);
    f32 inverse_height = (f32)(1.0 / (f64)value->texture_data->gpu_texture.height);

    for (i32 glyph = 0; glyph < value->header.glyph_count; ++glyph) {
        f32* coordinates = value->texture_coordinates + glyph * 4;

        coordinates[0] *= inverse_width;
        coordinates[1] *= inverse_height;
        coordinates[2] *= inverse_width;
        coordinates[3] *= inverse_height;
    }

    if (!references::resource_callback.read())
        references::fonts.get().insert(value);
}

void __cdecl ngl::remove_font(amalga::file*       owner,
                              amalga::file_entry* entry,
                              void**              mapped_sections,
                              void*               user_data) {

    (void)entry;
    (void)user_data;

    i32 image_section = owner->find_section_index
        (string_hash(four_cc('I', 'M', 'G')));

    font* value = (font*)mapped_sections[image_section];

    if (!references::resource_callback.read())
        references::fonts.get().erase(value);
}
