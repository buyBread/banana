#include "retail.hh"
#include "treyarch/app/app.hh"
#include "treyarch/game/game.hh"
#include "treyarch/ngl/scene/lifecycle.hh"
#include "treyarch/ngl/scene/references.hh"
#include "util/memory_reference.hh"

namespace treyarch {
    // fancy "we know what these are" statement
    class FEManager;
    class camera; // impl
    class movie_manager;
    class world_dynamics_system;
    class zombie_manager;

    namespace references {
        util::memory_reference<u8>                     render_flag_00bcd0ba      { 0x00BCD0BA };
        util::memory_reference<u32>                    ngl_mode                  { 0x00F528D8 };
        util::memory_reference<u8>                     render_flag_00f4cd40      { 0x00F4CD40 };
        util::memory_reference<u8>                     movie_clears_screen       { 0x0102CDDA };
        util::memory_reference<FEManager>              g_femanager               { 0x0102CFA8 };
        util::memory_reference<world_dynamics_system*> world                     { 0x0102CFFC };
        util::memory_reference<movie_manager*>         movies                    { 0x0102F2DC };
        util::memory_reference<zombie_manager*>        zombies                   { 0x0102FFF0 };
        util::memory_reference<ngl::scene*>            shadow_scene_0            { 0x01036E98 };
        util::memory_reference<ngl::scene*>            shadow_scene_1            { 0x01036E9C };
        util::memory_reference<void*>                  scene_callback_state      { 0x010FB390 };
        util::memory_reference<u8>                     viewport_override_enabled { 0x01115C98 };
    } // references

    namespace helpers {
        using namespace references;

        bool movie_is_playing(movie_manager* value) {
            return *(i32*)((u8*)value + 0x04) == 1;
        }

        bool clear_uses_game_scene() {
            return !render_flag_00bcd0ba.read() && scene_callback_state.read();
        }

        bool world_rendering_is_blocked() {
            u8* render_state = *(u8**)((u8*)world.read() + 0x12C);

            return render_state[0x0D] || render_state[0x8F90];
        }

        ngl::vector3 get_camera_position(u32* value) {
            u8* transform = *(u8**)((u8*)value + 0x10);

            return *(ngl::vector3*)(transform + 0x30);
        }
    } // helpers
} // treyarch

using namespace treyarch;

void game::render() {
    if (references::ngl_mode.read() != 4)
        retail::sub_9DC980(4);

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

    if (helpers::movie_is_playing(cur_movie_manager) && references::movie_clears_screen.read()) {
        retail::sub_6C7660((i32)cur_movie_manager); // render movie

        return;
    }

    if (!level_is_loaded) {
        retail::sub_6D78C0((i32)&references::g_femanager.get());

        return;
    }

    retail::sub_653750();

    if (retail::sub_97ACA0((u32*)this))
        retail::sub_975970(2, (u32*)retail::sub_97ACA0((u32*)this)); // publish camera view

    ngl::set_clear_flags(0);
    ngl::set_animation_time(0.0f);

    if (!helpers::world_rendering_is_blocked() && references::render_flag_00f4cd40.read()) {
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

    if (references::viewport_override_enabled.read())
        retail::sub_9D8AF0();

    retail::sub_9772D0();

    if (!helpers::world_rendering_is_blocked()) {
        ngl::vector3 camera_position = helpers::get_camera_position((u32*)retail::sub_97ACA0((u32*)references::game.read()));

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

    retail::sub_6D78C0((i32)&references::g_femanager.get());

    cur_movie_manager = references::movies.read();

    if (helpers::movie_is_playing(cur_movie_manager))
        retail::sub_6C7660((i32)cur_movie_manager); // render movie

    ngl::list_select_scene(game_scene);

    retail::sub_9797D0();

    ngl::list_end_scene();
}
