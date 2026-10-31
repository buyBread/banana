#include <algorithm>

#include "retail.hh"
#include "vc8/vector.hh"
#include "treyarch/chuck/vm/vm_dynamic_array_manager.hh"
#include "treyarch/mm/mm.hh"
#include "game/arch_base_vhandle.hh"
#include "game/event/chuck_library_parameter_event.hh"
#include "game/event/event_manager.hh"
#include "game/math/po.hh"
#include "game/quest_list.hh"
#include "game/region_pack_manager.hh"
#include "game/region_spawn_manager.hh"
#include "game/spawn/active_spawn_item.hh"
#include "game/spawn/quest_giver_spawn.hh"
#include "game/spawn/spawn_table_manager.hh"
#include "game/spawn_table_interface.hh"
#include "game/spawn_point_list_collection.hh"
#include "game/static_spawn_item_collection.hh"
#include "game/wds/ai/ai_pf_region_graph.hh"
#include "game/wds/entity/entity.hh"
#include "game/wds/references.hh"
#include "game/wds/world_dynamics_system.hh"

using namespace treyarch;

// sub_92EFC0
spawn_item_collection* spawn_table_manager::find_neighborhood_set() {
    if (!unk_04c || !unk_04c->table_interface)
        return nullptr;

    mash::string name = region_spawn_manager::inst()->get_neighborhood_name();

    return unk_04c->table_interface->find_spawn_item_collection(name);
}

// sub_92F050
spawn_item_collection* spawn_table_manager::find_city_wide_set() {
    if (!unk_04c || !unk_04c->table_interface)
        return nullptr;

    mash::string name = region_spawn_manager::inst()->get_city_wide_set_name();

    return unk_04c->table_interface->find_spawn_item_collection(name);
}

// sub_92F0E0
spawn_item_collection* spawn_table_manager::find_meta_set() {
    if (!unk_04c || !unk_04c->table_interface)
        return nullptr;

    mash::string name = region_spawn_manager::inst()->get_meta_neighborhood_name();

    return unk_04c->table_interface->find_spawn_item_collection(name);
}

// sub_93D860
void spawn_table_manager::free_spawn_record(spawn_point* record) { // a borrowed record is only forgotten
    spawn_point** owned = std::find(owned_spawn_records.begin(), owned_spawn_records.end(), record);

    if (owned != owned_spawn_records.end()) {
        if ((*owned)->region_point) {
            delete (*owned)->region_point;

            (*owned)->region_point = nullptr;
        }

        owned_spawn_records.erase(owned);

        if (record) {
            record->data.clear();

            mm::free(record);
        }

        return;
    }

    spawn_point** borrowed = std::find(borrowed_spawn_records.begin(), borrowed_spawn_records.end(), record);

    if (borrowed != borrowed_spawn_records.end())
        borrowed_spawn_records.erase(borrowed);
}

// sub_92ED30
void spawn_table_manager::get_entities(chuck::vm::script_instance* inst,
                                       i32                         number,
                                       vc8::vector<entity*>*       found,
                                       mash::string*               name) {

    for (active_spawn_item** it = unk_04c->active_spawn_items.begin(); it != unk_04c->active_spawn_items.end(); ++it)
        if ((*it)->has_instance(inst)) {
            (*it)->get_entities(number, found, name);

            return;
        }
}

// sub_92ED90
vector3d* spawn_table_manager::get_spawn_position(vector3d* out, chuck::vm::script_instance* inst) { // out is left alone without a match
    for (active_spawn_item** it = unk_04c->active_spawn_items.begin(); it != unk_04c->active_spawn_items.end(); ++it)
        if ((*it)->has_instance(inst)) {
            *out = (*it)->get_spawn_position();

            return out;
        }

    return out;
}

