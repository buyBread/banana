#include "retail.hh"
#include "game/frontend/finger_of_god.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/trigger.hh"
#include "game/wds/entity/entity_base.hh"
#include "game/wds/references.hh"
#include "game/wds/world_dynamics_system.hh"

using namespace treyarch;

// sub_6AFA00
finger_of_god::finger_of_god(vector3d initial_position,
                             f32      initial_radius,
                             f32      initial_unk_2c,
                             bool     initial_unk_32,
                             bool     initial_unk_33) : position(initial_position),
                                                        poi_index(-1),
                                                        tracker_id((entity_tracker_id)-1),
                                                        sphere_trigger(nullptr),
                                                        radius(initial_radius),
                                                        unk_28(0),
                                                        unk_2c(initial_unk_2c),
                                                        poi_active(false),
                                                        active(false),
                                                        unk_32(initial_unk_32),
                                                        unk_33(initial_unk_33) {}

// sub_6AF9A0
finger_of_god::finger_of_god(bool initial_unk_32) : position(0.0f, 0.0f, 0.0f),
                                                    poi_index(-1),
                                                    tracker_id((entity_tracker_id)-1),
                                                    sphere_trigger(nullptr),
                                                    radius(0.0f),
                                                    unk_28(0),
                                                    unk_2c(0.0f),
                                                    poi_active(false),
                                                    active(false),
                                                    unk_32(initial_unk_32),
                                                    unk_33(false) {}

// sub_6E1440
finger_of_god::~finger_of_god() {
    if (!ent.null()) {
        if (ui_frontend* igo = references::frontend.get().igo)
            retail::sub_6D9260((i32)igo, ent.value); // target icon removal

        retail::sub_7FBF20((u32*)&references::g_world_ptr.read()->ent_mgr, (i32*)ent.resolve()); // wds_entity_manager::destroy_entity

        ent = arch_base_vhandle();
    }

    if (tracker_id != (entity_tracker_id)-1) {
        references::frontend.get().igo->entity_tracker_mgr->destroy_entity_tracker(tracker_id);

        tracker_id = (entity_tracker_id)-1;
    }

    if (poi_index >= 0) {
        references::frontend.get().igo->zoom_map->remove_poi((u32)poi_index);

        poi_index = -1;
    }
}

// slot 52
bool finger_of_god::is_a_finger_of_god() const {
    return ((bool (__thiscall*)(const finger_of_god*))retail::sub_667F80)(this);
}

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
