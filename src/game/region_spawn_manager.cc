#include "retail.hh"
#include "game/frontend/igo/igo_3d_text.hh"
#include "game/neighborhood_interface.hh"
#include "game/region_spawn_manager.hh"
#include "game/spawn/spawn_table_manager.hh"

using namespace treyarch;

// sub_90F5C0
void region_spawn_manager::draw_text() {
    igo_3d_text* text = references::region_spawn_text.read();

    if (text)
        text->render();
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
    using update_point_of_interest_t = void (__thiscall*)(spawn_table_manager*, chuck::vm::script_instance*, mash::string, vector3d);

    ((update_point_of_interest_t)retail::sub_92EF10)(spawn_table_manager::inst(), inst, name, position);
}
