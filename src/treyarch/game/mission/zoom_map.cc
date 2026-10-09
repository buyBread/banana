#include "retail.hh"
#include "treyarch/game/frontend/frontend_manager.hh"
#include "treyarch/game/frontend/ui_frontend.hh"
#include "treyarch/game/mission/mission_manager.hh"

using namespace treyarch;

// sub_984130
void mission_manager::select_poi(const mission_icon_info &poi, bool) {
    selected_poi = poi;

    if (selected_poi_icon) {
        retail::sub_68C3F0((i32)selected_poi_icon, 1);
        retail::sub_6A1100((u32*)selected_poi_icon, 1);
    }

    last_zoom_map_selection_time = real_time;
    selected_poi_location_key    = -1;

    update_mission_icons();
}

// sub_97FA90
void mission_manager::clear_selected_poi(bool clear_zoom_map) {
    if (flags & mission_manager_flag_unk_04000000)
        return;

    flags                 |= mission_manager_flag_unk_04000000;
    selected_poi.instance  = nullptr;

    if (selected_poi_icon) {
        retail::sub_68C3F0((i32)selected_poi_icon, 0);
        retail::sub_6A1100((u32*)selected_poi_icon, 0);
    }

    selected_poi_location_key = -1;

    ui_frontend* igo = references::frontend.get().igo;

    if (clear_zoom_map && igo && igo->zoom_map)
        retail::sub_690A10((u32*)igo->zoom_map);

    flags &= ~mission_manager_flag_unk_04000000;
}

// sub_984190
void mission_manager::on_zoom_map_poi_selected(event*            raised_event,
                                               arch_base_vhandle recipient,
                                               void*             parameters) {

    ui_frontend* igo = references::frontend.get().igo;

    if (!igo)
        return;

    i32 poi_index = retail::sub_690A50((u32*)igo->zoom_map);

    if (poi_index < 0)
        return;

    on_zoom_map_poi_unselected(raised_event, recipient, parameters);

    mission_manager* missions = mission_manager::inst();

    for (mission_icon_info* trigger = missions->mission_triggers->begin(); trigger != missions->mission_triggers->end(); ++trigger) {
        if (trigger->poi_index == poi_index) {
            missions->select_poi(*trigger, false);

            return;
        }
    }

    for (mission_icon_info* icon = missions->mission_icons->begin(); icon != missions->mission_icons->end(); ++icon) {
        if (icon->poi_index == poi_index) {
            missions->select_poi(*icon, false);

            return;
        }
    }

    for (mission_poi_location_t* location = missions->poi_locations->begin(); location != missions->poi_locations->end(); ++location) {
        if (location->key == poi_index) {
            missions->selected_poi.instance = nullptr;

            if (missions->selected_poi_icon) {
                retail::sub_68C3F0((i32)missions->selected_poi_icon, 1);
                retail::sub_6A1100((u32*)missions->selected_poi_icon, 1);
            }

            missions->selected_poi_location_key      = location->key;
            missions->last_zoom_map_selection_time   = missions->real_time;
            missions->selected_poi_location_position = location->position;

            missions->update_mission_icons();

            return;
        }
    }
}

// sub_97FB80
void mission_manager::on_zoom_map_poi_unselected(event*, arch_base_vhandle, void*) {
    mission_manager* missions = mission_manager::inst();

    if (missions->flags & mission_manager_flag_unk_04000000)
        return;

    missions->flags                 |= mission_manager_flag_unk_04000000;
    missions->selected_poi.instance  = nullptr;

    if (missions->selected_poi_icon) {
        retail::sub_68C3F0((i32)missions->selected_poi_icon, 0);
        retail::sub_6A1100((u32*)missions->selected_poi_icon, 0);
    }

    missions->flags                     &= ~mission_manager_flag_unk_04000000;
    missions->selected_poi_location_key  = -1;
}
