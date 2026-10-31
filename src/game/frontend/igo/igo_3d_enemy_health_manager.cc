#include "game/frontend/igo/igo_3d_enemy_health_manager.hh"

using namespace treyarch;

// sub_68CFF0
void igo_3d_enemy_health_manager::draw() {
    if (boss_widget)
        boss_widget->render();

    if (grunt_widget)
        grunt_widget->render();
}

// sub_68D020
// inlined @ sub_6991F0
void igo_3d_enemy_health_manager::set_boss_enabled(bool enabled) {
    boss_enabled = enabled;

    if (!enabled)
        boss_icon_set = false;
}
