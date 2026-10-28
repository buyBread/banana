#include "game/frontend/city_life_tracker.hh"
#include "game/frontend/entity_tracker.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/wds/entity/entity.hh"

using namespace treyarch;

// sub_6E02D0
vector3d city_life_tracker::get_center_pos() { // the mean position of the entities whose radar icon is shown
    f32 x     = 0.0f;
    f32 y     = 0.0f;
    f32 z     = 0.0f;
    i32 count = 0;

    for (u32 i = 0; i < entities.size(); ++i) {
        auto* ent = (entity*)entities[i].resolve();

        if (!ent)
            continue;

        entity_tracker_manager* manager = references::frontend.get().igo->entity_tracker_mgr;

        if (!manager)
            continue;

        entity_tracker* tracker = manager->id_to_ptr(manager->get_tracker_by_entity(ent->my_handle));

        if (tracker && tracker->mm_icon->is_shown) {
            vector3d position = ent->my_abs_po->get_position();

            ++count;
            x = position.x + x;
            y = position.y + y;
            z = position.z + z;
        }
    }

    if (!count)
        return vector3d(0.0f, 0.0f, 0.0f);

    return vector3d(x / (f32)count, y / (f32)count, z / (f32)count);
}

// sub_8836C0
void city_life_tracker::add_quest_event(mash::string name) {
    quest_events.push_back(name.to_upper());
    quest_events_changed = true;
}

// sub_698550
mash::string city_life_tracker::quest_event_at(i32 index) {
    return quest_events[index];
}
