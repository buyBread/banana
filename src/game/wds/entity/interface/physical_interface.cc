#include "retail.hh"
#include "game/wds/entity/interface/physical_interface.hh"

using namespace treyarch;

void physical_interface::stop_biped_physics(bool on_destroyed) {
    retail::sub_67A440((i32)this, on_destroyed);
}

void physical_interface::cancel_all_velocity(const vector3d &dir, bool both_dir) {
    retail::sub_6253F0((i32)this, (f32*)&dir, both_dir);
}
