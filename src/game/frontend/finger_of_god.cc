#include "retail.hh"
#include "game/frontend/finger_of_god.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/trigger.hh"
#include "game/wds/entity/entity_base.hh"

using namespace treyarch;

// sub_6CA350
entity_tracker* finger_of_god::get_tracker() {
    if (tracker_id == (entity_tracker_id)-1)
        return nullptr;

    entity_tracker_manager* manager = references::frontend.get().igo->entity_tracker_mgr;

    return manager ? manager->id_to_ptr(tracker_id) : nullptr;
}

// sub_6CA2A0
void finger_of_god::set_pos(vector3d new_position) {
    if (poi_index >= 0) {
        igo_3d_zoom_map* zoom_map = references::frontend.get().igo->zoom_map;

        if (zoom_map && (u32)poi_index <= 0x1C1)
            zoom_map->pois[poi_index].position = new_position;
    }

    if (!ent.null())
        if (auto moved = (entity_base*)ent.resolve())
            moved->set_rel_position(new_position);

    position = new_position;
}

// sub_6948C0
void finger_of_god::set_radius(f32 new_radius) {
    if (new_radius < 0.0f)
        return;

    if (sphere_trigger)
        ((void (__thiscall*)(trigger*, f32))retail::sub_7456B0)(sphere_trigger, new_radius); // sphere_trigger::set_radius

    radius = new_radius;
}

// sub_6D48B0
void finger_of_god::set_poi_active(bool shown) {
    if (entity_tracker* tracker = get_tracker()) {
        tracker->set_poi_active(shown);
        tracker->set_fog_active(shown);
        tracker->mm_icon->is_shown = shown;
    }

    igo_3d_zoom_map* zoom_map = references::frontend.get().igo->zoom_map;

    if (zoom_map)
        zoom_map->set_poi_shown((u32)poi_index, shown);

    poi_active = shown;
}

// sub_6D4950
void finger_of_god::set_active(bool new_active) {
    active = new_active;
    unk_28 = 0;

    set_poi_active(new_active);
}

// sub_6F7640
void finger_of_god::set_poi_icon(i32 icon, i32) {
    if (!ent.null())
        retail::sub_6F6400((u32*)references::frontend.get().igo, ent.value, icon); // ui_frontend::set_target_icon
}

// sub_6D49A0
void finger_of_god::fog_requires_senses(bool requires_senses) {
    if (entity_tracker* tracker = get_tracker())
        tracker->set_fog_requires_senses(requires_senses);
}

// sub_6948F0
void finger_of_god::arrived_at_fog() {
    raise_event(references::reached_fog.read());
}
