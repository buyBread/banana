#include <cstring>

#include "retail.hh"
#include "game/spawn/quest_giver_spawn.hh"
#include "game/amalga/resource_manager.hh"
#include "game/math/po.hh"
#include "game/quest_list.hh"
#include "game/quest_manager.hh"
#include "game/wds/ai/ai_basic_inode.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/info_node.hh"
#include "game/wds/entity/actor.hh"
#include "game/wds/entity/entity.hh"
#include "game/wds/references.hh"
#include "game/wds/world_dynamics_system.hh"
#include "treyarch/mash/string_hash.hh"

namespace treyarch {
    using find_pack_slot_function = amalga::resource_pack_slot* (__cdecl*)(mash::string pack_name); // partition 3, then 10; owns the string
} // treyarch

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

// sub_905D70
mash::string quest_giver_spawn::get_pack_name() {
    return giver->pack_name;
}

// sub_905DA0
mash::string quest_giver_spawn::get_entity_type() {
    return giver->entity_type;
}

// sub_905DD0
bool quest_giver_spawn::giver_unk_2c() {
    return giver && giver->unk_2c;
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

// sub_920440
void quest_giver_spawn::spawn_entity() { // made inside the giver's pack, placed at the location and registered with the quest manager
    if (!location)
        return;

    amalga::resource_pack_slot* slot = giver && giver->unk_2c ?
        amalga::resource_manager::get_best_context(amalga::resource_partition_common) :
        ((find_pack_slot_function)retail::sub_90AAF0)(get_pack_name());

    amalga::resource_context_stack_object context((amalga::resource_pack_slot*)retail::sub_739BC0((i32)slot));

    string_hash entity_name;

    const char* name   = giver->name.c_str();
    u32         length = (u32)std::strlen(name);

    // a trailing '*' takes the next free name of the rest
    if (name[length - 1] == '*') {
        mash::string prefix(mash::string(name), 0, (i32)length - 1);

        string_hash next;
        entity_name = *(string_hash*)retail::sub_64CCA0(&next.source_hash_code, retail::sub_64CBB0((char*)&prefix), (void*)"");
    } else {
        string_hash hash;
        hash.initialize(mash::ALLOCATED, name);

        entity_name = hash;
    }

    mash::string type_name = get_entity_type();

    string_hash type;
    type.initialize(mash::ALLOCATED, type_name.c_str());

    spawned_entity = (entity*)retail::sub_8047E0((u32*)&references::g_world_ptr.read()->ent_mgr,
                                                 (i32)type.source_hash_code,
                                                 &entity_name.source_hash_code,
                                                 1);

    if (!spawned_entity)
        return;

    spawned_entity->ext_flags |= entity_ext_flag_invulnerable;

    po placement = *spawned_entity->my_abs_po;
    placement.set_position(position);

    vector3d facing = location->get_facing();

    placement.set_facing(math::vector4(facing.x, facing.y, facing.z, 0.0f));
    placement.fixup();

    spawned_entity->set_rel_po(placement);

    location->unk_024(this, true);

    if (!spawned_entity)
        return;

    quest_manager::inst()->giver_register_entity((actor*)spawned_entity, giver && giver->unk_2c);

    if (giver && giver->conversation != "" && !giver_unk_2c())
        retail::sub_7FF430((u32*)quest_manager::inst(), (i32)spawned_entity, (i32)&giver->conversation, 0); // attach_quest_conversation

    if (!spawned_entity->is_an_actor())
        return;

    ai_core* core = ((actor*)spawned_entity)->get_ai_core();

    if (!core)
        return;

    auto* basic = (ai_basic_inode*)core->get_info_node(info_node_type_ai_basic);

    if (!basic)
        return;

    basic->set_target_entity(11, references::g_world_ptr.read()->get_hero_ptr());
    basic->set_flag(394, true);
    basic->set_flag(104, true);
}

// sub_90F860
// inlined @ sub_92E970
void quest_giver_spawn::despawn_entity() {
    if (!spawned_entity)
        return;

    retail::sub_7FC070((u32*)quest_manager::inst(), (i32*)spawned_entity);                       // unregister_quest_giver
    retail::sub_7FBE10((u32*)&references::g_world_ptr.read()->ent_mgr, (i32*)spawned_entity); // wds_entity_manager destroy

    spawned_entity = nullptr;
}
