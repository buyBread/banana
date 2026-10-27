#include "game/frontend/conversation_menu_system.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/igo/igo_3d_scrapbook.hh"
#include "game/frontend/ui_frontend.hh"

using namespace treyarch;

// sub_68BF40
bool conversation_menu_system::is_conversation_active() {
    if (references::frontend.get().igo->scrapbook->is_active())
        return false;

    return active;
}
