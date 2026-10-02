#include "treyarch/game/frontend/igo/igo_3d_text.hh"
#include "treyarch/game/region_spawn_manager.hh"

using namespace treyarch;

// sub_90F5C0
void region_spawn_manager::draw_text() {
    igo_3d_text* text = references::region_spawn_text.read();

    if (text)
        text->render();
}
