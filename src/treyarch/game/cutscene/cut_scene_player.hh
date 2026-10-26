#pragma once

#include <cstddef>

#include "vc8/vector.hh"
#include "treyarch/game/animation/scene_anim.hh"
#include "treyarch/game/input/game_button.hh"
#include "treyarch/nal/scene_anim.hh"
#include "treyarch/shared/arch_base_vhandle.hh"
#include "treyarch/shared/math/algebra.hh"
#include "treyarch/shared/memory/heap.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    class cut_scene;
    class cut_scene_segment;
    class entity_class_entry;
    struct region;

    namespace mash {
        template<typename T>
        class avl_tree;
    } // mash

    class cut_scene_player {

    public:
        enum e_state_flags : u32 {
            radar_was_visible = 0x00001, // take_igo hid it
            hooked_up         = 0x00040, // the current segment's scene anim and sound run
            playing           = 0x00080,
            skipping          = 0x00100,
            segment_paused    = 0x00200, // play_current_segment paused the new scene anim
            skip_fade_in      = 0x00400, // the end fade stops on black
            give_back_igo     = 0x01000, // stop left it to the end fade
            toa_middle        = 0x02000, // the toa helper's: a success cut scene with more to come
            toa_first         = 0x04000, // its first success cut scene
            toa_last          = 0x08000, // its last success cut scene or a failure
            hero_was_hidden   = 0x10000
        };

        // SM3's ZEROVEC; every file kept its own copy
        inline static const vector3d zerovec = vector3d(0.0f, 0.0f, 0.0f);

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
        bool       is_skipping() const;
        cut_scene* get_current_cut_scene() const;
        bool       hero_unaffected() const;
        bool       has_sound() const;
        f32        get_sound_time() const;

        void frame_advance(f32 time_inc);
        bool frame_advance_lite(f32 time_inc);

        bool play(cut_scene* to_play);
        void clamp_settings();
        void stop(cut_scene* only);
        void finish();
        bool on_last_segment() const;

        bool advance_fade(f32 time_inc);
        void start_fade();
        void start_end_fade();

        void take_igo(bool give_back);

        void        start_entities(mash::avl_tree<entity_class_entry>* entries);
        void        start_entity(entity_class_entry* entry);
        void        restore_entities();
        void        release_entities();
        static void remove_entities(vc8::vector<arch_base_vhandle>* entities);

        static void reset_interfaces(mash::avl_tree<entity_class_entry>* entries);
        static void reset_interfaces(entity_class_entry* entry);

        void update_focus();

        void play_current_segment();
        void release_current_segment();
        bool districts_loaded(f32 time_inc);
        bool start_current_segment();
        void hookup_current_segment();

        static void raise_animation_events(mash::avl_tree<entity_class_entry>* entries, f32 t, f32 last_t, bool skipping);
        static void raise_animation_events(entity_class_entry* entry, f32 t, f32 last_t, bool skipping);
        void        raise_hero_animation_events(f32 t, f32 last_t, bool skipping);

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
