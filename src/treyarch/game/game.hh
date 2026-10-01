#pragma once

#include "util/types.hh"
#include "util/macros/sanity_assert.hh"
#include "treyarch/game/wds/camera/camera.hh"
#include "treyarch/shared/boolx.hh"
#include "treyarch/shared/mash/string.hh"
#include "treyarch/shared/dinkumware/vector.hh"
#include "treyarch/shared/math/types/vector3.hh"
#include "treyarch/shared/timing/hires_clock.hh"
#include "util/memory_reference.hh"

namespace treyarch {
    struct level_descriptor; // impl?
    class game_data; // impl?
    class localized_string_table; // impl?
    class message_board; // impl?
    class world_dynamics_system;

    enum game_state_e : i32; // todo: see if SM3 .ii still holds up

    namespace references {
        // set by game::handle_cameras on entry; cleared once per frame_advance_level
        inline util::memory_reference<u8> cameras_handled { 0x01111392 };

        inline util::memory_reference<u8>    region_spawns_enabled { 0x00BE73FE };
        inline util::memory_reference<void*> region_spawn_manager  { 0x010FA2C4 };
    } // references

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

        game_state_e get_cur_state() const {
            return flow[index];
        }

        void go_next_state() {
            ++index;
        }
    };

    namespace references {
        inline util::memory_reference<game_process> start_process { 0x00E77250 };
        inline util::memory_reference<game_process> main_process  { 0x00E77268 };

        // static string initialized to "megacity"
        inline util::memory_reference<mash::string> default_level_name { 0x01111434 };

        inline util::memory_reference<char> unk_01110e90      { 0x01110E90 };
        inline util::memory_reference<char> level_name_buffer { 0x01110F90 };
    } // references

    struct level_load_stuff {
        level_descriptor* descriptor;
        mash::string      name;
        mash::string      hero_name;
        vector3           hero_start_position;
        i32               loading_meter_val;
        u8                reserved_02c[0x04];
        hires_clock_t     level_clock;
        u8                load_widgets_created;
        u8                reserved_039[0x07];
        u8                load_complete_called;
        u8                load_this_level_finished;
        u8                reserved_042[0x06];

        level_load_stuff();
       ~level_load_stuff() = default;

        void reset_level_load_data();
    };

    class game {

    public:
        level_load_stuff                 level;
        u8                               disable_interface;
        u8                               disable_start_menu;
        u8                               i_quit;
        u8                               load_new_level;
        u8                               level_is_loaded;
        u8                               unk_04d;
        u8                               debug_single_step;
        u8                               debug_stop_physics;
        u32                              unk_050;
        f32                              unk_054;
        u8                               game_paused;
        u8                               unk_059; // set by sub_97CA40 on top of game_paused; scripts keep running while set
        u8                               use_default_hero_start_position;
        u8                               level_is_unloading;
        u8                               wait_for_intro_scene_anim;
        u8                               reserved_05d[0x03];
        i32                              hero_freeze_depth;
        world_dynamics_system*           the_world;
        u8                               reserved_068[0x04];
        camera_handle                    base_camera;
        camera_handle                    current_view_camera;
        camera_handle                    current_game_camera;
        u8                               unk_078;
        u8                               reserved_079[0x03];
        u32                              unk_07c;
        message_board*                   mb;
        f32                              level_time;
        dinkumware::vector<game_process> process_stack;
        localized_string_table*          string_localizer;
        vector3                          base_cam_position;
        u8                               reserved_0a8[0xF0];
        u32                              unk_198;
        u32                              frame_sequence;
        game_frame_timing                frame_timing;
        u8                               unk_1ac;
        u8                               reserved_1ad[0x03];
        hires_clock_t                    frame_clock;
        f32                              current_frame_delta;
        game_data*                       data;
        f32                              blur;
        u8                               reserved_1c4[0x08];
        u8                               unk_1cc;
        u8                               unk_1cd;
        u8                               unk_1ce;
        u8                               reserved_1cf;

        game();

        game_state_e get_cur_state();

        void push_process(const game_process &process);
        void pop_process();

        void handle_game_states(f32* time_inc);
        void advance_state_running(f32 time_inc);
        void advance_state_paused(f32 time_inc);
        void soft_reset_process();
        void frame_advance_game_overlays(f32 time_inc);

        camera_handle get_current_view_camera();
        
        void handle_frame_locking(f32* time_inc);

        void frame_advance(f32 time_inc);
        void frame_advance_level(f32 time_inc);

        void clear_screen();
        void render();

        static void release_device_resources();
        static void restore_device_resources();
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
    ASSERT_OFFSETOF(game, unk_050,                         0x050);
    ASSERT_OFFSETOF(game, unk_054,                         0x054);
    ASSERT_OFFSETOF(game, game_paused,                     0x058);
    ASSERT_OFFSETOF(game, use_default_hero_start_position, 0x05A);
    ASSERT_OFFSETOF(game, level_is_unloading,              0x05B);
    ASSERT_OFFSETOF(game, hero_freeze_depth,               0x060);
    ASSERT_OFFSETOF(game, the_world,                       0x064);
    ASSERT_OFFSETOF(game, base_camera,                     0x06C);
    ASSERT_OFFSETOF(game, current_view_camera,             0x070);
    ASSERT_OFFSETOF(game, current_game_camera,             0x074);
    ASSERT_OFFSETOF(game, unk_078,                         0x078);
    ASSERT_OFFSETOF(game, unk_07c,                         0x07C);
    ASSERT_OFFSETOF(game, mb,                              0x080);
    ASSERT_OFFSETOF(game, level_time,                      0x084);
    ASSERT_OFFSETOF(game, process_stack,                   0x088);
    ASSERT_OFFSETOF(game, string_localizer,                0x098);
    ASSERT_OFFSETOF(game, base_cam_position,               0x09C);
    ASSERT_OFFSETOF(game, frame_sequence,                  0x19C);
    ASSERT_OFFSETOF(game, frame_timing,                    0x1A0);
    ASSERT_OFFSETOF(game, unk_1ac,                         0x1AC);
    ASSERT_OFFSETOF(game, frame_clock,                     0x1B0);
    ASSERT_OFFSETOF(game, current_frame_delta,             0x1B8);
    ASSERT_OFFSETOF(game, data,                            0x1BC);
    ASSERT_OFFSETOF(game, blur,                            0x1C0);
    ASSERT_OFFSETOF(game, unk_1cc,                         0x1CC);
}
