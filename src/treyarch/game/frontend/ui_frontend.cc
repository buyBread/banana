#include "treyarch/app/app.hh"
#include "treyarch/game/frontend/frontend_manager.hh"
#include "treyarch/game/frontend/igo/igo_3d_loading_screen.hh"
#include "treyarch/game/frontend/igo/igo_3d_scrapbook.hh"

using namespace treyarch;

// sub_688B40
i32 ui_frontend::select_ui_environment_index() {
    if (scrapbook->is_active())
        return 1;

    if (pauseless_dialog->is_visible())
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

    if (current_igo->loading_screen && current_igo->loading_screen->is_visible())
        widget_suppression_conditions |= suppress_when_loading_screen;

    if (current_igo->letterbox && current_igo->letterbox->is_visible())
        widget_suppression_conditions |= suppress_when_letterbox_visible;

    u8 visible = current_igo->pauseless_dialog->is_visible();

    if (visible)
        widget_suppression_conditions |= suppress_when_pauseless_dialog;

    return visible;
}
