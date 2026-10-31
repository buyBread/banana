#include <algorithm>
#include <cstring>
#include <new>

#include "retail.hh"
#include "vc8/vector.hh"
#include "treyarch/chuck/vm/vm_dynamic_array_manager.hh"
#include "treyarch/mm/mm.hh"
#include "game/arch_base_vhandle.hh"
#include "game/event/chuck_library_parameter_event.hh"
#include "game/event/event_manager.hh"
#include "game/math/po.hh"
#include "game/mission/mission_manager.hh"
#include "game/mission/mission_memory_manager.hh"
#include "game/quest_list.hh"
#include "game/region_pack_manager.hh"
#include "game/region_spawn_manager.hh"
#include "game/spawn/active_spawn_item.hh"
#include "game/spawn/quest_giver_spawn.hh"
#include "game/spawn/spawn_table_manager.hh"
#include "game/spawn_item_collection.hh"
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

// sub_938FD0
void spawn_table_manager::clear_spawn_item_lists() {
    unk_04c->spawn_items              .clear();
    unk_04c->random_spawn_items       .clear();
    unk_04c->always_on_spawn_items    .clear();
    unk_04c->cross_neighborhood_spawns.clear();
    unk_04c->friendly_spawn_items     .clear();
}

// sub_948280
void spawn_table_manager_unk_04c::add_spawn_items_by_name(mash::vector<mash::string>* names) { // each item files itself by its own answers
    for (mash::string** name = names->begin(); name != names->end(); ++name) {
        static_spawn_item** added = std::find_if(spawn_items.begin(),
                                                 spawn_items.end(),
                                                 [name](static_spawn_item* item) { return !std::strncmp(item->name.c_str(), (*name)->c_str(), mash::string::max_length); });

        if (added != spawn_items.end())
            continue;

        static_spawn_item* item;

        {
            mash::string key = **name;

            item = (*table_interface->spawn_items_by_name)[key];
        }

        if (!item)
            continue;

        if (item->is_random())
            random_spawn_items.push_back(item);
        else if (item->is_cross_neighborhood()) {
            if (std::find(cross_neighborhood_spawns.begin(), cross_neighborhood_spawns.end(), item) != cross_neighborhood_spawns.end())
                continue;

            // __RTDynamicCast
            auto* spawn = (cross_neighborhood_spawn*)retail::sub_ADE86C(item,
                                                                        0,
                                                                        &references::static_spawn_item_type_descriptor.get(),
                                                                        &references::cross_neighborhood_spawn_type_descriptor.get(),
                                                                        false);

            cross_neighborhood_spawns.push_back(spawn);
        } else if (item->is_friendly())
            friendly_spawn_items.push_back(item);
        else if (item->is_always_on())
            always_on_spawn_items.push_back(item);
        else
            spawn_items.push_back(item);
    }
}

// inlined @ sub_952A80
void spawn_table_manager_unk_04c::destroy_actives_of(vc8::vector<active_spawn_item*>* list, static_spawn_item* item) {
    for (active_spawn_item** it = list->begin(); it != list->end();) {
        active_spawn_item* active = *it;

        if (active->item != item) {
            ++it;
            continue;
        }

        it = spawn_table_manager::inst()->deactivate_item(list, active, true);

        delete active;
    }
}

