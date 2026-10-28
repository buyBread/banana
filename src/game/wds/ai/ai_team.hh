#pragma once

#include <array>

#include "treyarch/mash/string_hash.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class ai_team {

    public:
        enum e_team : u32 {
            team_spiderman,
            team_police,
            team_shield,
            team_civilian,
            team_hidden_symbiote,
            team_boss,
            team_gang_a,
            team_gang_b,
            team_gang_c,
            team_gang_d,
            team_old_time,
            team_harlem_a,
            team_harlem_b,
            team_kingpin,
            team_symbiote,
            team_symbiote_pod,
            team_special_a,
            team_special_b,
            team_hate_all,
            team_true_neutral,
            num_teams
        };

        u32 enemy_team_mask;
        u32 friend_team_mask;
        u8  reserved_008[0x2C]; // reset every ai frame

        static bool   is_enemy        (e_team my_team, e_team other_team);
        static bool   is_friend       (e_team my_team, e_team other_team);
        static e_team get_enum_by_hash(string_hash the_hash);
    };

    ASSERT_SIZEOF  (ai_team,                   0x34);
    ASSERT_OFFSETOF(ai_team, friend_team_mask, 0x04);

    namespace references {
        inline util::memory_reference<string_hash> team_hash { 0x00FC3484 };

        inline util::memory_reference<std::array<string_hash, ai_team::num_teams>> team_hashes { 0x00FC3900 };

        // the script's hate and friend list natives rewrite teams
        inline util::memory_reference<std::array<u32,     ai_team::num_teams>> team_masks { 0x00FC33A0 };
        inline util::memory_reference<std::array<ai_team, ai_team::num_teams>> teams      { 0x00FC34D0 };
    } // references
} // treyarch
