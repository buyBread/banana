#include <new>

#include "retail.hh"
#include "treyarch/game/als/animation_logic_system.hh"
#include "treyarch/game/als/mocomp.hh"
#include "treyarch/game/als/state_machine.hh"
#include "treyarch/game/animation/animation_controller.hh"
#include "treyarch/game/wds/entity/actor.hh"
#include "treyarch/shared/math/references.hh"

using namespace treyarch;

string_hash als::motion_compensator::my_type_key { 0x2B582714 }; // "als::motion_compensator"
string_hash als::use_anim_only::my_type_key      { 0xB17739A6 }; // "als::use_anim_only"

i32 als::motion_compensator::last_created_size = 0;

// sub_5C3730
als::motion_compensator* als::motion_compensator::create_subclass_in_place(mash::virtual_types_key the_class_to_make, void* memory_buffer) {
    if (the_class_to_make == use_anim_only::my_type_key.source_hash_code) {
        last_created_size = sizeof(use_anim_only);

        return memory_buffer ? new (memory_buffer) use_anim_only : nullptr;
    }

    // the classes not in source yet
    using create_subclass_in_place_t = motion_compensator* (__cdecl*)(mash::virtual_types_key, void*);

    return ((create_subclass_in_place_t)retail::sub_5C3730)(the_class_to_make, memory_buffer);
}

// sub_5ADBB0
mash::virtual_types_key als::motion_compensator::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_718FB0
bool als::motion_compensator::is_subclass_of(mash::virtual_types_key parent_class) const {
    return mash::mash_virtual_base::get_virtual_type_key() == parent_class;
}

// sub_4C6780
i32 als::motion_compensator::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5C48A0
void als::motion_compensator::activate(animation_logic_system* the_als_core, layer_id layer) {
    the_als   = the_als_core;
    the_base  = the_als_core->get_als_layer_internal(layer);
    the_actor = the_als->get_actor();
}

// sub_5B4A70
void als::motion_compensator::deactivate() {}

// sub_5B4A70
void als::motion_compensator::transition() {}

// sub_689610
void als::motion_compensator::pre_anim_action(f32) {}

// sub_689610
void als::motion_compensator::post_anim_action(f32) {}

// sub_667F80
bool als::motion_compensator::is_request_fulfilled() {
    return true;
}

// sub_71F870
bool als::motion_compensator::force_reactivate() const {
    return false;
}

// sub_71F870
bool als::motion_compensator::unk_016() const {
    return false;
}

// sub_5B4B40
als::layer_id als::motion_compensator::get_layer_id() const {
    return the_base->get_layer_id();
}

// sub_5F1D40
void als::motion_compensator::get_po_curr_offset(po &curr_offset) {
    bool ignore_position = the_base->get_optional_pb_int(mocomp_ignore_positional_offset_id, 0, nullptr) > 0;

    curr_offset = the_als->the_controller->get_trajectory_po();

    if (ignore_position) {
        const vector3d &zero = treyarch::references::unk_00fd4370.get();

        curr_offset.get_matrix().w = vector4d(zero.x, zero.y, zero.z, 0.0f);

        return;
    }

    vector4d &position = curr_offset.get_matrix().w;

    // the hit params scale knockdowns and knockups vertically, knockbacks along the ground
    if (position.y <= 0.0f) {
        if (the_base->get_optional_pb_int(mocomp_allow_air_knockdown_scale_id, 0, nullptr) && the_base->has_ext_param_been_set(89))
            position.y = the_base->get_external_float(89) * position.y;
        else if (the_base->get_optional_pb_int(mocomp_allow_knockdown_scale_id, 0, nullptr) && the_base->has_ext_param_been_set(86))
            position.y = the_base->get_external_float(86) * position.y;
    } else {
        if (the_base->get_optional_pb_int(mocomp_allow_air_knockup_scale_id, 0, nullptr) && the_base->has_ext_param_been_set(88))
            position.y = the_base->get_external_float(88) * position.y;
        else if (the_base->get_optional_pb_int(mocomp_allow_knockup_scale_id, 0, nullptr) && the_base->has_ext_param_been_set(85))
            position.y = the_base->get_external_float(85) * position.y;
    }

    if (the_base->get_optional_pb_int(mocomp_allow_air_knockback_scale_id, 0, nullptr) && the_base->has_ext_param_been_set(87)) {
        f32 scale = the_base->get_external_float(87);

        position.x = position.x * scale;
        position.z = scale * position.z;

        return;
    }

    if (the_base->get_optional_pb_int(mocomp_allow_knockback_scale_id, 0, nullptr) && the_base->has_ext_param_been_set(84)) {
        f32 scale = the_base->get_external_float(84);

        position.x = position.x * scale;
        position.z = scale * position.z;
    }
}

// sub_5B4B10
// inlined @ sub_5C48D0
void als::motion_compensator::set_po_curr_offset(actor* target, po &curr_offset) {
    retail::sub_44E230((u64*)&curr_offset, (f32*)&curr_offset, (f32*)target->my_rel_po);
    retail::sub_9595B0((i32)&curr_offset);
    retail::sub_626BE0((i32)target, (u64*)&curr_offset); // entity_base::set_rel_po
}

// sub_5ADFF0
mash::virtual_types_key als::use_anim_only::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AE530
bool als::use_anim_only::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (motion_compensator::get_virtual_type_key() == parent_class)
        return true;

    if (motion_compensator::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4C6780
i32 als::use_anim_only::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5C48D0
void als::use_anim_only::post_anim_action(f32 delta_t) {
    vector3d prev_position = the_actor->my_abs_po->get_position();

    po curr_offset;
    retail::sub_4015B0((u64*)&curr_offset);

    get_po_curr_offset(curr_offset);

    // nothing to move by
    if (retail::sub_5AA570((f32*)&curr_offset, (f32*)&treyarch::references::po_identity_matrix.get()))
        return;

    motion_compensator::set_po_curr_offset(the_actor, curr_offset);

    const vector3d &position = the_actor->my_abs_po->get_position();

    the_actor->set_frame_delta_trans(vector3d(position.x - prev_position.x, position.y - prev_position.y, position.z - prev_position.z), delta_t);
}
