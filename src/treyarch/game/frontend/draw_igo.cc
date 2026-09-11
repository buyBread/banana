#include <windows.h>

#include "treyarch/app/app.hh"
#include "treyarch/game/frontend/frontend_manager.hh"
#include "treyarch/ngl/ngl.hh"
#include "treyarch/ngl/scene/lifecycle.hh"

using namespace treyarch;

void frontend_manager::draw_igo() {
    if (aspect_ratio_test_mode) {
        RECT rectangle;

        GetWindowRect(ngl::references::render_window.read(), &rectangle);

        aspect_ratio = (f32)((f32)(rectangle.right - rectangle.left) * 480.0 /
                             (f32)(rectangle.bottom - rectangle.top) / 640.0);
    }

    igo->clear_quad_list();

    ngl::list_begin_scene(ngl::scene_parameter_defaults);

    ngl::set_scene_name("FEManager::DrawIGO");
    ngl::set_clear_flags(0);
    ngl::set_z_test_enable(false);
    ngl::set_z_write_enable(false);

    if (references::game.read()->level_is_loaded)
        igo->draw();
    else {
        ngl::list_begin_scene(ngl::scene_parameter_defaults);

        ngl::set_scene_name("FEManager::DrawIGO");
        ngl::set_clear_flags(7);
        ngl::set_clear_color(0.0f, 0.0f, 0.0f, 0.0f);

        ngl::list_end_scene();

        if (fonts_loaded)
            igo->draw_startup(); // main menu
    }

    if (draw_cutscene_quad)
        ngl::list_add_quad(cutscene_quad);

    ngl::list_end_scene();

    igo->draw_quad_list();
    igo->clear_quad_list();
}
