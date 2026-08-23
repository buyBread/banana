#include <algorithm>

#include "treyarch/ngl/debug/debug.hh"
#include "treyarch/ngl/debug/performance_bar.hh"
#include "treyarch/ngl/debug/performance_display.hh"
#include "treyarch/ngl/display.hh"
#include "treyarch/ngl/quad/quad.hh"

using namespace treyarch;

void ngl::debug::render_performance_bar() {
    const performance_info &performance = references::performance.get();
    
    f32 screen_width  = (f32)get_screen_width();
    f32 screen_height = (f32)get_screen_height();

    f32 left = screen_width * 0.05f + 8.0f;
    f32 top  = screen_height * 0.05f + 8.0f;

    f32 usable_width   = screen_width - left * 2.0f;
    f32 interval_width = usable_width / (f32)references::performance_bar_vblank_count.read();

    f32 vblank_milliseconds = get_vblank_milliseconds();

    quad display_quad;
    init_quad(&display_quad);
    set_quad_z(&display_quad, -1.0e19f);

    set_quad_rect(&display_quad,
                  left - 3.0f,
                  top - 3.0f,
                  left + usable_width + 3.0f,
                  top + 13.0f);
    set_quad_color(&display_quad, 0xE0001020);
    list_add_quad(&display_quad);

    f32 render_width = std::min(performance.render_milliseconds / vblank_milliseconds * interval_width, usable_width);
    set_quad_rect(&display_quad,
                  left,
                  top,
                  left + render_width,
                  top + 5.0f);
    set_quad_color(&display_quad, 0xFFFFFF80);
    list_add_quad(&display_quad);

    f32 cpu_width = std::min(performance.cpu_milliseconds / vblank_milliseconds * interval_width, usable_width);
    set_quad_rect(&display_quad,
                  left,
                  top + 5.0f,
                  left + cpu_width,
                  top + 10.0f);
    set_quad_color(&display_quad, 0xFF8080FF);
    list_add_quad(&display_quad);

    set_quad_color(&display_quad, 0xFFFFFFFF);

    f32 interval_start = left;

    for (i32 index = 0; index < references::performance_bar_vblank_count.read(); ++index) {
        set_quad_rect(&display_quad,
                      interval_start,
                      top,
                      interval_start + 1.25f,
                      top + 10.0f);
        list_add_quad(&display_quad);

        f32 half = interval_start + interval_width * 0.5f;
        set_quad_rect(&display_quad,
                      half,
                      top,
                      half + 1.25f,
                      top + 5.0f);
        list_add_quad(&display_quad);

        f32 quarter = interval_start + interval_width * 0.25f;
        set_quad_rect(&display_quad,
                      quarter,
                      top,
                      quarter + 1.25f,
                      top + 2.5f);
        list_add_quad(&display_quad);

        f32 three_quarters = interval_start + interval_width * 0.75f;
        set_quad_rect(&display_quad,
                      three_quarters,
                      top,
                      three_quarters + 1.25f,
                      top + 2.5f);
        list_add_quad(&display_quad);

        interval_start += interval_width;
    }

    set_quad_rect(&display_quad,
                  left + usable_width,
                  top,
                  left + usable_width + 1.25f,
                  top + 10.0f);
    list_add_quad(&display_quad);
}
