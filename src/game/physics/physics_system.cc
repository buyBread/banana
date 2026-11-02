#include "retail.hh"
#include "game/physics/physics_system.hh"

using namespace treyarch;

// sub_81AE20
void treyarch::moving_body_cache_erase(actor* the_actor) {
    retail::sub_81AAD0((u32*)&references::unk_00ed61c0.get(), (u32)the_actor);
}

void rigid_body::add_force(const vector4d &force, const vector4d &point, f32 torque_mult) {
    retail::sub_9F4C90((f32*)this, (f32*)&force, (f32*)&point, torque_mult);
}

void nuge::apply_ballistic_target(rigid_body* const list_rigid_body[],
                                  i32                rbodies_count,
                                  const vector4d    &target,
                                  f32*               dist_sq) {

    retail::sub_9F2A20((i32)list_rigid_body, rbodies_count, (f32*)&target, dist_sq);
}
