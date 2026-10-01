#include "treyarch/game/wds/entity/entity.hh"

using namespace treyarch;

// inlined into sub_9772D0
void entity::invoke_render_phase() {
    player_ifc()->render_phase();
}
