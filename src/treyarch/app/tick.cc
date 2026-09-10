#include "retail.hh"
#include "treyarch/app/app.hh"
#include "treyarch/ngl/ngl.hh"
#include "util/gimmie/fn.hh"

namespace treyarch {    
    namespace references {
        util::memory_reference<u8>    master_clock_is_up { 0x00FBF230 };
        util::memory_reference<f32>   minimum_frame_time { 0x00FC2F8C };
        util::memory_reference<void*> cutscene_player    { 0x010886F4 };
        util::memory_reference<void*> input_manager      { 0x010FC63C };
    } // references
} // treyarch

using namespace treyarch;

void app::tick() {
    references::master_clock_is_up.write(1);

    f32 maximum_frame_time = retail::sub_805020((u32*)references::cutscene_player.read()) ? 0.5f : 0.05f;

    hires_clock_t total_timer;
    retail::sub_9C9DA0((u64*)&total_timer); // construct clock
    retail::sub_773AD0();
    retail::sub_9C8DD0((i64*)&total_timer); // reset clock
    retail::sub_686350();

    retail::sub_9592D0((u32*)references::input_manager.read()); // tick input manager

    f32 time_inc = 0.0f; do {
        time_inc = retail::sub_9C8E40((u64*)&this->real_clock); // check clock elapsed time

        retail::sub_97AD00((i32)&time_inc);

        if (time_inc > maximum_frame_time)
            time_inc = maximum_frame_time;
    } while (time_inc < references::minimum_frame_time.read());

    retail::sub_9C8DD0((i64*)&this->real_clock); // reset clock
    retail::sub_453700(0, 0);
    retail::sub_A173E0();
    retail::sub_734610((f32*)&references::callback_timers.get(), time_inc);
    retail::sub_7702F0(time_inc);
    retail::sub_5F8870(0);
    retail::sub_97CC40((i32)this->the_game, std::bit_cast<i32>(time_inc)); // IDA typed this wrong

    if (frames_to_skip) {
        --frames_to_skip;

        if (frames_to_skip < 0)
            frames_to_skip = 0;

        retail::sub_8FDF00(0, 1);
        
        return;
    }

    the_game->render();

    hires_clock_t flip_timer;
    retail::sub_9C9DA0((u64*)&flip_timer); // construct clock

    the_game->frame_timing.flip_delta = retail::sub_9C8E40((u64*)&flip_timer); // check clock elapsed time

    ngl::present();

    /*
        mega dead ad client here;
        no reason to include that dumb vtable thingamajig calling.
    */

    retail::sub_8FDF00(0, 1);

    the_game->frame_timing.flip_delta = retail::sub_9C8E40((u64*)&total_timer); // check clock elapsed time
    the_game->frame_timing.limit_delta = 0.0f;
}
