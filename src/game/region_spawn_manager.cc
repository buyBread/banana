#include "retail.hh"
#include "game/amalga/resource_manager.hh"
#include "game/frontend/igo/igo_3d_text.hh"
#include "game/neighborhood_interface.hh"
#include "game/region_spawn_manager.hh"
#include "game/resource_key.hh"
#include "game/spawn/spawn_table_manager.hh"
#include "game/summon_state.hh"

using namespace treyarch;

// sub_90F5C0
void region_spawn_manager::draw_text() {
    igo_3d_text* text = references::region_spawn_text.read();

    if (text)
        text->render();
}

// sub_954750
void region_spawn_manager::load(const char* level_name, amalga::resource_pack_slot* slot) {
    spawn_table_manager::inst()->load(level_name, slot);

    if (references::summon_state.read())
        retail::sub_9525D0((u32*)references::summon_state.read(), (i32)level_name, (i32)slot); // summon_state load

    string_hash name;
    name.initialize(mash::ALLOCATED, "neighborhoods");

    resource_key key;
    key.hash = string_hash();
    key.set(name, 63);

    neighborhoods = (neighborhood_interface*)amalga::resource_manager::get_resource(key, nullptr, nullptr);

    unk_00c = 0;

    for (i32 index = 0; index < 8; ++index) {
        region_spawn_interfaces[index] = nullptr;
        region_names[index]            = string_hash();
        regions[index]                 = nullptr;
    }
}

// sub_90F470
mash::string region_spawn_manager::get_neighborhood_name() {
    mash::string name;

    if (neighborhood)
        name = neighborhood->name;

    return name;
}

// sub_90F4E0
mash::string region_spawn_manager::get_meta_neighborhood_name() {
    mash::string name;

    if (meta_neighborhood)
        name = meta_neighborhood->name;

    return name;
}

// sub_90F550
mash::string region_spawn_manager::get_city_wide_set_name() {
    mash::string name;

    if (unk_09c)
        name = unk_09c->city_wide_set_name;

    return name;
}

// sub_83E510
void region_spawn_manager::update_point_of_interest(chuck::vm::script_instance* inst, mash::string name, vector3d position) {
    spawn_table_manager::inst()->update_point_of_interest(inst, name, position);
}
