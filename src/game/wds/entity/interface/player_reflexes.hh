#pragma once

#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class player_interface;

    // SM3 player_reflexes_state_e
    enum e_player_reflexes_state : i32 {
        player_reflexes_state_normal,
        player_reflexes_state_locked_on,
        player_reflexes_state_locked_off
    };

    class player_reflexes {

    public:
        player_interface*       owner;
        u8                      reserved_004[0x1C];
        e_player_reflexes_state state;
        bool                    enabled;
        u8                      pad_025[0x03];

        bool is_enabled() const {
            return enabled;
        }

        // leaves reflexes unless locked on, or when forced
        void disable(bool force);
    };

    // the spider-sense twin of player_reflexes; WoS only, so no recovered name
    class player_spider_senses {

    public:
        u8                      reserved_000[0x10];
        e_player_reflexes_state state; // the bindings use player_reflexes' values
        bool                    enabled;

        bool is_enabled() const {
            return enabled;
        }

        // ends spider senses unless locked on, or when forced
        void disable(bool unk, bool force);
    };

    ASSERT_SIZEOF  (player_reflexes,          0x28);
    ASSERT_OFFSETOF(player_reflexes, state,   0x20);
    ASSERT_OFFSETOF(player_reflexes, enabled, 0x24);

    ASSERT_OFFSETOF(player_spider_senses, state,   0x10);
    ASSERT_OFFSETOF(player_spider_senses, enabled, 0x14);
} // treyarch
