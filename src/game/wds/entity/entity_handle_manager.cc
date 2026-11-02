#include <cstdio>
#include <utility>

#include "game/wds/entity/entity_handle_manager.hh"
#include "treyarch/jq/native_locks.hh"
#include "treyarch/mash/string.hh"

using namespace treyarch;

// sub_640AE0
entity_base* entity_handle_manager::find_entity(const string_hash &target_entity, e_entity_flavor flavor, bool) { // an entity of another flavor builds a message nobody reads and is answered anyway
    const vc8::map<string_hash, entity_base*> &the_map = references::the_map.get();

    if (references::unk_00ffeb19.read()) {
        char buffer[0x200];

        for (auto* node = the_map.begin(); node != the_map.end(); node = node->next())
            std::snprintf(buffer, sizeof(buffer), "Ent:  %s", "");
    }

    auto* found = the_map.find(target_entity);

    if (found == the_map.end())
        return nullptr;

    if (flavor != entity_flavor_ignore && found->value.second->get_flavor() != flavor)
        mash::string message = mash::string("Entity ") + " is not a " + references::entity_flavor_names.get()[flavor];

    return found->value.second;
}

// sub_644EE0
void entity_handle_manager::register_entity(entity_base* e) {
    if (e->id == string_hash()) // SM3's generic_id::empty_id
        return;

    std::pair<const string_hash, entity_base*> value(e->id, e);

    references::the_map.get().insert(value);
}

// sub_644F20
void entity_handle_manager::deregister_entity(entity_base* e) {
    if (e->id == string_hash()) // SM3's generic_id::empty_id
        return;

    vc8::map<string_hash, entity_base*> &the_map = references::the_map.get();

    auto* found = the_map.find(e->id);

    if (found != the_map.end())
        the_map.erase(found);
}

// sub_65DFF0
void treyarch::generated_name_map_erase(const string_hash &id) {
    engine_lock_scope lock(jq::references::entity_id_manager_allocation_lock.read());

    vc8::map<string_hash, name_pool*> &generated_name_map = references::generated_name_map.get();

    auto* found = generated_name_map.find(id);

    if (found != generated_name_map.end()) {
        found->value.second->recycled_names.push_back(id);

        generated_name_map.erase(found);
    }
}
