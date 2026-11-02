#include "retail.hh"
#include "game/wds/entity/pendulum.hh"
#include "game/wds/entity/conglomerate.hh"
#include "game/wds/entity/entity.hh"
#include "game/math/po.hh"

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

// sub_456450
void pendulum::set_entity_base_pivot(entity_base* _pivot, const vector3d &_pivot_pos) {
    entity_base_pivot = _pivot ? _pivot->my_handle : arch_base_vhandle();
    abs_pivot_pos     = _pivot_pos;

    if (_pivot)
        rel_pivot_pos = _pivot->my_abs_po->inverse_xform(abs_pivot_pos);
    else
        rel_pivot_pos = abs_pivot_pos;

    get_pivot_abs_pos();

    unk_04f = true;
}

vector3d pendulum::get_pivot_abs_pos() const {
    vector3d result;
    retail::sub_60D3E0((char*)this, (i32)&result);

    return result;
}

// sub_60D520
bool pendulum::has_a_moving_anchor() const {
    auto* anchor = (entity_base*)entity_base_pivot.resolve();

    while (anchor) {
        if (!(anchor->flags & entity_flag_bone_connector) &&
            anchor->ext_flags & entity_ext_flag_added_to_list_of_moved_entities)

            return true;

        anchor = anchor->flags & entity_flag_member ?
            anchor->get_conglom_owner() :
            anchor->my_parent;
    }

    return false;
}

// sub_5FF710
void pendulum::set_attach_limb(i32 l) {
    if ((u32)l > 9)
        return;

    unk_04f     = attach_limb != l;
    attach_limb = l;
}
