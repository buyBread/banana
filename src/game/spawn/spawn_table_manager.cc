#include <algorithm>
#include <cstring>
#include <new>

#include "retail.hh"
#include "vc8/vector.hh"
#include "treyarch/chuck/vm/vm_dynamic_array_manager.hh"
#include "treyarch/mm/mm.hh"
#include "game/amalga/resource_manager.hh"
#include "game/app.hh"
#include "game/arch_base_vhandle.hh"
#include "game/event/chuck_library_parameter_event.hh"
#include "game/event/event_manager.hh"
#include "game/game.hh"
#include "game/game_meter.hh"
#include "game/game_meter_manager.hh"
#include "game/math/po.hh"
#include "game/math/references.hh"
#include "game/mission/mission_manager.hh"
#include "game/mission/mission_memory_manager.hh"
#include "game/progression_change.hh"
#include "game/quest_list.hh"
#include "game/region_pack_manager.hh"
#include "game/region_spawn_manager.hh"
#include "game/resource_key.hh"
#include "game/spawn/active_spawn_item.hh"
#include "game/spawn/quest_giver_spawn.hh"
#include "game/spawn/recent_despawn.hh"
#include "game/spawn/spawn_table_manager.hh"
#include "game/spawn_item_collection.hh"
#include "game/spawn_table_interface.hh"
#include "game/spawn_point_list_collection.hh"
#include "game/static_spawn_item_collection.hh"
#include "game/summon_state.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/ai_pf_region_graph.hh"
#include "game/wds/ai/info_node.hh"
#include "game/wds/entity/actor.hh"
#include "game/wds/entity/entity.hh"
#include "game/wds/entity/interface/damage_interface.hh"
#include "game/wds/entity/interface/physical_interface.hh"
#include "game/wds/entity/interface/player_interface.hh"
#include "game/wds/references.hh"
#include "game/wds/world_dynamics_system.hh"

using namespace treyarch;

// sub_94D560
spawn_table_manager* spawn_table_manager::create() {
    spawn_table_manager* manager = new spawn_table_manager();

    instance() = manager;

    return manager;
}

// sub_94D460
spawn_table_manager::spawn_table_manager() : unk_004(80),
                                             spawn_placement_figures{ 90.0f, 200.0f, 300.0f, 24.0f },
                                             street_percent_override(false),
                                             street_percent(0.2f),
                                             unk_020(0.2f),
                                             unk_044(0),
                                             unk_048(false),
                                             unk_049(false),
                                             collectibles_disabled(false),
                                             unk_050(false) {

    void* storage = mm::allocate(sizeof(spawn_table_manager_unk_04c));

    unk_04c = storage ? new (storage) spawn_table_manager_unk_04c() : nullptr;
}

// sub_94D260
spawn_table_manager_unk_04c::spawn_table_manager_unk_04c() : max_active_spawn_items(5), // unk_138 and the fixed entities stay as allocated
                                                             quest_spawn_count(0),
                                                             unk_008(0),
                                                             city_spawn_count(5),
                                                             unk_010(0),
                                                             chosen_quest_giver_spawn(nullptr),
                                                             table_interface(nullptr),
                                                             no_spawn_zones(nullptr),
                                                             arbitrary_regions(nullptr),
                                                             no_spawn_zone_set(nullptr),
                                                             pacing(nullptr),
                                                             mission_pacing_override(nullptr),
                                                             city_life_categories(nullptr),
                                                             quest_spawn_uncapped(false),
                                                             quest_spawn_cap(0),
                                                             unk_124(0.0f),
                                                             unk_128(0.0f),
                                                             quest_spawn_share(0.0f),
                                                             unk_130(0.0f),
                                                             spawn_count(0),
                                                             pacing_phase(2),
                                                             unk_140(0.0f),
                                                             stars_mode(false),
                                                             stars_level(0),
                                                             stars_mode_effects{ -1, -1 },
                                                             engagement_capacity(0),
                                                             threat_capacity(0) {}

// sub_958A90
spawn_table_manager::~spawn_table_manager() { // without a world the tables are left behind
    if (references::g_world_ptr.read()) {
        if (unk_04c)
            for (i32 index = 0; index < 15; ++index)
                if (unk_04c->fixed_entities[index]) {
                    retail::sub_7FBE10((u32*)&references::g_world_ptr.read()->ent_mgr, (i32*)unk_04c->fixed_entities[index]); // wds_entity_manager destroy

                    unk_04c->fixed_entities[index] = nullptr;
                }

        if (unk_04c) {
            unk_04c->~spawn_table_manager_unk_04c();

            mm::free(unk_04c);
        }
    }

    // back to singleton's vtable
    *(void**)this = &references::singleton_vtable.get();
}