// sub_952A80
void spawn_table_manager_unk_04c::remove_spawn_items_by_name(mash::vector<mash::string>* names) { // "ALL" empties every list instead
    for (mash::string** name = names->begin(); name != names->end(); ++name) {
        if (!std::strncmp((*name)->c_str(), "ALL", mash::string::max_length)) {
            spawn_table_manager::inst()->clear_spawn_item_lists();

            return;
        }

        for (static_spawn_item** it = spawn_items.begin(); it != spawn_items.end();)
            if (!std::strncmp((*it)->name.c_str(), (*name)->c_str(), mash::string::max_length))
                it = spawn_items.erase(it);
            else
                ++it;

        for (static_spawn_item** it = random_spawn_items.begin(); it != random_spawn_items.end();)
            if (!std::strncmp((*it)->name.c_str(), (*name)->c_str(), mash::string::max_length))
                it = random_spawn_items.erase(it);
            else
                ++it;

        static_spawn_item** always_on = always_on_spawn_items.begin();

        while (always_on != always_on_spawn_items.end()) {
            if (std::strncmp((*always_on)->name.c_str(), (*name)->c_str(), mash::string::max_length)) {
                ++always_on;
                continue;
            }

            destroy_actives_of(&active_spawn_items,         *always_on);
            destroy_actives_of(&pending_active_spawn_items, *always_on);

            always_on = always_on_spawn_items.erase(always_on);
        }

        for (cross_neighborhood_spawn** it = cross_neighborhood_spawns.begin(); it != cross_neighborhood_spawns.end();) {
            if (std::strncmp((*it)->name.c_str(), (*name)->c_str(), mash::string::max_length)) {
                ++it;
                continue;
            }

            destroy_actives_of(&active_spawn_items, *it);

            // matches against whatever sits at the always-on list's end, not this spawn
            destroy_actives_of(&pending_active_spawn_items, *always_on);

            if ((*it)->unk_d8)
                (*it)->unk_d8->quest_event_raised = false;

            it = cross_neighborhood_spawns.erase(it);
        }

        for (static_spawn_item** it = friendly_spawn_items.begin(); it != friendly_spawn_items.end();) {
            if (std::strncmp((*it)->name.c_str(), (*name)->c_str(), mash::string::max_length)) {
                ++it;
                continue;
            }

            destroy_actives_of(&active_spawn_items,         *it);
            destroy_actives_of(&pending_active_spawn_items, *it);

            it = friendly_spawn_items.erase(it);
        }
    }
}

// sub_92E7C0
void spawn_table_manager_unk_04c::update_quest_spawn_counts() {
    f32 share = (f32)((f64)0.05f * (f64)(u32)(quest_spawn_items.size() - 1)) + 0.5f;

    if (share > 0.5f)
        share = 0.5f;

    bool uncapped = false;
    i32  cap      = 0;

    for (static_spawn_item** it = quest_spawn_items.begin(); it != quest_spawn_items.end(); ++it) {
        if (!(*it)->is_scripted())
            continue;

        // __RTDynamicCast
        auto* scripted = (scripted_spawn_item*)retail::sub_ADE86C(*it,
                                                                  0,
                                                                  &references::static_spawn_item_type_descriptor.get(),
                                                                  &references::scripted_spawn_item_type_descriptor.get(),
                                                                  false);

        if (!scripted)
            continue;

        if (!scripted->spawn_limit)
            uncapped = true;

        cap += scripted->spawn_limit;
    }

    for (static_spawn_item** it = quest_always_on_spawn_items.begin(); it != quest_always_on_spawn_items.end(); ++it) {
        if (!(*it)->is_scripted())
            continue;

        // __RTDynamicCast
        auto* scripted = (scripted_spawn_item*)retail::sub_ADE86C(*it,
                                                                  0,
                                                                  &references::static_spawn_item_type_descriptor.get(),
                                                                  &references::scripted_spawn_item_type_descriptor.get(),
                                                                  false);

        if (!scripted)
            continue;

        if (!scripted->spawn_limit)
            uncapped = true;

        cap += scripted->spawn_limit;
    }

    quest_spawn_uncapped = uncapped;
    quest_spawn_cap      = cap + quest_cross_neighborhood_spawns.size();
    quest_spawn_share    = share;

    if (!quest_spawn_items.size())
        quest_spawn_share = 0.0f;
}

