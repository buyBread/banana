#include "retail.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/cpu_combat_pie.hh"
#include "game/wds/ai/info_node.hh"
#include "game/wds/entity/actor.hh"
#include "game/math/references.hh"

using namespace treyarch;

// sub_4E38A0
bool cpu_combat_pie_slice::is_valid() const {
    return pie != nullptr;
}

// sub_4E7640
bool cpu_combat_pie_slice::is_stale() const { // its time ran out or its target is gone
    if (!pie)
        return false;

    if (unk_10.time <= 0.0f)
        return true;

    return !pie->target.resolve();
}

// sub_4E7620
arch_base_vhandle cpu_combat_pie_slice::get_pie_target() const {
    return pie ? pie->target : arch_base_vhandle();
}

// sub_4E38B0
vector3d cpu_combat_pie_slice::get_pie_center() const {
    if (pie)
        return pie->center[orientation];

    return owner->get_actor()->my_abs_po->get_position();
}

// sub_4E3900
vector3d cpu_combat_pie_slice::get_pie_normal() const {
    if (pie)
        return pie->normal[orientation];

    return references::unk_00e6446c.get();
}

// sub_4E3950
vector3d cpu_combat_pie_slice::get_pie_facing() const {
    if (pie)
        return pie->facing[orientation];

    return references::unk_00e64478.get();
}

// sub_4E7670
f32 cpu_combat_pie_slice::get_pie_min_dist() const {
    if (!pie)
        return 2.0f;

    return get_ring_min_dist();
}

// sub_4E76E0
f32 cpu_combat_pie_slice::get_pie_max_dist() const {
    if (!pie)
        return 8.0f;

    return get_ring_max_dist();
}

// sub_4EBB70
i32 cpu_combat_pie_slice::get_pie_num_rings() const {
    if (!pie)
        return 1;

    return (i32)pie->pie_rings[orientation].size();
}

// sub_4EED80
i32 cpu_combat_pie_slice::get_pie_num_slices() const {
    if (!pie)
        return 1;

    cpu_combat_pie_ring* the_ring = pie->get_ring((u32)ring, orientation);

    return the_ring ? the_ring->num_slices : 0;
}

// sub_4EEDC0
bool cpu_combat_pie_slice::pos_inside_slice(const vector3d* pos) {
    return pie && pie->pos_in_slice(this, pos, false);
}

// sub_4EEDE0
vector3d cpu_combat_pie_slice::get_new_pie_pos() {
    if (pie)
        return pie->slot_position(this);

    return owner->get_actor()->my_abs_po->get_position();
}

// sub_4EBBD0
bool cpu_combat_pie::pos_in_slice(const cpu_combat_pie_slice* slice, const vector3d* pos, bool any_ring) {
    engine_lock_scope lock(&references::cpu_combat_pie_lock.get());

    i32  orientation = slice->orientation;
    bool unk         = false;

    f32 min_dist;
    f32 max_dist;

    if (any_ring) {
        max_dist = 9999.0f;
        min_dist = 0.0f;
    }
    else {
        max_dist = slice->get_ring_max_dist();
        min_dist = slice->get_ring_min_dist();
    }

    if (!pos)
        pos = &slice->owner->get_actor()->my_abs_po->get_position();

    if (orientation == 1 && normal[0].y <= 0.7f && -0.7f <= normal[0].y)
        unk = true;

    cpu_combat_pie_ring* the_ring = get_ring((u32)slice->ring, orientation);

    auto pos_in_slice_geometry = (bool (*)(const vector3d*, const vector3d*, const vector3d*, const vector3d*, i32, i32, f32, f32, bool, f32, bool))retail::sub_430950;

    return pos_in_slice_geometry(pos,
                                 &center[orientation],
                                 &normal[orientation],
                                 &facing[orientation],
                                 slice->slice,
                                 the_ring ? the_ring->num_slices : 0,
                                 min_dist,
                                 max_dist,
                                 false,
                                 slice->buffer_arc_pct,
                                 unk);
}

// sub_4EBDB0
vector3d cpu_combat_pie::slot_position(const cpu_combat_pie_slice* slice) {
    engine_lock_scope lock(&references::cpu_combat_pie_lock.get());

    i32 orientation = slice->orientation;

    loco_inode* loco = slice->owner->get_core()->my_loco_inode;

    bool unk_loco = loco && ((bool (__thiscall*)(loco_inode*))(*(void***)loco)[24])(loco); // slot 24
    bool unk      = false;

    if (orientation == 1 && normal[0].y <= 0.7f && -0.7f <= normal[0].y)
        unk = true;

    f32 max_dist = slice->get_ring_max_dist();
    f32 min_dist = slice->get_ring_min_dist();

    cpu_combat_pie_ring* the_ring = get_ring((u32)slice->ring, orientation);

    auto slot_position_geometry = (vector3d* (*)(vector3d*, const vector3d*, const vector3d*, const vector3d*, i32, i32, f32, f32, bool, f32, f32, bool))retail::sub_42FEB0;

    vector3d result;

    return *slot_position_geometry(&result,
                                   &center[orientation],
                                   &normal[orientation],
                                   &facing[orientation],
                                   slice->slice,
                                   the_ring ? the_ring->num_slices : 0,
                                   min_dist,
                                   max_dist,
                                   unk_loco,
                                   1.0f,
                                   slice->buffer_arc_pct,
                                   unk);
}
