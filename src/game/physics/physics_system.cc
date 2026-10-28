#include "retail.hh"
#include "game/physics/physics_system.hh"

using namespace treyarch;

void rigid_body::add_force(const vector4d &force, const vector4d &point, f32 torque_mult) {
    retail::sub_9F4C90((f32*)this, (f32*)&force, (f32*)&point, torque_mult);
}

void nuge::apply_ballistic_target(rigid_body* const list_rigid_body[],
                                  i32                rbodies_count,
                                  const vector4d    &target,
                                  f32*               dist_sq) {

    retail::sub_9F2A20((i32)list_rigid_body, rbodies_count, (f32*)&target, dist_sq);
}
