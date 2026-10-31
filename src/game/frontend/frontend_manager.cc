#include <windows.h>

#include "retail.hh"
#include "game/app.hh"
#include "game/fixed_string.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/hash/algo.hh"
#include "treyarch/mm/mm.hh"
#include "treyarch/ngl/ngl.hh"
#include "treyarch/ngl/quad/quad.hh"
#include "treyarch/ngl/scene/scene.hh"

using namespace treyarch;

// sub_6D78C0
void frontend_manager::draw_igo() {
    if (aspect_ratio_test_mode) {
        RECT rectangle;

        GetWindowRect(ngl::references::render_window.read(), &rectangle);

        aspect_ratio = (f32)((f32)(rectangle.right  - rectangle.left) * 480.0 /
                             (f32)(rectangle.bottom - rectangle.top)  / 640.0);
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
            igo->draw_startup();
    }

    if (draw_cutscene_quad)
        ngl::list_add_quad(cutscene_quad);

    ngl::list_end_scene();

    igo->draw_quad_list();
    igo->clear_quad_list();
}

// sub_698990
void frontend_manager::set_cutscene_texture(const mash::string* name) {
    if (name->length() <= 0) {
        draw_cutscene_quad = false;

        return;
    }

    fixed_string texture_name;

    texture_name.hash = string_hash(hash::djb2(name->c_str()));
    texture_name.text = nullptr;

    ngl::set_quad_tex(cutscene_quad, (ngl::texture*)retail::sub_9E45A0(&texture_name.text)); // ngl texture lookup

    if (texture_name.text)
        mm::free(texture_name.text);
}

// sub_6B13F0
void frontend_manager::display_cutscene(f32 duration, f32 delay) {
    draw_cutscene_quad = true;

    cutscene_timer.start(0.0f, 1.0f, duration, delay);
}

// sub_688020
void frontend_manager::hide_cutscene() {
    draw_cutscene_quad = false;
}

// sub_698B10
void frontend_manager::send_hide_cutscene_quad_signal() {
    if (draw_cutscene_quad &&
        cutscene_timer.active &&
        cutscene_timer.duration > cutscene_timer.remaining)

        draw_cutscene_quad = false;
}
