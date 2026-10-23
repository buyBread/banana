#pragma once

#include <cstddef>

#include "vc8/vector.hh"
#include "treyarch/game/animation/scene_anim.hh"
#include "treyarch/game/input/game_button.hh"
#include "treyarch/nal/scene_anim.hh"
#include "treyarch/shared/arch_base_vhandle.hh"
#include "treyarch/shared/memory/heap.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    class cut_scene;
    class cut_scene_segment;
    struct region;

    class cut_scene_player {

    public:
        enum e_state_flags : u32 {
            hooked_up = 0x040, // the current segment's scene anim, sound and lip sync run
            playing   = 0x080,
            skipping  = 0x100
        };

        cut_scene*              current_cut_scene;
        cut_scene_segment**     current_segment; // into current_cut_scene->segments
        vc8::vector
            <arch_base_vhandle> spawned_entities;
        vc8::vector
            <arch_base_vhandle> lip_sync_entities;
        vc8::vector
            <region*>           forced_regions;
        u32                     state_flags;
        i32                     saved_glass_house_level;
        i32                     loops_left;
        i32                     fade_state;      // 0 none, 1 fading out, 2 holding, 3 fading in
        f32                     fade_timer;
        f32                     fade_hold_time;
        f32                     fade_in_time;
        game_button             skip_buttons[2];
        f32                     loading_time;    // spent waiting on the segment's regions
        f32                     segment_time;
        f32                     last_time;
        f32                     time;
        scene_anim_handle_t     scene_anim_handle;
        nal::scene_anim*        current_scene_anim;
        bool                    has_focus;
        u8                      pad_105[0x03];
        f32                     focus[3];

        cut_scene_player();

        static void create_inst();

        bool       is_playing() const;
        cut_scene* get_current_cut_scene() const;
        bool       hero_unaffected() const;
        bool       has_sound() const;
        f32        get_sound_time() const;

        void frame_advance(f32 time_inc);
        bool frame_advance_lite(f32 time_inc);

        f32 get_total_time() const;
        f32 get_current_segment_start_time() const;
        f32 get_current_segment_end_time() const;
        f32 get_time() const;

        void* operator new(std::size_t size) noexcept {
            return memory::heap::allocate(size);
        }
    };

    namespace references {
        inline util::memory_reference<cut_scene_player*> cut_scene_player { 0x010886F4 };

        // set by the PARTICLE_EVENT handler; frame_advance skips one segment step and clears it
        inline util::memory_reference<bool> skip_next_segment_step { 0x010886F0 };
    } // references

    ASSERT_SIZEOF  (cut_scene_player,                          0x114);
    ASSERT_OFFSETOF(cut_scene_player, current_segment,         0x004);
    ASSERT_OFFSETOF(cut_scene_player, spawned_entities,        0x008);
    ASSERT_OFFSETOF(cut_scene_player, lip_sync_entities,       0x018);
    ASSERT_OFFSETOF(cut_scene_player, forced_regions,          0x028);
    ASSERT_OFFSETOF(cut_scene_player, state_flags,             0x038);
    ASSERT_OFFSETOF(cut_scene_player, saved_glass_house_level, 0x03C);
    ASSERT_OFFSETOF(cut_scene_player, fade_state,              0x044);
    ASSERT_OFFSETOF(cut_scene_player, skip_buttons,            0x054);
    ASSERT_OFFSETOF(cut_scene_player, loading_time,            0x0EC);
    ASSERT_OFFSETOF(cut_scene_player, scene_anim_handle,       0x0FC);
    ASSERT_OFFSETOF(cut_scene_player, current_scene_anim,      0x100);
    ASSERT_OFFSETOF(cut_scene_player, has_focus,               0x104);
    ASSERT_OFFSETOF(cut_scene_player, focus,                   0x108);
} // treyarch
