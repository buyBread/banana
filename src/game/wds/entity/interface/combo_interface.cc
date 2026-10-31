#include "retail.hh"
#include "game/wds/entity/interface/combo_interface.hh"

using namespace treyarch;

// slot 19
void combo_interface::frame_advance(f32 delta_t) {
    ((void (__thiscall*)(combo_interface*, f32))retail::sub_67ACC0)(this, delta_t);
}
