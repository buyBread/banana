#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    class cut_scene;
    class cut_scene_segment;

    class cut_scene_player {

    public:
        cut_scene*          current_cut_scene;
        cut_scene_segment** current_segment; // into current_cut_scene->segments
        u8                  reserved_008[0x30];
        u32                 state_flags;
        u8                  reserved_03c[0x08];
        i32                 fade_out_active;
        u8                  reserved_048[0xCC];

        bool is_playing() const;
    };

    namespace references {
        inline util::memory_reference<cut_scene_player*> cut_scene_player { 0x010886F4 };
    } // references

    ASSERT_SIZEOF  (cut_scene_player,                  0x114);
    ASSERT_OFFSETOF(cut_scene_player, current_segment, 0x004);
    ASSERT_OFFSETOF(cut_scene_player, state_flags,     0x038);
    ASSERT_OFFSETOF(cut_scene_player, fade_out_active, 0x044);
} // treyarch
