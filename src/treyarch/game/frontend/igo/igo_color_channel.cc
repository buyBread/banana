#include "treyarch/game/frontend/igo/igo_color_channel.hh"
#include "treyarch/game/post_process/post_process.hh"
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

// sub_688AC0; continuing at loc_6889E0
void treyarch::draw_background_channels() {
    i32 select = references::unk_0102cdb8.read() > 0;

    if (!select)
        return;

    igo_color_channel_set* set = (&references::background_channel_sets.get())[select];

    if (!set)
        return;

    for (igo_color_channel** entry = set->channels; *entry; ++entry) {
        igo_color_channel* channel = *entry;
        igo_color_track*   track   = channel->track;

        switch (track->type) {
            case 0:
                channel->draw_fullscreen_quad();
                break;

            case 1: {
                igo_color_frame* first  = track->frames[channel->frame_index];
                igo_color_frame* second = track->frames[channel->frame_index + 1];

                f32 strength = 0.0f;

                if (first) {
                    if (!second)
                        second = first;

                    f32 from = *(const f32*)first->colors;
                    f32 to   = *(const f32*)second->colors;

                    strength = (f32)(((f64)to - (f64)from) * (f64)channel->interpolation + (f64)from);
                }

                post_process::render_pause_menu_blur(strength);
                break;
            }

            default:
                break;
        }
    }
}
