#pragma once

#include "vc8/vector.hh"
#include "treyarch/chuck/vm/vm_dynamic_array_manager.hh"
#include "game/input/game_button.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/string.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class cut_scene;

    // plays a chain of cut scenes with button presses between them for wait_run_toa_cutscene
    class cut_scene_toa_helper {

    public:
        vc8::vector
            <mash::string> cutscene_list;           // a success and a failure per sequence, then the last success
        i32                current_sequence;
        i32                current_cutscene;        // into cutscene_list
        vc8::vector<i32>   button_num_per_sequence;
        vc8::vector<i32>   button_list;             // every sequence's game controls in order
        i32                current_button;
        i32                current_button_index;    // presses so far in this sequence
        vc8::vector<f32>   show_button_time_list;
        vc8::vector<f32>   end_button_time_list;
        game_button        buttons[25];             // one per game control
        bool               failed;
        bool               running;
        bool               cut_scene_started;       // the frame clears it
        bool               succeeded;
        bool               window_open;             // the button window was open at the end of the last frame
        bool               fade_endings;            // the last success and the failures keep their fades
        bool               scene_anim_paused;
        u8                 pad_7d3[0x01];

        cut_scene_toa_helper();
       ~cut_scene_toa_helper();

        static cut_scene_toa_helper* get();
        static void                  release_inst();

        bool setup(chuck::vm::script_dynamic_array<const char*> cutscenes,
                   chuck::vm::script_dynamic_array<f32>         button_nums,
                   chuck::vm::script_dynamic_array<f32>         button_ids,
                   chuck::vm::script_dynamic_array<f32>         show_times,
                   chuck::vm::script_dynamic_array<f32>         end_times,
                   bool                                         with_fades);
        void stop();
        void frame_advance(f32 time_inc);

        // sub_805390
        bool is_running() const {
            return running;
        }

        cut_scene* get_failure_category(i32 sequence);
        cut_scene* get_success_category(i32 sequence);
        i32        get_agility_test_button_corresponding_to_num(i32 num) const;
        i32        get_button(i32 _current_sequence, i32 index) const;

        static i32 get_num_corresponding_to_agility_test_buttons_t(i32 button);

        // sub_805920
        // inlined @ sub_806B80
        f32 get_show_button_time(i32 sequence) const { // -1 out of range
            if (sequence < 0 || sequence >= (i32)(cutscene_list.size() / 2))
                return -1.0f;

            return show_button_time_list[sequence];
        }

        // sub_805960
        // inlined @ sub_806C50
        // inlined @ sub_806D20
        f32 get_end_button_time(i32 sequence) const { // -1 out of range
            if (sequence < 0 || sequence >= (i32)(cutscene_list.size() / 2))
                return -1.0f;

            return end_button_time_list[sequence];
        }

        bool waits_for_press() const;
        void unpause_scene_anim();

        bool past_show_time() const;
        bool before_end_time() const;
        bool past_end_time() const;
        bool is_window_open() const;
        bool accepts_press() const;
        bool missed_window() const;
        bool wrong_button_pressed() const;

        void step_pressed_buttons();
        void update_prompt();
    };

    namespace references {
        inline util::memory_reference<cut_scene_toa_helper> cut_scene_toa_helper { 0x01088AA8 };

        inline util::memory_reference<u32> cut_scene_toa_helper_guard { 0x0108927C };

        inline util::memory_reference<string_hash> cut_scene_toa_begin              { 0x0102CC1C };
        inline util::memory_reference<string_hash> cut_scene_toa_button_shown       { 0x0102C2A8 };
        inline util::memory_reference<string_hash> cut_scene_toa_button_not_shown   { 0x0102C688 };
        inline util::memory_reference<string_hash> cut_scene_toa_hit_button_success { 0x0102C374 };
        inline util::memory_reference<string_hash> cut_scene_toa_success            { 0x0102CB70 };
        inline util::memory_reference<string_hash> cut_scene_toa_failure            { 0x0102C678 };
    } // references

    ASSERT_SIZEOF  (cut_scene_toa_helper,                          0x7D4);
    ASSERT_OFFSETOF(cut_scene_toa_helper, current_sequence,        0x010);
    ASSERT_OFFSETOF(cut_scene_toa_helper, button_num_per_sequence, 0x018);
    ASSERT_OFFSETOF(cut_scene_toa_helper, button_list,             0x028);
    ASSERT_OFFSETOF(cut_scene_toa_helper, current_button,          0x038);
    ASSERT_OFFSETOF(cut_scene_toa_helper, show_button_time_list,   0x040);
    ASSERT_OFFSETOF(cut_scene_toa_helper, end_button_time_list,    0x050);
    ASSERT_OFFSETOF(cut_scene_toa_helper, buttons,                 0x060);
    ASSERT_OFFSETOF(cut_scene_toa_helper, failed,                  0x7CC);
    ASSERT_OFFSETOF(cut_scene_toa_helper, running,                 0x7CD);
    ASSERT_OFFSETOF(cut_scene_toa_helper, window_open,             0x7D0);
    ASSERT_OFFSETOF(cut_scene_toa_helper, fade_endings,            0x7D1);
    ASSERT_OFFSETOF(cut_scene_toa_helper, scene_anim_paused,       0x7D2);
} // treyarch