// sub_948560
void spawn_table_manager::add_quest_spawn_item(mash::string name) {
    static_spawn_item* item = unk_04c->table_interface->find_spawn_item(name);

    if (!item)
        return;

    if (item->is_cross_neighborhood()) {
        // __RTDynamicCast
        auto* spawn = (cross_neighborhood_spawn*)retail::sub_ADE86C(item,
                                                                    0,
                                                                    &references::static_spawn_item_type_descriptor.get(),
                                                                    &references::cross_neighborhood_spawn_type_descriptor.get(),
                                                                    false);

        unk_04c->quest_cross_neighborhood_spawns.push_back(spawn);
    } else if (item->is_friendly())
        unk_04c->friendly_spawn_items.push_back(item);
    else if (item->is_always_on())
        unk_04c->quest_always_on_spawn_items.push_back(item);
    else
        unk_04c->quest_spawn_items.push_back(item);

    unk_04c->update_quest_spawn_counts();
}

// inlined @ sub_9530D0
void spawn_table_manager::deactivate_actives_of(vc8::vector<active_spawn_item*>* list, static_spawn_item* item) {
    for (active_spawn_item** it = list->begin(); it != list->end();)
        if ((*it)->item == item)
            it = deactivate_item(list, *it, false);
        else
            ++it;
}

// sub_9530D0
void spawn_table_manager::remove_quest_spawn_item(mash::string name) { // the actives are deactivated, not destroyed
    for (static_spawn_item** it = unk_04c->quest_spawn_items.begin(); it != unk_04c->quest_spawn_items.end(); ++it)
        if (!std::strncmp((*it)->name.c_str(), name.c_str(), mash::string::max_length)) {
            unk_04c->quest_spawn_items.erase(it);

            break;
        }

    for (static_spawn_item** it = unk_04c->quest_always_on_spawn_items.begin(); it != unk_04c->quest_always_on_spawn_items.end();) {
        if (std::strncmp((*it)->name.c_str(), name.c_str(), mash::string::max_length)) {
            ++it;
            continue;
        }

        if ((*it)->is_always_on()) {
            deactivate_actives_of(&unk_04c->active_spawn_items,         *it);
            deactivate_actives_of(&unk_04c->pending_active_spawn_items, *it);
        }

        it = unk_04c->quest_always_on_spawn_items.erase(it);
    }

    for (cross_neighborhood_spawn** it = unk_04c->quest_cross_neighborhood_spawns.begin(); it != unk_04c->quest_cross_neighborhood_spawns.end();) {
        if (std::strncmp((*it)->name.c_str(), name.c_str(), mash::string::max_length)) {
            ++it;
            continue;
        }

        deactivate_actives_of(&unk_04c->active_spawn_items,         *it);
        deactivate_actives_of(&unk_04c->pending_active_spawn_items, *it);

        if ((*it)->unk_d8)
            (*it)->unk_d8->quest_event_raised = false;

        (*it)->remove_map_markers();

        it = unk_04c->quest_cross_neighborhood_spawns.erase(it);
    }

    unk_04c->update_quest_spawn_counts();
}

// sub_948BA0
void spawn_table_manager::push_spawn_item_during_mission(mash::string name) { // a scripted item's pack is pushed for the running mission
    static_spawn_item* item = unk_04c->table_interface->find_spawn_item(name);

    unk_04c->spawn_items.push_back(item);

    if (!item->is_scripted())
        return;

    mash::string* pack = &((scripted_spawn_item*)item)->script_name;

    if (retail::sub_83F720((u32*)mission_memory_manager::inst(), (i32)pack))
        return;

    mash::string owner(mission_manager::inst()->get_running_mission_name());

    retail::sub_87D610((i32)mission_memory_manager::inst(), (i32*)&owner, (i32)pack, 1);
}

