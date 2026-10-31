#include "retail.hh"
#include "game/spawn/quest_giver_spawn.hh"
#include "game/math/po.hh"
#include "game/quest_list.hh"
#include "game/wds/entity/entity.hh"

using namespace treyarch;

// sub_90F780
vector3d quest_giver_spawn::get_position() { // the spawned giver's, cached on the way
    if (!spawned_entity)
        return position;

    position = spawned_entity->my_abs_po->get_position();

    return spawned_entity->my_abs_po->get_position();
}

// sub_90F7E0
vector3d quest_giver_spawn::get_facing() {
    if (spawned_entity) {
        position = spawned_entity->my_abs_po->get_position();

        return spawned_entity->my_abs_po->get_z_facing();
    }

    if (location)
        return location->get_facing();

    return vector3d(1.0f, 0.0f, 0.0f);
}

// sub_7EA940
vector3d quest_giver_spawn::get_marker_offset() {
    return giver->marker_offset;
}

// sub_927990
void quest_giver_spawn::reselect_location() {
    quest_giver_location_item* chosen = nullptr;

    for (quest_giver_location_item** it = giver->locations.begin(); it != giver->locations.end(); ++it)
        if (retail::sub_7F84B0((u32*)(*it)->condition)) // quest_condition::evaluate
            chosen = *it;

    if (location == chosen)
        return;

    if (location)
        location->unk_028(this);

    location = chosen;

    if (!chosen)
        return;

    chosen->unk_024(this, true);

    position = location->get_position();
}
