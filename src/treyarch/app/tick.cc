#include "retail.hh"
#include "treyarch/amalga/resource_manager.hh"
#include "treyarch/app/app.hh"
#include "treyarch/game/cutscene/cutscene_player.hh"
#include "treyarch/game/event/event_manager.hh"
#include "treyarch/game/input/input_mgr.hh"
#include "treyarch/nfl/nfl.hh"
#include "treyarch/ngl/ngl.hh"

namespace treyarch {    
    namespace references {
        util::memory_reference<f32> minimum_frame_time { 0x00FC2F8C };
    } // references
} // treyarch

using namespace treyarch;

// sub_429A90
void app::tick() {
    references::master_clock_is_up.write(1);

    f32 maximum_frame_time = references::cutscene_player.read()->is_playing() ? 0.5f : 0.05f;

    hires_clock_t total_timer;
    retail::sub_773AD0();
    total_timer.reset();
    
    event_manager::garbage_collect();

    input_mgr::inst()->poll_devices();

    f32 time_inc = 0.0f; do {
        time_inc = this->real_clock.elapsed();

        references::game.read()->handle_frame_locking(&time_inc);

        if (time_inc > maximum_frame_time)
            time_inc = maximum_frame_time;
    } while (time_inc < references::minimum_frame_time.read());

    this->real_clock.reset();
    retail::sub_453700(0, 0);
    nfl::update();
    retail::sub_734610((f32*)&references::callback_timers.get(), time_inc);
    amalga::resource_manager::frame_advance(time_inc);
    retail::sub_5F8870(0);
    this->the_game->frame_advance(time_inc);

    if (frames_to_skip) {
        --frames_to_skip;

        if (frames_to_skip < 0)
            frames_to_skip = 0;

        retail::sub_8FDF00(0, 1);
        
        return;
    }

    the_game->render();

    hires_clock_t flip_timer;

    the_game->frame_timing.flip_delta = flip_timer.elapsed();

    ngl::present();

    // dead ad client

    retail::sub_8FDF00(0, 1);

    the_game->frame_timing.total_delta = total_timer.elapsed();
    the_game->frame_timing.limit_delta = 0.0f;
}