// sub_92EE10
spawn_data_base* spawn_table_manager::find_optional_data(chuck::vm::script_instance* inst, mash::string name) { // the record's data first, then the item's
    for (active_spawn_item** it = unk_04c->active_spawn_items.begin(); it != unk_04c->active_spawn_items.end(); ++it)
        if ((*it)->has_instance(inst)) {
            spawn_data_base* data = (*it)->record->find_data(name);

            if (!data)
                data = (*it)->item->find_data(name);

            return data;
        }

    return nullptr;
}

// sub_92EEB0
bool spawn_table_manager::has_optional_data(chuck::vm::script_instance* inst) {
    for (active_spawn_item** it = unk_04c->active_spawn_items.begin(); it != unk_04c->active_spawn_items.end(); ++it)
        if ((*it)->has_instance(inst)) {
            bool record_data = (*it)->record->data.size > 0;

            if ((*it)->item->data.size > 0)
                return true;

            return record_data;
        }

    return false;
}

// sub_92EF10
void spawn_table_manager::update_point_of_interest(chuck::vm::script_instance* inst, mash::string name, vector3d position) {
    for (active_spawn_item** it = unk_04c->active_spawn_items.begin(); it != unk_04c->active_spawn_items.end(); ++it)
        if ((*it)->has_instance(inst)) {
            (*it)->update_point_of_interest(name, position);

            return;
        }
}

// sub_92F170
void spawn_table_manager::has_succeeded_city_life(chuck::vm::script_instance* inst) {
    for (active_spawn_item** it = unk_04c->active_spawn_items.begin(); it != unk_04c->active_spawn_items.end(); ++it) {
        if (!(*it)->has_instance(inst))
            continue;

        (*it)->unk_020(true);

        ++unk_04c->unk_138;
        unk_04c->unk_124 = 0.0f;
        unk_04c->unk_140 = 0.0f;

        void* storage = mm::get_default_space()->allocate(sizeof(chuck_sle_city_life_completed_t), 0);

        auto* completed = storage ? (chuck_sle_city_life_completed_t*)retail::sub_8598C0((u32*)storage) : nullptr;

        completed->spawn_id = (f32)(u32)(*it)->spawn_id;

        event_manager::raise_event(completed, arch_base_vhandle());

        delete completed;

        return;
    }
}

// sub_92F290
void spawn_table_manager::force_despawn_city_life(chuck::vm::script_instance* inst) {
    for (active_spawn_item** it = unk_04c->active_spawn_items.begin(); it != unk_04c->active_spawn_items.end(); ++it)
        if ((*it)->has_instance(inst)) {
            (*it)->forced_despawn = true;
            (*it)->unk_020(true);
        }
}

// sub_92F2E0
bool spawn_table_manager::is_valid_spawn(i32 spawn_id) {
    for (active_spawn_item** it = unk_04c->active_spawn_items.begin(); it != unk_04c->active_spawn_items.end(); ++it)
        if ((*it)->spawn_id == spawn_id)
            return true;

    return false;
}

// inlined @ sub_92F340
// inlined @ sub_92F3F0
cross_neighborhood_spawn* spawn_table_manager::find_cross_neighborhood_spawn(chuck::vm::script_instance* inst) { // the plain list first, then the quest one
    for (cross_neighborhood_spawn** it = unk_04c->cross_neighborhood_spawns.begin(); it != unk_04c->cross_neighborhood_spawns.end(); ++it)
        if ((*it)->unk_d8->active && (*it)->unk_d8->active->has_instance(inst))
            return *it;

    for (cross_neighborhood_spawn** it = unk_04c->quest_cross_neighborhood_spawns.begin(); it != unk_04c->quest_cross_neighborhood_spawns.end(); ++it)
        if ((*it)->unk_d8->active && (*it)->unk_d8->active->has_instance(inst))
            return *it;

    return nullptr;
}

// sub_92F340
void spawn_table_manager::set_safe_zone_wall_index(chuck::vm::script_instance* inst, i32 index) {
    cross_neighborhood_spawn* spawn = find_cross_neighborhood_spawn(inst);

    if (spawn)
        spawn->unk_d8->safe_zone_wall_index = index;
}

