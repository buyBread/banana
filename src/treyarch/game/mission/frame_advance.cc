#include "retail.hh"
#include "treyarch/app/app.hh"
#include "treyarch/game/game.hh"
#include "treyarch/game/mission/mission_manager.hh"

using namespace treyarch;

// sub_986520
void mission_manager::frame_advance(f32 time_inc) {
    retail::sub_9806F0((i32)this);

    switch (sky_swap_state) {
        case 0:
            retail::sub_980880((i32)this);

            break;

        case 1:
            if (retail::sub_902810())
                sky_swap_state = 2;

            break;

        case 2:
            retail::sub_97E610((i32)this);

            break;

        case 3:
            retail::sub_980950((u32*)this);

            break;
    }

    game* the_game = references::game.read();

    if (the_game->game_paused || !game_time_var || the_game->get_cur_state() != 9) {
        frame_clock.reset();

        return;
    }

    real_time_timer = (f32)((f64)real_time_timer + (f64)time_inc);

    bool real_time_ticked = false;

    if (real_time_timer >= 1.0f) {
        real_time_ticked = true;

        ++real_time;
        *real_time_var = (f32)real_time;

        real_time_timer = (f32)((f64)real_time_timer - 1.0);
    }

    frame_advance_game_time(time_inc);

    if ((flags & mission_manager_flag_unk_10000000) && !retail::sub_77C9E0(references::game.read()->data)) {
        retail::sub_79CAB0((u32*)references::game.read()->data, (i32)this);

        flags &= ~mission_manager_flag_unk_10000000;
    }

    if (real_time_ticked) {
        retail::sub_981D50((i32*)this);
        retail::sub_97F7B0((i32)this);
        retail::sub_97F550((i32)this);
    }

    switch (state) {
        case mission_manager_state_initial_startup:
            retail::sub_9847F0((i32)this, time_inc);

            break;

        case mission_manager_state_idle:
            retail::sub_9860E0((i32)this, time_inc);

            break;

        case mission_manager_state_start_loading:
            retail::sub_984960((i32)this);

            break;

        case mission_manager_state_loading:
            retail::sub_984C90((i32*)this);

            break;

        case mission_manager_state_running_mission:
            retail::sub_97EF50((u32*)this);

            break;

        case mission_manager_state_unk_5:
            retail::sub_980C10((i32)this);

            break;

        case mission_manager_state_unk_6:
            retail::sub_984EE0((i32)this);

            break;

        case mission_manager_state_start_unloading:
            retail::sub_985000((i32)this);

            break;

        case mission_manager_state_unloading:
            retail::sub_9853B0((i32)this);

            break;

        case mission_manager_state_maloring_player_wait_for_blackscreen:
            retail::sub_97F060((i32)this, time_inc);

            break;

        case mission_manager_state_maloring_player_wait_for_district:
            retail::sub_980D40((i32)this);

            break;

        case mission_manager_state_start_mission_failed_dialog:
            retail::sub_980FE0((i32)this);

            break;

        case mission_manager_state_running_mission_failed_dialog:
            retail::sub_9864B0((i32)this);

            break;

        case mission_manager_state_start_mission_succeeded_dialog:
            state = mission_manager_state_running_mission_succeeded_dialog;

            break;

        case mission_manager_state_running_mission_succeeded_dialog:
            retail::sub_97F2B0((i32)this);

            break;

        case mission_manager_state_blackscreen_on:
            retail::sub_981110((u32*)this);

            break;

        case mission_manager_state_wait_for_blackscreen_on:
            process_state_wait_for_blackscreen_on(time_inc);

            break;
    }

    frame_clock.reset();
}

// sub_97F2F0
void mission_manager::process_state_wait_for_blackscreen_on(f32 time_inc) {
    state_timer = (f32)((f64)time_inc + (f64)state_timer);

    if (state_timer > 1.5f) {
        state_timer = 0.0f;
        state       = mission_manager_state_start_loading;
    }
}
