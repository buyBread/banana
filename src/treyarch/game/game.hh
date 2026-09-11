#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/types.hh"
#include "treyarch/game/camera/camera.hh"
#include "treyarch/shared/boolx.hh"
#include "treyarch/shared/stringx.hh"
#include "treyarch/shared/dinkumware/vector.hh"
#include "treyarch/shared/math/types/vector3.hh"
#include "treyarch/shared/timing/hires_clock.hh"

namespace treyarch {
    struct level_descriptor; // impl?
    class game_data; // impl?
    class localized_string_table; // impl?
    class message_board; // impl?
    class wds_camera_manager; // impl?

    enum game_state_e : i32; // todo: see if SM3 .ii still holds up

    struct game_frame_timing {
        f32 total_delta;
        f32 flip_delta;
        f32 limit_delta;
    };

    struct game_process {
        const char*         name;
        const game_state_e* flow;
        i32                 index;
        i32                 num_states;
        f32                 timer;
        boolx               allow_override;
    };

    struct level_load_stuff {
        level_descriptor* descriptor;
        stringx           name;
        stringx           hero_name;
        vector3           hero_start_position;
        i32               loading_meter_val;
        u8                reserved_02c[0x04];
        hires_clock_t     level_clock;
        u8                load_widgets_created;
        u8                reserved_039[0x07];
        u8                load_complete_called;
        u8                load_this_level_finished;
        u8                reserved_042[0x06];
    };

    class game {

public:
        level_load_stuff                 level;
        u8                               disable_interface;
        u8                               disable_start_menu;
        u8                               i_quit;
        u8                               reserved_04b;
        u8                               level_is_loaded;
        u8                               reserved_04d[0x0B];
        u8                               game_paused;
        u8                               reserved_059;
        u8                               use_default_hero_start_position;
        u8                               level_is_unloading;
        u8                               reserved_05c[0x04];
        i32                              hero_freeze_depth;
        wds_camera_manager*              camera_manager;
        u8                               reserved_068[0x04];
        camera_handle                    base_camera;
        camera_handle                    current_view_camera;
        camera_handle                    current_game_camera;
        u8                               reserved_078[0x08];
        message_board*                   mb;
        f32                              level_time;
        dinkumware::vector<game_process> process_stack;
        localized_string_table*          string_localizer;
        vector3                          base_cam_position;
        u8                               reserved_0a8[0xF0];
        u32                              reserved_198;
        u32                              frame_sequence;
        game_frame_timing                frame_timing;
        u8                               reserved_1ac[0x04];
        hires_clock_t                    frame_clock;
        f32                              current_frame_delta;
        game_data*                       data;
        f32                              blur;
        u8                               reserved_1c4[0x0C];

        camera_handle get_current_view_camera();
        void          handle_frame_locking(f32* time_inc);
        void          clear_screen();
        void          render();
    };

    ASSERT_SIZEOF(game_frame_timing, 0x0C);

    ASSERT_SIZEOF  (game_process,                 0x18);
    ASSERT_OFFSETOF(game_process, name,           0x00);
    ASSERT_OFFSETOF(game_process, flow,           0x04);
    ASSERT_OFFSETOF(game_process, index,          0x08);
    ASSERT_OFFSETOF(game_process, num_states,     0x0C);
    ASSERT_OFFSETOF(game_process, timer,          0x10);
    ASSERT_OFFSETOF(game_process, allow_override, 0x14);

    ASSERT_SIZEOF  (level_load_stuff,                           0x48);
    ASSERT_OFFSETOF(level_load_stuff, descriptor,               0x00);
    ASSERT_OFFSETOF(level_load_stuff, name,                     0x04);
    ASSERT_OFFSETOF(level_load_stuff, hero_name,                0x10);
    ASSERT_OFFSETOF(level_load_stuff, hero_start_position,      0x1C);
    ASSERT_OFFSETOF(level_load_stuff, loading_meter_val,        0x28);
    ASSERT_OFFSETOF(level_load_stuff, level_clock,              0x30);
    ASSERT_OFFSETOF(level_load_stuff, load_widgets_created,     0x38);
    ASSERT_OFFSETOF(level_load_stuff, load_complete_called,     0x40);
    ASSERT_OFFSETOF(level_load_stuff, load_this_level_finished, 0x41);

    ASSERT_SIZEOF  (game,                                  0x1D0);
    ASSERT_OFFSETOF(game, level,                           0x000);
    ASSERT_OFFSETOF(game, disable_interface,               0x048);
    ASSERT_OFFSETOF(game, disable_start_menu,              0x049);
    ASSERT_OFFSETOF(game, i_quit,                          0x04A);
    ASSERT_OFFSETOF(game, level_is_loaded,                 0x04C);
    ASSERT_OFFSETOF(game, game_paused,                     0x058);
    ASSERT_OFFSETOF(game, use_default_hero_start_position, 0x05A);
    ASSERT_OFFSETOF(game, level_is_unloading,              0x05B);
    ASSERT_OFFSETOF(game, hero_freeze_depth,               0x060);
    ASSERT_OFFSETOF(game, camera_manager,                  0x064);
    ASSERT_OFFSETOF(game, base_camera,                     0x06C);
    ASSERT_OFFSETOF(game, current_view_camera,             0x070);
    ASSERT_OFFSETOF(game, current_game_camera,             0x074);
    ASSERT_OFFSETOF(game, mb,                              0x080);
    ASSERT_OFFSETOF(game, level_time,                      0x084);
    ASSERT_OFFSETOF(game, process_stack,                   0x088);
    ASSERT_OFFSETOF(game, string_localizer,                0x098);
    ASSERT_OFFSETOF(game, base_cam_position,               0x09C);
    ASSERT_OFFSETOF(game, frame_sequence,                  0x19C);
    ASSERT_OFFSETOF(game, frame_timing,                    0x1A0);
    ASSERT_OFFSETOF(game, frame_clock,                     0x1B0);
    ASSERT_OFFSETOF(game, current_frame_delta,             0x1B8);
    ASSERT_OFFSETOF(game, data,                            0x1BC);
    ASSERT_OFFSETOF(game, blur,                            0x1C0);
}