// sub_958750
spawn_table_manager_unk_04c::~spawn_table_manager_unk_04c() { // the interface came from the resource lookup, yet it's destroyed and freed like a heap one
    chosen_quest_giver_spawn = nullptr;

    if (table_interface) {
        retail::sub_9568C0((u32*)table_interface); // spawn_table_interface's destructor

        mm::free(table_interface);

        table_interface = nullptr;
    }

    if (no_spawn_zones) {
        no_spawn_zones->~map();

        mm::free(no_spawn_zones);

        no_spawn_zones = nullptr;
    }

    if (arbitrary_regions) {
        arbitrary_regions->~map();

        mm::free(arbitrary_regions);

        arbitrary_regions = nullptr;
    }

    no_spawn_zone_set = nullptr;
    pacing            = nullptr;

    if (mission_pacing_override) {
        mission_pacing_override->~spawn_item_collection_unk_38();

        mm::free(mission_pacing_override);

        mission_pacing_override = nullptr;
    }

    if (city_life_categories) {
        city_life_categories->~map();

        mm::free(city_life_categories);

        city_life_categories = nullptr;
    }
}

// sub_952F20
void spawn_table_manager::load(const char*, amalga::resource_pack_slot*) { // also makes the fixed entities, hidden
    string_hash name;
    name.initialize(mash::ALLOCATED, "global");

    resource_key key;
    key.hash = string_hash();
    key.set(name, 60);

    unk_04c->table_interface = (spawn_table_interface*)amalga::resource_manager::get_resource(key, nullptr, nullptr);

    if (unk_04c->table_interface)
        unk_04c->table_interface->build_name_maps();

    auto* common = (amalga::resource_pack_slot*)retail::sub_739BC0((i32)amalga::resource_manager::get_best_context(amalga::resource_partition_common));

    amalga::resource_context_stack_object context(common);

    for (i32 index = 0; index < 15; ++index) {
        string_hash collectible;
        collectible.initialize(mash::ALLOCATED, "ui_game_collectible");

        unk_04c->fixed_entities[index] = (entity*)retail::sub_804740((u32*)&references::g_world_ptr.read()->ent_mgr,
                                                                     (i32)collectible.source_hash_code,
                                                                     (i32*)1); // wds_entity_manager::spawn_entity_autoname

        if (unk_04c->fixed_entities[index])
            unk_04c->fixed_entities[index]->set_visible(false, false);
    }
}

// sub_9562C0
void spawn_table_manager::update(f32 dt) { // the spawning steps wait while a mission is running, unless it asks for city life
    retail::sub_948890(this);       // reposition_fixed_entities
    retail::sub_92E970((u32*)this); // destroy_despawned

    if (unk_04c->stars_mode && !references::game.read()->game_paused)
        stars_mode_damage_tick(dt);

    region_spawn_manager* spawns = region_spawn_manager::inst();

    references::updating_spawn_table_manager.write(this);

    if (!mission_manager::inst()->is_idle() && !spawns->populate_mission_with_entities)
        return;

    unk_04c->pacing_tick(dt);

    retail::sub_9533D0((i32)this);     // despawn_out_of_range
    retail::sub_954BC0((i32)this, dt); // activate_items

    start_ready_actives();
}

// sub_920810
void spawn_table_manager::stars_mode_damage_tick(f32 dt) { // hurts the hero once a second
    if (!unk_050)
        return;

    f32 timer = (f32)((f64)references::stars_mode_damage_timer.read() + (f64)dt);

    references::stars_mode_damage_timer.write(timer);

    if (!(timer >= 1.0f))
        return;

    const vector3d &facing = references::g_world_ptr.read()->hero_ptr->my_abs_po->get_z_facing();

    vector3d direction(-0.0f - facing.x, -0.0f - facing.y, -0.0f - facing.z);

    string_hash wounded_category;
    wounded_category.initialize(mash::ALLOCATED);

    references::g_world_ptr.read()->hero_ptr->damage_ifc()->apply_damage_with_combo_web_damage(nullptr,
                                                                                              nullptr,
                                                                                              references::stars_mode_damage.read(),
                                                                                              8,
                                                                                              2,
                                                                                              nullptr,
                                                                                              nullptr,
                                                                                              &references::unk_010f9c30.get(),
                                                                                              &direction,
                                                                                              0,
                                                                                              &wounded_category,
                                                                                              &references::unk_010f9c30.get(),
                                                                                              num_rb_phys_bones,
                                                                                              nullptr,
                                                                                              nullptr,
                                                                                              false);

    references::stars_mode_damage_timer.write(0.0f);
}

