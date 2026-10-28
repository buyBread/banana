#include "retail.hh"
#include "game/wds/entity/pendulum.hh"

using namespace treyarch;

// sub_625010
pendulum::pendulum() : attach_point(0.0f, 0.0f, 0.0f),
                       rel_pivot_pos(0.0f, 0.0f, 0.0f),
                       abs_pivot_pos(0.0f, 0.0f, 0.0f),
                       constraint(0.0f),
                       adjustment_factor(1.0f),
                       biped_physics_dist_constraint(nullptr),
                       biped_physics_point_constraint{ nullptr, nullptr, nullptr },
                       pivot_rigid_body(nullptr),
                       current_attach_limb(0),
                       attach_limb(0),
                       enabled(false),
                       maintain_rel_orient(false),
                       maintain_abs_orient(false),
                       unk_04f(true) {}

void pendulum::set_entity_base_pivot(entity* _pivot) {
    retail::sub_6210D0((char*)this, (i32)_pivot);
}

vector3d pendulum::get_pivot_abs_pos() const {
    vector3d result;
    retail::sub_60D3E0((char*)this, (i32)&result);

    return result;
}

// sub_5FF710
void pendulum::set_attach_limb(i32 l) {
    if ((u32)l > 9)
        return;

    unk_04f     = attach_limb != l;
    attach_limb = l;
}
