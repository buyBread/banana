#include <algorithm>

#include "game/region_spawn_manager.hh"
#include "game/spawn/spawn_table_manager.hh"
#include "game/spawn_table_interface.hh"
#include "game/spawn_point_list_collection.hh"
#include "game/wds/ai/ai_pf_region_graph.hh"
#include "treyarch/mm/mm.hh"

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
