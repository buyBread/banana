#include "retail.hh"
#include "game/amalga/resource_directory.hh"
#include "game/amalga/resource_manager.hh"
#include "game/four_cc.hh"
#include "game/hash/algo.hh"
#include "game/resource_key.hh"
#include "game/universal_resource.hh"

using namespace treyarch;

// sub_7C2D70
ngl::mesh* universal_resource::get_mesh_by_name(string_hash name) {
    string_hash name_000;

    name_000.initialize(mash::ALLOCATED, nullptr, hash::djb2(name.source_hash_code, "000"));

    if (auto mesh = (ngl::mesh*)amalga::resource_manager::get_merged_resource_or_default(name_000, four_cc('M', 'E', 'S', 'H'), nullptr))
        return mesh;

    return (ngl::mesh*)amalga::resource_manager::get_merged_resource_or_default(name, four_cc('M', 'E', 'S', 'H'), nullptr);
}

// sub_781DB0
void* universal_resource::get(amalga::e_resource_type type, string_hash name) {
    if (type == amalga::e_resource_type::texture)
        return amalga::resource_manager::get_merged_resource_or_default(name, four_cc('T', 'E', 'X'), nullptr);

    if (type == amalga::e_resource_type::mesh)
        return get_mesh_by_name(name);

    resource_key id;
    id.set(name, (u32)type);

    const amalga::resource_type_definition_t &definition = amalga::references::resource_type_definitions.get()[id.type];

    if (definition.held_in_apk)
        return amalga::resource_manager::get_merged_resource_or_default(id.get_hash(), definition.apk_type, nullptr);

    void* resource;
    retail::sub_762B50((u32*)&resource, (u32*)&id, nullptr, nullptr); // resource_manager::get_resource's out-pointer twin

    return resource;
}

// sub_7D3800
template<> ngl::texture* universal_resource::get<ngl::texture>(string_hash name) {
    return (ngl::texture*)get(amalga::e_resource_type::texture, name);
}

// sub_7D3830
template<> ngl::mesh* universal_resource::get<ngl::mesh>(string_hash name) {
    return (ngl::mesh*)get(amalga::e_resource_type::mesh, name);
}

// sub_794AF0
template<> tilemap* universal_resource::get<tilemap>(string_hash name) {
    return (tilemap*)get(amalga::e_resource_type::TILEMAP, name);
}

// sub_7D3860
template<> environment* universal_resource::get<environment>(string_hash name) {
    return (environment*)get(amalga::e_resource_type::ENVIRONMENT, name);
}

// sub_794B20
template<> light_behavior* universal_resource::get<light_behavior>(string_hash name) {
    return (light_behavior*)get(amalga::e_resource_type::LIGHTBEHAVIOR, name);
}

// sub_7D38C0
template<> meshmap* universal_resource::get<meshmap>(string_hash name) {
    return (meshmap*)get(amalga::e_resource_type::MESHMAP, name);
}

// sub_7D38F0
template<> adjustment_layers_library* universal_resource::get<adjustment_layers_library>(string_hash name) {
    return (adjustment_layers_library*)get(amalga::e_resource_type::ADJUSTMENTLAYERSLIBRARY, name);
}

// sub_7D3920
template<> outdoors* universal_resource::get<outdoors>(string_hash name) {
    return (outdoors*)get(amalga::e_resource_type::OUTDOORS, name);
}

// sub_7D3950
template<> day_sky_color* universal_resource::get<day_sky_color>(string_hash name) {
    return (day_sky_color*)get(amalga::e_resource_type::DAYSKYCOLOR, name);
}

// sub_7D3980
template<> night_sky_color* universal_resource::get<night_sky_color>(string_hash name) {
    return (night_sky_color*)get(amalga::e_resource_type::NIGHTSKYCOLOR, name);
}

// sub_7F2C80
template<> quest_descriptions* universal_resource::get<quest_descriptions>(string_hash name) {
    return (quest_descriptions*)get(amalga::e_resource_type::JSON_QUESTDESC, name);
}

// sub_9225B0
template<> spawn_item_collection* universal_resource::get<spawn_item_collection>(string_hash name) {
    return (spawn_item_collection*)get(amalga::e_resource_type::JSON_SPAWNPROGRESSION, name);
}

// sub_9225E0
template<> spawn_point_list_collection* universal_resource::get<spawn_point_list_collection>(string_hash name) {
    return (spawn_point_list_collection*)get(amalga::e_resource_type::JSON_SPAWNPOINTS, name);
}

// sub_922610
template<> static_spawn_item_collection* universal_resource::get<static_spawn_item_collection>(string_hash name) {
    return (static_spawn_item_collection*)get(amalga::e_resource_type::JSON_SPAWNITEMS, name);
}
