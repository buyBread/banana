#include "treyarch/app.hh"
#include "treyarch/game.hh"
#include "treyarch/ngl/scene/lifecycle.hh"
#include "treyarch/ngl/scene/references.hh"
#include "util/gimmie/fn.hh"
#include "util/memory_reference.hh"

namespace treyarch {
    class FEManager;
    class camera;
    class movie_manager;
    class world_dynamics_system;
    class zombie_manager;

    namespace game_render {
        using no_arg_fn             = void   (__cdecl*)();
        using u32_fn                = void   (__cdecl*)(u32);
        using untyped_method_fn     = void   (__thiscall*)(void*);
        using untyped_getter_fn     = void*  (__cdecl*)();
        using front_end_draw_fn     = void   (__thiscall*)(FEManager*);
        using movie_render_fn       = void   (__thiscall*)(movie_manager*);
        using zombie_render_fn      = void   (__thiscall*)(zombie_manager*);
        using camera_getter_fn      = camera*(__thiscall*)(game*);
        using camera_publish_fn     = void   (__cdecl*)(i32, camera*);
        using camera_position_fn    = void   (__cdecl*)(ngl::vector3);

        static util::memory_reference<u8>                     render_flag_00bcd0ba      { 0x00BCD0BA };
        static util::memory_reference<u32>                    ngl_mode                  { 0x00F528D8 };
        static util::memory_reference<u8>                     render_flag_00f4cd40      { 0x00F4CD40 };
        static util::memory_reference<u8>                     movie_clears_screen       { 0x0102CDDA };
        static util::memory_reference<FEManager>              g_femanager               { 0x0102CFA8 };
        static util::memory_reference<world_dynamics_system*> world                     { 0x0102CFFC };
        static util::memory_reference<movie_manager*>         movies                    { 0x0102F2DC };
        static util::memory_reference<zombie_manager*>        zombies                   { 0x0102FFF0 };
        static util::memory_reference<ngl::scene*>            shadow_scene_0            { 0x01036E98 };
        static util::memory_reference<ngl::scene*>            shadow_scene_1            { 0x01036E9C };
        static util::memory_reference<void*>                  scene_callback_state      { 0x010FB390 };
        static util::memory_reference<u8>                     viewport_override_enabled { 0x01115C98 };

        static bool movie_is_playing(movie_manager* value) {
            return *(i32*)((u8*)value + 0x04) == 1;
        }

        static bool clear_uses_game_scene() {
            return !render_flag_00bcd0ba.read() && scene_callback_state.read();
        }

        static bool world_rendering_is_blocked() {
            u8* render_state = *(u8**)((u8*)world.read() + 0x12C);

            return render_state[0x0D] || render_state[0x8F90];
        }

        static ngl::vector3 get_camera_position(camera* value) {
            u8* transform = *(u8**)((u8*)value + 0x10);

            return *(ngl::vector3*)(transform + 0x30);
        }
    } // game_render
} // treyarch

using namespace treyarch;

void game::render() {
    using namespace game_render;

    if (ngl_mode.read() != 4)
        util::gimmie::fn<u32_fn>(0x009DC980)(4);

    if (!ngl::references::current_scene.get()->callbacks[0].function) {
        ngl::set_scene_callback(ngl::scene_callback_pre,
                                util::gimmie::fn<ngl::scene_callback_function>(0x007DF4C0));

        ngl::set_scene_callback(ngl::scene_callback_4,
                                util::gimmie::fn<ngl::scene_callback_function>(0x007D9470));
    }

    shadow_scene_0.write(nullptr);
    shadow_scene_1.write(nullptr);

    util::gimmie::fn<no_arg_fn>(0x007BA6C0)();

    movie_manager* current_movie_manager = movies.read();

    if (movie_is_playing(current_movie_manager) && movie_clears_screen.read()) {
        util::gimmie::fn<movie_render_fn>(0x006C7660)(current_movie_manager);

        return;
    }

    if (!level_is_loaded) {
        util::gimmie::fn<front_end_draw_fn>(0x006D78C0)(&g_femanager.get());

        return;
    }

    util::gimmie::fn<no_arg_fn>(0x00653750)();

    auto get_current_view_camera = util::gimmie::fn<camera_getter_fn>(0x0097ACA0);

    if (get_current_view_camera(this))
        util::gimmie::fn<camera_publish_fn>(0x00975970)(2, get_current_view_camera(this));

    ngl::set_clear_flags(0);
    ngl::set_animation_time(0.0f);

    if (!world_rendering_is_blocked() && render_flag_00f4cd40.read()) {
        util::gimmie::fn<no_arg_fn>(0x00970CB0)();
        util::gimmie::fn<no_arg_fn>(0x0096F950)();
    }

    util::gimmie::fn<no_arg_fn>(0x00641B00)();

    ngl::scene* game_scene = ngl::list_begin_scene(ngl::scene_parameter_defaults);

    ngl::set_scene_name("game::render");
    util::gimmie::fn<no_arg_fn>(0x0096A220)();

    ngl::set_scene_option_group_0(false, true, false);
    ngl::set_scene_option_group_1(false, true, false);
    
    ngl::references::current_scene.get()->local_state_430 = 1;

    if (clear_uses_game_scene()) {
        ngl::set_clear_flags(7);
        ngl::set_clear_color(0.0f, 0.0f, 0.0f, 0.0f);

        ngl::references::current_scene.get()->callback_state = scene_callback_state.read();
    } else {
        ngl::set_clear_flags(0);

        ngl::list_begin_scene(ngl::scene_parameter_defaults);

        ngl::set_scene_name("game::render (clear)");
        ngl::set_clear_flags(7);
        ngl::set_clear_color(0.0f, 0.0f, 0.0f, 0.0f);

        ngl::references::current_scene.get()->local_state_430 = 1;

        ngl::list_end_scene();
    }

    util::gimmie::fn<no_arg_fn>(0x00970CB0)();

    if (viewport_override_enabled.read())
        util::gimmie::fn<no_arg_fn>(0x009D8AF0)();

    util::gimmie::fn<no_arg_fn>(0x009772D0)();

    if (!world_rendering_is_blocked()) {
        camera* resolved_camera = get_current_view_camera(references::game.read());

        util::gimmie::fn<camera_position_fn>(0x00590D30)(get_camera_position(resolved_camera));

        util::gimmie::fn<no_arg_fn>(0x007476C0)();

        zombie_manager* zombie_system = zombies.read();

        if (zombie_system)
            util::gimmie::fn<zombie_render_fn>(0x00748BD0)(zombie_system);

        util::gimmie::fn<no_arg_fn>(0x00590040)();
        util::gimmie::fn<no_arg_fn>(0x007D77A0)();
    }

    void* ise_effect_manager = util::gimmie::fn<untyped_getter_fn>(0x00A6FBE0)();

    util::gimmie::fn<untyped_method_fn>(0x00A6F7B0)(ise_effect_manager);

    ngl::list_end_scene();

    util::gimmie::fn<front_end_draw_fn>(0x006D78C0)(&g_femanager.get());

    current_movie_manager = movies.read();

    if (movie_is_playing(current_movie_manager))
        util::gimmie::fn<movie_render_fn>(0x006C7660)(current_movie_manager);

    ngl::list_select_scene(game_scene);

    util::gimmie::fn<no_arg_fn>(0x009797D0)();

    ngl::list_end_scene();
}
