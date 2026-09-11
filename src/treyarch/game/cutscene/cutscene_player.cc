#include "treyarch/game/cutscene/cutscene_player.hh"

using namespace treyarch;

bool cutscene_player::is_playing() const {
    return (state_flags & 0x80)  != 0 ||
           (state_flags & 0x100) != 0 ||
           fade_out_active;
}
