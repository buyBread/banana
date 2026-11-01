#pragma once

#include "treyarch/mash/string_hash.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
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
        f32                     world_time_dilation;
        f32                     player_time_dilation;
        f32                     depletion_rate;
        f32                     recharge_rate;
        f32                     max_recharge_percent;
        f32                     min_engage_percent;
        f32                     reflex_pool;
        e_player_reflexes_state state;
        bool                    enabled;
        u8                      pad_025[0x03];

        bool is_enabled() const {
            return enabled;
        }

        void enable();
        void disable(bool force); // leaves reflexes unless locked on, or when forced

        // inlined @ sub_841280
        void lock_on() {
            enable();
            state = player_reflexes_state_locked_on;
        }

        // inlined @ sub_8412C0
        void lock_off() {
            disable(false);
            state = player_reflexes_state_locked_off;
        }

        // inlined @ sub_841300
        void unlock() {
            disable(false);
            state = player_reflexes_state_normal;
        }

        void inc_reflex_percent(f32 percent);
    };

    // the spider-sense twin of player_reflexes; WoS only, so no recovered name
    class player_spider_senses {

    public:
        player_interface*       owner;
        f32                     unk_04;
        f32                     unk_08; // enable copies it into unk_04
        f32                     unk_0c;
        e_player_reflexes_state state;  // the bindings use player_reflexes' values
        bool                    enabled;
        bool                    unk_15;
        bool                    unk_16;
        u8                      pad_017[0x01];
        f32                     unk_18; // the owner's range for counting an entity in senses
        f32                     unk_1c;

        bool is_enabled() const {
            return enabled;
        }

        void enable(bool unk);
        void disable(bool unk, bool force); // ends spider senses unless locked on, or when forced
        void lock_on(bool unk);
        void lock_off(bool unk);
        void unlock(bool unk);
    };

    namespace references {
        inline util::memory_reference<string_hash> reflexes_entered       { 0x0102C6CC };
        inline util::memory_reference<string_hash> reflexes_exited        { 0x0102C3A4 };
        inline util::memory_reference<string_hash> spidey_sense_shockwave { 0x01088060 };
    } // references

    ASSERT_SIZEOF  (player_reflexes,                       0x28);
    ASSERT_OFFSETOF(player_reflexes, world_time_dilation,  0x04);
    ASSERT_OFFSETOF(player_reflexes, player_time_dilation, 0x08);
    ASSERT_OFFSETOF(player_reflexes, depletion_rate,       0x0C);
    ASSERT_OFFSETOF(player_reflexes, min_engage_percent,   0x18);
    ASSERT_OFFSETOF(player_reflexes, reflex_pool,          0x1C);
    ASSERT_OFFSETOF(player_reflexes, state,                0x20);
    ASSERT_OFFSETOF(player_reflexes, enabled,              0x24);

    ASSERT_SIZEOF  (player_spider_senses,          0x20);
    ASSERT_OFFSETOF(player_spider_senses, unk_0c,  0x0C);
    ASSERT_OFFSETOF(player_spider_senses, state,   0x10);
    ASSERT_OFFSETOF(player_spider_senses, enabled, 0x14);
    ASSERT_OFFSETOF(player_spider_senses, unk_18,  0x18);
} // treyarch
