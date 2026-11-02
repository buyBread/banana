#include <cstring>

#include "retail.hh"
#include "game/event/event_callback.hh"
#include "game/event/event_manager.hh"
#include "game/frontend/entity_tracker.hh"
#include "game/frontend/finger_of_god.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/igo/igo_3d_dialog_widget.hh"
#include "game/frontend/igo/igo_3d_pauseless_dialog_widget.hh"
#include "game/frontend/igo/igo_3d_zoom_map.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/math/po.hh"
#include "game/mission/mission_manager.hh"
#include "game/quest_manager.hh"
#include "game/region_spawn_manager.hh"
#include "game/spawn/spawn_table_manager.hh"
#include "game/static_spawn_item_collection.hh"
#include "game/wds/entity/entity.hh"
#include "game/wds/references.hh"
#include "game/wds/world_dynamics_system.hh"
#include "treyarch/mash/mash_info.hh"

using namespace treyarch;

// sub_933C70
void static_spawn_item_collection::construct_mashed_class() {
    self = nullptr;

    spawn_items.construct_mashed_class();
}

// sub_934C80
void static_spawn_item_collection::destruct_mashed_class() {
    spawn_items.destruct_mashed_class();
}

// sub_93DA00
void static_spawn_item_collection::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    spawn_items.unmash(mash_info, this, buffer);
}

// sub_927CD0
spawn_data_base* static_spawn_item::find_data(const mash::string &name) {
    for (spawn_data_base** it = data.begin(); it != data.end(); ++it)
        if (!std::strncmp((*it)->name.c_str(), name.c_str(), mash::string::max_length))
            return *it;

    return nullptr;
}

// sub_934CA0
void cross_neighborhood_spawn::remove_map_markers() {
    while (!unk_d8->radar_markers.empty()) {
        references::frontend.get().igo->destroy_fog(*unk_d8->radar_markers.begin());

        unk_d8->radar_markers.erase(unk_d8->radar_markers.begin());
    }

    while (!unk_d8->zoom_map_markers.empty()) {
        references::frontend.get().igo->zoom_map->remove_poi(*unk_d8->zoom_map_markers.begin());

        unk_d8->zoom_map_markers.erase(unk_d8->zoom_map_markers.begin());
    }

    unk_d8->active = nullptr;
}

// sub_950450
bool cross_neighborhood_spawn::tick(f32 dt) { // true while the hero is at a goto point; until then the current wall's timers run down
    vector3d hero = references::g_world_ptr.read()->get_hero_ptr()->my_abs_po->get_position();

    f32  radius_squared = (f32)((f64)radius * (f64)radius);
    bool arrived        = false;

    if (unk_d8->goto_points.empty()) {
        unk_d8->safe_zone_wall_timers.clear();

        for (finger_of_god** it = unk_d8->radar_markers.begin(); it != unk_d8->radar_markers.end(); it = unk_d8->radar_markers.erase(it))
            references::frontend.get().igo->destroy_fog(*it);

        for (u32* it = unk_d8->zoom_map_markers.begin(); it != unk_d8->zoom_map_markers.end(); it = unk_d8->zoom_map_markers.erase(it))
            references::frontend.get().igo->zoom_map->remove_poi(*it);

        if ((u32)unk_d8->safe_zone_wall_index < safe_zone_walls.size) {
            auto pick_list_record = (bool (__thiscall*)(spawn_table_manager*, mash::string, spawn_point**, bool, bool, bool))retail::sub_9494E0;

            spawn_point* record = nullptr; // a failed pick pushes the previous record again

            for (u32 wall = 0; wall < safe_zone_walls.size; ++wall)
                for (mash::string** list = safe_zone_walls[wall]->begin(); list != safe_zone_walls[wall]->end(); ++list) {
                    pick_list_record(spawn_table_manager::inst(), **list, &record, true, false, true);

                    unk_d8->goto_points.push_back(record);
                }
        }
    }

    if (radius != 0.0f)
        for (spawn_point** it = unk_d8->goto_points.begin(); it != unk_d8->goto_points.end(); ++it) {
            f32 dx = (f32)((f64)hero.x - (f64)(*it)->position.x);
            f32 dy = (f32)((f64)hero.y - (f64)(*it)->position.y);
            f32 dz = (f32)((f64)hero.z - (f64)(*it)->position.z);

            f32 distance_squared = (f32)((f64)dx * dx + (f64)dy * dy + (f64)dz * dz);

            if (radius_squared > distance_squared)
                arrived = true;
        }

    if (std::strncmp(neighborhood.c_str(), "", mash::string::max_length)) {
        mash::string current = region_spawn_manager::inst()->get_neighborhood_name();

        if (std::strncmp(current.c_str(), neighborhood.c_str(), mash::string::max_length))
            arrived = false;
        else if (radius == 0.0f)
            arrived = true;
    }

    update_arrival(arrived);

    u32 wall = unk_d8->safe_zone_wall_index;

    if (wall >= safe_zone_walls.size || !(safe_zone_wall_time > 0.0f) || arrived)
        return arrived;

    u32 first = 0;

    for (u32 index = 0; index < wall; ++index)
        first += safe_zone_walls[index]->size;

    u32 expired = 0;

    for (u32 point = 0; point < safe_zone_walls[unk_d8->safe_zone_wall_index]->size; ++point) {
        f32 &timer = unk_d8->safe_zone_wall_timers.begin()[first + point];

        timer = (f32)((f64)timer - (f64)dt);

        if (timer <= 0.0f) {
            ++expired;

            timer = 0.0f;
        }

        i32 state;

        if ((f64)timer > (f64)unk_9c * (f64)safe_zone_wall_time)
            state = 0;
        else if ((f64)timer > (f64)unk_a0 * (f64)safe_zone_wall_time)
            state = 1;
        else
            state = 2;

        references::frontend.get().igo->zoom_map->set_marker_state(unk_d8->zoom_map_markers.begin()[first + point], state);
    }

    // the next wall once every timer of this one ran out
    if (expired >= safe_zone_walls[unk_d8->safe_zone_wall_index]->size)
        ++unk_d8->safe_zone_wall_index;

    return arrived;
}

