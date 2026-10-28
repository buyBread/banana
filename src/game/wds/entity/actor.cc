#include "game/als/animation_logic_system.hh"
#include "game/wds/entity/actor.hh"

using namespace treyarch;

string_hash actor::my_type_key { 0x07138E99 }; // "actor"

// sub_602830
ai_core* actor::get_ai_core() const {
    return my_base_ai_data ? my_base_ai_data->core : nullptr;
}

// sub_602F30
als::animation_logic_system_interface* actor::get_als() const {
    if (adv_ptrs && adv_ptrs->my_als_data)
        return adv_ptrs->my_als_data->the_als;

    return nullptr;
}
