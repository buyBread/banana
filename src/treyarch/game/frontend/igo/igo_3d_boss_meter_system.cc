#include "treyarch/game/frontend/igo/igo_3d_boss_meter_system.hh"

using namespace treyarch;

// sub_6D0990
void igo_3d_boss_meter_system::draw() {
    for (auto* position = meters.begin(); position != meters.end(); position = position->next) {
        if (position->value)
            position->value->draw();
    }
}
