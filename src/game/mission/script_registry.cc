#include <utility>

#include "game/mission/script_registry.hh"

using namespace treyarch;

// sub_803670
void script_registry::insert_rational_key(string_hash the_entity, string_hash key, f32 default_value, f32* value, bool set_value) {
    // setting drops the old value, so the default below gets stored
    if (set_value) {
        entry search;
        search.entity = the_entity;

        auto* where = entries.find(search);

        if (where != entries.end())
            where->value.rational_keys.erase(key);
    }

    entry search;
    search.entity = the_entity;

    auto* where = entries.find(search);

    if (where == entries.end()) {
        std::pair<const string_hash, f32> pair(key, default_value);

        *value = default_value;

        entries.insert(search).first->value.rational_keys.insert(pair);
    } else {
        auto* found = where->value.rational_keys.find(key);

        if (found != where->value.rational_keys.end())
            *value = found->value.second;
        else {
            where->value.rational_keys.insert(std::pair<const string_hash, f32>(key, default_value));

            *value = default_value;
        }
    }
}

// sub_803800
void script_registry::insert_vector3d_key(string_hash the_entity, string_hash key, vector3d default_value, vector3d* value, bool set_value) {
    // setting drops the old value, so the default below gets stored
    if (set_value) {
        entry search;
        search.entity = the_entity;

        auto* where = entries.find(search);

        if (where != entries.end())
            where->value.vector3d_keys.erase(key);
    }

    entry search;
    search.entity = the_entity;

    auto* where = entries.find(search);

    if (where == entries.end()) {
        *value = default_value;

        std::pair<const string_hash, vector3d> pair(key, default_value);

        entries.insert(search).first->value.vector3d_keys.insert(pair);
    } else {
        auto* found = where->value.vector3d_keys.find(key);

        if (found != where->value.vector3d_keys.end())
            *value = found->value.second;
        else {
            where->value.vector3d_keys.insert(std::pair<const string_hash, vector3d>(key, default_value));

            *value = default_value;
        }
    }
}

// sub_8035A0
bool script_registry::has_vector3d_key(string_hash the_entity, string_hash key) const {
    entry search;
    search.entity = the_entity;

    auto* where = entries.find(search);

    if (where == entries.end())
        return false;

    return where->value.vector3d_keys.find(key) != where->value.vector3d_keys.end();
}
