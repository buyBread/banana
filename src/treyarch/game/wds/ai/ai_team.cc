#include "treyarch/game/wds/ai/ai_team.hh"

using namespace treyarch;

// sub_42B0D0
ai_team::e_team ai_team::get_enum_by_hash(string_hash the_hash) {
    std::array<string_hash, num_teams> &team_hashes = references::team_hashes.get();

    u32 team = 0;

    while (team < num_teams && team_hashes[team] != the_hash)
        ++team;

    return (e_team)team;
}
