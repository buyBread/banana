#include "retail.hh"
#include "game/wds/entity/interface/standing_interface.hh"

using namespace treyarch;

f32 standing_interface::get_pure_anim_floor_offset() const {
    return (f32)retail::sub_60CFA0((u32*)this);
}

f32 standing_interface::get_floor_offset() {
    return (f32)retail::sub_60D010((i32)this);
}

// sub_5FF550
void standing_interface::set_floor_offset_override(f32 new_override) {
    floor_offset_override = new_override;
}

void standing_interface::manage_standing(f32 delta_t, bool force, f32 max_force_delta) {
    retail::sub_676ED0((u32*)this, delta_t, force, max_force_delta);
}

// sub_4028A0
void standing_interface::manage_standing(bool force, f32 max_force_delta) {
    manage_standing(0.01f, force, max_force_delta);
}

void standing_interface::clear_effectively_standing() {
    retail::sub_60CF20((u32*)this);
}

void standing_interface::set_control_parent(entity* parent) {
    retail::sub_659160((i32)this, (i32)parent);
}
