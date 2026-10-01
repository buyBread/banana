#include "retail.hh"
#include "treyarch/game/frontend/frontend_manager.hh"
#include "treyarch/game/game.hh"
#include "treyarch/game/input/input_mgr.hh"
#include "treyarch/game/mission/mission_manager.hh"
#include "treyarch/game/quest_manager.hh"
#include "util/memory_reference.hh"

namespace treyarch {
    struct frame_delta_history {
        f32 samples[15];
    };

    ASSERT_SIZEOF(frame_delta_history, 0x3C);

    namespace references {
        util::memory_reference<frame_delta_history> delta_history      { 0x00F4D0E0 };
        util::memory_reference<void*>               raw_delta_consumer { 0x010F9BEC };
        util::memory_reference<i32>                 frame_delta_index  { 0x01111398 };

        // static data only read by frame_advance_level: 1 and 0 respectively
        util::memory_reference<u8> unk_00b88707 { 0x00B88707 };
        util::memory_reference<u8> unk_00bcd0b9 { 0x00BCD0B9 };
    } // references
} // treyarch

using namespace treyarch;

// sub_97CC40
void game::frame_advance(f32 time_inc) {
    this->current_frame_delta = time_inc;

    mission_manager* missions = references::mission_manager.read();
    bool block_external_updates = missions->is_idle()                  &&
                                  missions->script_globals_initialized &&
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

    this->frame_advance_level(averaged_delta);

    if (!block_external_updates)
        retail::sub_939B00(references::raw_delta_consumer.read(), time_inc);

    if (this->level_is_loaded)
        this->level_time = (f32)((f64)this->level_time + (f64)time_inc);
}

// sub_97CAE0
void game::frame_advance_level(f32 time_inc) {
    input_mgr* input_manager = references::input_manager.read();

    references::cameras_handled.write(0);
    retail::sub_9CC680((u32*)input_manager);

    i32 state = 0;

    if (references::unk_00bcd0b9.read())
        state = 2;
    else if (retail::sub_967DF0())
        state = 1;

    retail::sub_95D570(state);

    if (this->get_current_view_camera())
        retail::sub_975970(0, (u32*)this->get_current_view_camera());

    retail::sub_863340(time_inc);

    if (references::unk_00b88707.read())
        retail::sub_9044D0(time_inc);

    retail::sub_8FDF30(time_inc);
    retail::sub_8FF040();
    retail::sub_959220((i32*)references::input_manager.read(), time_inc);

    if (time_inc != 0.0f)
        retail::sub_9C8EA0(time_inc);

    references::mission_manager.read()->frame_advance(time_inc);
    
    retail::sub_801790((i32)references::quest_manager.read(), time_inc);
    this->handle_game_states(&time_inc);

    if (!references::cameras_handled.read())
        retail::sub_97C060((i32)this, (i32)input_manager, &time_inc);

    retail::sub_7D1EB0();
}

// sub_97AF90
void game::frame_advance_game_overlays(f32 time_inc) {
    frontend_manager &frontend = references::frontend.get();

    frontend.igo->vtable->update(frontend.igo, time_inc);
    frontend.pause_menu_timer = (f32)((f64)frontend.pause_menu_timer - (f64)time_inc);

    retail::sub_698A30(&frontend, time_inc);
}
