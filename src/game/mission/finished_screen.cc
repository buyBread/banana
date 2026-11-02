#include "retail.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/igo/igo_3d_pauseless_dialog_widget.hh"
#include "game/frontend/igo/igo_3d_scrapbook.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/mission/mission_manager.hh"
#include "game/quest_manager.hh"
#include "game/region_pack_manager.hh"
#include "game/region_spawn_manager.hh"

using namespace treyarch;

// sub_980BC0
void mission_manager::set_mission_finished_screen_has_appeared(bool appeared, bool despawn_quest_giver) {
    mission_finished_screen_has_appeared = appeared;

    if (!appeared)
        return;

    retail::sub_7FEF20((u32*)quest_manager::inst(), (i32)&current_mission);

    if (references::region_spawns_enabled.read() && despawn_quest_giver && region_pack_manager::inst())
        region_pack_manager::inst()->despawn_and_lock_quest_giver_in_mission(false);
}

// sub_97ED90
void mission_manager::handle_finished_dialog_selection() {
    i32 selection = references::frontend.get().igo->pauseless_dialog->get_selected_button();

    if ((!success_screen_type && selection == 2) || success_screen_type == 1) {
        references::frontend.get().igo->scrapbook->activate(false);

        retail::sub_68FF60((u32*)references::frontend.get().igo->scrapbook);
        references::frontend.get().igo->scrapbook->set_page(1, true, 0);
        retail::sub_711FB0((u32*)references::frontend.get().igo->scrapbook, 0);
    } else if (success_screen_type == 2) {
        references::frontend.get().igo->scrapbook->activate(false);

        retail::sub_68FF60((u32*)references::frontend.get().igo->scrapbook);
        references::frontend.get().igo->scrapbook->set_page(1, true, 0);
        retail::sub_711FB0((u32*)references::frontend.get().igo->scrapbook, 2);
        retail::sub_6BCC90((u32*)references::frontend.get().igo->scrapbook, success_screen_parameter);
    }
}