// sub_938DB0
void spawn_table_manager::end_stars_mode() {
    for (i32 index = 0; index < 2; ++index)
        if (unk_04c->stars_mode_effects[index] >= 0) {
            references::g_world_ptr.read()->hero_ptr->stop_looping_effect(unk_04c->stars_mode_effects[index]);

            unk_04c->stars_mode_effects[index] = -1;
        }

    player_interface* player = references::g_world_ptr.read()->hero_ptr->player_ifc();

    if (player)
        ((void (__thiscall*)(player_interface*, bool, bool))((void**)player->vtable)[523])(player, false, unk_050); // slot 523, lock_suit

    region_pack_manager::inst()->despawn_and_lock_quest_giver_in_mission(false);

    summon_state* summons = references::summon_state.read();

    summons->end_requested = true;
    summons->unk_098       = false;
    summons->unk_099       = unk_050;

    {
        i32 max_hit_points = references::g_world_ptr.read()->hero_ptr->damage_ifc()->get_max_hit_points();

        references::g_world_ptr.read()->hero_ptr->damage_ifc()->set_hit_points(max_hit_points);
    }

    // the RVB meter; a missing one isn't caught
    game_meter_base* meter = references::game_meter_manager.read()->meter_dict.find(1)->value.second;

    ((void (__thiscall*)(game_meter_base*, f32))(*(void***)meter)[13])(meter, -1.0f); // slot 13

    retail::sub_42E750(0); // team_relations_reset_to_default

    unk_04c->stars_mode = false;
    unk_04c->stars_mode_spawn_items.clear();

    auto* hero   = (actor*)references::g_world_ptr.read()->hero_ptr;
    auto* target = (combat_target_inode*)hero->get_ai_core()->get_info_node(info_node_type_combat_target);

    if (target) {
        target->set_threat_capacity(unk_04c->threat_capacity);
        target->set_engagement_capacity(unk_04c->engagement_capacity);
    }
}

// sub_9484B0
void spawn_table_manager::start_ready_actives() {
    for (active_spawn_item** it = unk_04c->pending_active_spawn_items.begin(); it != unk_04c->pending_active_spawn_items.end();) {
        if (!(*it)->ready_to_start()) {
            ++it;
            continue;
        }

        unk_04c->active_spawn_items.push_back(*it);

        it = unk_04c->pending_active_spawn_items.erase(it);
    }
}

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

// inlined @ sub_9547F0
bool spawn_table_manager_unk_04c::apply_progression_changes(spawn_item_collection* collection) { // true when any change applied
    bool applied = false;

    for (progression_change** change = collection->changes.begin(); change != collection->changes.end(); ++change) {
        if (!retail::sub_7F84B0((u32*)(*change)->condition)) // quest_condition::evaluate
            continue;

        remove_spawn_items_by_name(&(*change)->removed);
        add_spawn_items_by_name(&(*change)->added);

        applied = true;
    }

    return applied;
}

