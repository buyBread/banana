#include "retail.hh"
#include "game/app.hh"
#include "game/frontend/finger_of_god.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/igo/igo_3d_loading_screen.hh"
#include "game/frontend/igo/igo_3d_scrapbook.hh"
#include "game/ui_character_info_list.hh"
#include "game/wds/entity/entity.hh"
#include "game/wds/references.hh"
#include "game/wds/world_dynamics_system.hh"
#include "treyarch/mash/string.hh"

using namespace treyarch;

// sub_688B40
i32 ui_frontend::select_ui_environment_index() {
    if (scrapbook->is_active())
        return 1;

    if (pauseless_dialog->is_shown())
        return 3;

    return references::frontend.get().igo->zoom_map->is_open_or_zooming() ? 2 : 0;
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

    if (current_igo->zoom_map->is_open_or_zooming())
        widget_suppression_conditions |= suppress_when_zoom_map_open;

    if (current_igo->loading_screen && current_igo->loading_screen->is_shown())
        widget_suppression_conditions |= suppress_when_loading_screen;

    if (current_igo->letterbox_widget && current_igo->letterbox_widget->is_shown())
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
void ui_frontend::set_dialog_box(dialog_box* box, bool freeze_hero) { // deletes the one before and shows the new one
    if (dialog)
        delete dialog;

    dialog = box;
    box->show(freeze_hero);
}

// sub_699210
void ui_frontend::set_generic_meter_label(mash::string* label) {
    auto set_label = (void (__thiscall*)(igo_3d_generic_meter_widget*, mash::string, bool))retail::sub_68C960;

    if (mission_specific_meter)
        set_label(mission_specific_meter, mash::string(*label), false);

    label->finalize(mash::ALLOCATED);
}

// sub_6E77C0
void ui_frontend::destroy_fog(finger_of_god* fog) {
    igo_3d_radar_widget* radar = radar_widget;

    if (!radar || !fog)
        return;

    finger_of_god** position = radar->fingers_of_god.begin();

    while (position != radar->fingers_of_god.end() && *position != fog)
        ++position;

    if (position == radar->fingers_of_god.end())
        return;

    delete *position;

    *position = nullptr;

    radar->fingers_of_god.erase(position);
}

// sub_6E7000
void ui_frontend::letterbox(f32 time, f32 height) {
    if (!letterbox_widget)
        return;

    if ((f64)height > 0.0001) {
        ui_frontend* igo = references::frontend.get().igo;

        references::spider_senses_on.write(0);
        igo->background_effect_mode = 0;
        retail::sub_6B17B0(igo, 0.0f); // the background effect update

        if (references::g_world_ptr.read()->get_hero_ptr())
            references::g_world_ptr.read()->get_hero_ptr()->raise_event(references::senses_exited.read());
    }

    if (references::unk_0102d00c.read() != 1.0f)
        height = (f32)(height * 0.5);

    letterbox_widget->letterbox(time, height);
}

// sub_6990D0
void ui_frontend::slide_in_ally_health_widget() {
    if (ally_health_widget)
        retail::sub_68CE10((u32*)ally_health_widget);
}

// sub_6990E0
void ui_frontend::slide_out_ally_health_widget() {
    if (ally_health_widget)
        retail::sub_68CEB0((u32*)ally_health_widget);
}

// sub_6990F0
void ui_frontend::set_ally_health_meter_value(f32 value) {
    ally_health_widget_t* widget = ally_health_widget;

    if (!widget)
        return;

    f32 previous = widget->value;

    widget->value = value;

    if (previous > value) {
        if (value != 0.0f) {
            if (!widget->dropping) {
                widget->dropping  = true;
                widget->drop_from = previous;
            }

            return;
        }
    } else if (!(value > previous))
        return;

    widget->dropping  = false;
    widget->drop_from = widget->value;
}

// sub_699170
void ui_frontend::set_ally_entity(arch_base_vhandle entity) {
    if (ally_health_widget)
        ally_health_widget->entity = entity;
}

// sub_6F21F0
void ui_frontend::set_ally_health_icon(i32 icon) {
    string_hash key;

    retail::sub_6EFF40((u32*)references::ui_character_info_map.read(), (u32*)&key, icon); // the key for the icon

    if (ally_health_widget)
        ((void (__thiscall*)(ally_health_widget_t*, string_hash))retail::sub_6EC1F0)(ally_health_widget, key); // set_icon
}

// sub_6991F0
void ui_frontend::set_boss_enabled(bool enabled) {
    if (enemy_health_manager)
        enemy_health_manager->set_boss_enabled(enabled);
}

// sub_699190
void ui_frontend::slide_in_boss_health_widget() {
    if (enemy_health_manager && enemy_health_manager->boss_widget) {
        enemy_health_manager->boss_widget->set_unk_1c0(true);
        enemy_health_manager->boss_widget->slide_in();
    }
}

// sub_6991C0
void ui_frontend::slide_out_boss_health_widget() {
    if (enemy_health_manager && enemy_health_manager->boss_widget) {
        enemy_health_manager->boss_widget->set_unk_1c0(false);
        enemy_health_manager->boss_widget->slide_out();
    }
}

// sub_6F2230
void ui_frontend::set_boss_health_icon(i32 icon) {
    string_hash key;

    retail::sub_6EFF40((u32*)references::ui_character_info_map.read(), (u32*)&key, icon); // the key for the icon

    igo_3d_enemy_health_manager* manager = enemy_health_manager;

    if (manager && manager->boss_widget) {
        ((void (__thiscall*)(igo_3d_enemy_health_widget*, string_hash))retail::sub_6EC1F0)(manager->boss_widget, key); // set_icon

        manager->boss_icon_set = true;
    }
}

// sub_6894C0
void ui_frontend::set_boss_entity(arch_base_vhandle entity) {
    if (enemy_health_manager)
        enemy_health_manager->boss_entity = entity;
}

// sub_6894E0
void ui_frontend::set_grunt_enabled(bool enabled) {
    if (enemy_health_manager)
        enemy_health_manager->grunt_enabled = enabled;
}
