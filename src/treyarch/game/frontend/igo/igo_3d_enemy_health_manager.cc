#include "treyarch/game/frontend/igo/igo_3d_enemy_health_manager.hh"

using namespace treyarch;

// sub_68CFF0
void igo_3d_enemy_health_manager::draw() {
    if (unk_04)
        unk_04->draw();

    if (unk_08)
        unk_08->draw();
}