// sub_9547F0
void spawn_table_manager_unk_04c::rebuild_spawn_tables() {
    if (!table_interface)
        return;

    for (quest_giver_spawn** it = quest_giver_spawns.begin(); it != quest_giver_spawns.end(); ++it)
        (*it)->reselect_location();

    if (!mission_manager::inst()->is_idle())
        return;

    spawn_table_manager::inst()->clear_spawn_item_lists();

    mash::string name = region_spawn_manager::inst()->get_neighborhood_name();

    if (!std::strncmp(name.c_str(), "", mash::string::max_length)) {
        region_pack_manager* packs = region_pack_manager::inst();

        // the held packs go
        retail::sub_947590((i32)packs, 1, 0);
        retail::sub_938C60((u32*)packs, 1);

        return;
    }

    retail::sub_94F010((i32)region_pack_manager::inst()); // region_pack_manager::update_pack_selection

    bool applied = false;

    spawn_item_collection* neighborhood_set = table_interface->find_spawn_item_collection(name);

    if (neighborhood_set) {
        if (apply_progression_changes(neighborhood_set))
            applied = true;

        for (spawn_item_collection_unk_38** it = neighborhood_set->pacings.begin(); it != neighborhood_set->pacings.end(); ++it)
            if (retail::sub_7F84B0((u32*)(*it)->condition)) { // quest_condition::evaluate
                pacing  = *it;
                applied = true;
            }

        for (spawn_item_collection_unk_4c** it = neighborhood_set->no_spawn_zone_sets.begin(); it != neighborhood_set->no_spawn_zone_sets.end(); ++it)
            if (retail::sub_7F84B0((u32*)(*it)->condition)) { // quest_condition::evaluate
                no_spawn_zone_set = *it;
                applied           = true;
            }
    }

    spawn_item_collection* city_wide_set = spawn_table_manager::inst()->find_city_wide_set();

    if (city_wide_set && apply_progression_changes(city_wide_set))
        applied = true;

    spawn_item_collection* meta_set = spawn_table_manager::inst()->find_meta_set();

    if (meta_set && apply_progression_changes(meta_set))
        applied = true;

    // the recent despawns no longer match the tables
    if (applied) {
        while (recent_despawns.begin() != recent_despawns.end()) {
            recent_despawn* despawn = *recent_despawns.begin();

            spawn_table_manager::inst()->free_spawn_record(despawn->record);

            despawn->record = nullptr;
            despawn->item   = nullptr;

            recent_despawns.erase(recent_despawns.begin());

            if (despawn)
                mm::free(despawn);
        }
    }

    spawn_item_collection* hard_coded_set = table_interface->find_spawn_item_collection("cross_neighborhood_hard_coded");

    if (hard_coded_set)
        apply_progression_changes(hard_coded_set);

    if (stars_mode) {
        spawn_table_unk_7c* act   = table_interface->unk_7c[mission_manager::inst()->get_current_act()];
        i32                 level = stars_level;

        stars_mode_spawn_items.clear();

        for (mash::string** it = act->spawn_items[level]->begin(); it != act->spawn_items[level]->end(); ++it) {
            static_spawn_item* item = table_interface->find_spawn_item(**it);

            if (item)
                stars_mode_spawn_items.push_back(item);
        }
    }
}

// inlined @ sub_947CC0
void spawn_table_manager_unk_04c::split_spawn_count() { // the quest items' share and the rest
    f32 share = quest_spawn_share;
    u32 quest = (u32)(i64)((f32)spawn_count * share);

    if (!quest_spawn_uncapped && (u32)quest_spawn_cap < quest)
        quest = quest_spawn_cap;

    quest_spawn_count = quest;

    if (quest < 1 && share != 0.0f)
        quest = 1;

    quest_spawn_count = quest;
    city_spawn_count  = spawn_count - quest;
}

