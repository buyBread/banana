#include "treyarch/game/event/event_manager.hh"
#include "treyarch/game/mission/mission_manager.hh"

namespace treyarch {
    namespace references {
        util::memory_reference<string_hash> time_minute_inc { 0x0102CC58 };
        util::memory_reference<string_hash> time_hour_inc   { 0x0102C534 };
        util::memory_reference<string_hash> time_day_inc    { 0x0102C3B0 };
    } // references
} // treyarch

using namespace treyarch;

// sub_97E730
i32 mission_manager::game_time_get_military_time_hour() const {
    return (i32)(game_time / 60) / 60 % 24;
}

// sub_97E780
u32 mission_manager::game_time_get_seconds_since_midnight() const {
    return game_time % 86400;
}

// sub_97E7A0
f32 mission_manager::game_time_get_full_time() const {
    return (f32)game_time + game_time_timer;
}

// sub_97E7D0
bool mission_manager::is_daytime() const {
    return (u32)((i32)(game_time / 60) / 60 % 24 - 6) <= 12;
}

// sub_97F340
void mission_manager::frame_advance_game_time(f32 time_inc) {
    // retail ignores the pushed delta and times itself from frame_clock
    game_time_timer = frame_clock.elapsed() * (f32)game_time_rate + game_time_timer;

    if (!(game_time_timer >= 1.0f))
        return;

    do {
        if (!(game_time % 60)) {
            event_manager::raise_event(references::time_minute_inc.read(), arch_base_vhandle());

            if (!(game_time / 60 % 60))
                event_manager::raise_event(references::time_hour_inc.read(), arch_base_vhandle());
        }

        if (game_time > 86400) {
            game_time -= 86400;

            *game_days_var = (f32)((f64)*game_days_var + 1.0);
            event_manager::raise_event(references::time_day_inc.read(), arch_base_vhandle());

            *game_day_of_the_week_var = (f32)((f64)*game_day_of_the_week_var + 1.0);

            if (*game_day_of_the_week_var > 6.0f)
                *game_day_of_the_week_var = 0.0f;
        }

        *game_time_var = (f32)game_time;

        game_time_timer = (f32)((f64)game_time_timer - 1.0);
    } while (game_time_timer >= 1.0f);
}