// sub_93F8D0
void spawn_table_manager::pop_spawn_item_during_mission(mash::string name) { // an unlisted item takes the last one with it
    static_spawn_item* item = unk_04c->table_interface->find_spawn_item(name);

    static_spawn_item** listed = std::find(unk_04c->spawn_items.begin(), unk_04c->spawn_items.end(), item);

    if (item->is_scripted()) {
        mash::string* pack = &((scripted_spawn_item*)item)->script_name;

        if (retail::sub_83F720((u32*)mission_memory_manager::inst(), (i32)pack)) {
            mash::string owner(mission_manager::inst()->get_running_mission_name());

            retail::sub_87D730((i32)mission_memory_manager::inst(), (i32*)&owner, (i32)pack, 1);
        }
    }

    unk_04c->spawn_items.erase(listed);
}

// sub_951E80
active_spawn_item** spawn_table_manager::deactivate_item(vc8::vector<active_spawn_item*>* list, active_spawn_item* active, bool flag) { // returns the position after it; the active isn't destroyed
    if (unk_04c->active_persistence_data[active]) {
        persistence_data* persistence = unk_04c->active_persistence_data[active];

        for (auto* node = persistence->records.begin(); node != persistence->records.end();) {
            delete node->value.second;

            node = persistence->records.erase(node);
        }

        persistence->~persistence_data();

        mm::free(persistence);
    }

    active->unk_008(true);

    retail::sub_934680((u32*)region_pack_manager::inst(), (i32)active, false);

    if (!active->item->is_always_on() && !active->item->is_friendly()) {
        if (active->unk_25 && flag)
            --unk_04c->unk_008;
        else
            --unk_04c->unk_010;
    }

    // a miss erases at the end
    return list->erase(std::find(list->begin(), list->end(), active));
}

// sub_953060
void spawn_table_manager::force_populate_mission_cleanup() {
    for (active_spawn_item** it = unk_04c->active_spawn_items.begin(); it != unk_04c->active_spawn_items.end();)
        it = deactivate_item(&unk_04c->active_spawn_items, *it, true);

    for (active_spawn_item** it = unk_04c->pending_active_spawn_items.begin(); it != unk_04c->pending_active_spawn_items.end();)
        it = deactivate_item(&unk_04c->pending_active_spawn_items, *it, true);

    unk_04c->unk_010 = 0;
    unk_04c->unk_008 = 0;
}

// sub_939240
void spawn_table_manager::clear_spawn_lists_and_markers() {
    for (cross_neighborhood_spawn** it = unk_04c->quest_cross_neighborhood_spawns.begin(); it != unk_04c->quest_cross_neighborhood_spawns.end();) {
        if ((*it)->unk_d8)
            (*it)->unk_d8->quest_event_raised = false;

        (*it)->remove_map_markers();

        it = unk_04c->quest_cross_neighborhood_spawns.erase(it);
    }

    for (cross_neighborhood_spawn** it = unk_04c->cross_neighborhood_spawns.begin(); it != unk_04c->cross_neighborhood_spawns.end();) {
        if ((*it)->unk_d8)
            (*it)->unk_d8->quest_event_raised = false;

        (*it)->remove_map_markers();

        it = unk_04c->cross_neighborhood_spawns.erase(it);
    }

    unk_04c->always_on_spawn_items      .clear();
    unk_04c->random_spawn_items         .clear();
    unk_04c->quest_spawn_items          .clear();
    unk_04c->quest_always_on_spawn_items.clear();
}

// sub_950110
void spawn_table_manager::delete_item_maps() { // every area goes with its map
    if (unk_04c->no_spawn_zones) {
        for (auto* node = unk_04c->no_spawn_zones->begin(); node != unk_04c->no_spawn_zones->end();) {
            no_spawn_base* zone = node->value.second;

            node = unk_04c->no_spawn_zones->erase(node);

            delete zone;
        }

        unk_04c->no_spawn_zones->clear();

        if (unk_04c->no_spawn_zones) {
            unk_04c->no_spawn_zones->~map();

            mm::free(unk_04c->no_spawn_zones);
        }

        unk_04c->no_spawn_zones = nullptr;
    }

    if (unk_04c->arbitrary_regions) {
        for (auto* node = unk_04c->arbitrary_regions->begin(); node != unk_04c->arbitrary_regions->end();) {
            no_spawn_base* region = node->value.second;

            node = unk_04c->arbitrary_regions->erase(node);

            delete region;
        }

        unk_04c->arbitrary_regions->clear();

        if (unk_04c->arbitrary_regions) {
            unk_04c->arbitrary_regions->~map();

            mm::free(unk_04c->arbitrary_regions);
        }

        unk_04c->arbitrary_regions = nullptr;
    }
}