// sub_947CC0
void spawn_table_manager_unk_04c::pacing_tick(f32 dt) { // climbs to the high count, steps down to the low one, holds, over and over
    if (!table_interface)
        return;

    for (static_spawn_item** it = random_spawn_items.begin(); it != random_spawn_items.end(); ++it)
        if ((*it)->tick(dt))
            spawn_items.push_back(*it);

    for (static_spawn_item** it = always_on_spawn_items.begin(); it != always_on_spawn_items.end(); ++it)
        (*it)->tick(dt);

    spawn_item_collection_unk_38* current = pacing;

    if ((current || mission_pacing_override) && !references::game.read()->game_paused) {
        if (mission_pacing_override)
            current = mission_pacing_override;

        unk_140 = (f32)((f64)unk_140 + (f64)dt);

        if (unk_140 >= 75.0f && !mission_manager::inst()->is_mission_running()) {
            event_manager::raise_event(references::long_time_last_city_life.read(), arch_base_vhandle());

            unk_140 = 0.0f;
        }

        unk_128 = (f32)((f64)unk_128 + (f64)dt);

        if (pacing_phase == 0) {
            unk_130 = (f32)((f64)unk_130 + (f64)dt);

            u32 low   = current->low_spawn_count;
            u32 climb = (u32)(i64)(unk_130 / current->ramp_seconds * (f32)(current->high_spawn_count - low));

            spawn_count = climb + low;

            if (spawn_count > current->high_spawn_count)
                spawn_count = current->high_spawn_count;

            split_spawn_count();

            f32 ramp_seconds = current->ramp_seconds;

            if (unk_130 >= ramp_seconds || unk_138) {
                u32 steps   = current->step_count;
                u32 climbed = (u32)(i64)(unk_130 / ramp_seconds * (f32)steps);

                unk_130      = 0.0f;
                pacing_phase = 1;
                unk_138      = steps - climbed;
            }
        } else {
            unk_124 = (f32)((f64)unk_124 + (f64)dt);

            if (pacing_phase == 1) {
                u32 high    = current->high_spawn_count;
                u32 descent = (u32)(i64)((f32)unk_138 / (f32)current->step_count * (f32)(high - current->low_spawn_count));

                spawn_count = high - descent;

                if (spawn_count > current->high_spawn_count)
                    spawn_count = current->high_spawn_count;

                split_spawn_count();

                if (spawn_count == current->low_spawn_count) {
                    unk_138      = 0;
                    unk_124      = 0.0f;
                    pacing_phase = 2;
                }

                if (unk_124 >= current->give_up_seconds &&
                    spawn_count < current->high_spawn_count) {

                    u32 steps_back = unk_138 / current->step_count;

                    unk_138      = 0;
                    unk_124      = 0.0f;
                    pacing_phase = 0;
                    unk_130      = (f32)steps_back * current->ramp_seconds;
                }
            } else {
                spawn_count = current->low_spawn_count;

                split_spawn_count();

                if (unk_124 >= current->hold_seconds) {
                    unk_124      = 0.0f;
                    pacing_phase = 0;
                }
            }
        }
    }

    if (spawn_table_manager::inst()->unk_04c->stars_mode) {
        quest_spawn_count >>= 1;
        city_spawn_count  >>= 1;
    }
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

// sub_94F970
void spawn_table_manager::add_no_spawn_zone(mash::string name, vector3d center, f32 radius) { // a name already taken drops its old zone without freeing it
    if (!unk_04c->no_spawn_zones) {
        void* storage = mm::allocate(sizeof(vc8::map<mash::string, no_spawn_base*>));

        unk_04c->no_spawn_zones = storage ? new (storage) vc8::map<mash::string, no_spawn_base*>() : nullptr;
    }

    (*unk_04c->no_spawn_zones)[name];

    auto* zone = new no_spawn_point;

    zone->center = center;
    zone->radius = radius;
    zone->name   = name;

    (*unk_04c->no_spawn_zones)[name] = zone;
}

// sub_94FA90
void spawn_table_manager::add_no_spawn_zone(mash::string name, chuck::vm::script_dynamic_array<vector3d>* points) { // a name already taken drops its old zone without freeing it
    if (!unk_04c->no_spawn_zones) {
        void* storage = mm::allocate(sizeof(vc8::map<mash::string, no_spawn_base*>));

        unk_04c->no_spawn_zones = storage ? new (storage) vc8::map<mash::string, no_spawn_base*>() : nullptr;
    }

    (*unk_04c->no_spawn_zones)[name];

    auto* zone = new no_spawn_quad;

    zone->name = name;

    for (auto* it = points->array->begin(); it != points->array->end(); ++it)
        zone->points.push_back(*(vector3d*)it->stuff);

    (*unk_04c->no_spawn_zones)[name] = zone;
}

// sub_94FC70
void spawn_table_manager::remove_no_spawn_zone(mash::string name) { // the map goes with its last zone
    if (!unk_04c->no_spawn_zones)
        return;

    auto* found = unk_04c->no_spawn_zones->find(name);

    if (found != unk_04c->no_spawn_zones->end()) {
        no_spawn_base* zone = found->value.second;

        unk_04c->no_spawn_zones->erase(found);

        delete zone;
    }

    if (unk_04c->no_spawn_zones->size())
        return;

    if (unk_04c->no_spawn_zones) {
        unk_04c->no_spawn_zones->~map();

        mm::free(unk_04c->no_spawn_zones);
    }

    unk_04c->no_spawn_zones = nullptr;
}

// sub_93F9F0
bool spawn_table_manager::is_point_within_no_spawn_zone(mash::string name, const vector3d* point) {
    if (!unk_04c->no_spawn_zones)
        return false;

    if (unk_04c->no_spawn_zones->find(name) == unk_04c->no_spawn_zones->end())
        return false;

    return (*unk_04c->no_spawn_zones)[name]->contains_point(point);
}

// sub_94FD40
void spawn_table_manager::add_arbitrary_region(mash::string name, vector3d center, f32 radius) { // a name already taken drops its old region without freeing it
    if (!unk_04c->arbitrary_regions) {
        void* storage = mm::allocate(sizeof(vc8::map<mash::string, no_spawn_base*>));

        unk_04c->arbitrary_regions = storage ? new (storage) vc8::map<mash::string, no_spawn_base*>() : nullptr;
    }

    (*unk_04c->arbitrary_regions)[name];

    auto* region = new no_spawn_point;

    region->center = center;
    region->radius = radius;
    region->name   = name;

    (*unk_04c->arbitrary_regions)[name] = region;
}

// sub_94FE60
void spawn_table_manager::add_arbitrary_region(mash::string name, chuck::vm::script_dynamic_array<vector3d>* points) { // a name already taken drops its old region without freeing it
    if (!unk_04c->arbitrary_regions) {
        void* storage = mm::allocate(sizeof(vc8::map<mash::string, no_spawn_base*>));

        unk_04c->arbitrary_regions = storage ? new (storage) vc8::map<mash::string, no_spawn_base*>() : nullptr;
    }

    (*unk_04c->arbitrary_regions)[name];

    auto* region = new no_spawn_quad;

    region->name = name;

    for (auto* it = points->array->begin(); it != points->array->end(); ++it)
        region->points.push_back(*(vector3d*)it->stuff);

    (*unk_04c->arbitrary_regions)[name] = region;
}

// sub_950040
void spawn_table_manager::remove_arbitrary_region(mash::string name) { // the map goes with its last region
    if (!unk_04c->arbitrary_regions)
        return;

    auto* found = unk_04c->arbitrary_regions->find(name);

    if (found != unk_04c->arbitrary_regions->end()) {
        no_spawn_base* region = found->value.second;

        unk_04c->arbitrary_regions->erase(found);

        delete region;
    }

    if (unk_04c->arbitrary_regions->size())
        return;

    if (unk_04c->arbitrary_regions) {
        unk_04c->arbitrary_regions->~map();

        mm::free(unk_04c->arbitrary_regions);
    }

    unk_04c->arbitrary_regions = nullptr;
}

// sub_93FA80
bool spawn_table_manager::is_point_within_arbitrary_region(mash::string name, const vector3d* point) {
    if (!unk_04c->arbitrary_regions)
        return false;

    if (unk_04c->arbitrary_regions->find(name) == unk_04c->arbitrary_regions->end())
        return false;

    return (*unk_04c->arbitrary_regions)[name]->contains_point(point);
}

// sub_9340A0
void spawn_table_manager::destroy_debris_inside(no_spawn_base* zone) {
    vc8::vector<debris_entity_list*> &lists = references::debris_entity_lists.get();

    for (u32 index = 0; lists.begin() && index < lists.size(); ++index) {
        debris_entity_list* list = lists[index];

        if (!list || !list->entities)
            continue;

        for (entity** it = list->entities->begin(); it != list->entities->end();) {
            if (!zone->contains_point(&(*it)->my_abs_po->get_position())) {
                ++it;
                continue;
            }

            retail::sub_7FBF20((u32*)&references::g_world_ptr.read()->ent_mgr, (i32*)*it); // wds_entity_manager::destroy_entity

            it = list->entities->erase(it);
        }
    }
}

// sub_9385A0
void spawn_table_manager::add_debris_no_spawn_zone(mash::string name, vector3d center, f32 radius) { // the debris already inside goes
    if (!references::debris_no_spawn_zones.read()) {
        void* storage = mm::allocate(sizeof(mash::vector<no_spawn_base>));

        references::debris_no_spawn_zones.write(storage ? new (storage) mash::vector<no_spawn_base>() : nullptr);
    }

    auto* zone = new no_spawn_point;

    zone->center = center;
    zone->radius = radius;
    zone->name   = name;

    references::debris_no_spawn_zones.read()->push_back(zone);

    destroy_debris_inside(zone);
}

// sub_9386A0
void spawn_table_manager::add_debris_no_spawn_zone(mash::string name, chuck::vm::script_dynamic_array<vector3d>* points) { // the debris already inside goes
    if (!references::debris_no_spawn_zones.read()) {
        void* storage = mm::allocate(sizeof(mash::vector<no_spawn_base>));

        references::debris_no_spawn_zones.write(storage ? new (storage) mash::vector<no_spawn_base>() : nullptr);
    }

    auto* zone = new no_spawn_quad;

    zone->name = name;

    for (auto* it = points->array->begin(); it != points->array->end(); ++it)
        zone->points.push_back(*(vector3d*)it->stuff);

    references::debris_no_spawn_zones.read()->push_back(zone);

    destroy_debris_inside(zone);
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
// inlined @ sub_9502B0
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

// sub_9502B0
void spawn_table_manager::save_safe_zone_wall_percentages(chuck::vm::script_instance* inst, chuck::vm::script_dynamic_array<f32>* percentages) {
    cross_neighborhood_spawn* spawn = find_cross_neighborhood_spawn(inst);

    if (spawn)
        retail::sub_94B920((i32)spawn, (i32)percentages); // cross_neighborhood_spawn::save_safe_zone_wall_percentages
}

// sub_953BB0
void spawn_table_manager::get_safe_zone_wall_percentages(chuck::vm::script_instance* inst, chuck::vm::script_dynamic_array<f32>* percentages) {
    cross_neighborhood_spawn* spawn = nullptr;

    for (cross_neighborhood_spawn** it = unk_04c->cross_neighborhood_spawns.begin(); it != unk_04c->cross_neighborhood_spawns.end(); ++it)
        if ((*it)->unk_d8->active && (*it)->unk_d8->active->has_instance(inst)) {
            spawn = *it;
            break;
        }

    // the quest list's search never steps past its first spawn, so a miss there never returns
    if (!spawn)
        for (cross_neighborhood_spawn** it = unk_04c->quest_cross_neighborhood_spawns.begin(); it != unk_04c->quest_cross_neighborhood_spawns.end();)
            if ((*it)->unk_d8->active && (*it)->unk_d8->active->has_instance(inst)) {
                spawn = *it;
                break;
            }

    if (spawn)
        retail::sub_952440((i32)spawn, (u32**)percentages); // cross_neighborhood_spawn::get_safe_zone_wall_percentages
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

// sub_9581E0
void spawn_table_manager::get_all_city_life_of_category(mash::string name, chuck::vm::script_dynamic_array<f32>* spawns) { // the actives carrying it, then the mission's entries under it
    for (active_spawn_item** it = unk_04c->active_spawn_items.begin(); it != unk_04c->active_spawn_items.end(); ++it) {
        mash::vector<mash::string> &categories = (*it)->item->categories;

        mash::string** found = std::find_if(categories.begin(),
                                            categories.end(),
                                            [&name](mash::string* category) { return !std::strncmp(category->c_str(), name.c_str(), mash::string::max_length); });

        if (found == categories.end())
            continue;

        chuck::vm::chuck_dynamic_array_element_t element;
        *(f32*)element.stuff = (f32)(u32)(*it)->spawn_id;

        spawns->array->push_back(element);
    }

    if (!unk_04c->city_life_categories)
        return;

    if (unk_04c->city_life_categories->find(name) == unk_04c->city_life_categories->end())
        return;

    for (city_life_category_entry* it = (*unk_04c->city_life_categories)[name].begin(); it != (*unk_04c->city_life_categories)[name].end(); ++it) {
        chuck::vm::chuck_dynamic_array_element_t element;
        *(f32*)element.stuff = (f32)(u32)it->spawn_id;

        spawns->array->push_back(element);
    }
}

// sub_958440
void spawn_table_manager::create_category_for_mission(mash::string name, chuck::vm::script_instance* inst) { // one entry per instance under each name
    if (!unk_04c->city_life_categories) {
        void* storage = mm::allocate(sizeof(vc8::map<mash::string, vc8::vector<city_life_category_entry>>));

        unk_04c->city_life_categories = storage ? new (storage) vc8::map<mash::string, vc8::vector<city_life_category_entry>>() : nullptr;
    }

    bool found = false;

    for (city_life_category_entry* it = (*unk_04c->city_life_categories)[name].begin(); it != (*unk_04c->city_life_categories)[name].end(); ++it)
        if (it->instance == inst)
            found = true;

    if (found)
        return;

    city_life_category_entry entry;
    entry.spawn_id = references::next_spawn_id.get()++;
    entry.instance = inst;

    (*unk_04c->city_life_categories)[name].push_back(entry);
}

// sub_93FE70
void spawn_table_manager::register_important_city_life_entity(mash::string                name,
                                                              arch_base_vhandle           handle,
                                                              chuck::vm::script_instance* inst) {

    for (active_spawn_item** it = unk_04c->active_spawn_items.begin(); it != unk_04c->active_spawn_items.end(); ++it)
        if ((*it)->has_instance(inst)) {
            (*it)->important_entities[name] = handle;

            return;
        }

    // a mission instance files it under each of its categories
    if (!unk_04c->city_life_categories)
        return;

    for (auto* node = unk_04c->city_life_categories->begin(); node != unk_04c->city_life_categories->end(); node = node->next())
        for (city_life_category_entry* entry = node->value.second.begin(); entry != node->value.second.end(); ++entry)
            if (entry->instance == inst)
                entry->important_entities[name] = handle;
}

// sub_93FFB0
arch_base_vhandle* spawn_table_manager::get_important_city_life_entity(arch_base_vhandle* out, i32 spawn_id, mash::string name) { // a null handle when nothing has it
    *out = arch_base_vhandle();

    active_spawn_item** found = std::find_if(unk_04c->active_spawn_items.begin(),
                                             unk_04c->active_spawn_items.end(),
                                             [spawn_id](active_spawn_item* active) { return active->spawn_id == spawn_id; });

    if (found != unk_04c->active_spawn_items.end()) {
        *out = (*found)->important_entities[name];

        return out;
    }

    if (!unk_04c->city_life_categories)
        return out;

    // the last matching entry wins
    for (auto* node = unk_04c->city_life_categories->begin(); node != unk_04c->city_life_categories->end(); node = node->next())
        for (city_life_category_entry* entry = node->value.second.begin(); entry != node->value.second.end(); ++entry)
            if (entry->spawn_id == spawn_id)
                *out = entry->important_entities[name];

    return out;
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

// sub_951CB0
void spawn_table_manager::start_persistence_data_save(chuck::vm::script_instance* inst, f32 number) { // a block already open for the instance is dropped, not freed
    void* storage = mm::allocate(sizeof(persistence_data));

    unk_04c->saved_persistence_data[inst->name]         = storage ? new (storage) persistence_data : nullptr;
    unk_04c->saved_persistence_data[inst->name]->unk_00 = number;
}

// sub_9391B0
void spawn_table_manager::end_persistence_data_save(chuck::vm::script_instance* inst) { // the block goes to the instance's recent despawn; without one it stays open
    for (recent_despawn** it = unk_04c->recent_despawns.begin(); it != unk_04c->recent_despawns.end(); ++it)
        if ((*it)->instance_name == inst->name) {
            // the find isn't checked
            auto* saved = unk_04c->saved_persistence_data.find(inst->name);

            (*it)->persistence = saved->value.second;

            unk_04c->saved_persistence_data.erase(saved);

            return;
        }
}

// sub_93FB10
void spawn_table_manager::save_persistence_vector3d(chuck::vm::script_instance* inst, mash::string name, vector3d value) {
    auto* record = new persistence_vector3d;

    record->value = value;

    unk_04c->saved_persistence_data[inst->name]->records[name] = record;
}

// sub_949D40
void spawn_table_manager::save_persistence_vector3d_array(chuck::vm::script_instance*                inst,
                                                          mash::string                               name,
                                                          chuck::vm::script_dynamic_array<vector3d>* values) {

    auto* record = new persistence_vector3d_array;

    for (auto* it = values->array->begin(); it != values->array->end(); ++it)
        record->values.push_back(*(vector3d*)it->stuff);

    unk_04c->saved_persistence_data[inst->name]->records[name] = record;
}

// sub_93FC10
void spawn_table_manager::save_persistence_str(chuck::vm::script_instance* inst, mash::string name, mash::string value) {
    auto* record = new persistence_str;

    record->value = value;

    unk_04c->saved_persistence_data[inst->name]->records[name] = record;
}

// sub_951D80
void spawn_table_manager::save_persistence_str_array(chuck::vm::script_instance*           inst,
                                                     mash::string                          name,
                                                     chuck::vm::script_dynamic_array<u32>* values) {

    auto* record = new persistence_str_array;

    for (auto* it = values->array->begin(); it != values->array->end(); ++it)
        record->values.push_back(mash::string(*(const char**)it->stuff));

    unk_04c->saved_persistence_data[inst->name]->records[name] = record;
}

// sub_93FCD0
void spawn_table_manager::save_persistence_num(chuck::vm::script_instance* inst, mash::string name, f32 value) {
    auto* record = new persistence_num;

    record->value = value;

    unk_04c->saved_persistence_data[inst->name]->records[name] = record;
}

// sub_949E70
void spawn_table_manager::save_persistence_num_array(chuck::vm::script_instance*           inst,
                                                     mash::string                          name,
                                                     chuck::vm::script_dynamic_array<f32>* values) {

    auto* record = new persistence_num_array;

    for (auto* it = values->array->begin(); it != values->array->end(); ++it)
        record->values.push_back(*(f32*)it->stuff);

    unk_04c->saved_persistence_data[inst->name]->records[name] = record;
}

// sub_93FD70
persistence_data_base* spawn_table_manager::find_persistence_data(chuck::vm::script_instance* inst, mash::string name) { // a name the block lacks is added to it as null
    for (auto* node = unk_04c->active_persistence_data.begin(); node != unk_04c->active_persistence_data.end(); node = node->next())
        if (node->value.first->has_instance(inst))
            return node->value.second->records[name];

    return nullptr;
}
