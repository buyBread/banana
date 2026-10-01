#include "treyarch/game/wds/entity/entity.hh"

using namespace treyarch;

void entity::invoke_render_phase() {
    player_ifc()->render_phase();
}
