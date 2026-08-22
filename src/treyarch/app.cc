#include "treyarch/app.hh"
#include "util/gimmie/fn.hh"
#include "banana/logging.hh"

namespace treyarch {
    /*
        todo: clean up all this garbage into something that isn't AIDS to read
              ...which involves at least half-implementing these methods properly...
              sigh
    */
    
    using clock_construct_fn = hires_clock_t*(__thiscall*)(      hires_clock_t*);
    using clock_reset_fn     = void          (__thiscall*)(      hires_clock_t*);
    using clock_elapsed_fn   = f32           (__thiscall*)(const hires_clock_t*);

    using no_arg_fn = void(__cdecl*)();
    using u8_u8_fn  = void(__cdecl*)(u8, u8);
    using u8_fn     = void(__cdecl*)(u8);
    using f32_fn    = void(__cdecl*)(f32);

    using object_tick_fn    = void(__thiscall*)(void*);
    using callback_tick_fn  = void(__thiscall*)(void*, f32);
    using game_f32_fn       = void(__thiscall*)(game*, f32);
    using game_f32ref_fn    = void(__thiscall*)(game*, f32*);
    using game_tick_fn      = void(__thiscall*)(game*);
    using cutscene_state_fn = bool(__thiscall*)(void*);

    namespace references {
        inline util::memory_reference<u8>    master_clock_is_up { 0x00FBF230 };
        inline util::memory_reference<f32>   minimum_frame_time { 0x00FC2F8C };
        inline util::memory_reference<void*> advertising        { 0x01086F24 };
        inline util::memory_reference<void*> cutscene_player    { 0x010886F4 };
        inline util::memory_reference<void*> input_manager      { 0x010FC63C };
    }
}

using namespace treyarch;

/*
    we don't really want to touch much of anything here beyond the render stuffs.
*/

void app::tick() {
    auto construct_clock = util::gimmie::fn<clock_construct_fn>(0x009C9DA0);
    auto reset_clock     = util::gimmie::fn<clock_reset_fn>    (0x009C8DD0);
    auto elapsed         = util::gimmie::fn<clock_elapsed_fn>  (0x009C8E40);

    references::master_clock_is_up.write(1);

    f32 maximum_frame_time = util::gimmie::fn<cutscene_state_fn>
        (0x00805020)(references::cutscene_player.read()) ? 0.5f : 0.05f;

    hires_clock_t total_timer;
    construct_clock(&total_timer);
    util::gimmie::fn<no_arg_fn>(0x00773AD0)();
    reset_clock(&total_timer);
    util::gimmie::fn<no_arg_fn>(0x00686350)();

    util::gimmie::fn<object_tick_fn>(0x009592D0)(references::input_manager.read());

    f32 time_inc = 0.0f; do {
        time_inc = elapsed(&real_clock);

        util::gimmie::fn<game_f32ref_fn>(0x0097AD00)(references::game.read(), &time_inc);

        if (time_inc > maximum_frame_time)
            time_inc = maximum_frame_time;
    } while (time_inc < references::minimum_frame_time.read());

    reset_clock(&real_clock);

    util::gimmie::fn<u8_u8_fn>        (0x00453700)(0, 0);
    util::gimmie::fn<no_arg_fn>       (0x00A173E0)();
    util::gimmie::fn<callback_tick_fn>(0x00734610)((void*)&references::callback_timers.get(), time_inc);
    util::gimmie::fn<f32_fn>          (0x007702F0)(time_inc);
    util::gimmie::fn<u8_fn>           (0x005F8870)(0);
    util::gimmie::fn<game_f32_fn>     (0x0097CC40)(the_game, time_inc);

    if (frames_to_skip) {
        --frames_to_skip;

        if (frames_to_skip < 0)
            frames_to_skip = 0;

        util::gimmie::fn<u8_u8_fn>(0x008FDF00)(0, 1);
        
        return;
    }

    util::gimmie::fn<game_tick_fn>(0x00783800)(the_game); // impl: game::render

    hires_clock_t flip_timer;
    construct_clock(&flip_timer);
    game_frame_timing &frame_timing = *(game_frame_timing*)((u8*)the_game + 0x01A0);
    frame_timing.flip_delta = elapsed(&flip_timer);

    util::gimmie::fn<no_arg_fn>(0x009DA500)(); // impl: nglPresent

    // we don't need to tick this, the ad client is mega dead
    /*
        void*  advertising        = references::advertising.read();
        void** advertising_vtable = *(void***)advertising;
        util::gimmie::fn<object_tick_fn>((u32)advertising_vtable[3])(advertising);
    */

    util::gimmie::fn<u8_u8_fn>(0x008FDF00)(0, 1);

    frame_timing.total_delta = elapsed(&total_timer);
    frame_timing.limit_delta = 0.0f;

    banana::log.dbg("tick");
}