// sub_93D770
void spawn_table_manager::clear_debris_no_spawn_zones() {
    mash::vector<no_spawn_base>* zones = references::debris_no_spawn_zones.read();

    if (zones) {
        zones->clear();

        mm::free(zones);
    }

    references::debris_no_spawn_zones.write(nullptr);
}

// sub_9585E0
void spawn_table_manager::reset_city_life() {
    for (cross_neighborhood_spawn** it = unk_04c->quest_cross_neighborhood_spawns.begin(); it != unk_04c->quest_cross_neighborhood_spawns.end(); ++it)
        (*it)->remove_map_markers();

    for (cross_neighborhood_spawn** it = unk_04c->cross_neighborhood_spawns.begin(); it != unk_04c->cross_neighborhood_spawns.end(); ++it)
        (*it)->remove_map_markers();

    force_populate_mission_cleanup();
    clear_spawn_item_lists();
    delete_item_maps();

    spawn_placement_figures[0] = 90.0f;
    spawn_placement_figures[1] = 200.0f;
    spawn_placement_figures[2] = 300.0f;
    spawn_placement_figures[3] = 24.0f;

    collectibles_disabled = false;

    if (unk_04c->city_life_categories) {
        unk_04c->city_life_categories->~map();

        mm::free(unk_04c->city_life_categories);

        unk_04c->city_life_categories = nullptr;
    }

    region_pack_manager* packs = region_pack_manager::inst();

    if (packs->quest_giver_locked_in_mission) {
        packs->despawn_and_lock_quest_giver_in_mission(false);

        if (!retail::sub_90F6E0((i32)region_pack_manager::inst(), (i32)unk_04c->chosen_quest_giver_spawn))
            unk_04c->chosen_quest_giver_spawn = nullptr;
    }

    if (unk_04c->mission_pacing_override) {
        unk_04c->mission_pacing_override->~spawn_item_collection_unk_38();

        mm::free(unk_04c->mission_pacing_override);
    }

    unk_04c->mission_pacing_override = nullptr;

    unk_048 = false;
}

// sub_927A20
void spawn_table_manager::toggle_override_spawn_pacing_in_mission(bool on,
                                                                  u32  low_spawn_count,
                                                                  u32  high_spawn_count,
                                                                  f32  ramp_seconds,
                                                                  f32  hold_seconds,
                                                                  f32  give_up_seconds,
                                                                  u32  step_count) {

    if (!on) {
        if (unk_04c->mission_pacing_override) {
            unk_04c->mission_pacing_override->~spawn_item_collection_unk_38();

            mm::free(unk_04c->mission_pacing_override);
        }

        unk_04c->mission_pacing_override = nullptr;

        return;
    }

    if (!unk_04c->mission_pacing_override) {
        void* storage = mm::allocate(sizeof(spawn_item_collection_unk_38));

        unk_04c->mission_pacing_override = storage ? new (storage) spawn_item_collection_unk_38() : nullptr;
    }

    unk_04c->mission_pacing_override->low_spawn_count  = low_spawn_count;
    unk_04c->mission_pacing_override->high_spawn_count = high_spawn_count;
    unk_04c->mission_pacing_override->ramp_seconds     = ramp_seconds;
    unk_04c->mission_pacing_override->hold_seconds     = hold_seconds;
    unk_04c->mission_pacing_override->unk_20           = 0.0f;
    unk_04c->mission_pacing_override->give_up_seconds  = give_up_seconds;
    unk_04c->mission_pacing_override->step_count       = step_count;
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