// sub_92F3F0
i32 spawn_table_manager::get_safe_zone_wall_index(chuck::vm::script_instance* inst) {
    cross_neighborhood_spawn* spawn = find_cross_neighborhood_spawn(inst);

    if (spawn)
        return spawn->unk_d8->safe_zone_wall_index;

    return -1;
}

// sub_92F4A0
vector3d* spawn_table_manager::get_neighborhood_center(vector3d* out, chuck::vm::script_instance* inst) { // out is left alone without a centre
    for (active_spawn_item** it = unk_04c->active_spawn_items.begin(); it != unk_04c->active_spawn_items.end(); ++it)
        if ((*it)->has_instance(inst)) {
            if ((*it)->neighborhood_center)
                *out = *(*it)->neighborhood_center;

            return out;
        }

    return out;
}

// sub_92F510
vector3d* spawn_table_manager::get_meta_neighborhood_center(vector3d* out, chuck::vm::script_instance* inst) { // out is left alone without a centre
    for (active_spawn_item** it = unk_04c->active_spawn_items.begin(); it != unk_04c->active_spawn_items.end(); ++it)
        if ((*it)->has_instance(inst)) {
            if ((*it)->meta_neighborhood_center)
                *out = *(*it)->meta_neighborhood_center;

            return out;
        }

    return out;
}

// sub_92F580
void spawn_table_manager::get_current_flagsets(chuck::vm::script_instance* inst, chuck::vm::script_dynamic_array<u32>* flagsets) {
    for (active_spawn_item** it = unk_04c->active_spawn_items.begin(); it != unk_04c->active_spawn_items.end(); ++it)
        if ((*it)->has_instance(inst))
            (*it)->get_current_flagsets(flagsets);
}

// sub_92F5D0
void spawn_table_manager::flag_always_on_for_despawn(chuck::vm::script_instance* inst) { // the cast isn't checked
    for (active_spawn_item** it = unk_04c->active_spawn_items.begin(); it != unk_04c->active_spawn_items.end(); ++it)
        if ((*it)->has_instance(inst)) {
            // __RTDynamicCast
            auto* always_on = (always_on_scripted_spawn_item*)retail::sub_ADE86C((*it)->item,
                                                                                 0,
                                                                                 &references::static_spawn_item_type_descriptor.get(),
                                                                                 &references::always_on_scripted_spawn_item_type_descriptor.get(),
                                                                                 false);

            always_on->always_on_for_despawn = true;
        }
}

// sub_939120
bool spawn_table_manager::has_persistence_data(chuck::vm::script_instance* inst) {
    for (auto* node = unk_04c->active_persistence_data.begin(); node != unk_04c->active_persistence_data.end(); node = node->next())
        if (node->value.first->has_instance(inst))
            return true;

    return false;
}

// sub_951FC0
void spawn_table_manager::get_all_categories_on_city_life(i32 spawn_id, chuck::vm::script_dynamic_array<u32>* categories) {
    active_spawn_item** found = std::find_if(unk_04c->active_spawn_items.begin(),
                                             unk_04c->active_spawn_items.end(),
                                             [spawn_id](active_spawn_item* active) { return active->spawn_id == spawn_id; });

    if (found == unk_04c->active_spawn_items.end())
        return;

    // each cell carries the category's own characters, not a thread string
    for (mash::string** it = (*found)->item->categories.begin(); it != (*found)->item->categories.end(); ++it) {
        chuck::vm::chuck_dynamic_array_element_t element;
        *(const char**)element.stuff = (*it)->c_str();

        categories->array->push_back(element);
    }
}

