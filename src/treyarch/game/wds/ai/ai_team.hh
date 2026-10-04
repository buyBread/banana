#pragma once

#include <array>

#include "treyarch/shared/hash/string_hash.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

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

        static e_team get_enum_by_hash(string_hash the_hash);
    };

    namespace references {
        inline util::memory_reference<string_hash> team_hash { 0x00FC3484 };

        inline util::memory_reference<std::array<string_hash, ai_team::num_teams>> team_hashes { 0x00FC3900 };
    } // references
} // treyarch
