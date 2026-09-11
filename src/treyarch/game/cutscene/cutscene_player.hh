#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    class cutscene_player {

    public:
        u8  reserved_000[0x38];
        u32 state_flags;
        u8  reserved_03c[0x08];
        i32 fade_out_active;
        u8  reserved_048[0xCC];

        bool is_playing() const;
    };

    namespace references {
        inline util::memory_reference<cutscene_player*> cutscene_player { 0x010886F4 };
    } // references

    ASSERT_SIZEOF  (cutscene_player,                  0x114);
    ASSERT_OFFSETOF(cutscene_player, state_flags,     0x038);
    ASSERT_OFFSETOF(cutscene_player, fade_out_active, 0x044);
} // treyarch
