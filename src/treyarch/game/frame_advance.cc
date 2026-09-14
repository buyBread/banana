#include "retail.hh"
#include "treyarch/game/game.hh"
#include "util/memory_reference.hh"

namespace treyarch {
    struct frame_delta_history {
        f32 samples[15];
    };

    ASSERT_SIZEOF(frame_delta_history, 0x3C);

    namespace references {
        util::memory_reference<u8>                  region_spawns_enabled { 0x00BE73FE };
        util::memory_reference<frame_delta_history> delta_history         { 0x00F4D0E0 };
        util::memory_reference<void*>               raw_delta_consumer    { 0x010F9BEC };
        util::memory_reference<void*>               region_spawn_manager  { 0x010FA2C4 };
        util::memory_reference<i32>                 frame_delta_index     { 0x01111398 };
    } // references
} // treyarch

using namespace treyarch;

void game::frame_advance(f32 time_inc) {
    this->current_frame_delta = time_inc;

    u8* game_state_bytes = references::game_state.read();
    bool block_external_updates = retail::sub_97E1E0(game_state_bytes) &&
                                  *(void**)(game_state_bytes + 0x2CC)  &&
                                  !retail::sub_77C9E0(this->data);

    ++this->frame_sequence;

    i32 sample_index = references::frame_delta_index.read();
    references::delta_history.get().samples[sample_index] = time_inc;
    references::frame_delta_index.write((sample_index + 1) % 15);

    f32 averaged_delta = 0.0f;

    for (const f32 sample : references::delta_history.get().samples)
        averaged_delta = (f32)((f64)sample + (f64)averaged_delta);

    bool advance_region_spawns = references::region_spawns_enabled.read() != 0;

    averaged_delta = (f32)((f64)averaged_delta / 15.0);

    if (advance_region_spawns) {
        void* manager = references::region_spawn_manager.read();

        if (manager && this->level_is_loaded && !block_external_updates)
            retail::sub_956960(manager, averaged_delta);
    }

    retail::sub_97CAE0(this, averaged_delta);

    if (!block_external_updates)
        retail::sub_939B00(references::raw_delta_consumer.read(), time_inc);

    if (this->level_is_loaded)
        this->level_time = (f32)((f64)this->level_time + (f64)time_inc);
}
