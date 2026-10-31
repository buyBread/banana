#include "retail.hh"
#include "game/app.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/igo/igo_3d_loading_screen.hh"
#include "game/frontend/igo/igo_3d_scrapbook.hh"
#include "treyarch/mash/string.hh"

using namespace treyarch;

// sub_688B40
i32 ui_frontend::select_ui_environment_index() {
    if (scrapbook->is_active())
        return 1;

    if (pauseless_dialog->is_shown())
        return 3;

    return references::frontend.get().igo->zoom_map->blocks_world_rendering() ? 2 : 0;
}

// sub_689400
i32 ui_frontend::update_widget_suppression_conditions() {
    widget_suppression_conditions = 0;

    game* current_game = references::game.read();

    if (current_game->game_paused)
        widget_suppression_conditions = suppress_when_paused;

    if (current_game->disable_interface)
        widget_suppression_conditions |= suppress_when_interface_disabled;

    ui_frontend* current_igo = references::frontend.get().igo;

    if (current_igo->zoom_map->blocks_world_rendering())
        widget_suppression_conditions |= suppress_when_zoom_map_open;

    if (current_igo->loading_screen && current_igo->loading_screen->is_shown())
        widget_suppression_conditions |= suppress_when_loading_screen;

    if (current_igo->letterbox && current_igo->letterbox->is_shown())
        widget_suppression_conditions |= suppress_when_letterbox_visible;

    u8 visible = current_igo->pauseless_dialog->is_shown();

    if (visible)
        widget_suppression_conditions |= suppress_when_pauseless_dialog;

    return visible;
}

// sub_689500
void ui_frontend::set_generic_meter_visible(bool visible) {
    if (mission_specific_meter)
        mission_specific_meter->set_shown(visible);
}

// sub_689520
void ui_frontend::set_generic_meter_value(f32 value) {
    if (mission_specific_meter)
        mission_specific_meter->set_value(value);
}

// sub_698E90
void ui_frontend::set_dialog_box(dialog_box* box, u32 unk) { // deletes the one before and shows the new one
    if (dialog)
        dialog->destroy();

    dialog = box;
    box->show(unk);
}

// sub_699210
void ui_frontend::set_generic_meter_label(mash::string* label) {
    auto set_label = (void (__thiscall*)(igo_3d_generic_meter_widget*, mash::string, bool))retail::sub_68C960;

    if (mission_specific_meter)
        set_label(mission_specific_meter, mash::string(*label), false);

    label->finalize(mash::ALLOCATED);
}