// sub_952040
void spawn_table_manager::get_spawn_point_point_data(chuck::vm::script_instance*                inst,
                                                     chuck::vm::script_dynamic_array<vector3d>* points) {

    for (active_spawn_item** it = unk_04c->active_spawn_items.begin(); it != unk_04c->active_spawn_items.end(); ++it) {
        if (!(*it)->has_instance(inst))
            continue;

        base_region_point* region_point = (*it)->record->region_point;

        if (!region_point ||
            (region_point->kind != region_point_kind_wall && region_point->kind != region_point_kind_hand_selected_wall))

            return;

        // __RTDynamicCast, unchecked
        auto* wall = (region_wall_point*)retail::sub_ADE86C(region_point,
                                                            0,
                                                            &references::base_region_point_type_descriptor.get(),
                                                            &references::region_wall_point_type_descriptor.get(),
                                                            false);

        chuck::vm::chuck_dynamic_array_element_t element;

        *(vector3d*)element.stuff = wall->unk_14;
        points->array->push_back(element);

        *(vector3d*)element.stuff = wall->unk_20;
        points->array->push_back(element);

        *(vector3d*)element.stuff = wall->unk_2c;
        points->array->push_back(element);

        return;
    }
}

// sub_920990
bool spawn_table_manager::is_spidey_near_quest_giver(f32 radius) {
    quest_giver_spawn* spawn = unk_04c->chosen_quest_giver_spawn;

    if (!spawn || region_pack_manager::inst()->quest_giver_locked_in_mission)
        return false;

    vector3d hero  = references::g_world_ptr.read()->get_hero_ptr()->my_abs_po->get_position();
    vector3d giver = spawn->get_position();

    f32 dx = (f32)((f64)hero.x - (f64)giver.x);
    f32 dy = (f32)((f64)hero.y - (f64)giver.y);
    f32 dz = (f32)((f64)hero.z - (f64)giver.z);

    f32 distance_squared = (f32)((f64)dx * dx + (f64)dy * dy + (f64)dz * dz);
    f32 radius_squared   = (f32)((f64)radius * radius);

    return radius_squared > distance_squared;
}

// sub_92F630
bool spawn_table_manager::get_nearest_quest_giver_restart_position(vector3d* position, vector3d* facing) { // in front of the nearest giver, facing it
    vector3d hero = references::g_world_ptr.read()->get_hero_ptr()->my_abs_po->get_position();

    f32                nearest = -1.0f;
    quest_giver_spawn* chosen  = nullptr;

    for (quest_giver_spawn** it = unk_04c->quest_giver_spawns.begin(); it != unk_04c->quest_giver_spawns.end(); ++it) {
        quest_giver_spawn* spawn = *it;

        if (!spawn->location)
            continue;

        if (spawn->giver && spawn->giver->unk_2c)
            continue;

        vector3d giver = spawn->get_position();

        // the giver's height counts from 0
        f32 dx = (f32)((f64)giver.x - (f64)hero.x);
        f32 dy = (f32)(0.0 - (f64)hero.y);
        f32 dz = (f32)((f64)giver.z - (f64)hero.z);

        f32 distance_squared = (f32)((f64)dx * dx + (f64)dy * dy + (f64)dz * dz);

        if ((0.0f > nearest || nearest > distance_squared) && spawn->enabled) {
            nearest = distance_squared;
            chosen  = spawn;
        }
    }

    if (!chosen)
        return false;

    *position = chosen->get_position();

    vector3d away = chosen->get_facing();

    facing->x = (f32)((f64)away.x * -1.0);
    facing->y = (f32)((f64)away.y * -1.0);
    facing->z = (f32)((f64)away.z * -1.0);

    vector3d ahead = chosen->get_facing();

    f32 step_x = (f32)((f64)ahead.x * 3.0);
    f32 step_y = (f32)((f64)ahead.y * 3.0);
    f32 step_z = (f32)((f64)ahead.z * 3.0);

    position->x = (f32)((f64)position->x + (f64)step_x);
    position->y = (f32)((f64)position->y + (f64)step_y);
    position->z = (f32)((f64)position->z + (f64)step_z);

    return true;
}
