#include "treyarch/game/frontend/igo/igo_color_channel.hh"
#include "treyarch/ngl/display.hh"
#include "treyarch/ngl/quad/quad.hh"

using namespace treyarch;

// sub_688550
u32 interpolate_igo_color(f32 interpolation, u32 first, u32 second) {
    u32 color = 0;

    for (u32 shift = 0; shift < 32; shift += 8) {
        i32 first_channel  = (first >> shift) & 0xFF;
        i32 second_channel = (second >> shift) & 0xFF;
        i32 delta = (i32)((f64)interpolation * (f64)(second_channel - first_channel));

        color |= (u32)(first_channel + delta) << shift;
    }

    return color;
}

// sub_6887D0
u32 igo_color_channel::sample_color(u32 color_index) {
    if (!track)
        return 0;

    igo_color_frame* first  = track->frames[frame_index];
    igo_color_frame* second = track->frames[frame_index + 1];

    if (!first)
        return 0;

    if (!second)
        second = first;

    return interpolate_igo_color(interpolation,
                                 first->colors[color_index],
                                 second->colors[color_index]);
}

// sub_688830
ngl::scene* igo_color_channel::draw_fullscreen_quad() {
    ngl::quad value;

    ngl::init_quad(&value);
    ngl::set_quad_rect(&value,
                       0.0f, 0.0f,
                       (f32)ngl::get_screen_width(),
                       (f32)ngl::get_screen_height());

    value.blend_mode = ((u64)5 << 32) | 0x06C10000;

    ngl::set_quad_color(&value, sample_color(0));

    return ngl::list_add_quad(&value);
}
