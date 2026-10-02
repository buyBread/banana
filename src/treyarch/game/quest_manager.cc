#include "treyarch/game/frontend/frontend_manager.hh"
#include "treyarch/game/frontend/igo/igo_3d_text.hh"
#include "treyarch/game/frontend/ui_frontend.hh"
#include "treyarch/game/quest_manager.hh"

using namespace treyarch;

// sub_7EA9D0
void quest_manager::draw_text() {
    igo_3d_text* text = references::quest_text.read();

    if (!text)
        return;

    if (!references::frontend.get().igo->conversation_menu_system->is_conversation_active())
        text->render();
}
