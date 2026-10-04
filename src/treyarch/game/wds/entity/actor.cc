#include "treyarch/game/wds/entity/actor.hh"

using namespace treyarch;

// sub_602830
ai_core* actor::get_ai_core() const {
    return my_base_ai_data ? my_base_ai_data->core : nullptr;
}
