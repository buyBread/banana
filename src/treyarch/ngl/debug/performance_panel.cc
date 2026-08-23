#include <cstdio>

#include "treyarch/ngl/debug/debug.hh"
#include "treyarch/ngl/debug/performance_display.hh"
#include "treyarch/ngl/display.hh"
#include "treyarch/ngl/font/font.hh"
#include "treyarch/ngl/font/text.hh"
#include "treyarch/ngl/list/arena.hh"
#include "treyarch/ngl/quad/quad.hh"
#include "treyarch/ngl/version.hh"

using namespace treyarch;

void ngl::debug::render_performance_info() {
    const debug_state      &state       = references::synchronized_debug.get();
    const performance_info &performance = references::synchronized_performance.get();

    char text[2048];

    if (state.show_performance_info == 1) {
        std::sprintf(text,
            //  802020FF    1.1
            "\1[D30A69FF]\2[2]NGL " NGL_VERSION "\2[1]\1[FFFFFFFF]\n"
            "%7.2f FPS\n"
            "%5.2fms CPU\n"
            "%5.2fms GPU\n"
            "%5.2fms SCENE BUILD\n"
            "%5.2fms SCENE SUBMIT\n"
            "% 7d POLYS\n"
            "% 7d NODES\n"
            "LIST % 6d/% 6d\n"
            "GEOM % 6d(% 6d)\n",
            performance.frames_per_second,
            performance.cpu_milliseconds,
            performance.render_milliseconds,
            performance.list_send_milliseconds,
            performance.list_submit_milliseconds,
            performance.total_polygons,
            performance.node_count,
            performance.list_work_bytes_used,
            list::references::arena.get().capacity,
            performance.scratch_mesh_bytes_used,
            performance.max_scratch_mesh_bytes_used);
    } else {
        std::sprintf(text,
            "%.2f FPS\n%.2fms\n",
            performance.frames_per_second,
            performance.render_milliseconds);
    }

    font* system_font = references::system_font.read();
    
    u32 text_width;
    u32 text_height;
    get_string_dimensions(system_font, text, &text_width, &text_height, 1.0f, 1.0f);

    quad background;
    init_quad(&background);

    f32 screen_width = (f32)get_screen_width();

    set_quad_rect(&background,
                  screen_width - (f32)text_width - 50.0f,
                  20.0f,
                  screen_width - 30.0f,
                  (f32)(text_height + 20));
    set_quad_color(&background, 0xC0000000);
    set_quad_z(&background, -9999.0f);
    list_add_quad(&background);
    list_add_string(system_font,
                    text,
                    screen_width - (f32)text_width - 40.0f,
                    30.0f,
                    -9999.0f,
                    0xFFFFFFFF,
                    1.0f,
                    1.0f);
}
