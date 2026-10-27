#include "game/als/animation_logic_system.hh"
#include "game/wds/entity/actor.hh"

using namespace treyarch;

// sub_602830
ai_core* actor::get_ai_core() const {
    return my_base_ai_data ? my_base_ai_data->core : nullptr;
}

// sub_602F30
als::animation_logic_system_interface* actor::get_als() const {
    if (unk_0b0 && unk_0b0->als_data)
        return unk_0b0->als_data->the_als;

    return nullptr;
}