// sub_94B390
void cross_neighborhood_spawn::update_arrival(bool arrived) { // marks the goto points while the hero is away; his arrival brings up the dialog or raises quest_event, once
    if (arrived) {
        for (finger_of_god** it = unk_d8->radar_markers.begin(); it != unk_d8->radar_markers.end(); it = unk_d8->radar_markers.erase(it))
            references::frontend.get().igo->destroy_fog(*it);

        for (u32* it = unk_d8->zoom_map_markers.begin(); it != unk_d8->zoom_map_markers.end(); it = unk_d8->zoom_map_markers.erase(it))
            references::frontend.get().igo->zoom_map->remove_poi(*it);

        if (arrival_dialog) {
            if (!arrival_dialog_armed)
                return;

            references::frontend.get().igo->pauseless_dialog->clear();

            references::frontend.get().igo->pauseless_dialog->set_button_2_text(410);
            references::frontend.get().igo->pauseless_dialog->show_button_2(true);
            references::frontend.get().igo->pauseless_dialog->set_button_1_text(234);
            references::frontend.get().igo->pauseless_dialog->show_button_1(true);

            references::frontend.get().igo->pauseless_dialog->set_title_by_id(3330);
            references::frontend.get().igo->pauseless_dialog->set_text("");
            references::frontend.get().igo->pauseless_dialog->select_button(2);
            references::frontend.get().igo->pauseless_dialog->igo_3d_dialog_widget::bring_up(1);

            dialog_callback_id = event_manager::add_default_callback(references::dialog_3d_selection.read(),
                                                                     (code_event_callback_function)retail::sub_920F60,
                                                                     this,
                                                                     false);

            arrival_dialog_armed = false;

            return;
        }

        if (!std::strncmp(quest_event.c_str(), "", mash::string::max_length) ||
            unk_d8->quest_event_raised ||
            retail::sub_6884D0((u32*)references::frontend.get().igo) || // the loading screen is up
            mission_manager::inst()->needs_to_malor() ||
            mission_manager::inst()->is_maloring())

            return;

        retail::sub_800160((u32*)quest_manager::inst(), 5, (i32)&quest_event, 0); // the quest event core, script category

        unk_d8->quest_event_raised = true;

        return;
    }

    arrival_dialog_armed = true;

    if (safe_zone_wall_time > 0.0f) {
        if (unk_d8->goto_points.size() == unk_d8->zoom_map_markers.size())
            return;

        for (spawn_point** it = unk_d8->goto_points.begin(); it != unk_d8->goto_points.end(); ++it) {
            u32 marker = references::frontend.get().igo->zoom_map->add_safe_zone_marker((*it)->position);

            unk_d8->zoom_map_markers.push_back(marker);

            i32 state = 0;

            // the new marker's timer, once every goto point has one
            if (unk_d8->safe_zone_wall_timers.size() == unk_d8->goto_points.size()) {
                if ((f64)unk_d8->safe_zone_wall_timers.begin()[unk_d8->zoom_map_markers.size() - 1] > (f64)unk_9c * (f64)safe_zone_wall_time)
                    state = 0;
                else if ((f64)unk_d8->safe_zone_wall_timers.begin()[unk_d8->zoom_map_markers.size() - 1] > (f64)unk_a0 * (f64)safe_zone_wall_time)
                    state = 1;
                else
                    state = 2;
            } else if (unk_d8->safe_zone_wall_timers.size() < unk_d8->zoom_map_markers.size())
                unk_d8->safe_zone_wall_timers.push_back(safe_zone_wall_time);

            references::frontend.get().igo->zoom_map->set_marker_state(marker, state);
        }

        return;
    }

    if (unk_d8->goto_points.size() == unk_d8->radar_markers.size())
        return;

    for (spawn_point** it = unk_d8->goto_points.begin(); it != unk_d8->goto_points.end(); ++it) {
        finger_of_god* fog = references::frontend.get().igo->create_fog((*it)->position, radius, 0.0f, false, false);

        unk_d8->radar_markers.push_back(fog);

        if (entity_tracker* tracker = fog->get_tracker()) {
            tracker->set_gta_widget_active(true, false);
            tracker->set_gta_widget_render_scale(radius);
        }
    }
}
