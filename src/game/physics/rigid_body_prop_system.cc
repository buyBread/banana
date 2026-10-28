#include "retail.hh"
#include "game/physics/rigid_body_prop_system.hh"

using namespace treyarch;

rigid_body_prop_head* prop_system::create_prop(entity*         e,
                                               u32             flags,
                                               const vector3d &speed,
                                               f32             random_variance_constant,
                                               bool            in_collision,
                                               void*           unk) {

    return (rigid_body_prop_head*)retail::sub_81C7F0((u32*)e, flags, (f32*)&speed, random_variance_constant, in_collision, (f32*)unk);
}

rigid_body_prop_head* prop_system::find_rigid_body_prop_head(entity* e) {
    return (rigid_body_prop_head*)retail::sub_811DA0((u32*)e);
}

bool prop_system::is_stable(rigid_body_prop_head* rbph) {
    return retail::sub_80FAD0((i32)rbph);
}
