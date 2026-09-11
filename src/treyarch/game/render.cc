#include "retail.hh"
#include "treyarch/app/app.hh"
#include "treyarch/game/frontend/frontend_manager.hh"
#include "treyarch/game/game.hh"
#include "treyarch/game/movie_manager.hh"
#include "treyarch/ngl/frame_lock.hh"
#include "treyarch/ngl/scene/lifecycle.hh"
#include "treyarch/ngl/scene/references.hh"
#include "treyarch/ngl/scene/viewport.hh"
#include "util/memory_reference.hh"

namespace treyarch {
    // fancy "we know what these are" statement
    class zombie_manager;

    namespace references {
        util::memory_reference<u8>                     render_flag_00bcd0ba      { 0x00BCD0BA };
        util::memory_reference<u8>                     render_flag_00f4cd40      { 0x00F4CD40 };
        util::memory_reference<u8>                     movie_clears_screen       { 0x0102CDDA };
        util::memory_reference<frontend_manager>       frontend                  { 0x0102CFA8 };
        util::memory_reference<movie_manager*>         movies                    { 0x0102F2DC };
        util::memory_reference<zombie_manager*>        zombies                   { 0x0102FFF0 };
        util::memory_reference<ngl::scene*>            shadow_scene_0            { 0x01036E98 };
        util::memory_reference<ngl::scene*>            shadow_scene_1            { 0x01036E9C };
        util::memory_reference<void*>                  scene_callback_state      { 0x010FB390 };
    } // references

    namespace helpers {
        using namespace references;

        bool clear_uses_game_scene() {
            return !render_flag_00bcd0ba.read() && scene_callback_state.read();
        }

    } // helpers
} // treyarch

using namespace treyarch;

void game::render() {
    if (ngl::references::current_frame_lock.read() != ngl::frame_lock_two_or_immediate)
        ngl::set_frame_lock(ngl::frame_lock_two_or_immediate);

    if (!ngl::references::current_scene.get()->callbacks[0].function) {
        ngl::set_scene_callback(ngl::scene_callback_pre,
                                util::gimmie::fn<ngl::scene_callback_function>(0x007DF4C0));
        ngl::set_scene_callback(ngl::scene_callback_4,
                                util::gimmie::fn<ngl::scene_callback_function>(0x007D9470));
    }

    references::shadow_scene_0.write(nullptr);
    references::shadow_scene_1.write(nullptr);

    retail::sub_7BA6C0();

    movie_manager* cur_movie_manager = references::movies.read();

    if (cur_movie_manager->is_playing() && references::movie_clears_screen.read()) {
        retail::sub_6C7660((i32)cur_movie_manager); // render movie

        return;
    }

    if (!level_is_loaded) {
        references::frontend.get().draw_igo();

        return;
    }

    retail::sub_653750();

    if (this->get_current_view_camera())
        retail::sub_975970(2, (u32*)this->get_current_view_camera()); // publish camera view

    ngl::set_clear_flags(0);
    ngl::set_animation_time(0.0f);

    if (!references::frontend.get().igo->blocks_world_rendering() && references::render_flag_00f4cd40.read()) {
        retail::sub_970CB0();
        retail::sub_96F950();
    }

    retail::sub_641B00();

    ngl::scene* game_scene = ngl::list_begin_scene(ngl::scene_parameter_defaults);

    ngl::set_scene_name("game::render");

    retail::sub_96A220();

    ngl::set_scene_option_group_0(false, true, false);
    ngl::set_scene_option_group_1(false, true, false);
    
    ngl::references::current_scene.get()->local_state_430 = 1;

    if (helpers::clear_uses_game_scene()) {
        ngl::set_clear_flags(7);
        ngl::set_clear_color(0.0f, 0.0f, 0.0f, 0.0f);

        ngl::references::current_scene.get()->callback_state = references::scene_callback_state.read();
    } else {
        ngl::set_clear_flags(0);

        ngl::list_begin_scene(ngl::scene_parameter_defaults);

        ngl::set_scene_name("game::render (clear)");
        ngl::set_clear_flags(7);
        ngl::set_clear_color(0.0f, 0.0f, 0.0f, 0.0f);

        ngl::references::current_scene.get()->local_state_430 = 1;

        ngl::list_end_scene();
    }

    retail::sub_970CB0();

    if (ngl::is_viewport_override_enabled())
        ngl::apply_active_viewport();

    retail::sub_9772D0();

    if (!references::frontend.get().igo->blocks_world_rendering()) {
        vector3 camera_position = this->get_current_view_camera()->get_abs_position();

        // IDA typed it as a no-argument void method, but it has to take a vector3
        __asm {
            push camera_position.z
            push camera_position.y
            push camera_position.x
            
            call retail::sub_590D30

            add esp, 0Ch
        }

        retail::sub_7476C0();

        retail::sub_748BD0((u32*)references::zombies.read()); // render zombie symbiotes

        retail::sub_590040();
        retail::sub_7D77A0();
    }

    retail::sub_A6F7B0((i32)retail::sub_A6FBE0());

    ngl::list_end_scene();

    references::frontend.get().draw_igo();

    cur_movie_manager = references::movies.read();

    if (cur_movie_manager->is_playing())
        retail::sub_6C7660((i32)cur_movie_manager); // render movie

    ngl::list_select_scene(game_scene);

    retail::sub_9797D0();

    ngl::list_end_scene();
}
