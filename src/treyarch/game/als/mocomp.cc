#include <cmath>
#include <new>

#include "retail.hh"
#include "treyarch/game/als/animation_logic_system.hh"
#include "treyarch/game/als/mocomp.hh"
#include "treyarch/game/als/state_machine.hh"
#include "treyarch/game/animation/animation_controller.hh"
#include "treyarch/game/event/event_manager.hh"
#include "treyarch/game/wds/ai/ai_core.hh"
#include "treyarch/game/wds/ai/info_node.hh"
#include "treyarch/game/wds/entity/actor.hh"
#include "treyarch/game/wds/entity/interface/physical_interface.hh"
#include "treyarch/game/wds/entity/interface/player_interface.hh"
#include "treyarch/game/wds/entity/interface/standing_interface.hh"
#include "treyarch/game/wds/references.hh"
#include "treyarch/game/wds/world_dynamics_system.hh"
#include "treyarch/shared/math/algebra/functions.hh"
#include "treyarch/shared/math/references.hh"
#include "treyarch/shared/math/vmath/trig.hh"
#include "treyarch/shared/math/vmath/vector.hh"

using namespace treyarch;

string_hash als::motion_compensator::my_type_key { 0x2B582714 }; // "als::motion_compensator"
string_hash als::use_anim_only::my_type_key      { 0xB17739A6 }; // "als::use_anim_only"

string_hash als::simple_orientation::my_type_key     { 0x096B7F29 }; // "als::simple_orientation"
string_hash als::strafe_mocomp::my_type_key          { 0x08E92C03 }; // "als::strafe_mocomp"
string_hash als::relative_orientation::my_type_key   { 0x9E35853B }; // "als::relative_orientation"
string_hash als::relative_strafe_mocomp::my_type_key { 0x1B85359E }; // "als::relative_strafe_mocomp"

string_hash als::simple_orientation_on_foot::my_type_key     { 0xBCDF51FC }; // "als::simple_orientation_on_foot"
string_hash als::strafe_mocomp_on_foot::my_type_key          { 0xB74138D6 }; // "als::strafe_mocomp_on_foot"
string_hash als::relative_orientation_on_foot::my_type_key   { 0x970F4A0E }; // "als::relative_orientation_on_foot"
string_hash als::relative_strafe_mocomp_on_foot::my_type_key { 0x8D2EAD71 }; // "als::relative_strafe_mocomp_on_foot"

string_hash als::velocity_orientation::my_type_key       { 0xC42161AE }; // "als::velocity_orientation"
string_hash als::vertical_orientation::my_type_key       { 0x3EE226B9 }; // "als::vertical_orientation"
string_hash als::momentum_grapple_unsynched::my_type_key { 0x938D8220 }; // "als::momentum_grapple_unsynched"

string_hash als::initial_velocity_orientation::my_type_key   { 0x00CD0297 }; // "als::initial_velocity_orientation"
string_hash als::initial3D_velocity_orientation::my_type_key { 0x57D7B16E }; // "als::initial3D_velocity_orientation"

string_hash als::strict_simple_orientation::my_type_key { 0x17919801 }; // "als::strict_simple_orientation"
string_hash als::direct_mocomp::my_type_key             { 0xD27304F9 }; // "als::direct_mocomp"

string_hash als::input_orientation::my_type_key { 0x422CBEEF }; // "als::input_orientation"
string_hash als::towed_orientation::my_type_key { 0x6ABE6902 }; // "als::towed_orientation"

string_hash als::ikcrawl_mocomp::my_type_key    { 0xEA76894B }; // "als::ikcrawl_mocomp"
string_hash als::wallsprint_mocomp::my_type_key { 0xF1C5C0CE }; // "als::wallsprint_mocomp"

string_hash als::move_and_face::my_type_key { 0xE9A7D0EB }; // "als::move_and_face"

string_hash als::move_and_face_no_anim_movement::my_type_key   { 0x3184F095 }; // "als::move_and_face_no_anim_movement"
string_hash als::action_move_and_face::my_type_key             { 0xEDC86DE8 }; // "als::action_move_and_face"
string_hash als::action_face_only::my_type_key                 { 0x57E971C1 }; // "als::action_face_only"
string_hash als::action_move_only::my_type_key                 { 0x297CAF69 }; // "als::action_move_only"
string_hash als::combat_move_and_face::my_type_key             { 0x7287F900 }; // "als::combat_move_and_face"
string_hash als::combat_face_only::my_type_key                 { 0xC02F30D9 }; // "als::combat_face_only"
string_hash als::combat_move_only::my_type_key                 { 0x91C26E81 }; // "als::combat_move_only"
string_hash als::combat_move_and_face_with_attack::my_type_key { 0xF4B9E872 }; // "als::combat_move_and_face_with_attack"
string_hash als::combat_face_only_with_attack::my_type_key     { 0xA77AADCB }; // "als::combat_face_only_with_attack"
string_hash als::signal_move_and_face::my_type_key             { 0x4352D6C8 }; // "als::signal_move_and_face"
string_hash als::signal_face_only::my_type_key                 { 0xD9E26AA1 }; // "als::signal_face_only"
string_hash als::signal_move_only::my_type_key                 { 0xAB75A849 }; // "als::signal_move_only"
string_hash als::constant_move_and_face::my_type_key           { 0x0F0E00B4 }; // "als::constant_move_and_face"
string_hash als::constant_face_only::my_type_key               { 0xD79D7E8D }; // "als::constant_face_only"
string_hash als::constant_move_only::my_type_key               { 0xA930BC35 }; // "als::constant_move_only"
string_hash als::wall_recover_move_and_face::my_type_key       { 0x5010014F }; // "als::wall_recover_move_and_face"

string_hash als::crawl_transition::my_type_key     { 0xCA7C59D7 }; // "als::crawl_transition"
string_hash als::rvb_crawl_transition::my_type_key { 0x24C44C00 }; // "als::rvb_crawl_transition"

string_hash als::attach_hand_to_point::my_type_key        { 0x11A13AEE }; // "als::attach_hand_to_point"
string_hash als::constant_move_hand_to_point::my_type_key { 0x3420FEB9 }; // "als::constant_move_hand_to_point"
string_hash als::combat_pole_swing_attach::my_type_key    { 0xA771F9B4 }; // "als::combat_pole_swing_attach"

string_hash als::sync_move_and_face::my_type_key          { 0x8E26B3C7 }; // "als::sync_move_and_face"
string_hash als::momentum_sync_move_and_face::my_type_key { 0xB44DA4B8 }; // "als::momentum_sync_move_and_face"

string_hash flight_mocomp::my_type_key { 0x9161F0E8 }; // "flight_mocomp"

i32 als::motion_compensator::last_created_size = 0;

// sub_5C3730
als::motion_compensator* als::motion_compensator::create_subclass_in_place(mash::virtual_types_key the_class_to_make, void* memory_buffer) {
    if (the_class_to_make == use_anim_only::my_type_key.source_hash_code) {
        last_created_size = sizeof(use_anim_only);

        return memory_buffer ? new (memory_buffer) use_anim_only : nullptr;
    }

    if (the_class_to_make == simple_orientation::my_type_key.source_hash_code) {
        last_created_size = sizeof(simple_orientation);

        return memory_buffer ? new (memory_buffer) simple_orientation : nullptr;
    }

    if (the_class_to_make == strafe_mocomp::my_type_key.source_hash_code) {
        last_created_size = sizeof(strafe_mocomp);

        return memory_buffer ? new (memory_buffer) strafe_mocomp : nullptr;
    }

    if (the_class_to_make == relative_orientation::my_type_key.source_hash_code) {
        last_created_size = sizeof(relative_orientation);

        return memory_buffer ? new (memory_buffer) relative_orientation : nullptr;
    }

    if (the_class_to_make == relative_strafe_mocomp::my_type_key.source_hash_code) {
        last_created_size = sizeof(relative_strafe_mocomp);

        return memory_buffer ? new (memory_buffer) relative_strafe_mocomp : nullptr;
    }

    if (the_class_to_make == simple_orientation_on_foot::my_type_key.source_hash_code) {
        last_created_size = sizeof(simple_orientation_on_foot);

        return memory_buffer ? new (memory_buffer) simple_orientation_on_foot : nullptr;
    }

    if (the_class_to_make == strafe_mocomp_on_foot::my_type_key.source_hash_code) {
        last_created_size = sizeof(strafe_mocomp_on_foot);

        return memory_buffer ? new (memory_buffer) strafe_mocomp_on_foot : nullptr;
    }

    if (the_class_to_make == relative_orientation_on_foot::my_type_key.source_hash_code) {
        last_created_size = sizeof(relative_orientation_on_foot);

        return memory_buffer ? new (memory_buffer) relative_orientation_on_foot : nullptr;
    }

    if (the_class_to_make == relative_strafe_mocomp_on_foot::my_type_key.source_hash_code) {
        last_created_size = sizeof(relative_strafe_mocomp_on_foot);

        return memory_buffer ? new (memory_buffer) relative_strafe_mocomp_on_foot : nullptr;
    }

    if (the_class_to_make == velocity_orientation::my_type_key.source_hash_code) {
        last_created_size = sizeof(velocity_orientation);

        return memory_buffer ? new (memory_buffer) velocity_orientation : nullptr;
    }

    if (the_class_to_make == vertical_orientation::my_type_key.source_hash_code) {
        last_created_size = sizeof(vertical_orientation);

        return memory_buffer ? new (memory_buffer) vertical_orientation : nullptr;
    }

    if (the_class_to_make == momentum_grapple_unsynched::my_type_key.source_hash_code) {
        last_created_size = sizeof(momentum_grapple_unsynched);

        return memory_buffer ? new (memory_buffer) momentum_grapple_unsynched : nullptr;
    }

    if (the_class_to_make == initial_velocity_orientation::my_type_key.source_hash_code) {
        last_created_size = sizeof(initial_velocity_orientation);

        return memory_buffer ? new (memory_buffer) initial_velocity_orientation : nullptr;
    }

    if (the_class_to_make == initial3D_velocity_orientation::my_type_key.source_hash_code) {
        last_created_size = sizeof(initial3D_velocity_orientation);

        return memory_buffer ? new (memory_buffer) initial3D_velocity_orientation : nullptr;
    }

    if (the_class_to_make == strict_simple_orientation::my_type_key.source_hash_code) {
        last_created_size = sizeof(strict_simple_orientation);

        return memory_buffer ? new (memory_buffer) strict_simple_orientation : nullptr;
    }

    if (the_class_to_make == direct_mocomp::my_type_key.source_hash_code) {
        last_created_size = sizeof(direct_mocomp);

        return memory_buffer ? new (memory_buffer) direct_mocomp : nullptr;
    }

    if (the_class_to_make == input_orientation::my_type_key.source_hash_code) {
        last_created_size = sizeof(input_orientation);

        return memory_buffer ? new (memory_buffer) input_orientation : nullptr;
    }

    if (the_class_to_make == towed_orientation::my_type_key.source_hash_code) {
        last_created_size = sizeof(towed_orientation);

        return memory_buffer ? new (memory_buffer) towed_orientation : nullptr;
    }

    if (the_class_to_make == ikcrawl_mocomp::my_type_key.source_hash_code) {
        last_created_size = sizeof(ikcrawl_mocomp);

        return memory_buffer ? new (memory_buffer) ikcrawl_mocomp : nullptr;
    }

    if (the_class_to_make == wallsprint_mocomp::my_type_key.source_hash_code) {
        last_created_size = sizeof(wallsprint_mocomp);

        return memory_buffer ? new (memory_buffer) wallsprint_mocomp : nullptr;
    }

    if (the_class_to_make == flight_mocomp::my_type_key.source_hash_code) {
        last_created_size = sizeof(flight_mocomp);

        return memory_buffer ? new (memory_buffer) flight_mocomp : nullptr;
    }

    if (the_class_to_make == move_and_face::my_type_key.source_hash_code) {
        last_created_size = sizeof(move_and_face);

        return memory_buffer ? new (memory_buffer) move_and_face : nullptr;
    }

    if (the_class_to_make == move_and_face_no_anim_movement::my_type_key.source_hash_code) {
        last_created_size = sizeof(move_and_face_no_anim_movement);

        return memory_buffer ? new (memory_buffer) move_and_face_no_anim_movement : nullptr;
    }

    if (the_class_to_make == action_move_and_face::my_type_key.source_hash_code) {
        last_created_size = sizeof(action_move_and_face);

        return memory_buffer ? new (memory_buffer) action_move_and_face : nullptr;
    }

    if (the_class_to_make == action_face_only::my_type_key.source_hash_code) {
        last_created_size = sizeof(action_face_only);

        return memory_buffer ? new (memory_buffer) action_face_only : nullptr;
    }

    if (the_class_to_make == action_move_only::my_type_key.source_hash_code) {
        last_created_size = sizeof(action_move_only);

        return memory_buffer ? new (memory_buffer) action_move_only : nullptr;
    }

    if (the_class_to_make == combat_move_and_face::my_type_key.source_hash_code) {
        last_created_size = sizeof(combat_move_and_face);

        return memory_buffer ? new (memory_buffer) combat_move_and_face : nullptr;
    }

    if (the_class_to_make == combat_face_only::my_type_key.source_hash_code) {
        last_created_size = sizeof(combat_face_only);

        return memory_buffer ? new (memory_buffer) combat_face_only : nullptr;
    }

    if (the_class_to_make == combat_move_only::my_type_key.source_hash_code) {
        last_created_size = sizeof(combat_move_only);

        return memory_buffer ? new (memory_buffer) combat_move_only : nullptr;
    }

    if (the_class_to_make == combat_move_and_face_with_attack::my_type_key.source_hash_code) {
        last_created_size = sizeof(combat_move_and_face_with_attack);

        return memory_buffer ? new (memory_buffer) combat_move_and_face_with_attack : nullptr;
    }

    if (the_class_to_make == combat_face_only_with_attack::my_type_key.source_hash_code) {
        last_created_size = sizeof(combat_face_only_with_attack);

        return memory_buffer ? new (memory_buffer) combat_face_only_with_attack : nullptr;
    }

    if (the_class_to_make == signal_move_and_face::my_type_key.source_hash_code) {
        last_created_size = sizeof(signal_move_and_face);

        return memory_buffer ? new (memory_buffer) signal_move_and_face : nullptr;
    }

    if (the_class_to_make == signal_face_only::my_type_key.source_hash_code) {
        last_created_size = sizeof(signal_face_only);

        return memory_buffer ? new (memory_buffer) signal_face_only : nullptr;
    }

    if (the_class_to_make == signal_move_only::my_type_key.source_hash_code) {
        last_created_size = sizeof(signal_move_only);

        return memory_buffer ? new (memory_buffer) signal_move_only : nullptr;
    }

    if (the_class_to_make == constant_move_and_face::my_type_key.source_hash_code) {
        last_created_size = sizeof(constant_move_and_face);

        return memory_buffer ? new (memory_buffer) constant_move_and_face : nullptr;
    }

    if (the_class_to_make == constant_face_only::my_type_key.source_hash_code) {
        last_created_size = sizeof(constant_face_only);

        return memory_buffer ? new (memory_buffer) constant_face_only : nullptr;
    }

    if (the_class_to_make == constant_move_only::my_type_key.source_hash_code) {
        last_created_size = sizeof(constant_move_only);

        return memory_buffer ? new (memory_buffer) constant_move_only : nullptr;
    }

    if (the_class_to_make == wall_recover_move_and_face::my_type_key.source_hash_code) {
        last_created_size = sizeof(wall_recover_move_and_face);

        return memory_buffer ? new (memory_buffer) wall_recover_move_and_face : nullptr;
    }

    if (the_class_to_make == crawl_transition::my_type_key.source_hash_code) {
        last_created_size = sizeof(crawl_transition);

        return memory_buffer ? new (memory_buffer) crawl_transition : nullptr;
    }

    if (the_class_to_make == rvb_crawl_transition::my_type_key.source_hash_code) {
        last_created_size = sizeof(rvb_crawl_transition);

        return memory_buffer ? new (memory_buffer) rvb_crawl_transition : nullptr;
    }

    if (the_class_to_make == attach_hand_to_point::my_type_key.source_hash_code) {
        last_created_size = sizeof(attach_hand_to_point);

        return memory_buffer ? new (memory_buffer) attach_hand_to_point : nullptr;
    }

    if (the_class_to_make == constant_move_hand_to_point::my_type_key.source_hash_code) {
        last_created_size = sizeof(constant_move_hand_to_point);

        return memory_buffer ? new (memory_buffer) constant_move_hand_to_point : nullptr;
    }

    if (the_class_to_make == combat_pole_swing_attach::my_type_key.source_hash_code) {
        last_created_size = sizeof(combat_pole_swing_attach);

        return memory_buffer ? new (memory_buffer) combat_pole_swing_attach : nullptr;
    }

    if (the_class_to_make == sync_move_and_face::my_type_key.source_hash_code) {
        last_created_size = sizeof(sync_move_and_face);

        return memory_buffer ? new (memory_buffer) sync_move_and_face : nullptr;
    }

    if (the_class_to_make == momentum_sync_move_and_face::my_type_key.source_hash_code) {
        last_created_size = sizeof(momentum_sync_move_and_face);

        return memory_buffer ? new (memory_buffer) momentum_sync_move_and_face : nullptr;
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
// inlined @ sub_5C49E0
// inlined @ sub_5D89D0
// inlined @ sub_5DA810
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
bool als::motion_compensator::is_synced() const {
    return false;
}

// sub_71F870
bool als::motion_compensator::force_reactivate() const {
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
// inlined @ sub_5BFAE0
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

// sub_5AE000
als::simple_orientation::simple_orientation() : currently_rotating(false),
                                                use_movement_window(false),
                                                inside_movement_window(false),
                                                use_accel_decel_mode(false),
                                                use_instant_facing(false) {}

// sub_5AE080
als::simple_orientation::~simple_orientation() {}

// sub_5B4A40
void als::simple_orientation::construct_mashed_class() {
    move_begin_callback.construct_mashed_class();
    move_end_callback.construct_mashed_class();
    rotate_begin_callback.construct_mashed_class();
    rotate_end_callback.construct_mashed_class();
}

// sub_5B4AE0
void als::simple_orientation::destruct_mashed_class() {
    move_begin_callback.destruct_mashed_class();
    move_end_callback.destruct_mashed_class();
    rotate_begin_callback.destruct_mashed_class();
    rotate_end_callback.destruct_mashed_class();
}

// sub_5E33C0
void als::simple_orientation::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    move_begin_callback.unmash(mash_info, this, buffer);
    move_end_callback.unmash(mash_info, this, buffer);
    rotate_begin_callback.unmash(mash_info, this, buffer);
    rotate_end_callback.unmash(mash_info, this, buffer);
}

// sub_5AE100
mash::virtual_types_key als::simple_orientation::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AE530
bool als::simple_orientation::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (motion_compensator::get_virtual_type_key() == parent_class)
        return true;

    if (motion_compensator::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4D6250
i32 als::simple_orientation::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5C49E0
void als::simple_orientation::activate(animation_logic_system* the_als_core, layer_id layer) {
    motion_compensator::activate(the_als_core, layer);

    turn_rate          = 0.0f;
    currently_rotating = false;

    move_begin_callback.add  (the_actor->my_handle, references::move_begin.read(),   generic_event_callback::count_raise, nullptr);
    move_end_callback.add    (the_actor->my_handle, references::move_end.read(),     generic_event_callback::count_raise, nullptr);
    rotate_begin_callback.add(the_actor->my_handle, references::rotate_begin.read(), generic_event_callback::count_raise, nullptr);
    rotate_end_callback.add  (the_actor->my_handle, references::rotate_end.read(),   generic_event_callback::count_raise, nullptr);
}

// sub_5C4AB0
void als::simple_orientation::deactivate() {
    move_begin_callback.remove  (the_actor->my_handle, references::move_begin.read());
    move_end_callback.remove    (the_actor->my_handle, references::move_end.read());
    rotate_begin_callback.remove(the_actor->my_handle, references::rotate_begin.read());
    rotate_end_callback.remove  (the_actor->my_handle, references::rotate_end.read());
}

// sub_5B4B50
void als::simple_orientation::transition() {
    inside_movement_window = false;
    use_movement_window    = the_base->get_optional_pb_int(use_movement_window_id, 0, nullptr) > 0;
    use_accel_decel_mode   = the_base->get_optional_pb_int(use_accel_decel_mode_id, 0, nullptr) > 0;
    use_instant_facing     = the_base->get_optional_pb_int(use_instant_facing_id, 0, nullptr) != 0;

    move_begin_callback.times_raised   = 0;
    move_end_callback.times_raised     = 0;
    rotate_begin_callback.times_raised = 0;
    rotate_end_callback.times_raised   = 0;
}

// sub_5C4B30
void als::simple_orientation::post_anim_action(f32 delta_t) {
    update_movement_window();

    po curr_offset;
    retail::sub_4015B0((u64*)&curr_offset);

    get_po_curr_offset(curr_offset);

    if (use_movement_window && !inside_movement_window) {
        turn_rate          = 0.0f;
        currently_rotating = false;
    } else if (use_instant_facing)
        face_desired_direction();
    else {
        // the actor's own axes, unit length
        auto unitized = [](const vector3d &axis) {
            math::vector4 direction(axis.x, axis.y, axis.z, 0.0f);
            math::vector4 unit;

            retail::sub_401AA0((u64*)&unit, (f32*)&direction);

            return vector3d(unit[0], unit[1], unit[2]);
        };

        vector3d curr_dir = unitized(the_actor->my_abs_po->get_z_facing());
        vector3d curr_up  = unitized(the_actor->my_abs_po->get_y_facing());

        turn_toward_desired(curr_offset, delta_t, curr_dir, curr_up);
    }

    vector3d prev_position = the_actor->my_abs_po->get_position();

    other_po_changes(delta_t, &curr_offset);

    if (!retail::sub_5AA570((f32*)&curr_offset, (f32*)&treyarch::references::po_identity_matrix.get()))
        set_po_curr_offset(the_actor, curr_offset);

    const vector3d &position = the_actor->my_abs_po->get_position();

    the_actor->set_frame_delta_trans(vector3d(position.x - prev_position.x, position.y - prev_position.y, position.z - prev_position.z), delta_t);
}

// inlined @ sub_5C4B30
// inlined @ sub_5C68C0
void als::simple_orientation::update_movement_window() {
    if (use_movement_window) {
        if (move_begin_callback.times_raised || rotate_begin_callback.times_raised) {
            inside_movement_window = true;
            move_begin_signal_time = the_base->get_current_anim_time();

            // the window runs to the matching end signal, else the other one, else the anim's end
            string_hash end_signal = move_begin_callback.times_raised ? references::move_end.read() : references::rotate_end.read();

            f32 time_to_end = the_base->get_time_to_signal(end_signal, nullptr, true, nullptr);
            move_end_signal_time = time_to_end;

            if (time_to_end < 0.0f) {
                string_hash other_end_signal = move_begin_callback.times_raised ? references::rotate_end.read() : references::move_end.read();

                time_to_end = the_base->get_time_to_signal(other_end_signal, nullptr, true, nullptr);
                move_end_signal_time = time_to_end;
            }

            if (time_to_end < 0.0f)
                move_end_signal_time = the_base->get_duration();
            else
                move_end_signal_time = time_to_end + move_begin_signal_time;
        }

        if (move_end_callback.times_raised || rotate_end_callback.times_raised)
            inside_movement_window = false;
    } else
        inside_movement_window = false;

    move_begin_callback.times_raised   = 0;
    move_end_callback.times_raised     = 0;
    rotate_begin_callback.times_raised = 0;
    rotate_end_callback.times_raised   = 0;
}

// inlined @ sub_5C4B30
// inlined @ sub_5C5BE0
// inlined @ sub_5C68C0
void als::simple_orientation::turn_toward_desired(po &curr_offset, f32 delta_t, vector3d curr_dir, vector3d curr_up) {
    vector3d desired_dir;
    vector3d desired_up;

    get_directions(the_als, the_base, &desired_dir, &desired_up);

    if (desired_up.length2() < 0.0001f)
        desired_up = curr_up;
    else
        retail::sub_402050((f32*)&desired_up);

    if (desired_dir.length2() < 0.0001f)
        desired_dir = curr_dir;
    else
        retail::sub_402050((f32*)&desired_dir);

    // the up only goes part of the way per frame
    f32 reorient_speed = the_base->get_optional_pb_float(reorient_to_vertical_speed_id, 1.0f, nullptr);

    if (reorient_speed > 0.0f && reorient_speed < 1.0f)
        desired_up = (desired_up - curr_up) * reorient_speed + curr_up;
    else if (reorient_speed == 0.0f)
        desired_up = curr_up;

    // both directions in the plane of the desired up
    desired_dir = desired_dir - desired_up * (f32)(desired_dir * desired_up);

    if (desired_dir.length2() >= 0.0001f) {
        retail::sub_402050((f32*)&desired_dir);

        curr_dir = curr_dir - desired_up * (f32)(curr_dir * desired_up);

        if (curr_dir.length2() < 0.0001f)
            curr_dir = desired_dir;
        else
            retail::sub_402050((f32*)&curr_dir);

        f32 heading_dot = curr_dir * desired_dir;
        f32 up_dot      = curr_up * desired_up;

        if (!currently_rotating && get_dir_tolerance() < heading_dot && get_up_tolerance() < up_dot)
            turn_rate = 0.0f;
        else {
            const vector3d &zero = treyarch::references::unk_00fd4370.get();

            math::vector4 basis_dir(curr_dir.x, curr_dir.y, curr_dir.z, 0.0f);
            math::vector4 basis_up(desired_up.x, desired_up.y, desired_up.z, 0.0f);
            math::vector4 basis_origin(zero.x, zero.y, zero.z, 0.0f);

            po basis;
            retail::sub_402260((u64*)&basis, (f32*)&basis_dir, (f32*)&basis_up, (i64*)&basis_origin);

            currently_rotating = true;

            if (heading_dot < -0.999999f)
                heading_dot = -0.999999f;
            else if (heading_dot > 0.999999f)
                heading_dot = 0.999999f;

            f32 heading_change = math::acos(heading_dot);

            if (desired_dir * basis.get_x_facing() > 0.0f)
                heading_change *= -1.0f;

            // the state can force the way round
            i32 turn_direction = the_base->get_optional_pb_int(turn_direction_id, 0, nullptr);

            if (turn_direction == -1) {
                if (heading_change < 0.0f)
                    heading_change += 6.2831855f;
            } else if (turn_direction == 1 && heading_change > 0.0f)
                heading_change -= 6.2831855f;

            turn_rate = get_turn_rate(delta_t, the_als, the_base);

            f32  step = turn_rate * delta_t;
            bool overshoots;

            if (heading_change < 0.0f) {
                step *= -1.0f;

                overshoots = heading_change > step;
            } else
                overshoots = step > heading_change;

            if (overshoots) {
                currently_rotating = false;
                step               = heading_change;
            }

            apply_rotation(&curr_offset, delta_t, step, curr_up, curr_dir, desired_up, desired_dir);
        }
    }
}

// sub_5DBE40
void als::simple_orientation::get_directions(animation_logic_system*, state_machine* base, vector3d* desired_dir, vector3d* desired_up) const {
    bool inverse_facing = base->get_optional_pb_int(inverse_facing_id, 0, nullptr) != 0;
    bool use_x_facing   = base->get_optional_pb_int(use_x_facing_id, 0, nullptr) != 0;

    const po* abs_po = the_actor->my_abs_po;

    // facing away from the destination, the direction param, or the actor's own
    bool facing_found = false;

    if (base->get_optional_pb_int(use_destination_facing_id, 0, nullptr) && base->has_ext_param_been_set(145)) {
        vector3d destination = base->get_external_vector(145);

        if (abs_po->get_position().x != destination.x || abs_po->get_position().y != destination.y || abs_po->get_position().z != destination.z) {
            destination = base->get_external_vector(145);

            vector3d away(abs_po->get_position().x - destination.x, abs_po->get_position().y - destination.y, abs_po->get_position().z - destination.z);
            retail::sub_402050((f32*)&away);

            *desired_dir = away;
            facing_found = true;
        }
    }

    if (!facing_found) {
        if (base->has_ext_param_been_set(143))
            *desired_dir = base->get_external_vector(143);
        else
            *desired_dir = abs_po->get_z_facing();
    }

    if (base->has_ext_param_been_set(142))
        *desired_up = base->get_external_vector(142);
    else {
        bool own_up = false;

        if (the_actor->has_ifc(entity_ifc_player)) {
            player_interface* player = the_actor->player_ifc();

            // slot 485
            own_up = ((bool (__thiscall*)(player_interface*))((void**)player->vtable)[485])(player);

            if (!own_up) {
                player = the_actor->player_ifc();

                // slot 487
                own_up = ((bool (__thiscall*)(player_interface*))((void**)player->vtable)[487])(player);
            }
        }

        *desired_up = own_up ? abs_po->get_y_facing() : treyarch::references::unk_00e6808c.get();
    }

    // the facing goes to the side
    if (use_x_facing) {
        const vector3d &zero = treyarch::references::unk_00fd4370.get();

        math::vector4 basis_dir(desired_dir->x, desired_dir->y, desired_dir->z, 0.0f);
        math::vector4 basis_up(desired_up->x, desired_up->y, desired_up->z, 0.0f);
        math::vector4 basis_origin(zero.x, zero.y, zero.z, 0.0f);

        po basis;
        retail::sub_402260((u64*)&basis, (f32*)&basis_dir, (f32*)&basis_up, (i64*)&basis_origin);

        *desired_dir = basis.get_x_facing();
    }

    if (inverse_facing) {
        desired_dir->x = desired_dir->x * -1.0f;
        desired_dir->y = desired_dir->y * -1.0f;
        desired_dir->z = desired_dir->z * -1.0f;
    }
}

// sub_5F2050
f32 als::simple_orientation::get_turn_rate(f32 delta_t, animation_logic_system*, state_machine* base) {
    f32 scale = base->has_ext_param_been_set(3) ? std::fabs(base->get_external_float(3)) : 1.0f;
    f32 rate  = base->has_ext_param_been_set(2) ? base->get_external_float(2) : 0.0f;

    if (rate <= 0.0001f)
        rate = 720.0f;

    // degrees a second
    rate = base->get_optional_pb_float(turn_rate_id, rate, nullptr);
    rate = rate * 0.017453293f;
    rate = rate * scale;

    if (retail::sub_601C50((u32*)the_actor)) {
        turn_rate = rate;

        return turn_rate;
    }

    // ramps up and down through the movement window
    if (use_accel_decel_mode) {
        f32 default_rate = rate > 0.0001f ? (f32)((f64)rate * 57.29577791868205) : 720.0f;

        rate = base->get_optional_pb_float(turn_rate_id, default_rate, nullptr);
        rate = rate * 0.017453293f;

        f32 time     = base->get_current_anim_time();
        f32 progress = (f32)(((f64)time - move_begin_signal_time) / ((f64)move_end_signal_time - move_begin_signal_time));

        if (turn_ramp_up_end > progress) {
            turn_rate = progress / turn_ramp_up_end * rate;

            return turn_rate;
        }

        if (progress > turn_ramp_down_start) {
            f32 remaining = (f32)(1.0 - ((f64)progress - turn_ramp_down_start) / (1.0 - turn_ramp_down_start));

            turn_rate = remaining * rate;

            return turn_rate;
        }

        turn_rate = rate;

        return turn_rate;
    }

    // speeds up to the rate over the accel time; it never slows down here
    if ((f64)rate - 0.0001f > turn_rate) {
        f32 accel_time = base->has_ext_param_been_set(4) ? std::fabs(base->get_external_float(4))
                                                         : base->get_optional_pb_float(turn_accel_time_id, 0.25f, nullptr);

        if (accel_time > 0.01f) {
            turn_rate = (f32)((f64)rate / accel_time * delta_t + turn_rate);

            if (turn_rate > rate)
                turn_rate = rate;

            return turn_rate;
        }

        turn_rate = rate;
    }

    return turn_rate;
}

// sub_AD2D40
void als::simple_orientation::other_po_changes(f32, po*) {}

// sub_5C5560
void als::simple_orientation::apply_rotation(po*, f32, f32 heading_change, const vector3d &, const vector3d &curr_dir, const vector3d &desired_up, const vector3d &) {
    // turns the facing about the desired up
    po rotation;
    retail::sub_4015B0((u64*)&rotation);

    math::vector4 axis(desired_up.x, desired_up.y, desired_up.z, 0.0f);
    retail::sub_40A750((u64*)&rotation, (f32*)&axis, &heading_change);

    const matrix4x4 &m = rotation.get_matrix();

    math::vector4 turned((f32)((f64)m.y.x * curr_dir.y + (f64)m.x.x * curr_dir.x + (f64)m.z.x * curr_dir.z + (f64)m.w.x * 0.0),
                         (f32)((f64)m.y.y * curr_dir.y + (f64)m.x.y * curr_dir.x + (f64)m.z.y * curr_dir.z + (f64)m.w.y * 0.0),
                         (f32)((f64)m.y.z * curr_dir.y + (f64)m.x.z * curr_dir.x + (f64)m.z.z * curr_dir.z + (f64)m.w.z * 0.0),
                         0.0f);

    math::vector4 up(desired_up.x, desired_up.y, desired_up.z, 0.0f);

    const vector3d &position = the_actor->my_abs_po->get_position();
    math::vector4   origin(position.x, position.y, position.z, 0.0f);

    po facing;
    retail::sub_402260((u64*)&facing, (f32*)&turned, (f32*)&up, (i64*)&origin);
    retail::sub_627180((i32)the_actor, (f32*)&facing); // entity_base::set_abs_po
}

// sub_5C57C0
void als::simple_orientation::face_desired_direction() {
    vector3d desired_dir;
    vector3d desired_up;

    get_directions(the_als, the_base, &desired_dir, &desired_up);

    // nothing to build a facing from when they're parallel
    vector3d side = cross(desired_dir, desired_up);

    if ((side - treyarch::references::unk_00fd4370.get()).length2() > 0.00000001f) {
        math::vector4 basis_dir(desired_dir.x, desired_dir.y, desired_dir.z, 0.0f);
        math::vector4 basis_up(desired_up.x, desired_up.y, desired_up.z, 0.0f);

        const vector3d &position = the_actor->my_abs_po->get_position();
        math::vector4   origin(position.x, position.y, position.z, 0.0f);

        po facing;
        retail::sub_402260((u64*)&facing, (f32*)&basis_dir, (f32*)&basis_up, (i64*)&origin);
        retail::sub_627180((i32)the_actor, (f32*)&facing); // entity_base::set_abs_po
    }
}

// sub_5B4BF0
f32 als::simple_orientation::get_dir_tolerance() const {
    return retail::sub_601C50((u32*)the_actor) ? 0.9999f : 0.99f;
}

// sub_5B4BF0
f32 als::simple_orientation::get_up_tolerance() const {
    return retail::sub_601C50((u32*)the_actor) ? 0.9999f : 0.99f;
}

// sub_5AE210
mash::virtual_types_key als::strafe_mocomp::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AEC80
bool als::strafe_mocomp::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (simple_orientation::get_virtual_type_key() == parent_class)
        return true;

    if (simple_orientation::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4D6250
i32 als::strafe_mocomp::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5F2B70
void als::strafe_mocomp::other_po_changes(f32 delta_t, po* curr_offset) {
    vector3d direction = the_actor->my_abs_po->get_z_facing();

    if (the_base->has_ext_param_been_set(144))
        direction = the_base->get_external_vector(144);

    math::vector4 strafe(direction.x, direction.y, direction.z, 0.0f);
    math::vector4 unit;

    retail::sub_401AA0((u64*)&unit, (f32*)&strafe);

    // as fast as the anim moves along the ground, or the state's speed when it doesn't
    vector3d offset = curr_offset->get_position();
    f32      speed  = std::sqrt(offset.x * offset.x + offset.z * offset.z);

    if (speed < 0.0001f) {
        f32 default_speed = the_base->has_ext_param_been_set(0) ? the_base->get_external_float(0) : 1.0f;

        speed = the_base->get_optional_pb_float(movement_speed_id, default_speed, nullptr);
        speed = speed * delta_t;
    }

    const vector3d &position = the_actor->my_abs_po->get_position();
    vector3d        moved(unit[0] * speed + position.x, unit[1] * speed + position.y, position.z + unit[2] * speed);

    retail::sub_6271F0((u32*)the_actor, (i32)&moved); // entity_base::set_abs_position

    // only the height is left for the offset
    curr_offset->get_matrix().w = vector4d(0.0f, offset.y, 0.0f, 1.0f);
}

// sub_5AEE30
mash::virtual_types_key als::relative_orientation::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AEC80
bool als::relative_orientation::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (simple_orientation::get_virtual_type_key() == parent_class)
        return true;

    if (simple_orientation::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4D6250
i32 als::relative_orientation::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5DC240
void als::relative_orientation::get_directions(animation_logic_system*, state_machine* base, vector3d* desired_dir, vector3d* desired_up) const {
    bool inverse_facing = base->get_optional_pb_int(inverse_facing_id, 0, nullptr) != 0;
    bool use_x_facing   = base->get_optional_pb_int(use_x_facing_id, 0, nullptr) != 0;

    const po* abs_po = the_actor->my_abs_po;

    if (base->has_ext_param_been_set(143))
        *desired_dir = base->get_external_vector(143);
    else
        *desired_dir = abs_po->get_z_facing();

    if (base->has_ext_param_been_set(142))
        *desired_up = base->get_external_vector(142);
    else
        *desired_up = abs_po->get_y_facing();

    // the facing goes to the side
    if (use_x_facing) {
        const vector3d &zero = treyarch::references::unk_00fd4370.get();

        math::vector4 basis_dir(desired_dir->x, desired_dir->y, desired_dir->z, 0.0f);
        math::vector4 basis_up(desired_up->x, desired_up->y, desired_up->z, 0.0f);
        math::vector4 basis_origin(zero.x, zero.y, zero.z, 0.0f);

        po basis;
        retail::sub_402260((u64*)&basis, (f32*)&basis_dir, (f32*)&basis_up, (i64*)&basis_origin);

        *desired_dir = basis.get_x_facing();
    }

    if (inverse_facing) {
        desired_dir->x = desired_dir->x * -1.0f;
        desired_dir->y = desired_dir->y * -1.0f;
        desired_dir->z = desired_dir->z * -1.0f;
    }
}

// sub_5AE280
mash::virtual_types_key als::relative_strafe_mocomp::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AE290
bool als::relative_strafe_mocomp::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (strafe_mocomp::get_virtual_type_key() == parent_class)
        return true;

    if (strafe_mocomp::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4D6250
i32 als::relative_strafe_mocomp::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5DC240
void als::relative_strafe_mocomp::get_directions(animation_logic_system*, state_machine* base, vector3d* desired_dir, vector3d* desired_up) const {
    bool inverse_facing = base->get_optional_pb_int(inverse_facing_id, 0, nullptr) != 0;
    bool use_x_facing   = base->get_optional_pb_int(use_x_facing_id, 0, nullptr) != 0;

    const po* abs_po = the_actor->my_abs_po;

    if (base->has_ext_param_been_set(143))
        *desired_dir = base->get_external_vector(143);
    else
        *desired_dir = abs_po->get_z_facing();

    if (base->has_ext_param_been_set(142))
        *desired_up = base->get_external_vector(142);
    else
        *desired_up = abs_po->get_y_facing();

    // the facing goes to the side
    if (use_x_facing) {
        const vector3d &zero = treyarch::references::unk_00fd4370.get();

        math::vector4 basis_dir(desired_dir->x, desired_dir->y, desired_dir->z, 0.0f);
        math::vector4 basis_up(desired_up->x, desired_up->y, desired_up->z, 0.0f);
        math::vector4 basis_origin(zero.x, zero.y, zero.z, 0.0f);

        po basis;
        retail::sub_402260((u64*)&basis, (f32*)&basis_dir, (f32*)&basis_up, (i64*)&basis_origin);

        *desired_dir = basis.get_x_facing();
    }

    if (inverse_facing) {
        desired_dir->x = desired_dir->x * -1.0f;
        desired_dir->y = desired_dir->y * -1.0f;
        desired_dir->z = desired_dir->z * -1.0f;
    }
}

// sub_5AE130
als::simple_orientation_on_foot::simple_orientation_on_foot() : use_fake_foot_ik(false),
                                                                use_fake_leg_ik(false) {}

// sub_7191C0
als::simple_orientation_on_foot::simple_orientation_on_foot(mash::generating_vtable_version so_do_nothing) : simple_orientation(so_do_nothing),
                                                                                                             use_fake_foot_ik(false),
                                                                                                             use_fake_leg_ik(false) {}

// sub_5AE150
mash::virtual_types_key als::simple_orientation_on_foot::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AEC80
bool als::simple_orientation_on_foot::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (simple_orientation::get_virtual_type_key() == parent_class)
        return true;

    if (simple_orientation::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE250
i32 als::simple_orientation_on_foot::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5C59A0
void als::simple_orientation_on_foot::activate(animation_logic_system* the_als_core, layer_id layer) {
    simple_orientation::activate(the_als_core, layer);

    string_hash right_foot = the_base->get_optional_pb_hash(r_foot_id, references::bip01_r_foot.read(), nullptr);
    string_hash left_foot  = the_base->get_optional_pb_hash(l_foot_id, references::bip01_l_foot.read(), nullptr);

    foot_index[0] = the_actor->is_a_conglomerate() ? retail::sub_62F880((u32*)the_actor, (u32*)&right_foot) : -2; // conglomerate::get_member_index
    foot_index[1] = the_actor->is_a_conglomerate() ? retail::sub_62F880((u32*)the_actor, (u32*)&left_foot)  : -2;

    po foot;
    retail::sub_4015B0((u64*)&foot);

    for (u32 index = 0; index < 2; ++index) {
        if (foot_index[index] == -2)
            continue;

        retail::sub_62FC10((i32)the_actor, foot_index[index], (u64*)&foot, 0); // conglomerate::get_member_abs_po

        foot_world_facing[index] = foot.get_z_facing();
        foot_world_pos[index]    = foot.get_position();
    }
}

// sub_5B4C30
void als::simple_orientation_on_foot::transition() {
    simple_orientation::transition();

    use_fake_foot_ik = the_actor->is_a_conglomerate() && the_base->get_optional_pb_int(use_fake_foot_ik_id, 0, nullptr) > 0;
    use_fake_leg_ik  = the_actor->is_a_conglomerate() && the_base->get_optional_pb_int(use_fake_leg_ik_id, 0, nullptr) > 0;
    return_rot_speed = the_base->get_optional_pb_float(return_rot_speed_id, 90.0f, nullptr) * 0.017453293f;
    lock_foot        = the_base->get_optional_pb_int(lock_foot_id, -1, nullptr);
    rot_foot         = -1;
}

// sub_5C5BC0
void als::simple_orientation_on_foot::post_anim_action(f32 delta_t) {
    simple_orientation::post_anim_action(delta_t);
}

// sub_5C5B20
void als::simple_orientation_on_foot::apply_rotation(      po*       curr_offset,
                                                           f32       delta_t,
                                                           f32       heading_change,
                                                     const vector3d &curr_up,
                                                     const vector3d &curr_dir,
                                                     const vector3d &desired_up,
                                                     const vector3d &desired_dir) {

    if (foot_index[1] == -2 || foot_index[0] == -2 ||
        !rotate_about_foot(curr_offset, the_actor, delta_t, heading_change, curr_up, curr_dir, desired_up, desired_dir, foot_index[1], foot_index[0], lock_foot, &rot_foot)) {

        rot_foot = -1;

        simple_orientation::apply_rotation(curr_offset, delta_t, heading_change, curr_up, curr_dir, desired_up, desired_dir);
    }
}

// sub_5C4580
bool als::simple_orientation_on_foot::rotate_about_foot(      po*       curr_offset,
                                                              actor*    target,
                                                              f32       delta_t,
                                                              f32       heading_change,
                                                        const vector3d &,
                                                        const vector3d &curr_dir,
                                                        const vector3d &desired_up,
                                                        const vector3d &,
                                                              i16       left_foot,
                                                              i16       right_foot,
                                                              i32       lock,
                                                              i32*      pivot_foot) {

    vector3d foot_positions[2];

    if (!target->is_a_conglomerate() ||
        !retail::sub_62FA60((i32)target, right_foot, (f32*)&foot_positions[0], 0) || // conglomerate::get_member_abs_position
        !retail::sub_62FA60((i32)target, left_foot,  (f32*)&foot_positions[1], 0))
        return false;

    po inverse;
    retail::sub_48E2C0((f32*)target->my_abs_po, (u64*)&inverse);

    const matrix4x4 &m        = inverse.get_matrix();
    const vector3d  &position = target->my_abs_po->get_position();

    // each foot's height in the actor's frame, as a world height
    vector3d right_offset(foot_positions[0].x - position.x, foot_positions[0].y - position.y, foot_positions[0].z - position.z);
    f32      right_local  = (f32)((f64)m.x.y * right_offset.x + (f64)m.y.y * right_offset.y + (f64)m.z.y * right_offset.z + (f64)m.w.y * 0.0);
    f32      right_height = right_local + position.y;

    vector3d left_offset(foot_positions[1].x - position.x, foot_positions[1].y - position.y, foot_positions[1].z - position.z);
    f32      left_local  = (f32)((f64)m.x.y * left_offset.x + (f64)m.y.y * left_offset.y + (f64)m.z.y * left_offset.z + (f64)m.w.y * 0.0);
    f32      left_height = left_local + position.y;

    // with no lock, level feet give no pivot
    if ((u32)lock > 1 && !(std::fabs(right_height - left_height) > 0.025f))
        return false;

    if (pivot_foot)
        *pivot_foot = (u32)lock > 1 ? left_height <= right_height : lock;

    const vector3d &pivot = (u32)lock > 1 ? (left_height > right_height ? foot_positions[0] : foot_positions[1]) : foot_positions[lock];

    vector3d prev_position = position;

    rotate_about_point(curr_offset, target, desired_up, curr_dir, pivot, desired_up, heading_change);

    const vector3d &moved = target->my_abs_po->get_position();

    target->set_frame_delta_trans(vector3d(moved.x - prev_position.x, moved.y - prev_position.y, moved.z - prev_position.z), delta_t);

    return true;
}

// sub_5C4320
void als::simple_orientation_on_foot::rotate_about_point(      po*,
                                                               actor*    target,
                                                         const vector3d &up,
                                                         const vector3d &dir,
                                                         const vector3d &pivot,
                                                         const vector3d &axis,
                                                               f32       angle) {

    po rotation;
    retail::sub_4015B0((u64*)&rotation);

    math::vector4 axis_dir(axis.x, axis.y, axis.z, 0.0f);
    retail::sub_40A750((u64*)&rotation, (f32*)&axis_dir, &angle);

    const vector3d &position = target->my_abs_po->get_position();

    // the actor's frame about the pivot, turned
    math::vector4 basis_dir(dir.x, dir.y, dir.z, 0.0f);
    math::vector4 basis_up(up.x, up.y, up.z, 0.0f);
    math::vector4 basis_origin(position.x - pivot.x, position.y - pivot.y, position.z - pivot.z, 0.0f);

    po turned;
    retail::sub_402260((u64*)&turned, (f32*)&basis_dir, (f32*)&basis_up, (i64*)&basis_origin);
    retail::sub_44E230((u64*)&turned, (f32*)&turned, (f32*)&rotation);

    const vector4d &rotated = turned.get_matrix().w;
    vector3d        moved(rotated.x + pivot.x, rotated.y + pivot.y, rotated.z + pivot.z);

    const vector3d &zero = treyarch::references::unk_00fd4370.get();

    turned.get_matrix().w = vector4d(zero.x, zero.y, zero.z, 1.0f);
    retail::sub_9595B0((i32)&turned);

    turned.get_matrix().w = vector4d(moved.x, moved.y, moved.z, 1.0f);
    retail::sub_627180((i32)target, (f32*)&turned); // entity_base::set_abs_po
}

// sub_7191F0
als::strafe_mocomp_on_foot::strafe_mocomp_on_foot(mash::generating_vtable_version so_do_nothing) : simple_orientation_on_foot(so_do_nothing) {}

// sub_5AE240
mash::virtual_types_key als::strafe_mocomp_on_foot::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AEF70
bool als::strafe_mocomp_on_foot::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (simple_orientation_on_foot::get_virtual_type_key() == parent_class)
        return true;

    if (simple_orientation_on_foot::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE250
i32 als::strafe_mocomp_on_foot::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5F2B70
void als::strafe_mocomp_on_foot::other_po_changes(f32 delta_t, po* curr_offset) {
    vector3d direction = the_actor->my_abs_po->get_z_facing();

    if (the_base->has_ext_param_been_set(144))
        direction = the_base->get_external_vector(144);

    math::vector4 strafe(direction.x, direction.y, direction.z, 0.0f);
    math::vector4 unit;

    retail::sub_401AA0((u64*)&unit, (f32*)&strafe);

    // as fast as the anim moves along the ground, or the state's speed when it doesn't
    vector3d offset = curr_offset->get_position();
    f32      speed  = std::sqrt(offset.x * offset.x + offset.z * offset.z);

    if (speed < 0.0001f) {
        f32 default_speed = the_base->has_ext_param_been_set(0) ? the_base->get_external_float(0) : 1.0f;

        speed = the_base->get_optional_pb_float(movement_speed_id, default_speed, nullptr);
        speed = speed * delta_t;
    }

    const vector3d &position = the_actor->my_abs_po->get_position();
    vector3d        moved(unit[0] * speed + position.x, unit[1] * speed + position.y, position.z + unit[2] * speed);

    retail::sub_6271F0((u32*)the_actor, (i32)&moved); // entity_base::set_abs_position

    // only the height is left for the offset
    curr_offset->get_matrix().w = vector4d(0.0f, offset.y, 0.0f, 1.0f);
}

// sub_7197D0
als::relative_orientation_on_foot::relative_orientation_on_foot(mash::generating_vtable_version so_do_nothing) : simple_orientation_on_foot(so_do_nothing) {}

// sub_5AEF60
mash::virtual_types_key als::relative_orientation_on_foot::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AEF70
bool als::relative_orientation_on_foot::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (simple_orientation_on_foot::get_virtual_type_key() == parent_class)
        return true;

    if (simple_orientation_on_foot::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE250
i32 als::relative_orientation_on_foot::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5DC240
void als::relative_orientation_on_foot::get_directions(animation_logic_system*, state_machine* base, vector3d* desired_dir, vector3d* desired_up) const {
    bool inverse_facing = base->get_optional_pb_int(inverse_facing_id, 0, nullptr) != 0;
    bool use_x_facing   = base->get_optional_pb_int(use_x_facing_id, 0, nullptr) != 0;

    const po* abs_po = the_actor->my_abs_po;

    if (base->has_ext_param_been_set(143))
        *desired_dir = base->get_external_vector(143);
    else
        *desired_dir = abs_po->get_z_facing();

    if (base->has_ext_param_been_set(142))
        *desired_up = base->get_external_vector(142);
    else
        *desired_up = abs_po->get_y_facing();

    // the facing goes to the side
    if (use_x_facing) {
        const vector3d &zero = treyarch::references::unk_00fd4370.get();

        math::vector4 basis_dir(desired_dir->x, desired_dir->y, desired_dir->z, 0.0f);
        math::vector4 basis_up(desired_up->x, desired_up->y, desired_up->z, 0.0f);
        math::vector4 basis_origin(zero.x, zero.y, zero.z, 0.0f);

        po basis;
        retail::sub_402260((u64*)&basis, (f32*)&basis_dir, (f32*)&basis_up, (i64*)&basis_origin);

        *desired_dir = basis.get_x_facing();
    }

    if (inverse_facing) {
        desired_dir->x = desired_dir->x * -1.0f;
        desired_dir->y = desired_dir->y * -1.0f;
        desired_dir->z = desired_dir->z * -1.0f;
    }
}

// sub_719220
als::relative_strafe_mocomp_on_foot::relative_strafe_mocomp_on_foot(mash::generating_vtable_version so_do_nothing) : strafe_mocomp_on_foot(so_do_nothing) {}

// sub_5AE320
mash::virtual_types_key als::relative_strafe_mocomp_on_foot::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AE330
bool als::relative_strafe_mocomp_on_foot::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (strafe_mocomp_on_foot::get_virtual_type_key() == parent_class)
        return true;

    if (strafe_mocomp_on_foot::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE250
i32 als::relative_strafe_mocomp_on_foot::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5DC240
void als::relative_strafe_mocomp_on_foot::get_directions(animation_logic_system*, state_machine* base, vector3d* desired_dir, vector3d* desired_up) const {
    bool inverse_facing = base->get_optional_pb_int(inverse_facing_id, 0, nullptr) != 0;
    bool use_x_facing   = base->get_optional_pb_int(use_x_facing_id, 0, nullptr) != 0;

    const po* abs_po = the_actor->my_abs_po;

    if (base->has_ext_param_been_set(143))
        *desired_dir = base->get_external_vector(143);
    else
        *desired_dir = abs_po->get_z_facing();

    if (base->has_ext_param_been_set(142))
        *desired_up = base->get_external_vector(142);
    else
        *desired_up = abs_po->get_y_facing();

    // the facing goes to the side
    if (use_x_facing) {
        const vector3d &zero = treyarch::references::unk_00fd4370.get();

        math::vector4 basis_dir(desired_dir->x, desired_dir->y, desired_dir->z, 0.0f);
        math::vector4 basis_up(desired_up->x, desired_up->y, desired_up->z, 0.0f);
        math::vector4 basis_origin(zero.x, zero.y, zero.z, 0.0f);

        po basis;
        retail::sub_402260((u64*)&basis, (f32*)&basis_dir, (f32*)&basis_up, (i64*)&basis_origin);

        *desired_dir = basis.get_x_facing();
    }

    if (inverse_facing) {
        desired_dir->x = desired_dir->x * -1.0f;
        desired_dir->y = desired_dir->y * -1.0f;
        desired_dir->z = desired_dir->z * -1.0f;
    }
}

// sub_5AEC10
mash::virtual_types_key als::velocity_orientation::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AEC80
bool als::velocity_orientation::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (simple_orientation::get_virtual_type_key() == parent_class)
        return true;

    if (simple_orientation::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4D6250
i32 als::velocity_orientation::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5F37B0
void als::velocity_orientation::get_directions(animation_logic_system*, state_machine* base, vector3d* desired_dir, vector3d* desired_up) const {
    // the actor's ground velocity, unless param 105 says otherwise
    if (the_actor->has_ifc(entity_ifc_physical) && (!base->has_ext_param_been_set(105) || base->get_external_float(105) == 0.0f)) {
        vector3d velocity;
        retail::sub_60F130((i32)the_actor->physical_ifc(), (i32)&velocity); // physical_interface::get_velocity

        *desired_dir = vector3d(velocity.x, 0.0f, velocity.z);
    } else if (base->has_ext_param_been_set(143))
        *desired_dir = base->get_external_vector(143);
    else
        *desired_dir = the_actor->my_abs_po->get_z_facing();

    // too slow to say
    if (desired_dir->length2() < 1.0f)
        *desired_dir = the_actor->my_abs_po->get_z_facing();

    math::vector4 direction(desired_dir->x, desired_dir->y, desired_dir->z, 0.0f);
    math::vector4 unit;

    retail::sub_401AA0((u64*)&unit, (f32*)&direction);

    *desired_dir = vector3d(unit[0], unit[1], unit[2]);
    *desired_up  = treyarch::references::unk_00e6808c.get();
}

// sub_5AEE60
mash::virtual_types_key als::vertical_orientation::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AEE70
bool als::vertical_orientation::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (relative_orientation::get_virtual_type_key() == parent_class)
        return true;

    if (relative_orientation::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4D6250
i32 als::vertical_orientation::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5DCF50
void als::vertical_orientation::get_directions(animation_logic_system*, state_machine* base, vector3d* desired_dir, vector3d* desired_up) const {
    bool inverse_facing = base->get_optional_pb_int(inverse_facing_id, 0, nullptr) != 0;
    bool use_x_facing   = base->get_optional_pb_int(use_x_facing_id, 0, nullptr) != 0;

    *desired_dir = treyarch::references::unk_00e6808c.get();

    if (base->has_ext_param_been_set(142))
        *desired_up = base->get_external_vector(142);
    else
        *desired_up = the_actor->my_abs_po->get_y_facing();

    // the facing goes to the side
    if (use_x_facing) {
        const vector3d &zero = treyarch::references::unk_00fd4370.get();

        math::vector4 basis_dir(desired_dir->x, desired_dir->y, desired_dir->z, 0.0f);
        math::vector4 basis_up(desired_up->x, desired_up->y, desired_up->z, 0.0f);
        math::vector4 basis_origin(zero.x, zero.y, zero.z, 0.0f);

        po basis;
        retail::sub_402260((u64*)&basis, (f32*)&basis_dir, (f32*)&basis_up, (i64*)&basis_origin);

        *desired_dir = basis.get_x_facing();
    }

    if (inverse_facing) {
        desired_dir->x = desired_dir->x * -1.0f;
        desired_dir->y = desired_dir->y * -1.0f;
        desired_dir->z = desired_dir->z * -1.0f;
    }
}

// sub_5AEAF0
mash::virtual_types_key als::momentum_grapple_unsynched::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AEC80
bool als::momentum_grapple_unsynched::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (simple_orientation::get_virtual_type_key() == parent_class)
        return true;

    if (simple_orientation::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4D6250
i32 als::momentum_grapple_unsynched::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5DABE0
void als::momentum_grapple_unsynched::activate(animation_logic_system* the_als_core, layer_id layer) {
    simple_orientation::activate(the_als_core, layer);

    if (the_actor->has_ifc(entity_ifc_standing))
        ((standing_interface*)the_actor->my_ifc_storage->get_ifc(entity_ifc_standing))->unk_044 &= ~1;

    if (the_actor->has_ifc(entity_ifc_physical))
        retail::sub_6007A0((u32*)the_actor->physical_ifc(), false); // physical_interface: flags bit 0
}

// sub_5DAC50
void als::momentum_grapple_unsynched::deactivate() {
    simple_orientation::deactivate();

    if (the_actor->has_ifc(entity_ifc_standing))
        ((standing_interface*)the_actor->my_ifc_storage->get_ifc(entity_ifc_standing))->unk_044 |= 1;

    if (the_actor->has_ifc(entity_ifc_physical))
        retail::sub_6007A0((u32*)the_actor->physical_ifc(), true);
}

// sub_5AEC70
mash::virtual_types_key als::initial_velocity_orientation::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AEC80
bool als::initial_velocity_orientation::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (simple_orientation::get_virtual_type_key() == parent_class)
        return true;

    if (simple_orientation::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4E1E90
i32 als::initial_velocity_orientation::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5DC4A0
void als::initial_velocity_orientation::activate(animation_logic_system* the_als_core, layer_id layer) {
    auto unitized = [](const vector3d &value) {
        math::vector4 direction(value.x, value.y, value.z, 0.0f);
        math::vector4 unit;

        retail::sub_401AA0((u64*)&unit, (f32*)&direction);

        return vector3d(unit[0], unit[1], unit[2]);
    };

    simple_orientation::activate(the_als_core, layer);

    bool inverse_velocity = the_base->get_optional_pb_int(inverse_velocity_id, 0, nullptr) != 0;
    bool use_x_facing     = the_base->get_optional_pb_int(use_x_facing_id, 0, nullptr) != 0;

    // the actor's velocity, the direction param, or its facing; along the ground
    if (the_actor->has_ifc(entity_ifc_physical))
        retail::sub_60F130((i32)the_actor->physical_ifc(), (i32)&initial_dir); // physical_interface::get_velocity
    else if (the_base->has_ext_param_been_set(143))
        initial_dir = the_base->get_external_vector(143);
    else
        initial_dir = the_actor->my_abs_po->get_z_facing();

    initial_dir.y = 0.0f;

    if (initial_dir.length2() < 1.0f)
        initial_dir = the_actor->my_abs_po->get_z_facing();

    initial_dir = unitized(initial_dir);

    // straight up or down has no ground direction
    if (vector3d(cross(treyarch::references::unk_00e6808c.get(), initial_dir)).length2() < 0.01f)
        initial_dir = treyarch::references::unk_00e68098.get();

    if (inverse_velocity) {
        initial_dir.x = initial_dir.x * -1.0f;
        initial_dir.y = initial_dir.y * -1.0f;
        initial_dir.z = initial_dir.z * -1.0f;
    }

    // the facing goes to the other side
    if (use_x_facing) {
        const vector3d &up   = treyarch::references::unk_00e6808c.get();
        const vector3d &zero = treyarch::references::unk_00fd4370.get();

        math::vector4 basis_dir(initial_dir.x, initial_dir.y, initial_dir.z, 0.0f);
        math::vector4 basis_up(up.x, up.y, up.z, 0.0f);
        math::vector4 basis_origin(zero.x, zero.y, zero.z, 0.0f);

        po basis;
        retail::sub_402260((u64*)&basis, (f32*)&basis_dir, (f32*)&basis_up, (i64*)&basis_origin);

        initial_dir = vector3d(-basis.get_x_facing().x, -basis.get_x_facing().y, -basis.get_x_facing().z);
    }
}

// sub_667F80
bool als::initial_velocity_orientation::force_reactivate() const {
    return true;
}

// sub_5B4D50
void als::initial_velocity_orientation::get_directions(animation_logic_system*, state_machine*, vector3d* desired_dir, vector3d* desired_up) const {
    *desired_dir = initial_dir;
    *desired_up  = treyarch::references::unk_00e6808c.get();
}

// sub_5AED00
mash::virtual_types_key als::initial3D_velocity_orientation::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AEC80
bool als::initial3D_velocity_orientation::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (simple_orientation::get_virtual_type_key() == parent_class)
        return true;

    if (simple_orientation::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AED10
i32 als::initial3D_velocity_orientation::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5DC880
void als::initial3D_velocity_orientation::activate(animation_logic_system* the_als_core, layer_id layer) {
    auto unitized = [](const vector3d &value) {
        math::vector4 direction(value.x, value.y, value.z, 0.0f);
        math::vector4 unit;

        retail::sub_401AA0((u64*)&unit, (f32*)&direction);

        return vector3d(unit[0], unit[1], unit[2]);
    };

    simple_orientation::activate(the_als_core, layer);

    bool inverse_velocity = the_base->get_optional_pb_int(inverse_velocity_id, 0, nullptr) != 0;
    bool use_x_facing     = the_base->get_optional_pb_int(use_x_facing_id, 0, nullptr) != 0;

    // the actor's velocity, the direction param, or its facing; along the ground
    if (the_actor->has_ifc(entity_ifc_physical))
        retail::sub_60F130((i32)the_actor->physical_ifc(), (i32)&initial_dir); // physical_interface::get_velocity
    else if (the_base->has_ext_param_been_set(143))
        initial_dir = the_base->get_external_vector(143);
    else
        initial_dir = the_actor->my_abs_po->get_z_facing();

    initial_dir.y = 0.0f;

    if (initial_dir.length2() < 1.0f)
        initial_dir = the_actor->my_abs_po->get_z_facing();

    initial_dir = unitized(initial_dir);

    // straight up or down has no ground direction
    if (vector3d(cross(treyarch::references::unk_00e6808c.get(), initial_dir)).length2() < 0.01f)
        initial_dir = treyarch::references::unk_00e68098.get();

    if (inverse_velocity) {
        initial_dir.x = initial_dir.x * -1.0f;
        initial_dir.y = initial_dir.y * -1.0f;
        initial_dir.z = initial_dir.z * -1.0f;
    }

    // the facing goes to the other side
    if (use_x_facing) {
        const vector3d &up   = treyarch::references::unk_00e6808c.get();
        const vector3d &zero = treyarch::references::unk_00fd4370.get();

        math::vector4 basis_dir(initial_dir.x, initial_dir.y, initial_dir.z, 0.0f);
        math::vector4 basis_up(up.x, up.y, up.z, 0.0f);
        math::vector4 basis_origin(zero.x, zero.y, zero.z, 0.0f);

        po basis;
        retail::sub_402260((u64*)&basis, (f32*)&basis_dir, (f32*)&basis_up, (i64*)&basis_origin);

        initial_dir = vector3d(-basis.get_x_facing().x, -basis.get_x_facing().y, -basis.get_x_facing().z);
    }

    // and the up the velocity leans to
    if (the_actor->has_ifc(entity_ifc_physical)) {
        vector3d velocity;
        retail::sub_60F130((i32)the_actor->physical_ifc(), (i32)&velocity);

        vector3d heading = unitized(velocity);
        vector3d side    = unitized(cross(heading, treyarch::references::unk_00e6808c.get()));

        initial_up = cross(side, heading);
        initial_up = unitized(initial_up);
    } else if (the_base->has_ext_param_been_set(142))
        initial_up = the_base->get_external_vector(142);
    else
        initial_up = treyarch::references::unk_00e6808c.get();
}

// sub_667F80
bool als::initial3D_velocity_orientation::force_reactivate() const {
    return true;
}

// sub_5B4DA0
void als::initial3D_velocity_orientation::get_directions(animation_logic_system*, state_machine*, vector3d* desired_dir, vector3d* desired_up) const {
    *desired_dir = initial_dir;
    *desired_up  = initial_up;
}

// sub_5AE190
als::strict_simple_orientation::strict_simple_orientation() : inside_rotate_window(false) {}

// sub_5AE1B0
mash::virtual_types_key als::strict_simple_orientation::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AEC80
bool als::strict_simple_orientation::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (simple_orientation::get_virtual_type_key() == parent_class)
        return true;

    if (simple_orientation::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4BF700
i32 als::strict_simple_orientation::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5C5BD0
void als::strict_simple_orientation::activate(animation_logic_system* the_als_core, layer_id layer) {
    simple_orientation::activate(the_als_core, layer);
}

// sub_5C5BE0
void als::strict_simple_orientation::post_anim_action(f32 delta_t) {
    if (move_begin_callback.times_raised)
        inside_movement_window = true;

    if (move_end_callback.times_raised)
        inside_movement_window = false;

    // the rotate window runs to its end signal, else the anim's end
    if (rotate_begin_callback.times_raised) {
        inside_rotate_window   = true;
        move_begin_signal_time = the_base->get_current_anim_time();

        f32 time_to_end = the_base->get_time_to_signal(references::rotate_end.read(), nullptr, true, nullptr);
        move_end_signal_time = time_to_end;

        if (time_to_end >= 0.0f)
            move_end_signal_time = time_to_end + move_begin_signal_time;
        else
            move_end_signal_time = the_base->get_duration();
    }

    if (rotate_end_callback.times_raised)
        inside_rotate_window = false;

    move_begin_callback.times_raised   = 0;
    move_end_callback.times_raised     = 0;
    rotate_begin_callback.times_raised = 0;
    rotate_end_callback.times_raised   = 0;

    po curr_offset;
    retail::sub_4015B0((u64*)&curr_offset);

    get_po_curr_offset(curr_offset);

    if (!inside_rotate_window) {
        turn_rate          = 0.0f;
        currently_rotating = false;
    } else if (use_instant_facing)
        face_desired_direction();
    else {
        vector3d curr_dir = the_actor->my_abs_po->get_z_facing();
        vector3d curr_up  = the_actor->my_abs_po->get_y_facing();

        retail::sub_402050((f32*)&curr_dir);
        retail::sub_402050((f32*)&curr_up);

        turn_toward_desired(curr_offset, delta_t, curr_dir, curr_up);
    }

    vector3d prev_position = the_actor->my_abs_po->get_position();

    if (inside_movement_window)
        other_po_changes(delta_t, &curr_offset);

    if (!retail::sub_5AA570((f32*)&curr_offset, (f32*)&treyarch::references::po_identity_matrix.get()))
        set_po_curr_offset(the_actor, curr_offset);

    const vector3d &position = the_actor->my_abs_po->get_position();

    the_actor->set_frame_delta_trans(vector3d(position.x - prev_position.x, position.y - prev_position.y, position.z - prev_position.z), delta_t);
}

// sub_5F23B0
void als::strict_simple_orientation::other_po_changes(f32 delta_t, po*) {
    vector3d direction = the_actor->my_abs_po->get_z_facing();

    if (the_base->has_ext_param_been_set(144))
        direction = the_base->get_external_vector(144);

    math::vector4 strafe(direction.x, direction.y, direction.z, 0.0f);
    math::vector4 unit;

    retail::sub_401AA0((u64*)&unit, (f32*)&strafe);

    f32 speed = 1.0f;

    if (the_base->has_ext_param_been_set(0))
        speed = the_base->get_external_float(0);

    f32 step = speed * delta_t;

    const vector3d &position = the_actor->my_abs_po->get_position();
    vector3d        moved(step * unit[0] + position.x, unit[1] * step + position.y, position.z + unit[2] * step);

    retail::sub_6271F0((u32*)the_actor, (i32)&moved); // entity_base::set_abs_position
}

// sub_5AE390
mash::virtual_types_key als::direct_mocomp::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AEC80
bool als::direct_mocomp::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (simple_orientation::get_virtual_type_key() == parent_class)
        return true;

    if (simple_orientation::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE3A0
i32 als::direct_mocomp::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5F4FF0
void als::direct_mocomp::activate(animation_logic_system* the_als_core, layer_id layer) {
    simple_orientation::activate(the_als_core, layer);

    direct_mocomp::update_params(0.0001f);
}

// sub_5CD0E0
void als::direct_mocomp::post_anim_action(f32 delta_t) {
    update_params(delta_t);

    simple_orientation::post_anim_action(delta_t);
}

// sub_667F80
bool als::direct_mocomp::force_reactivate() const {
    return true;
}

// sub_5B6BB0
void als::direct_mocomp::get_directions(animation_logic_system*, state_machine*, vector3d* desired_dir, vector3d* desired_up) const {
    *desired_dir = des_face_dir;
    *desired_up  = des_up_dir;
}

// sub_5B6BE0
void als::direct_mocomp::other_po_changes(f32 delta_t, po*) {
    f32 step = des_speed * delta_t;

    const vector3d &position = the_actor->my_abs_po->get_position();
    vector3d        moved(des_move_dir.x * step + position.x, des_move_dir.y * step + position.y, position.z + des_move_dir.z * step);

    retail::sub_6271F0((u32*)the_actor, (i32)&moved); // entity_base::set_abs_position
}

// sub_5F3EC0
void als::direct_mocomp::update_params(f32) {
    if (the_base->has_ext_param_been_set(144))
        des_move_dir = the_base->get_external_vector(144);
    else
        des_move_dir = the_actor->my_abs_po->get_z_facing();

    if (the_base->has_ext_param_been_set(143))
        des_face_dir = the_base->get_external_vector(143);
    else
        des_face_dir = des_move_dir;

    if (the_base->has_ext_param_been_set(142))
        des_up_dir = the_base->get_external_vector(142);
    else
        des_up_dir = the_actor->my_abs_po->get_y_facing();

    des_speed = the_base->has_ext_param_been_set(0) ? the_base->get_external_float(0) : 1.0f;
}

// sub_5AEED0
mash::virtual_types_key als::input_orientation::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AEE70
bool als::input_orientation::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (relative_orientation::get_virtual_type_key() == parent_class)
        return true;

    if (relative_orientation::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4D6250
i32 als::input_orientation::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5C6510
void als::input_orientation::deactivate() {
    the_base->set_speed(1.0f);

    relative_orientation::deactivate();
}

// sub_5C6530
void als::input_orientation::post_anim_action(f32 delta_t) {
    relative_orientation::post_anim_action(delta_t);

    f32 speed     = the_base->get_speed();
    f32 max_speed = the_base->get_optional_pb_float(max_playback_speed_id, 1.0f, nullptr);
    f32 min_speed = the_base->get_optional_pb_float(min_playback_speed_id, 1.0f, nullptr);

    if (min_speed > speed)
        the_base->set_speed(min_speed);
    else if (speed > max_speed)
        the_base->set_speed(max_speed);
}

// sub_5F2530
void als::input_orientation::other_po_changes(f32 delta_t, po* curr_offset) {
    auto unitized = [](const vector3d &value) {
        math::vector4 direction(value.x, value.y, value.z, 0.0f);
        math::vector4 unit;

        retail::sub_401AA0((u64*)&unit, (f32*)&direction);

        return vector3d(unit[0], unit[1], unit[2]);
    };

    // the anim only gives the speed
    vector3d anim_offset = curr_offset->get_position();
    f32      anim_speed  = anim_offset.length() / delta_t;

    const vector3d &zero = treyarch::references::unk_00fd4370.get();

    curr_offset->get_matrix().w = vector4d(zero.x, zero.y, zero.z, 1.0f);

    // the anim's direction in the world, the move param or the facing
    vector3d move_dir = the_actor->my_abs_po->get_z_facing();

    if (the_base->get_optional_pb_int(move_dir_from_anim_id, 0, nullptr)) {
        vector3d dir = unitized(anim_offset);

        const po       &abs_po   = *the_actor->my_abs_po;
        const vector3d &x_row    = abs_po.get_x_facing();
        const vector3d &y_row    = abs_po.get_y_facing();
        const vector3d &z_row    = abs_po.get_z_facing();
        const vector3d &position = abs_po.get_position();

        move_dir = vector3d((f32)((f64)y_row.x * dir.y + (f64)dir.x * x_row.x + (f64)z_row.x * dir.z + (f64)position.x * 0.0),
                            (f32)((f64)x_row.y * dir.x + (f64)y_row.y * dir.y + (f64)z_row.y * dir.z + (f64)position.y * 0.0),
                            (f32)((f64)x_row.z * dir.x + (f64)y_row.z * dir.y + (f64)z_row.z * dir.z + (f64)position.z * 0.0));
    } else if (the_base->has_ext_param_been_set(144))
        move_dir = the_base->get_external_vector(144);

    // pulled back toward the facing
    f32 blend = the_base->get_optional_pb_float(facing_input_dir_blend_id, -1.0f, nullptr);

    if (blend >= 0.0f) {
        const vector3d &facing = the_actor->my_abs_po->get_z_facing();

        f32 keep = 1.0f - blend;

        move_dir = unitized(vector3d(facing.x * keep + move_dir.x * blend,
                                     facing.y * keep + move_dir.y * blend,
                                     facing.z * keep + move_dir.z * blend));
    }

    f32 speed = anim_speed;

    if (speed_override_enabled > 0 && the_base->get_optional_pb_int(allow_speed_override_id, 0, nullptr) && the_base->has_ext_param_been_set(0))
        speed = the_base->get_external_float(0);

    // plays the anim as fast as the actor moves
    if (match_playback_speed) {
        f32 max_speed = the_base->get_optional_pb_float(max_playback_speed_id, 1.0f, nullptr);
        f32 min_speed = the_base->get_optional_pb_float(min_playback_speed_id, 1.0f, nullptr);
        f32 rate      = speed / (anim_speed / the_base->get_speed());

        if (std::fabs(1.0f - rate) < 0.0001f)
            rate = 1.0f;

        if (min_speed > rate)
            rate = min_speed;
        else if (rate > max_speed)
            rate = max_speed;

        the_base->set_speed(rate);
    }

    f32 step = speed * delta_t;

    const vector3d &position = the_actor->my_abs_po->get_position();
    vector3d        moved(step * move_dir.x + position.x, position.y + move_dir.y * step, position.z + move_dir.z * step);

    retail::sub_6271F0((u32*)the_actor, (i32)&moved); // entity_base::set_abs_position
}

// sub_5AEFE0
mash::virtual_types_key als::towed_orientation::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AEC80
bool als::towed_orientation::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (simple_orientation::get_virtual_type_key() == parent_class)
        return true;

    if (simple_orientation::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4D6250
i32 als::towed_orientation::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5C7EF0
void als::towed_orientation::post_anim_action(f32 delta_t) {
    auto unitized = [](const vector3d &value) {
        math::vector4 direction(value.x, value.y, value.z, 0.0f);
        math::vector4 unit;

        retail::sub_401AA0((u64*)&unit, (f32*)&direction);

        return vector3d(unit[0], unit[1], unit[2]);
    };

    vector3d tow_point = get_tow_point();
    vector3d position  = the_actor->my_abs_po->get_position();

    // one unit ahead of the actor without a towing interface
    vector3d hitch_point(0.0f, 0.0f, 1.0f);

    if (the_actor->has_ifc(entity_ifc_towing))
        hitch_point = the_actor->towing_ifc()->hitch_point;

    vector3d back_from_hitch(-hitch_point.x, -hitch_point.y, -hitch_point.z);
    vector3d to_tow_point = tow_point - position;

    // straight under or over the tow point there's no way to face
    if (to_tow_point.xz_length2() > 0.0001f) {
        vector3d heading = unitized(to_tow_point);
        vector3d side    = unitized(cross(heading, treyarch::references::unk_00e6808c.get()));
        vector3d up      = unitized(cross(side, heading));

        vector3d placed(tow_point.x + side.x * back_from_hitch.x + back_from_hitch.y * up.x + back_from_hitch.z * heading.x,
                        tow_point.y + side.y * back_from_hitch.x + up.y * back_from_hitch.y + heading.y * back_from_hitch.z,
                        tow_point.z + side.z * back_from_hitch.x + up.z * back_from_hitch.y + heading.z * back_from_hitch.z);

        math::vector4 basis_dir(heading.x, heading.y, heading.z, 0.0f);
        math::vector4 basis_up(up.x, up.y, up.z, 0.0f);
        math::vector4 basis_origin(placed.x, placed.y, placed.z, 0.0f);

        po towed;
        retail::sub_402260((u64*)&towed, (f32*)&basis_dir, (f32*)&basis_up, (i64*)&basis_origin);

        vector3d prev_position = the_actor->my_abs_po->get_position();

        retail::sub_627180((i32)the_actor, (f32*)&towed); // entity_base::set_abs_po

        const vector3d &moved = the_actor->my_abs_po->get_position();

        vector3d delta(moved.x - prev_position.x, moved.y - prev_position.y, moved.z - prev_position.z);
        vector3d settled(delta.x + prev_position.x, delta.y + prev_position.y, delta.z + prev_position.z);

        retail::sub_6271F0((u32*)the_actor, (i32)&settled); // entity_base::set_abs_position

        the_actor->set_frame_delta_trans(delta, delta_t);
    }
}

// sub_5C7C70
vector3d als::towed_orientation::get_tow_point() const {
    if (the_actor->has_ifc(entity_ifc_towing)) {
        towing_interface* towing = the_actor->towing_ifc();

        if (towing && towing->tower.resolve()) {
            entity* tower = (entity*)towing->tower.resolve();

            if (tower && tower->has_ifc(entity_ifc_towing)) {
                vector3d local = tower->towing_ifc()->tow_point;

                const po       &tower_po = *tower->my_abs_po;
                const vector3d &x_row    = tower_po.get_x_facing();
                const vector3d &y_row    = tower_po.get_y_facing();
                const vector3d &z_row    = tower_po.get_z_facing();
                const vector3d &position = tower_po.get_position();

                return vector3d(x_row.x * local.x + position.x + y_row.x * local.y + z_row.x * local.z,
                                x_row.y * local.x + position.y + y_row.y * local.y + z_row.y * local.z,
                                x_row.z * local.x + position.z + y_row.z * local.y + z_row.z * local.z);
            }
        }
    }

    return the_actor->my_abs_po->get_position();
}

// sub_5AEF30
mash::virtual_types_key als::ikcrawl_mocomp::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AEE70
bool als::ikcrawl_mocomp::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (relative_orientation::get_virtual_type_key() == parent_class)
        return true;

    if (relative_orientation::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4D6250
i32 als::ikcrawl_mocomp::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5CD090
void als::ikcrawl_mocomp::activate(animation_logic_system* the_als_core, layer_id layer) {
    using ikcrawl_begin_t = void (__cdecl*)(actor*, animation_logic_system*, layer_id);

    // the_actor is still the last activation's here
    ((ikcrawl_begin_t)retail::sub_5CCF00)(the_actor, the_als_core, layer); // ikcrawl begin

    relative_orientation::activate(the_als_core, layer);
}

// sub_5CD0C0
void als::ikcrawl_mocomp::deactivate() {
    retail::sub_5CCFE0((i32)the_actor); // ikcrawl end

    relative_orientation::deactivate();
}

// sub_5B6BA0
void als::ikcrawl_mocomp::transition() {
    if (!ikcrawl_enabled)
        relative_orientation::transition();
}

// sub_5E7770
void als::ikcrawl_mocomp::post_anim_action(f32 delta_t) {
    if (!ikcrawl_enabled) {
        relative_orientation::post_anim_action(delta_t);

        return;
    }

    using ikcrawl_gather_t = bool (__cdecl*)(actor*, state_machine*, f32);

    // ikcrawl: gathers the geometry around the actor
    if (!((ikcrawl_gather_t)retail::sub_5E63D0)(the_actor, the_base, delta_t))
        return;

    vector3d prev_position = the_actor->my_abs_po->get_position();

    po curr_offset;
    retail::sub_4015B0((u64*)&curr_offset);

    po moved;
    retail::sub_4015B0((u64*)&moved);

    get_po_curr_offset(curr_offset);

    retail::sub_44E230((u64*)&moved, (f32*)&curr_offset, (f32*)the_actor->my_rel_po);
    retail::sub_9595B0((i32)&moved);

    retail::sub_5E6760((u32*)the_actor, delta_t, (f32*)&prev_position, (i32)&curr_offset, (f32*)&moved); // ikcrawl: crawls on it
}

// sub_5DD2B0
void als::ikcrawl_mocomp::get_directions(animation_logic_system* als_core, state_machine* base, vector3d* desired_dir, vector3d* desired_up) const {
    if (!ikcrawl_enabled)
        relative_orientation::get_directions(als_core, base, desired_dir, desired_up);
}

// sub_5AEFD0
mash::virtual_types_key als::wallsprint_mocomp::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AEE70
bool als::wallsprint_mocomp::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (relative_orientation::get_virtual_type_key() == parent_class)
        return true;

    if (relative_orientation::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4D6250
i32 als::wallsprint_mocomp::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5C68C0
void als::wallsprint_mocomp::post_anim_action(f32 delta_t) {
    auto unitized = [](const vector3d &value) {
        math::vector4 direction(value.x, value.y, value.z, 0.0f);
        math::vector4 unit;

        retail::sub_401AA0((u64*)&unit, (f32*)&direction);

        return vector3d(unit[0], unit[1], unit[2]);
    };

    wall_sprint_inode* wall = (wall_sprint_inode*)the_actor->get_ai_core()->get_info_node(info_node_type_wall_sprint);

    update_movement_window();

    po curr_offset;
    retail::sub_4015B0((u64*)&curr_offset);

    get_po_curr_offset(curr_offset);

    if (use_movement_window && !inside_movement_window) {
        turn_rate          = 0.0f;
        currently_rotating = false;
    } else if (use_instant_facing)
        face_desired_direction();
    else {
        // the wall's axes, unit length
        vector3d wall_dir = wall->get_wall_po()->get_z_facing();
        vector3d wall_up  = wall->get_wall_po()->get_y_facing();

        vector3d curr_dir = unitized(wall_dir);
        vector3d curr_up  = unitized(wall_up);

        turn_toward_desired(curr_offset, delta_t, curr_dir, curr_up);
    }

    // the actor takes the wall's axes, a step at a time while the inode asks for more than one
    po placed;

    if (wall->unk_058 > 1) {
        const po &abs_po   = *the_actor->my_abs_po;
        vector3d  position = abs_po.get_position();
        vector4d  y_row    = abs_po.get_matrix().y;
        vector4d  z_row    = abs_po.get_matrix().z;

        const matrix4x4 &wall_m = wall->get_wall_po()->get_matrix();

        f32 step = 1.0f / (f32)wall->unk_058;

        vector3d dir(z_row.x + (wall_m.z.x - z_row.x) * step, z_row.y + (wall_m.z.y - z_row.y) * step, (wall_m.z.z - z_row.z) * step + z_row.z);
        vector3d up(y_row.x + (wall_m.y.x - y_row.x) * step, (wall_m.y.y - y_row.y) * step + y_row.y, (wall_m.y.z - y_row.z) * step + y_row.z);
        vector3d side = cross(up, dir);

        vector3d unit_dir  = unitized(dir);
        vector3d unit_up   = unitized(up);
        vector3d unit_side = unitized(side);

        retail::sub_4015B0((u64*)&placed);

        placed.get_matrix().x = vector4d(unit_side.x, unit_side.y, unit_side.z, 0.0f);
        placed.get_matrix().y = vector4d(unit_up.x, unit_up.y, unit_up.z, 0.0f);
        placed.get_matrix().z = vector4d(unit_dir.x, unit_dir.y, unit_dir.z, 0.0f);
        placed.get_matrix().w = vector4d(position.x, position.y, position.z, 0.0f);
    } else {
        vector3d position = the_actor->my_abs_po->get_position();

        const matrix4x4 &wall_m = wall->get_wall_po()->get_matrix();

        placed.get_matrix().x = wall_m.x;
        placed.get_matrix().y = wall_m.y;
        placed.get_matrix().z = wall_m.z;
        placed.get_matrix().w = vector4d(position.x, position.y, position.z, 0.0f);
    }

    retail::sub_627180((i32)the_actor, (f32*)&placed); // entity_base::set_abs_po

    vector3d prev_position = the_actor->my_abs_po->get_position();

    other_po_changes(delta_t, &curr_offset);

    if (!retail::sub_5AA570((f32*)&curr_offset, (f32*)&treyarch::references::po_identity_matrix.get()))
        set_po_curr_offset(the_actor, curr_offset);

    const vector3d &position = the_actor->my_abs_po->get_position();

    the_actor->set_frame_delta_trans(vector3d(position.x - prev_position.x, position.y - prev_position.y, position.z - prev_position.z), delta_t);
}

// sub_5F3990
void als::wallsprint_mocomp::other_po_changes(f32, po* curr_offset) {
    if (retail::sub_5AA570((f32*)curr_offset, (f32*)&treyarch::references::po_identity_matrix.get()))
        return;

    wall_sprint_inode* wall = (wall_sprint_inode*)the_actor->get_ai_core()->get_info_node(info_node_type_wall_sprint);

    // the actor's turn undone, about the origin
    const po       &abs_po = *the_actor->my_abs_po;
    const vector3d &x_row  = abs_po.get_x_facing();
    const vector3d &y_row  = abs_po.get_y_facing();
    const vector3d &z_row  = abs_po.get_z_facing();
    const vector3d &zero   = treyarch::references::unk_00fd4370.get();

    po unturn;
    retail::sub_4015B0((u64*)&unturn);

    matrix4x4 transposed;
    transposed.x = vector4d(x_row.x, y_row.x, z_row.x, 0.0f);
    transposed.y = vector4d(x_row.y, y_row.y, z_row.y, 0.0f);
    transposed.z = vector4d(x_row.z, y_row.z, z_row.z, 0.0f);
    transposed.w = vector4d(-(f32)((f64)x_row.x * zero.x + (f64)x_row.y * zero.y + (f64)x_row.z * zero.z),
                            -(f32)((f64)y_row.y * zero.y + (f64)y_row.x * zero.x + (f64)y_row.z * zero.z),
                            -(f32)((f64)z_row.y * zero.y + (f64)z_row.x * zero.x + (f64)z_row.z * zero.z),
                            0.0f);

    retail::sub_44DA50((u64*)&unturn, (i32)&transposed); // po from the matrix's 3x4

    po wall_po = *wall->get_wall_po();

    // the anim's offset, from the actor's frame into the wall's
    vector4d        &offset = curr_offset->get_matrix().w;
    const matrix4x4 &u      = unturn.get_matrix();

    offset = vector4d((f32)((f64)u.x.x * offset.x + (f64)u.y.x * offset.y + (f64)u.z.x * offset.z + (f64)u.w.x * 0.0),
                      (f32)((f64)u.y.y * offset.y + (f64)u.x.y * offset.x + (f64)u.z.y * offset.z + (f64)u.w.y * 0.0),
                      (f32)((f64)u.y.z * offset.y + (f64)u.x.z * offset.x + (f64)u.z.z * offset.z + (f64)u.w.z * 0.0),
                      0.0f);

    const matrix4x4 &w = wall_po.get_matrix();

    offset = vector4d((f32)((f64)w.x.x * offset.x + (f64)w.y.x * offset.y + (f64)w.z.x * offset.z + (f64)w.w.x * 0.0),
                      (f32)((f64)w.y.y * offset.y + (f64)w.x.y * offset.x + (f64)w.z.y * offset.z + (f64)w.w.y * 0.0),
                      (f32)((f64)w.y.z * offset.y + (f64)w.x.z * offset.x + (f64)w.z.z * offset.z + (f64)w.w.z * 0.0),
                      0.0f);
}

// sub_5C78B0
void als::wallsprint_mocomp::apply_rotation(po*, f32, f32 heading_change, const vector3d &, const vector3d &curr_dir, const vector3d &desired_up, const vector3d &) {
    // turns the facing about the desired up
    po rotation;
    retail::sub_4015B0((u64*)&rotation);

    math::vector4 axis(desired_up.x, desired_up.y, desired_up.z, 0.0f);
    retail::sub_40A750((u64*)&rotation, (f32*)&axis, &heading_change);

    const matrix4x4 &m = rotation.get_matrix();

    math::vector4 turned((f32)((f64)m.y.x * curr_dir.y + (f64)m.x.x * curr_dir.x + (f64)m.z.x * curr_dir.z + (f64)m.w.x * 0.0),
                         (f32)((f64)m.y.y * curr_dir.y + (f64)m.x.y * curr_dir.x + (f64)m.z.y * curr_dir.z + (f64)m.w.y * 0.0),
                         (f32)((f64)m.y.z * curr_dir.y + (f64)m.x.z * curr_dir.x + (f64)m.z.z * curr_dir.z + (f64)m.w.z * 0.0),
                         0.0f);

    math::vector4 up(desired_up.x, desired_up.y, desired_up.z, 0.0f);

    const vector3d &position = the_actor->my_abs_po->get_position();
    math::vector4   origin(position.x, position.y, position.z, 0.0f);

    po facing;
    retail::sub_402260((u64*)&facing, (f32*)&turned, (f32*)&up, (i64*)&origin);

    // the wall turns, the post-action brings the actor along
    wall_sprint_inode* wall = (wall_sprint_inode*)the_actor->get_ai_core()->get_info_node(info_node_type_wall_sprint);

    using set_wall_po_t = void (__thiscall*)(wall_sprint_inode*, vector3d, vector3d);

    ((set_wall_po_t)retail::sub_54A880)(wall, facing.get_z_facing(), facing.get_y_facing()); // wall_sprint_inode: wall_po from a facing and up
}

// sub_5C7B20
void als::wallsprint_mocomp::face_desired_direction() {
    vector3d desired_dir;
    vector3d desired_up;

    get_directions(the_als, the_base, &desired_dir, &desired_up);

    const vector3d &position = the_actor->my_abs_po->get_position();

    math::vector4 basis_dir(desired_dir.x, desired_dir.y, desired_dir.z, 0.0f);
    math::vector4 basis_up(desired_up.x, desired_up.y, desired_up.z, 0.0f);
    math::vector4 origin(position.x, position.y, position.z, 0.0f);

    po facing;
    retail::sub_402260((u64*)&facing, (f32*)&basis_dir, (f32*)&basis_up, (i64*)&origin);
    retail::sub_627180((i32)the_actor, (f32*)&facing); // entity_base::set_abs_po

    wall_sprint_inode* wall = (wall_sprint_inode*)the_actor->get_ai_core()->get_info_node(info_node_type_wall_sprint);

    using set_wall_po_t = void (__thiscall*)(wall_sprint_inode*, vector3d, vector3d);

    ((set_wall_po_t)retail::sub_54A880)(wall, facing.get_z_facing(), facing.get_y_facing()); // wall_sprint_inode: wall_po from a facing and up
}

// sub_453B40
void flight_mocomp::construct_mashed_class() {
    simple_orientation::construct_mashed_class();
}

// sub_453B50
void flight_mocomp::destruct_mashed_class() {
    simple_orientation::destruct_mashed_class();
}

// sub_453B60
void flight_mocomp::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    simple_orientation::unmash(mash_info, containing_class_ptr, buffer);
}

// sub_5AFC40
mash::virtual_types_key flight_mocomp::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AEC80
bool flight_mocomp::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (simple_orientation::get_virtual_type_key() == parent_class)
        return true;

    if (simple_orientation::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4BF700
i32 flight_mocomp::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_46C170
void flight_mocomp::activate(als::animation_logic_system* the_als_core, als::layer_id layer) {
    simple_orientation::activate(the_als_core, layer);

    flight_inode = nullptr;

    if (the_actor->has_ifc(entity_ifc_physical))
        retail::sub_5FFD60((u32*)the_actor->physical_ifc(), false); // physical_interface: flags bit 2
}

// sub_47D5B0
void flight_mocomp::post_anim_action(f32 delta_t) {
    simple_orientation::post_anim_action(delta_t);

    // the actor's loco inode, once it's a flight layer
    if (!flight_inode) {
        ai_core* core = the_actor->get_ai_core();

        if (core && core->my_loco_inode) {
            loco_inode* loco = core->my_loco_inode;

            if (loco->get_virtual_type_key() == flight_layer_inode_key.source_hash_code || loco->get_virtual_type_key() == swift_flight_layer_inode_key.source_hash_code)
                flight_inode = (flight_layer_inode*)loco;
        }
    }

    if (!flight_inode)
        return;

    if (flight_inode->my_param_block.get_optional_pb_int(shaba_simple_helicopter_id, 0, nullptr)) {
        retail::sub_47C510((i32)flight_inode, delta_t); // flight_layer_inode: the helicopter flies itself

        return;
    }

    if (the_actor->has_ifc(entity_ifc_physical))
        retail::sub_5FFD60((u32*)the_actor->physical_ifc(), false);

    vector3d move;

    if (flight_inode->unk_115) {
        const vector3d &velocity = flight_inode->unk_0b4;

        move = vector3d(velocity.x * delta_t, velocity.y * delta_t, velocity.z * delta_t);
    } else {
        move = flight_inode->unk_0f4;

        f32 dist = move.length();

        if (dist > 0.0001f) {
            // the speed for how far it's going, within the inode's limits
            f32 ratio = (f32)retail::sub_4505C0((i32)flight_inode);

            bool slow = ratio <= 0.55f;
            bool mid  = ratio > 0.55f && ratio <= 1.0f;
            bool fast = ratio > 1.0f && ratio <= 2.0f;

            f32 tier;

            if (slow)
                tier = (f32)retail::sub_453D60((u32*)flight_inode);
            else if (mid)
                tier = (f32)retail::sub_453D90((u32*)flight_inode);
            else if (fast)
                tier = (f32)retail::sub_453DC0((u32*)flight_inode);
            else
                tier = (f32)retail::sub_453DF0((u32*)flight_inode);

            f32 speed = (f32)retail::sub_453E20((u32*)flight_inode) * tier;

            if (speed < 0.0001f)
                speed = 0.0001f;
            else if (speed > (f32)retail::sub_453D00((u32*)flight_inode))
                speed = (f32)retail::sub_453D00((u32*)flight_inode);

            f32 step = speed * delta_t;

            if (step > dist)
                step = dist;

            // slower through tight turns
            f32 turn_scale = 1.0f;

            if (retail::sub_453B70((u32*)flight_inode)) { // flight_layer_inode: allow_speed_alteration_during_turns
                f32 turn_angle = 0.0f;

                if (move.xz_length2() > 0.01f) {
                    vector3d facing = the_actor->my_abs_po->get_z_facing();
                    facing.y = 0.0f;

                    retail::sub_402050((f32*)&facing);

                    vector3d heading(move.x, 0.0f, move.z);
                    retail::sub_402050((f32*)&heading);

                    f32 along  = (f32)((f64)heading.z * facing.z + (f64)-heading.x * -facing.x);
                    f32 across = (f32)((f64)-heading.x * facing.z - (f64)heading.z * -facing.x);

                    turn_angle = std::fabs((f32)retail::sub_44D790(across, along));
                }

                auto slowed = [](f64 over) {
                    f32 scale = (f32)(1.0 - over);

                    return scale < 0.0f ? 0.0f : scale;
                };

                f64 range = slow ? 2.617993950843811 : 1.0471975803375244;

                if (dist <= 15.0 && turn_angle > 0.1745329300562541)
                    turn_scale = slowed(((f64)turn_angle - 0.1745329300562541) / 0.7853981852531433);
                else if (dist > 30.0 && turn_angle > 0.52359879f)
                    turn_scale = slowed(((f64)turn_angle - 0.5235987901687622) / range);
                else if (dist > 10.0f && turn_angle > 0.2617994f)
                    turn_scale = slowed(((f64)turn_angle - 0.2617993950843811) / range);
            }

            if (flight_inode->unk_08c) {
                f32 scale = step / dist;

                move = vector3d(scale * move.x, scale * move.y, move.z * scale);
            } else {
                f32 scale = (f32)((f64)step / dist * turn_scale);

                move = vector3d(scale * move.x, scale * move.y, move.z * scale);

                // the run goes along the facing, the climb stays
                vector3d facing = the_actor->my_abs_po->get_z_facing();
                facing.y = 0.0f;

                retail::sub_402050((f32*)&facing);

                f32 climb = move.y;
                move.y    = 0.0f;

                f32 run = (f32)retail::sub_402020((f32*)&move);

                move = vector3d(facing.x * run, climb, facing.z * run);
            }
        }
    }

    if (flight_inode->my_param_block.get_optional_pb_int(reverse_mode_id, 0, nullptr)) {
        move.x = move.x * -1.0f;
        move.y = move.y * -1.0f;
        move.z = move.z * -1.0f;
    }

    using fly_t = void (__thiscall*)(flight_layer_inode*, vector3d*, f32);

    ((fly_t)retail::sub_47CD90)(flight_inode, &move, delta_t); // flight_layer_inode: flies the move
}

// sub_5AE3E0
// inlined @ sub_5AE420
// inlined @ sub_5AE460
// inlined @ sub_5AE4A0
// inlined @ sub_5AE590
// inlined @ sub_5AE5D0
// inlined @ sub_5AE650
// inlined @ sub_5AE690
// inlined @ sub_5AE720
// inlined @ sub_5AE760
// inlined @ sub_5AE7A0
// inlined @ sub_5AE820
// inlined @ sub_5AE860
// inlined @ sub_5AE8A0
// inlined @ sub_5AE920
// inlined @ sub_5AEB30
// inlined @ sub_5AEBB0
// inlined @ sub_5AED50
// inlined @ sub_5AED90
// inlined @ sub_5AF020
// inlined @ sub_5AF070
// inlined @ sub_5AF0C0
als::move_and_face::move_and_face() : orientation_end_signal_required(false),
                                      translation_end_signal_required(false),
                                      orientation_start_signal_required(false),
                                      translation_start_signal_required(false),
                                      destination_point_valid(false),
                                      desired_facing_valid(false) {}

// sub_719250
// inlined @ sub_719290
// inlined @ sub_7192D0
// inlined @ sub_719310
// inlined @ sub_719390
// inlined @ sub_7193D0
// inlined @ sub_719410
// inlined @ sub_719450
// inlined @ sub_719490
// inlined @ sub_7194D0
// inlined @ sub_719510
// inlined @ sub_719550
// inlined @ sub_719590
// inlined @ sub_7195D0
// inlined @ sub_719610
// inlined @ sub_7196D0
// inlined @ sub_719710
// inlined @ sub_719750
// inlined @ sub_719790
// inlined @ sub_719800
// inlined @ sub_719840
// inlined @ sub_719880
als::move_and_face::move_and_face(mash::generating_vtable_version so_do_nothing) : motion_compensator(so_do_nothing),
                                                                                   orientation_end_signal_required(false),
                                                                                   translation_end_signal_required(false),
                                                                                   orientation_start_signal_required(false),
                                                                                   translation_start_signal_required(false),
                                                                                   destination_point_valid(false),
                                                                                   desired_facing_valid(false) {}

// sub_5AE410
mash::virtual_types_key als::move_and_face::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AE530
bool als::move_and_face::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (motion_compensator::get_virtual_type_key() == parent_class)
        return true;

    if (motion_compensator::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE710
i32 als::move_and_face::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5D89D0
void als::move_and_face::activate(animation_logic_system* the_als_core, layer_id layer) {
    motion_compensator::activate(the_als_core, layer);

    pre_signal_translation_action  = mocomp_action_procedural;
    pre_signal_orientation_action  = mocomp_action_procedural;
    max_movement_speed             = 0.0f;
    max_turn_speed                 = 0.0f;
    constant_movement_speed        = -1.0f;
    constant_turn_speed            = -1.0f;
    pre_start_translation_action   = mocomp_action_from_anim;
    pre_start_orientation_action   = mocomp_action_from_anim;
    post_signal_translation_action = mocomp_action_from_anim;
    post_signal_orientation_action = mocomp_action_from_anim;
    translation_algorithm          = mocomp_algorithm_simple_linear;
    orientation_algorithm          = mocomp_algorithm_simple_linear;
    orientation_time_left          = 0.0f;
    translation_time_left          = 0.0f;
    orientation_start_time         = 0.0f;
    translation_start_time         = 0.0f;
    orientation_total_time         = 0.0f;
    translation_total_time         = 0.0f;

    last_anim_time = the_base->get_current_anim_time();
    start_point    = the_actor->my_abs_po->get_position();

    set_start_key_signals();
    set_end_key_signals();
    set_start_time();
    set_time_left();

    // the time left runs from the start
    orientation_time_left = orientation_time_left - orientation_start_time;

    if (orientation_time_left < 0.0001f)
        orientation_time_left = 0.0001f;

    translation_time_left = translation_time_left - translation_start_time;

    if (translation_time_left < 0.0001f)
        translation_time_left = 0.0001f;

    orientation_total_time = orientation_time_left;
    translation_total_time = translation_time_left;

    // where the actor is, unless the params say otherwise
    destination_point = the_actor->my_abs_po->get_position();
    desired_facing    = the_actor->my_abs_po->get_z_facing();
    desired_up        = the_actor->my_abs_po->get_y_facing();

    destination_point_valid = false;
    desired_facing_valid    = false;

    set_destination_point();
    set_desired_facing();
    set_desired_up();

    use_ik_crawl = the_base->get_optional_pb_int(use_ik_crawl_id, 0, nullptr) != 0;

    if (use_ik_crawl) {
        using ikcrawl_begin_t = void (__cdecl*)(actor*, animation_logic_system*, layer_id);

        ((ikcrawl_begin_t)retail::sub_5CCF00)(the_actor, the_als_core, layer); // ikcrawl begin
    }
}

// sub_5D8BF0
void als::move_and_face::deactivate() {
    // the destination, facing and up params only outlast a requested transition
    if (!the_base->did_transition_succeed_from_request()) {
        const vector3d &zero = treyarch::references::unk_00fd4370.get();

        auto clear = [&](param_id param) {
            u32 index = param - num_external_float_params;

            the_base->our_smi.requested_params->vector_params[index] = zero;
            the_base->our_smi.requested_params->vector_set[index]    = false;

            the_base->pending_requests.pending_params->vector_params[index] = zero;
            the_base->pending_requests.pending_params->vector_set[index]    = false;
        };

        clear(143);
        clear(142);
        clear(145);
    }

    if (use_ik_crawl)
        retail::sub_5CCFE0((i32)the_actor); // ikcrawl end
}

// sub_5B29C0
void als::move_and_face::post_anim_action(f32) {
    // the anim's own time drives it, not the frame's
    f32 normalized_time = the_base->get_current_normalized_time();
    f32 anim_time       = the_base->get_duration() * normalized_time;
    f32 anim_delta      = anim_time - last_anim_time;

    if (anim_delta > 0.0001f) {
        update_params(anim_delta);

        f32 time_left = orientation_time_left;
        orientation_time_left = time_left - decrement_start_time(anim_delta, &orientation_start_time);

        time_left = translation_time_left;
        translation_time_left = time_left - decrement_start_time(anim_delta, &translation_start_time);

        clamp_time_left(anim_delta);
        face_and_arrive_by(anim_delta);

        if (orientation_start_time < 0.0001f)
            decrement_time_left(anim_delta, &orientation_time_left);

        if (translation_start_time < 0.0001f)
            decrement_time_left(anim_delta, &translation_time_left);

        reached_dest_signal(anim_delta);
        restart_time_signals();

        last_anim_time = anim_time;
    }
}

// sub_667F80
bool als::move_and_face::force_reactivate() const {
    return true;
}

// sub_5B1B70
void als::move_and_face::set_start_time() {
    set_orientation_start_time();
    set_translation_start_time();
}

// sub_5B1B90
void als::move_and_face::set_time_left() {
    set_orientation_time_left();
    set_translation_time_left();
}

// sub_5B1C50
void als::move_and_face::set_translation_start_time() {
    if (translation_start_key_signals_count <= 0 || !set_time_signal(&translation_start_time, translation_start_key_signals, translation_start_key_signals_count, translation_start_signal_required))
        translation_start_time = 0.0f;
}

// sub_5B1CA0
void als::move_and_face::set_translation_time_left() {
    set_time_signal(&translation_time_left, translation_end_key_signals, translation_end_key_signals_count, translation_end_signal_required);

    if (translation_time_left < 0.0001f)
        translation_time_left = 0.0001f;
}

// sub_5B1BB0
void als::move_and_face::set_orientation_start_time() {
    if (orientation_start_key_signals_count <= 0 || !set_time_signal(&orientation_start_time, orientation_start_key_signals, orientation_start_key_signals_count, orientation_start_signal_required))
        orientation_start_time = 0.0f;
}

// sub_5B1C00
void als::move_and_face::set_orientation_time_left() {
    set_time_signal(&orientation_time_left, orientation_end_key_signals, orientation_end_key_signals_count, orientation_end_signal_required);

    if (orientation_time_left < 0.0001f)
        orientation_time_left = 0.0001f;
}

// sub_5D88B0
void als::move_and_face::set_destination_point() {
    if (the_base->has_ext_param_been_set(145)) {
        destination_point       = the_base->get_external_vector(145);
        destination_point_valid = true;
    }
}

// sub_5D8910
void als::move_and_face::set_desired_facing() {
    if (the_base->has_ext_param_been_set(143)) {
        desired_facing       = the_base->get_external_vector(143);
        desired_facing_valid = true;
    }
}

// sub_5D8970
void als::move_and_face::set_desired_up() {
    if (the_base->has_ext_param_been_set(142))
        desired_up = the_base->get_external_vector(142);
}

// sub_5B1CF0
void als::move_and_face::set_end_key_signals() {
    orientation_end_key_signals       = nullptr;
    orientation_end_key_signals_count = 0;
    orientation_end_signal_required   = false;
    translation_end_key_signals       = nullptr;
    translation_end_key_signals_count = 0;
    translation_end_signal_required   = false;
}

// sub_5B1D20
void als::move_and_face::set_start_key_signals() {
    orientation_start_key_signals       = nullptr;
    orientation_start_key_signals_count = 0;
    orientation_start_signal_required   = false;
    translation_start_key_signals       = nullptr;
    translation_start_key_signals_count = 0;
    translation_start_signal_required   = false;
}

// sub_5B4A70
void als::move_and_face::update_start_time() {}

// sub_5B4A70
void als::move_and_face::update_time_left() {}

// sub_5B1D50
void als::move_and_face::update_destination_point() {
    set_destination_point();
}

// sub_5B1D60
void als::move_and_face::update_desired_facing() {
    set_desired_facing();
}

// sub_5B1D70
void als::move_and_face::update_desired_up() {
    set_desired_up();
}

// sub_5B1D80
void als::move_and_face::update_params(f32) {
    update_time_left();
    update_destination_point();
    update_desired_facing();
    update_desired_up();
}

// sub_5B1DC0
vector3d als::move_and_face::simple_linear_translation(f32 delta_t, f32 time_percentage, const po &old_po) {
    auto unitized = [](const vector3d &value) {
        math::vector4 direction(value.x, value.y, value.z, 0.0f);
        math::vector4 unit;

        retail::sub_401AA0((u64*)&unit, (f32*)&direction);

        return vector3d(unit[0], unit[1], unit[2]);
    };

    if (the_base->get_optional_pb_int(use_alt_linear_id, 0, nullptr)) {
        // to where the time puts it on the line from the start, no faster than the max
        f32      total    = (destination_point - start_point).length();
        vector3d position = old_po.get_position();
        vector3d dir      = unitized(destination_point - position);

        if (position.x == destination_point.x && position.y == destination_point.y && position.z == destination_point.z)
            return position;

        f32 remaining = (f32)((1.0 - (f64)time_percentage) * total);

        vector3d target(-dir.x * remaining + destination_point.x, -dir.y * remaining + destination_point.y, -dir.z * remaining + destination_point.z);
        vector3d move = target - position;

        if (max_movement_speed > 0.0001f && move.length() / delta_t > max_movement_speed) {
            f32 step = max_movement_speed * delta_t;

            retail::sub_402050((f32*)&move);

            target = vector3d(position.x + move.x * step, position.y + move.y * step, position.z + move.z * step);
        }

        return target;
    }

    vector3d position = old_po.get_position();
    vector3d delta    = destination_point - position;

    if (position.x == destination_point.x && position.y == destination_point.y && position.z == destination_point.z)
        return position;

    f32      dist = delta.length();
    vector3d dir  = delta;

    if (dist > 0.0001f)
        dir = vector3d(delta.x / dist, delta.y / dist, delta.z / dist);

    // the constant speed while the destination is further than a step of it, else the time's share of the way
    f32 constant_step = constant_movement_speed * delta_t;
    f32 step;

    if (constant_movement_speed > -1.0f && dist > constant_step)
        step = constant_step;
    else
        step = dist * time_percentage;

    if (max_movement_speed > 0.0001f && step / delta_t > max_movement_speed)
        step = max_movement_speed * delta_t;

    return vector3d(position.x + dir.x * step, position.y + dir.y * step, position.z + dir.z * step);
}

// sub_5BF190
po als::move_and_face::simple_linear_orientation(f32 delta_t, f32 time_percentage, const po &old_po) {
    auto unitized = [](const vector3d &value) {
        math::vector4 direction(value.x, value.y, value.z, 0.0f);
        math::vector4 unit;

        retail::sub_401AA0((u64*)&unit, (f32*)&direction);

        return vector3d(unit[0], unit[1], unit[2]);
    };

    // no faster than the max turn speed, either way round
    auto capped = [&](f32 step) {
        if (max_turn_speed > 0.0001f) {
            f32 rate = step / delta_t;

            if (std::fabs(rate) > max_turn_speed)
                return (rate < 0.0f ? -max_turn_speed : max_turn_speed) * delta_t;
        }

        return step;
    };

    po result;
    retail::sub_583280((u64*)&result, (u64*)&old_po); // po copy

    vector3d up  = unitized(desired_up);
    vector3d dir = unitized(desired_facing);

    vector3d y_row = result.get_y_facing();
    vector3d z_row = result.get_z_facing();
    vector3d back(-result.get_x_facing().x, -result.get_x_facing().y, -result.get_x_facing().z);

    // the up first, about the axis between the two
    if (up.length2() > 0.0001f) {
        f32 tilt = math::acos(y_row * up);

        if (tilt > 0.01f || tilt < -0.01f) {
            f32 constant_step = constant_turn_speed * delta_t;
            f32 step;

            if (constant_turn_speed > -1.0f && tilt > constant_step)
                step = constant_step;
            else
                step = tilt * time_percentage;

            step = capped(step);

            vector3d axis = unitized(cross(y_row, up));

            if (axis.length2() < 0.01f)
                axis = z_row;

            po turn;
            retail::sub_4015B0((u64*)&turn);

            step = -step;

            math::vector4 axis_4(axis.x, axis.y, axis.z, 0.0f);
            retail::sub_40A750((u64*)&turn, (f32*)&axis_4, &step);
            retail::sub_44E230((u64*)&result, (f32*)&result, (f32*)&turn);

            z_row = result.get_z_facing();
            back  = vector3d(-result.get_x_facing().x, -result.get_x_facing().y, -result.get_x_facing().z);
        }
    }

    // then the heading, in the plane of the up
    vector3d flat_dir = dir - up * (f32)(up * dir);

    if (flat_dir.length2() > 0.0001f) {
        dir = unitized(flat_dir);

        vector3d flat_z = z_row - up * (f32)(up * z_row);

        if (flat_z.length2() > 0.0001f) {
            retail::sub_402050((f32*)&flat_z);

            vector3d flat_back = back - up * (f32)(up * back);
            retail::sub_402050((f32*)&flat_back);

            f32 along          = flat_z * dir;
            f32 across         = flat_back * dir;
            f32 heading_change = (f32)retail::sub_44D790(across, along);

            if (heading_change > 0.01f || heading_change < -0.01f) {
                // the state can force the way round
                i32 turn_direction = the_base->get_optional_pb_int(turn_direction_id, 0, nullptr);

                if (turn_direction == -1) {
                    if (heading_change < 0.0f)
                        heading_change += 6.2831855f;
                } else if (turn_direction == 1 && heading_change > 0.0f)
                    heading_change -= 6.2831855f;

                f32 step = capped(heading_change * time_percentage);

                po turn;
                retail::sub_4015B0((u64*)&turn);

                math::vector4 axis(desired_up.x, desired_up.y, desired_up.z, 0.0f);
                retail::sub_40A750((u64*)&turn, (f32*)&axis, &step);
                retail::sub_44E230((u64*)&result, (f32*)&result, (f32*)&turn);
            }
        }
    }

    return result;
}

// sub_5B2430
vector3d als::move_and_face::compute_translation(f32 delta_t, const po &old_po, const po &) {
    // the anim's step, at its playback speeds
    f32 duration  = the_base->get_duration();
    f32 time      = the_base->get_current_anim_time();
    f32 secondary = the_base->curr_state->get_secondary_playback_speed(time, duration);
    f32 step_time = the_base->get_playback_speed() * secondary * delta_t;

    f32 time_percentage;

    if (translation_time_left <= 0.0f)
        time_percentage = 1.0f;
    else {
        f32 time_left = translation_time_left > translation_total_time ? translation_total_time : translation_time_left;

        time_percentage = (f32)(1.0 - ((f64)time_left - step_time) / translation_total_time);

        if (time_percentage < 0.0f)
            time_percentage = 0.0f;
        else if (time_percentage > 1.0f)
            time_percentage = 1.0f;
    }

    if (translation_algorithm != mocomp_algorithm_simple_linear)
        return old_po.get_position();

    return simple_linear_translation(step_time, time_percentage, old_po);
}

// sub_5B2590
vector3d als::move_and_face::hybrid_translation(f32 delta_t, const po &, const po &anim_po) {
    return compute_translation(delta_t, anim_po, anim_po);
}

// sub_5F03D0
vector3d als::move_and_face::translation_action(f32 delta_t, const po &old_po, const po &anim_po) {
    mocomp_action_enum action = pre_start_translation_action;

    if (translation_start_time <= 0.0f)
        action = pre_signal_translation_action;

    if (translation_time_left <= 0.0f)
        action = post_signal_translation_action;

    // with no destination, or when param 104 asks, the anim moves it
    if (action != mocomp_action_none && !destination_point_valid)
        action = mocomp_action_from_anim;
    else if (action != mocomp_action_from_anim && the_base->has_ext_param_been_set(104) && the_base->get_external_float(104) > 0.0001f)
        action = mocomp_action_from_anim;

    vector3d position;

    switch (action) {
        case mocomp_action_none:
            position = old_po.get_position();
            break;

        case mocomp_action_procedural:
            position = compute_translation(delta_t, old_po, anim_po);
            break;

        case mocomp_action_from_anim:
            return anim_po.get_position();

        case mocomp_action_hybrid:
            position = hybrid_translation(delta_t, old_po, anim_po);
            break;

        default:
            return position;
    }

    // the mocomp moves it, not the physics
    if (the_actor->has_ifc(entity_ifc_physical))
        the_actor->physical_ifc()->cancel_all_velocity(treyarch::references::unk_00fd4370.get(), true);

    return position;
}

// sub_5B25C0
po als::move_and_face::compute_orientation(f32 delta_t, const po &old_po, const po &) {
    // the anim's step, at its playback speeds
    f32 duration  = the_base->get_duration();
    f32 time      = the_base->get_current_anim_time();
    f32 secondary = the_base->curr_state->get_secondary_playback_speed(time, duration);
    f32 step_time = the_base->get_playback_speed() * secondary * delta_t;

    f32 time_percentage;

    if (orientation_time_left <= 0.0f)
        time_percentage = 1.0f;
    else {
        f32 time_left = orientation_time_left > orientation_total_time ? orientation_total_time : orientation_time_left;

        time_percentage = (f32)(1.0 - ((f64)time_left - step_time) / orientation_total_time);

        if (time_percentage < 0.0f)
            time_percentage = 0.0f;
        else if (time_percentage > 1.0f)
            time_percentage = 1.0f;
    }

    if (orientation_algorithm != mocomp_algorithm_simple_linear) {
        po result;
        retail::sub_583280((u64*)&result, (u64*)&old_po); // po copy

        return result;
    }

    return simple_linear_orientation(step_time, time_percentage, old_po);
}

// sub_5B2720
po als::move_and_face::hybrid_orientation(f32 delta_t, const po &, const po &anim_po) {
    return compute_orientation(delta_t, anim_po, anim_po);
}

// sub_5F0560
po als::move_and_face::orientation_action(f32 delta_t, const po &old_po, const po &anim_po) {
    mocomp_action_enum action = pre_start_orientation_action;

    if (orientation_start_time <= 0.0f)
        action = pre_signal_orientation_action;

    if (orientation_time_left <= 0.0f)
        action = post_signal_orientation_action;

    // with no facing, or when param 105 asks, the anim turns it
    if (action != mocomp_action_none && !desired_facing_valid)
        action = mocomp_action_from_anim;
    else if (action != mocomp_action_from_anim && the_base->has_ext_param_been_set(105) && the_base->get_external_float(105) > 0.0001f)
        action = mocomp_action_from_anim;

    po result;
    retail::sub_4015B0((u64*)&result);

    switch (action) {
        case mocomp_action_none:
            retail::sub_583280((u64*)&result, (u64*)&old_po); // po copy
            break;

        case mocomp_action_procedural: {
            po computed = compute_orientation(delta_t, old_po, anim_po);
            retail::sub_583280((u64*)&result, (u64*)&computed);
            break;
        }

        case mocomp_action_from_anim:
            retail::sub_583280((u64*)&result, (u64*)&anim_po);
            break;

        case mocomp_action_hybrid: {
            po hybrid = hybrid_orientation(delta_t, old_po, anim_po);
            retail::sub_583280((u64*)&result, (u64*)&hybrid);
            break;
        }

        default:
            break;
    }

    return result;
}

// sub_5BFAE0
po als::move_and_face::calculate_anim_motion() {
    po curr_offset;
    retail::sub_4015B0((u64*)&curr_offset);

    get_po_curr_offset(curr_offset);

    motion_compensator::set_po_curr_offset(the_actor, curr_offset);

    po moved;
    retail::sub_583280((u64*)&moved, (u64*)the_actor->my_abs_po); // po copy

    return moved;
}

// sub_5BFB50
void als::move_and_face::reached_dest_signal(f32) {
    if (std::fabs(orientation_time_left - 0.0f) < 0.0001f && std::fabs(translation_time_left - 0.0f) < 0.0001f)
        event_manager::raise_event(references::anim_dest_reached.read(), the_actor->my_handle);
}

// sub_5B2750
void als::move_and_face::restart_time_signals() {
    if (0.0f > orientation_time_left && has_time_signal(orientation_start_key_signals, orientation_start_key_signals_count)) {
        orientation_start_signal_required = false;
        orientation_end_signal_required   = false;

        set_orientation_start_time();
        set_orientation_time_left();

        orientation_time_left = orientation_time_left - orientation_start_time;

        if (orientation_time_left < 0.0001f)
            orientation_time_left = 0.0001f;

        orientation_total_time = orientation_time_left;
    }

    if (0.0f > translation_time_left && has_time_signal(translation_start_key_signals, translation_start_key_signals_count)) {
        translation_start_signal_required = false;
        translation_end_signal_required   = false;

        set_translation_start_time();
        set_translation_time_left();

        translation_time_left = translation_time_left - translation_start_time;

        if (translation_time_left < 0.0001f)
            translation_time_left = 0.0001f;

        translation_total_time = translation_time_left;
    }
}

// sub_5B2870
f32 als::move_and_face::decrement_start_time(f32 delta_t, f32* start_time) {
    f32 past_start = 0.0f;

    if (*start_time > 0.0f) {
        *start_time = *start_time - delta_t;

        if (0.0f > *start_time) {
            past_start  = std::fabs(*start_time);
            *start_time = 0.0f;
        }
    }

    return past_start;
}

// sub_5B28D0
void als::move_and_face::clamp_time_left(f32 delta_t) {
    // a started stretch lasts at least the step
    if (orientation_start_time < 0.0001f && orientation_time_left > 0.0f && delta_t > orientation_time_left)
        orientation_time_left = delta_t;

    if (translation_start_time < 0.0001f && translation_time_left > 0.0f && delta_t > translation_time_left)
        translation_time_left = delta_t;

    if (orientation_total_time > 0.0f && orientation_time_left > orientation_total_time)
        orientation_total_time = orientation_time_left;

    if (translation_total_time > 0.0f && translation_time_left > translation_total_time)
        translation_total_time = translation_time_left;
}

// sub_5B2990
void als::move_and_face::decrement_time_left(f32 delta_t, f32* time_left) {
    *time_left = *time_left - delta_t;
}

// sub_5F0670
void als::move_and_face::face_and_arrive_by(f32 delta_t) {
    using ikcrawl_gather_t = bool (__cdecl*)(actor*, state_machine*, f32);

    bool crawling = false;

    if (use_ik_crawl)
        crawling = ((ikcrawl_gather_t)retail::sub_5E63D0)(the_actor, the_base, delta_t); // ikcrawl: gathers the geometry around the actor

    po old_po  = *the_actor->my_abs_po;
    po anim_po = calculate_anim_motion();
    po facing  = orientation_action(delta_t, old_po, anim_po);

    // once the up is near param 152, the facing turns to the rest of it
    if (the_base->has_ext_param_been_set(60) && the_base->get_external_float(60) > 0.5f && the_base->has_ext_param_been_set(152)) {
        vector3d target = the_base->get_external_vector(152);
        f32      dot    = facing.get_y_facing() * target;

        if (dot > 0.9f) {
            target = target - facing.get_y_facing() * dot;

            retail::sub_402050((f32*)&target);
            retail::sub_44E1E0((f32*)&facing, target.x, target.y, target.z); // po: looks at the point
        }
    }

    vector3d position = translation_action(delta_t, old_po, anim_po);

    facing.get_matrix().w = vector4d(position.x, position.y, position.z, 1.0f);

    if (use_ik_crawl && crawling) {
        po moved;
        retail::sub_4015B0((u64*)&moved);

        po inverse;
        retail::sub_48E2C0((f32*)&old_po, (u64*)&inverse);
        retail::sub_44E230((u64*)&moved, (f32*)&facing, (f32*)&inverse);

        retail::sub_5E6760((u32*)the_actor, delta_t, (f32*)&old_po.get_position(), (i32)&moved, (f32*)&facing); // ikcrawl: crawls on it

        return;
    }

    retail::sub_627180((i32)the_actor, (f32*)&facing); // entity_base::set_abs_po

    const vector3d &moved_to   = the_actor->my_abs_po->get_position();
    const vector3d &moved_from = old_po.get_position();

    the_actor->set_frame_delta_trans(vector3d(moved_to.x - moved_from.x, moved_to.y - moved_from.y, moved_to.z - moved_from.z), delta_t);
}

// sub_5B1A30
bool als::move_and_face::set_time_signal(f32* time, const string_hash** key_signals, i32 key_signals_count, bool) {
    bool found = false;

    if (!time)
        return found;

    for (i32 index = 0; index < key_signals_count; ++index) {
        f32 time_to_signal = the_base->get_time_to_signal(*key_signals[index], nullptr, false, nullptr);

        if (time_to_signal >= 0.0f && (!found || *time > time_to_signal)) {
            *time = time_to_signal;
            found = true;
        }
    }

    if (!found) {
        *time = the_base->get_duration();
        *time -= the_base->get_current_anim_time();
    }

    return found;
}

// sub_5B1B00
bool als::move_and_face::has_time_signal(const string_hash** key_signals, i32 key_signals_count) {
    for (i32 index = 0; index < key_signals_count; ++index) {
        if (the_base->get_time_to_signal(*key_signals[index], nullptr, false, nullptr) >= 0.0f)
            return true;
    }

    return false;
}

// sub_5AE420
als::move_and_face_no_anim_movement::move_and_face_no_anim_movement() {}

// sub_719290
als::move_and_face_no_anim_movement::move_and_face_no_anim_movement(mash::generating_vtable_version so_do_nothing) : move_and_face(so_do_nothing) {}

// sub_5AE450
mash::virtual_types_key als::move_and_face_no_anim_movement::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AEDD0
bool als::move_and_face_no_anim_movement::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (move_and_face::get_virtual_type_key() == parent_class)
        return true;

    if (move_and_face::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE710
i32 als::move_and_face_no_anim_movement::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5D8E80
void als::move_and_face_no_anim_movement::activate(animation_logic_system* the_als_core, layer_id layer) {
    move_and_face::activate(the_als, layer); // the stored als, not the_als_core

    post_signal_translation_action = mocomp_action_none;
    post_signal_orientation_action = mocomp_action_none;
}

// sub_5AE720
als::action_move_and_face::action_move_and_face() {}

// sub_719490
als::action_move_and_face::action_move_and_face(mash::generating_vtable_version so_do_nothing) : move_and_face(so_do_nothing) {}

// sub_5AE750
mash::virtual_types_key als::action_move_and_face::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AEDD0
bool als::action_move_and_face::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (move_and_face::get_virtual_type_key() == parent_class)
        return true;

    if (move_and_face::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE710
i32 als::action_move_and_face::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5DA700
void als::action_move_and_face::activate(animation_logic_system* the_als_core, layer_id layer) {
    move_and_face::activate(the_als, layer); // the stored als, not the_als_core

    f32 default_speed = retail::sub_601C50((u32*)the_actor) ? 50.0f : 25.0f;

    max_movement_speed = the_base->get_optional_pb_float(movement_speed_id, default_speed, nullptr);
    max_turn_speed     = (f32)(the_base->get_optional_pb_float(turn_rate_id, 0.0f, nullptr) * 0.017453293005625408);
}

// sub_5B3100
void als::action_move_and_face::set_end_key_signals() {
    static const string_hash* signals[] = { &references::anim_action.get() };

    orientation_end_key_signals       = signals;
    orientation_end_key_signals_count = 1;
    orientation_end_signal_required   = true;
    translation_end_key_signals       = signals;
    translation_end_key_signals_count = 1;
    translation_end_signal_required   = true;
}

// sub_5AE7A0
als::action_face_only::action_face_only() {}

// sub_719510
als::action_face_only::action_face_only(mash::generating_vtable_version so_do_nothing) : action_move_and_face(so_do_nothing) {}

// sub_5AE7D0
mash::virtual_types_key als::action_face_only::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AE7E0
bool als::action_face_only::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (action_move_and_face::get_virtual_type_key() == parent_class)
        return true;

    if (action_move_and_face::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE710
i32 als::action_face_only::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5DA7B0
void als::action_face_only::activate(animation_logic_system* the_als_core, layer_id layer) {
    action_move_and_face::activate(the_als, layer); // the stored als, not the_als_core

    // the anim keeps the translation
    pre_signal_translation_action  = mocomp_action_from_anim;
    post_signal_translation_action = mocomp_action_from_anim;
}

// sub_5AE760
als::action_move_only::action_move_only() {}

// sub_7194D0
als::action_move_only::action_move_only(mash::generating_vtable_version so_do_nothing) : action_move_and_face(so_do_nothing) {}

// sub_5AE790
mash::virtual_types_key als::action_move_only::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AE7E0
bool als::action_move_only::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (action_move_and_face::get_virtual_type_key() == parent_class)
        return true;

    if (action_move_and_face::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE710
i32 als::action_move_only::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5DA7E0
void als::action_move_only::activate(animation_logic_system* the_als_core, layer_id layer) {
    action_move_and_face::activate(the_als, layer); // the stored als, not the_als_core

    // the anim keeps the orientation
    pre_signal_orientation_action  = mocomp_action_from_anim;
    post_signal_orientation_action = mocomp_action_from_anim;
}

// sub_5AE4A0
als::combat_move_and_face::combat_move_and_face() {}

// sub_719310
als::combat_move_and_face::combat_move_and_face(mash::generating_vtable_version so_do_nothing) : move_and_face(so_do_nothing) {}

// sub_5AE4D0
mash::virtual_types_key als::combat_move_and_face::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AEDD0
bool als::combat_move_and_face::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (move_and_face::get_virtual_type_key() == parent_class)
        return true;

    if (move_and_face::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE710
i32 als::combat_move_and_face::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5DA700
void als::combat_move_and_face::activate(animation_logic_system* the_als_core, layer_id layer) {
    move_and_face::activate(the_als, layer); // the stored als, not the_als_core

    f32 default_speed = retail::sub_601C50((u32*)the_actor) ? 50.0f : 25.0f;

    max_movement_speed = the_base->get_optional_pb_float(movement_speed_id, default_speed, nullptr);
    max_turn_speed     = (f32)(the_base->get_optional_pb_float(turn_rate_id, 0.0f, nullptr) * 0.017453293005625408);
}

// sub_5B2C00
void als::combat_move_and_face::set_end_key_signals() {
    static const string_hash* orientation_signals[] = { &references::rotate_end.get(), &references::move_end.get(), &references::attack.get(), &references::attack_begin.get() };
    static const string_hash* translation_signals[] = { &references::translate_end.get(), &references::move_end.get(), &references::attack.get(), &references::attack_begin.get() };

    orientation_end_key_signals       = orientation_signals;
    orientation_end_key_signals_count = 4;
    orientation_end_signal_required   = true;
    translation_end_key_signals       = translation_signals;
    translation_end_key_signals_count = 4;
    translation_end_signal_required   = true;
}

// sub_5B2BC0
void als::combat_move_and_face::set_start_key_signals() {
    static const string_hash* orientation_signals[] = { &references::rotate_begin.get(), &references::move_begin.get() };
    static const string_hash* translation_signals[] = { &references::translate_begin.get(), &references::move_begin.get() };

    orientation_start_key_signals       = orientation_signals;
    orientation_start_key_signals_count = 2;
    orientation_start_signal_required   = false;
    translation_start_key_signals       = translation_signals;
    translation_start_key_signals_count = 2;
    translation_start_signal_required   = false;
}

// sub_5AE650
als::combat_face_only::combat_face_only() {}

// sub_719410
als::combat_face_only::combat_face_only(mash::generating_vtable_version so_do_nothing) : combat_move_and_face(so_do_nothing) {}

// sub_5AE680
mash::virtual_types_key als::combat_face_only::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AE610
bool als::combat_face_only::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (combat_move_and_face::get_virtual_type_key() == parent_class)
        return true;

    if (combat_move_and_face::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE710
i32 als::combat_face_only::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5DA7B0
void als::combat_face_only::activate(animation_logic_system* the_als_core, layer_id layer) {
    combat_move_and_face::activate(the_als, layer); // the stored als, not the_als_core

    // the anim keeps the translation
    pre_signal_translation_action  = mocomp_action_from_anim;
    post_signal_translation_action = mocomp_action_from_anim;
}

// sub_5AE590
als::combat_move_only::combat_move_only() {}

// sub_719390
als::combat_move_only::combat_move_only(mash::generating_vtable_version so_do_nothing) : combat_move_and_face(so_do_nothing) {}

// sub_5AE5C0
mash::virtual_types_key als::combat_move_only::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AE610
bool als::combat_move_only::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (combat_move_and_face::get_virtual_type_key() == parent_class)
        return true;

    if (combat_move_and_face::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE710
i32 als::combat_move_only::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5DA7E0
void als::combat_move_only::activate(animation_logic_system* the_als_core, layer_id layer) {
    combat_move_and_face::activate(the_als, layer); // the stored als, not the_als_core

    // the anim keeps the orientation
    pre_signal_orientation_action  = mocomp_action_from_anim;
    post_signal_orientation_action = mocomp_action_from_anim;
}

// sub_5AE5D0
als::combat_move_and_face_with_attack::combat_move_and_face_with_attack() {}

// sub_7193D0
als::combat_move_and_face_with_attack::combat_move_and_face_with_attack(mash::generating_vtable_version so_do_nothing) : combat_move_and_face(so_do_nothing) {}

// sub_5AE600
mash::virtual_types_key als::combat_move_and_face_with_attack::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AE610
bool als::combat_move_and_face_with_attack::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (combat_move_and_face::get_virtual_type_key() == parent_class)
        return true;

    if (combat_move_and_face::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE710
i32 als::combat_move_and_face_with_attack::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5D8EA0
void als::combat_move_and_face_with_attack::activate(animation_logic_system* the_als_core, layer_id layer) {
    combat_move_and_face::activate(the_als, layer); // the stored als, not the_als_core
}

// sub_5B2C80
void als::combat_move_and_face_with_attack::set_end_key_signals() {
    static const string_hash* orientation_signals[] = { &references::rotate_end.get(), &references::move_end.get() };
    static const string_hash* translation_signals[] = { &references::translate_end.get(), &references::move_end.get() };

    orientation_end_key_signals       = orientation_signals;
    orientation_end_key_signals_count = 2;
    orientation_end_signal_required   = false;
    translation_end_key_signals       = translation_signals;
    translation_end_key_signals_count = 2;
    translation_end_signal_required   = false;
}

// sub_5B2C40
void als::combat_move_and_face_with_attack::set_start_key_signals() {
    static const string_hash* orientation_signals[] = { &references::rotate_begin.get(), &references::move_begin.get() };
    static const string_hash* translation_signals[] = { &references::translate_begin.get(), &references::move_begin.get() };

    orientation_start_key_signals       = orientation_signals;
    orientation_start_key_signals_count = 2;
    orientation_start_signal_required   = false;
    translation_start_key_signals       = translation_signals;
    translation_start_key_signals_count = 2;
    translation_start_signal_required   = false;
}

// sub_5AE690
als::combat_face_only_with_attack::combat_face_only_with_attack() {}

// sub_719450
als::combat_face_only_with_attack::combat_face_only_with_attack(mash::generating_vtable_version so_do_nothing) : combat_move_and_face_with_attack(so_do_nothing) {}

// sub_5AE6C0
mash::virtual_types_key als::combat_face_only_with_attack::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AE6D0
bool als::combat_face_only_with_attack::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (combat_move_and_face_with_attack::get_virtual_type_key() == parent_class)
        return true;

    if (combat_move_and_face_with_attack::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE710
i32 als::combat_face_only_with_attack::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5DA7B0
void als::combat_face_only_with_attack::activate(animation_logic_system* the_als_core, layer_id layer) {
    combat_move_and_face_with_attack::activate(the_als, layer); // the stored als, not the_als_core

    // the anim keeps the translation
    pre_signal_translation_action  = mocomp_action_from_anim;
    post_signal_translation_action = mocomp_action_from_anim;
}

// sub_5AE820
als::signal_move_and_face::signal_move_and_face() {}

// sub_719550
als::signal_move_and_face::signal_move_and_face(mash::generating_vtable_version so_do_nothing) : move_and_face(so_do_nothing) {}

// sub_5AE850
mash::virtual_types_key als::signal_move_and_face::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AEDD0
bool als::signal_move_and_face::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (move_and_face::get_virtual_type_key() == parent_class)
        return true;

    if (move_and_face::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE710
i32 als::signal_move_and_face::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5DA700
void als::signal_move_and_face::activate(animation_logic_system* the_als_core, layer_id layer) {
    move_and_face::activate(the_als, layer); // the stored als, not the_als_core

    f32 default_speed = retail::sub_601C50((u32*)the_actor) ? 50.0f : 25.0f;

    max_movement_speed = the_base->get_optional_pb_float(movement_speed_id, default_speed, nullptr);
    max_turn_speed     = (f32)(the_base->get_optional_pb_float(turn_rate_id, 0.0f, nullptr) * 0.017453293005625408);
}

// sub_5B3170
void als::signal_move_and_face::set_end_key_signals() {
    static const string_hash* orientation_signals[] = { &references::rotate_end.get(), &references::move_end.get() };
    static const string_hash* translation_signals[] = { &references::translate_end.get(), &references::move_end.get() };

    orientation_end_key_signals       = orientation_signals;
    orientation_end_key_signals_count = 2;
    orientation_end_signal_required   = false;
    translation_end_key_signals       = translation_signals;
    translation_end_key_signals_count = 2;
    translation_end_signal_required   = false;
}

// sub_5B3130
void als::signal_move_and_face::set_start_key_signals() {
    static const string_hash* orientation_signals[] = { &references::rotate_begin.get(), &references::move_begin.get() };
    static const string_hash* translation_signals[] = { &references::translate_begin.get(), &references::move_begin.get() };

    orientation_start_key_signals       = orientation_signals;
    orientation_start_key_signals_count = 2;
    orientation_start_signal_required   = false;
    translation_start_key_signals       = translation_signals;
    translation_start_key_signals_count = 2;
    translation_start_signal_required   = false;
}

// sub_5AE8A0
als::signal_face_only::signal_face_only() {}

// sub_7195D0
als::signal_face_only::signal_face_only(mash::generating_vtable_version so_do_nothing) : signal_move_and_face(so_do_nothing) {}

// sub_5AE8D0
mash::virtual_types_key als::signal_face_only::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AE8E0
bool als::signal_face_only::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (signal_move_and_face::get_virtual_type_key() == parent_class)
        return true;

    if (signal_move_and_face::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE710
i32 als::signal_face_only::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5DA7B0
void als::signal_face_only::activate(animation_logic_system* the_als_core, layer_id layer) {
    signal_move_and_face::activate(the_als, layer); // the stored als, not the_als_core

    // the anim keeps the translation
    pre_signal_translation_action  = mocomp_action_from_anim;
    post_signal_translation_action = mocomp_action_from_anim;
}

// sub_5AE860
als::signal_move_only::signal_move_only() {}

// sub_719590
als::signal_move_only::signal_move_only(mash::generating_vtable_version so_do_nothing) : signal_move_and_face(so_do_nothing) {}

// sub_5AE890
mash::virtual_types_key als::signal_move_only::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AE8E0
bool als::signal_move_only::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (signal_move_and_face::get_virtual_type_key() == parent_class)
        return true;

    if (signal_move_and_face::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE710
i32 als::signal_move_only::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5DA7E0
void als::signal_move_only::activate(animation_logic_system* the_als_core, layer_id layer) {
    signal_move_and_face::activate(the_als, layer); // the stored als, not the_als_core

    // the anim keeps the orientation
    pre_signal_orientation_action  = mocomp_action_from_anim;
    post_signal_orientation_action = mocomp_action_from_anim;
}

// sub_5AE920
als::constant_move_and_face::constant_move_and_face() {}

// sub_719610
als::constant_move_and_face::constant_move_and_face(mash::generating_vtable_version so_do_nothing) : move_and_face(so_do_nothing) {}

// sub_5AE950
mash::virtual_types_key als::constant_move_and_face::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AEDD0
bool als::constant_move_and_face::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (move_and_face::get_virtual_type_key() == parent_class)
        return true;

    if (move_and_face::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE710
i32 als::constant_move_and_face::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5F0C30
void als::constant_move_and_face::activate(animation_logic_system* the_als_core, layer_id layer) {
    move_and_face::activate(the_als_core, layer);

    max_movement_speed = the_base->get_optional_pb_float(movement_speed_id, 5.0f, nullptr);

    f32 default_turn_rate = the_base->has_ext_param_been_set(2) ? the_base->get_external_float(2) : 720.0f;

    max_turn_speed = (f32)(the_base->get_optional_pb_float(turn_rate_id, default_turn_rate, nullptr) * 0.017453293005625408);
}

// sub_5B41E0
void als::constant_move_and_face::set_time_left() {
    orientation_time_left = 0.0002f;
    translation_time_left = 0.0002f;
}

// sub_5B41E0
void als::constant_move_and_face::update_time_left() {
    orientation_time_left = 0.0002f;
    translation_time_left = 0.0002f;
}

// sub_5AEBB0
als::constant_face_only::constant_face_only() {}

// sub_719710
als::constant_face_only::constant_face_only(mash::generating_vtable_version so_do_nothing) : constant_move_and_face(so_do_nothing) {}

// sub_5AEBE0
mash::virtual_types_key als::constant_face_only::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AEB70
bool als::constant_face_only::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (constant_move_and_face::get_virtual_type_key() == parent_class)
        return true;

    if (constant_move_and_face::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE710
i32 als::constant_face_only::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5F0D30
void als::constant_face_only::activate(animation_logic_system* the_als_core, layer_id layer) {
    constant_move_and_face::activate(the_als, layer); // the stored als, not the_als_core

    // the anim keeps the translation
    pre_signal_translation_action  = mocomp_action_from_anim;
    post_signal_translation_action = mocomp_action_from_anim;
}

// sub_5AEB30
als::constant_move_only::constant_move_only() {}

// sub_7196D0
als::constant_move_only::constant_move_only(mash::generating_vtable_version so_do_nothing) : constant_move_and_face(so_do_nothing) {}

// sub_5AEB60
mash::virtual_types_key als::constant_move_only::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AEB70
bool als::constant_move_only::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (constant_move_and_face::get_virtual_type_key() == parent_class)
        return true;

    if (constant_move_and_face::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE710
i32 als::constant_move_only::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5F0D00
void als::constant_move_only::activate(animation_logic_system* the_als_core, layer_id layer) {
    constant_move_and_face::activate(the_als, layer); // the stored als, not the_als_core

    // the anim keeps the orientation
    pre_signal_orientation_action  = mocomp_action_from_anim;
    post_signal_orientation_action = mocomp_action_from_anim;
}

// sub_5AE460
als::wall_recover_move_and_face::wall_recover_move_and_face() {}

// sub_7192D0
als::wall_recover_move_and_face::wall_recover_move_and_face(mash::generating_vtable_version so_do_nothing) : move_and_face(so_do_nothing) {}

// sub_5AE490
mash::virtual_types_key als::wall_recover_move_and_face::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AEDD0
bool als::wall_recover_move_and_face::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (move_and_face::get_virtual_type_key() == parent_class)
        return true;

    if (move_and_face::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE710
i32 als::wall_recover_move_and_face::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5D8D50
void als::wall_recover_move_and_face::activate(animation_logic_system* the_als_core, layer_id layer) {
    move_and_face::activate(the_als, layer); // the stored als, not the_als_core

    pre_start_translation_action   = mocomp_action_from_anim;
    pre_start_orientation_action   = mocomp_action_from_anim;
    pre_signal_translation_action  = mocomp_action_procedural;
    pre_signal_orientation_action  = mocomp_action_procedural;
    post_signal_translation_action = mocomp_action_procedural;
    post_signal_orientation_action = mocomp_action_procedural;

    max_movement_speed      = the_base->get_optional_pb_float(movement_speed_id, 20.0f, nullptr);
    max_turn_speed          = (f32)(the_base->get_optional_pb_float(turn_rate_id, 1000.0f, nullptr) * 0.017453293005625408);
    constant_movement_speed = the_base->get_optional_pb_float(force_move_speed_id, -1.0f, nullptr);
    constant_turn_speed     = (f32)(the_base->get_optional_pb_float(force_turn_rate_id, -1.0f, nullptr) * 0.017453293005625408);
}

// sub_5B2B80
void als::wall_recover_move_and_face::set_end_key_signals() {
    static const string_hash* orientation_signals[] = { &references::rotate_end.get() };
    static const string_hash* translation_signals[] = { &references::translate_end.get(), &references::move_end.get() };

    orientation_end_key_signals       = orientation_signals;
    orientation_end_key_signals_count = 1;
    orientation_end_signal_required   = false;
    translation_end_key_signals       = translation_signals;
    translation_end_key_signals_count = 2;
    translation_end_signal_required   = false;
}

// sub_5B2B40
void als::wall_recover_move_and_face::set_start_key_signals() {
    static const string_hash* orientation_signals[] = { &references::rotate_begin.get() };
    static const string_hash* translation_signals[] = { &references::translate_begin.get(), &references::move_begin.get() };

    orientation_start_key_signals       = orientation_signals;
    orientation_start_key_signals_count = 1;
    orientation_start_signal_required   = false;
    translation_start_key_signals       = translation_signals;
    translation_start_key_signals_count = 2;
    translation_start_signal_required   = false;
}

// sub_5AED90
als::crawl_transition::crawl_transition() {}

// sub_719790
als::crawl_transition::crawl_transition(mash::generating_vtable_version so_do_nothing) : move_and_face(so_do_nothing) {}

// sub_5AEDC0
mash::virtual_types_key als::crawl_transition::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AEDD0
bool als::crawl_transition::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (move_and_face::get_virtual_type_key() == parent_class)
        return true;

    if (move_and_face::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AEE00
i32 als::crawl_transition::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5F0D60
void als::crawl_transition::activate(animation_logic_system* the_als_core, layer_id layer) {
    move_and_face::activate(the_als_core, layer);

    vector3d first_up = the_actor->my_abs_po->get_y_facing();

    desired_up     = first_up;
    desired_facing = the_actor->my_abs_po->get_z_facing();

    bool     on_top = false;
    vector3d normal = desired_up;

    wall_offset = the_base->get_optional_pb_float(wall_offset_id, 0.4f, nullptr);

    f32  vert_wall_offset   = the_base->get_optional_pb_float(vert_wall_offset_id, 0.0f, nullptr);
    bool wall_normal_is_up  = the_base->get_optional_pb_int(wall_normal_is_up_id, 0, nullptr) != 0;
    bool use_instant_facing = the_base->get_optional_pb_int(use_instant_facing_id, 0, nullptr) != 0;
    bool facing_from_normal = the_base->has_ext_param_been_set(7) && the_base->get_external_float(7) == 1.0f;

    // retail reads every external vector through this one temporary, and the climb onto a top uses whichever came last
    vector3d param;

    if (the_base->has_ext_param_been_set(141)) {
        param  = the_base->get_external_vector(141);
        normal = param;

        if (wall_normal_is_up)
            desired_up = param;

        on_top = desired_facing.y >= 0.7f && desired_up.y >= 0.7f;
    }

    if (the_base->has_ext_param_been_set(143)) {
        param          = the_base->get_external_vector(143);
        desired_facing = param;
    } else if (facing_from_normal)
        desired_facing = normal;
    else
        desired_facing = the_actor->my_abs_po->get_z_facing();

    destination_point = the_actor->my_abs_po->get_position();

    if (the_base->has_ext_param_been_set(146)) {
        param             = the_base->get_external_vector(146);
        destination_point = param;
    } else if (the_actor->has_ifc(entity_ifc_player))
        destination_point = the_actor->player_ifc()->unk_2a0;
    else if (the_actor->has_ifc(entity_ifc_standing)) {
        ((standing_interface*)the_actor->my_ifc_storage->get_ifc(entity_ifc_standing))->unk_044 |= 1;
        ((standing_interface*)the_actor->my_ifc_storage->get_ifc(entity_ifc_standing))->unk_044 |= 8;
    }

    if (on_top) {
        // over the edge: along the surface to the destination, then out past param 146 against the facing
        const vector3d &position = the_actor->my_abs_po->get_position();
        const vector3d &facing   = the_actor->my_abs_po->get_z_facing();
        const vector3d &zero     = treyarch::references::unk_00fd4370.get();

        vector3d to_destination = destination_point - position;

        math::vector4 basis_dir(-facing.x, -facing.y, -facing.z, -0.0f);
        math::vector4 basis_up(param.x, param.y, param.z, 0.0f);
        math::vector4 basis_origin(zero.x, zero.y, zero.z, 0.0f);

        po basis;
        retail::sub_402260((u64*)&basis, (f32*)&basis_dir, (f32*)&basis_up, (i64*)&basis_origin);

        vector3d start = the_actor->my_abs_po->get_position();

        param = the_base->get_external_vector(146);

        const vector3d &basis_y = basis.get_y_facing();
        const vector3d &basis_z = basis.get_z_facing();

        vector3d reach(param.x + basis_z.x * 1.1f - start.x, param.y + basis_z.y * 1.1f - start.y, param.z + basis_z.z * 1.1f - start.z);
        f32      rise = basis_y * reach;

        reach = reach - basis_y * rise;

        vector3d from = the_actor->my_abs_po->get_position();
        f32      out  = basis_z * to_destination;

        vector3d across = to_destination - basis_z * out;

        destination_point = vector3d(across.x + from.x + reach.x, across.y + from.y + reach.y, across.z + from.z + reach.z);
    } else
        destination_point = vector3d(normal.x * wall_offset + destination_point.x + first_up.x * vert_wall_offset,
                                     normal.y * wall_offset + destination_point.y + first_up.y * vert_wall_offset,
                                     normal.z * wall_offset + destination_point.z + first_up.z * vert_wall_offset);

    if (use_instant_facing) {
        const vector3d &zero = treyarch::references::unk_00fd4370.get();

        vector3d side = cross(desired_facing, desired_up);

        if (!((side - zero).length2() <= 0.00000001f)) {
            const vector3d &position = the_actor->my_abs_po->get_position();

            math::vector4 basis_dir(desired_facing.x, desired_facing.y, desired_facing.z, 0.0f);
            math::vector4 basis_up(desired_up.x, desired_up.y, desired_up.z, 0.0f);
            math::vector4 origin(position.x, position.y, position.z, 0.0f);

            po facing;
            retail::sub_402260((u64*)&facing, (f32*)&basis_dir, (f32*)&basis_up, (i64*)&origin);
            retail::sub_627180((i32)the_actor, (f32*)&facing); // entity_base::set_abs_po
        }
    }

    destination_point_valid = true;
    desired_facing_valid    = true;

    transition_destination = destination_point;
    start_position         = the_actor->my_abs_po->get_position();
}

// sub_5DB1A0
void als::crawl_transition::deactivate() {
    move_and_face::deactivate();
}

// sub_5B42D0
void als::crawl_transition::post_anim_action(f32 delta_t) {
    move_and_face::post_anim_action(delta_t);
}

// sub_5B4200
void als::crawl_transition::set_destination_point() {
    destination_point_valid = true;
}

// sub_5B42E0
void als::crawl_transition::set_desired_facing() {
    desired_facing_valid = true;
}

// sub_5B4A70
void als::crawl_transition::set_desired_up() {}

// sub_5B4290
void als::crawl_transition::set_end_key_signals() {
    static const string_hash* signals[] = { &references::move_end.get() };

    orientation_end_key_signals_count = 1;
    translation_end_key_signals_count = 1;
    orientation_end_key_signals       = signals;
    translation_end_key_signals       = signals;
    orientation_end_signal_required   = false;
    translation_end_signal_required   = false;
}

// sub_5AED50
als::rvb_crawl_transition::rvb_crawl_transition() {}

// sub_719750
als::rvb_crawl_transition::rvb_crawl_transition(mash::generating_vtable_version so_do_nothing) : move_and_face(so_do_nothing) {}

// sub_5AED80
mash::virtual_types_key als::rvb_crawl_transition::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AEDD0
bool als::rvb_crawl_transition::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (move_and_face::get_virtual_type_key() == parent_class)
        return true;

    if (move_and_face::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE710
i32 als::rvb_crawl_transition::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5DACB0
void als::rvb_crawl_transition::activate(animation_logic_system* the_als_core, layer_id layer) {
    move_and_face::activate(the_als_core, layer);

    bool use_desired_normal    = the_base->get_optional_pb_int(use_desired_normal_id, 1, nullptr) != 0;
    bool use_instant_facing    = the_base->get_optional_pb_int(use_instant_facing_id, 1, nullptr) != 0;
    bool use_anim_translation  = the_base->get_optional_pb_int(use_anim_translation_id, 1, nullptr) != 0;
    bool use_anim_orientation  = the_base->get_optional_pb_int(use_anim_orientation_id, 1, nullptr) != 0;
    f32  dest_point_up_offset  = the_base->get_optional_pb_float(dest_point_up_offset_id, 0.0f, nullptr);
    bool use_fakeroot_position = the_base->get_optional_pb_int(use_fakeroot_position_id, 1, nullptr) != 0;

    desired_up        = the_actor->my_abs_po->get_y_facing();
    desired_facing    = the_actor->my_abs_po->get_z_facing();
    destination_point = the_actor->my_abs_po->get_position();

    if (use_desired_normal && the_base->has_ext_param_been_set(141))
        desired_up = the_base->get_external_vector(141);

    if (the_base->has_ext_param_been_set(143))
        desired_facing = the_base->get_external_vector(143);

    // the destination param, raised along the up
    if (!use_fakeroot_position && the_base->has_ext_param_been_set(146)) {
        destination_point = the_base->get_external_vector(146);
        destination_point = vector3d(destination_point.x + desired_up.x * dest_point_up_offset,
                                     destination_point.y + desired_up.y * dest_point_up_offset,
                                     destination_point.z + desired_up.z * dest_point_up_offset);
    }

    if (use_instant_facing) {
        const vector3d &position = the_actor->my_abs_po->get_position();

        math::vector4 basis_dir(desired_facing.x, desired_facing.y, desired_facing.z, 0.0f);
        math::vector4 basis_up(desired_up.x, desired_up.y, desired_up.z, 0.0f);
        math::vector4 origin(position.x, position.y, position.z, 0.0f);

        po facing;
        retail::sub_402260((u64*)&facing, (f32*)&basis_dir, (f32*)&basis_up, (i64*)&origin);

        const vector3d &zero = treyarch::references::unk_00fd4370.get();

        vector3d side = cross(desired_facing, desired_up);

        if (!((side - zero).length2() <= 0.00000001f))
            retail::sub_627180((i32)the_actor, (f32*)&facing); // entity_base::set_abs_po
    }

    if (use_anim_translation) {
        pre_signal_translation_action  = mocomp_action_from_anim;
        post_signal_translation_action = mocomp_action_from_anim;
    }

    if (use_anim_orientation || use_instant_facing) {
        pre_signal_orientation_action  = mocomp_action_from_anim;
        post_signal_orientation_action = mocomp_action_from_anim;
    }

    desired_facing_valid    = true;
    destination_point_valid = true;
}

// sub_5DB1A0
void als::rvb_crawl_transition::deactivate() {
    move_and_face::deactivate();
}

// sub_5B42D0
void als::rvb_crawl_transition::post_anim_action(f32 delta_t) {
    move_and_face::post_anim_action(delta_t);
}

// sub_5B4200
void als::rvb_crawl_transition::set_destination_point() {
    destination_point_valid = true;
}

// sub_5B42E0
void als::rvb_crawl_transition::set_desired_facing() {
    desired_facing_valid = true;
}

// sub_5B4A70
void als::rvb_crawl_transition::set_desired_up() {}

// sub_5B4250
void als::rvb_crawl_transition::set_end_key_signals() {
    static const string_hash* orientation_signals[] = { &references::rotate_end.get(), &references::move_end.get() };
    static const string_hash* translation_signals[] = { &references::translate_end.get(), &references::move_begin.get() }; // MOVE_BEGIN, as retail has it

    orientation_end_key_signals       = orientation_signals;
    orientation_end_key_signals_count = 2;
    orientation_end_signal_required   = false;
    translation_end_key_signals       = translation_signals;
    translation_end_key_signals_count = 2;
    translation_end_signal_required   = false;
}

// sub_5B4210
void als::rvb_crawl_transition::set_start_key_signals() {
    static const string_hash* orientation_signals[] = { &references::rotate_begin.get(), &references::move_begin.get() };
    static const string_hash* translation_signals[] = { &references::translate_begin.get(), &references::move_begin.get() };

    orientation_start_key_signals       = orientation_signals;
    orientation_start_key_signals_count = 2;
    orientation_start_signal_required   = false;
    translation_start_key_signals       = translation_signals;
    translation_start_key_signals_count = 2;
    translation_start_signal_required   = false;
}

// sub_5AF070
als::attach_hand_to_point::attach_hand_to_point() {}

// sub_719840
als::attach_hand_to_point::attach_hand_to_point(mash::generating_vtable_version so_do_nothing) : signal_move_and_face(so_do_nothing) {}

// sub_5AF0A0
mash::virtual_types_key als::attach_hand_to_point::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AE8E0
bool als::attach_hand_to_point::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (signal_move_and_face::get_virtual_type_key() == parent_class)
        return true;

    if (signal_move_and_face::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AF0B0
i32 als::attach_hand_to_point::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5DB8F0
void als::attach_hand_to_point::activate(animation_logic_system* the_als_core, layer_id layer) {
    signal_move_and_face::activate(the_als_core, layer);

    max_movement_speed = the_base->get_optional_pb_float(movement_speed_id, 500.0f, nullptr);
    start_position     = the_actor->my_abs_po->get_position();
    dest_reached       = false;
}

// sub_5B4320
void als::attach_hand_to_point::post_anim_action(f32 delta_t) {
    signal_move_and_face::post_anim_action(delta_t);

    // there, it stays on the point while the state locks it
    i32 destination_lock = the_base->get_optional_pb_int(destination_lock_id, 0, nullptr);

    if (dest_reached && destination_lock) {
        set_destination_point();

        retail::sub_6271F0((u32*)the_actor, (i32)&destination_point); // entity_base::set_abs_position
    }
}

// sub_5DB760
void als::attach_hand_to_point::set_destination_point() {
    if (!the_base->has_ext_param_been_set(145))
        return;

    vector3d point       = the_base->get_external_vector(145);
    i32      anchor_hand = the_base->get_optional_pb_int(anchor_hand_id, 1, nullptr);

    destination_point = the_actor->my_abs_po->get_position();

    // the player's prop hand onto the point, the body following
    if (the_actor->has_ifc(entity_ifc_player)) {
        arch_base*   body = the_actor->my_handle.resolve();
        string_hash* hand = anchor_hand ? &references::bip01_prop_r_hand.get() : &references::bip01_prop_l_hand.get();

        vector3d hand_position;
        retail::sub_62FA60((i32)body, retail::sub_62F880((u32*)body, (u32*)hand), (f32*)&hand_position, 0); // conglomerate::get_member_abs_position

        destination_point = vector3d(point.x + (destination_point.x - hand_position.x),
                                     point.y + (destination_point.y - hand_position.y),
                                     point.z + (destination_point.z - hand_position.z));

        destination_point_valid = true;
    }
}

// sub_5C3690
void als::attach_hand_to_point::reached_dest_signal(f32) {
    if (orientation_time_left <= 0.0001f && translation_time_left <= 0.0001f && !dest_reached) {
        event_manager::raise_event(references::anim_dest_reached.read(), the_actor->my_handle);

        dest_reached = true;
    }
}

// sub_5F1820
void als::attach_hand_to_point::face_and_arrive_by(f32 delta_t) {
    // turned and moved from where it started, not from where the actor is
    const matrix4x4 &abs_m = the_actor->my_abs_po->get_matrix();

    po old_po;
    old_po.get_matrix().x = abs_m.x;
    old_po.get_matrix().y = abs_m.y;
    old_po.get_matrix().z = abs_m.z;
    old_po.get_matrix().w = vector4d(start_position.x, start_position.y, start_position.z, 1.0f);

    po anim_po = calculate_anim_motion();
    po facing  = orientation_action(delta_t, old_po, anim_po);

    // a near-vertical param 152 faces it along the ground
    if (the_base->has_ext_param_been_set(60) && the_base->get_external_float(60) > 0.5f && the_base->has_ext_param_been_set(152)) {
        vector3d target = the_base->get_external_vector(152);

        if (target.y > 0.9f) {
            target.y = 0.0f;

            retail::sub_402050((f32*)&target);
            retail::sub_44E1E0((f32*)&facing, target.x, target.y, target.z); // po: looks at the point
        }
    }

    vector3d position = translation_action(delta_t, old_po, anim_po);

    facing.get_matrix().w = vector4d(position.x, position.y, position.z, 1.0f);

    retail::sub_627180((i32)the_actor, (f32*)&facing); // entity_base::set_abs_po

    const vector3d &moved_to   = the_actor->my_abs_po->get_position();
    const vector3d &moved_from = old_po.get_position();

    the_actor->set_frame_delta_trans(vector3d(moved_to.x - moved_from.x, moved_to.y - moved_from.y, moved_to.z - moved_from.z), delta_t);
}

// sub_5AF0C0
als::constant_move_hand_to_point::constant_move_hand_to_point() {}

// sub_719880
als::constant_move_hand_to_point::constant_move_hand_to_point(mash::generating_vtable_version so_do_nothing) : attach_hand_to_point(so_do_nothing) {}

// sub_5AF0F0
mash::virtual_types_key als::constant_move_hand_to_point::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AF100
bool als::constant_move_hand_to_point::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (attach_hand_to_point::get_virtual_type_key() == parent_class)
        return true;

    if (attach_hand_to_point::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_A70E20
i32 als::constant_move_hand_to_point::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5DB970
void als::constant_move_hand_to_point::activate(animation_logic_system* the_als_core, layer_id layer) {
    attach_hand_to_point::activate(the_als_core, layer);

    arrived     = false;
    first_frame = true;
}

// sub_5F1A90
vector3d als::constant_move_hand_to_point::compute_translation(f32 delta_t, const po &old_po, const po &) {
    auto unitized = [](const vector3d &value) {
        math::vector4 direction(value.x, value.y, value.z, 0.0f);
        math::vector4 unit;

        retail::sub_401AA0((u64*)&unit, (f32*)&direction);

        return vector3d(unit[0], unit[1], unit[2]);
    };

    // straight at the point, at param 0's speed
    vector3d position = old_po.get_position();
    vector3d dir      = unitized(destination_point - position);

    f32 speed = the_base->has_ext_param_been_set(0) ? the_base->get_external_float(0) : 1.0f;
    f32 step  = speed * delta_t;

    if (position.x == destination_point.x && position.y == destination_point.y && position.z == destination_point.z)
        return position;

    return vector3d(position.x + dir.x * step, position.y + dir.y * step, position.z + dir.z * step);
}

// sub_5C36F0
void als::constant_move_hand_to_point::reached_dest_signal(f32) {
    if (arrived && !dest_reached) {
        event_manager::raise_event(references::anim_dest_reached.read(), the_actor->my_handle);

        dest_reached = true;
    }
}

// sub_5B4390
void als::constant_move_hand_to_point::face_and_arrive_by(f32 delta_t) {
    po       old_po       = *the_actor->my_abs_po;
    vector3d old_position = old_po.get_position();

    po anim_po = calculate_anim_motion();
    po facing  = orientation_action(delta_t, old_po, anim_po);

    vector3d to_destination = destination_point - old_position;

    // the first frame holds still unless it's there already
    if (!first_frame || to_destination.length() == 0.0f) {
        vector3d position = translation_action(delta_t, old_po, anim_po);

        facing.get_matrix().w = vector4d(position.x, position.y, position.z, 1.0f);

        retail::sub_627180((i32)the_actor, (f32*)&facing); // entity_base::set_abs_po

        const vector3d &moved_to   = the_actor->my_abs_po->get_position();
        const vector3d &moved_from = old_po.get_position();

        the_actor->set_frame_delta_trans(vector3d(moved_to.x - moved_from.x, moved_to.y - moved_from.y, moved_to.z - moved_from.z), delta_t);

        // arrived once the point is behind it or reached
        if (!dest_reached) {
            vector3d remaining = destination_point - position;

            if (to_destination * remaining < 0.0f || remaining.length() < 0.0001f)
                arrived = true;
        }
    }

    first_frame = false;
}

// sub_5AF020
als::combat_pole_swing_attach::combat_pole_swing_attach() {}

// sub_719800
als::combat_pole_swing_attach::combat_pole_swing_attach(mash::generating_vtable_version so_do_nothing) : signal_move_and_face(so_do_nothing) {}

// sub_5AF050
mash::virtual_types_key als::combat_pole_swing_attach::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AE8E0
bool als::combat_pole_swing_attach::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (signal_move_and_face::get_virtual_type_key() == parent_class)
        return true;

    if (signal_move_and_face::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AF060
i32 als::combat_pole_swing_attach::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5DB360
void als::combat_pole_swing_attach::activate(animation_logic_system* the_als_core, layer_id layer) {
    auto unitized = [](const vector3d &value) {
        math::vector4 direction(value.x, value.y, value.z, 0.0f);
        math::vector4 unit;

        retail::sub_401AA0((u64*)&unit, (f32*)&direction);

        return vector3d(unit[0], unit[1], unit[2]);
    };

    signal_move_and_face::activate(the_als_core, layer);

    pole_point         = the_actor->my_abs_po->get_position();
    facing             = the_actor->my_abs_po->get_z_facing();
    max_movement_speed = the_base->get_optional_pb_float(movement_speed_id, 500.0f, nullptr);

    if (the_actor->has_ifc(entity_ifc_player)) {
        void* pole  = the_actor->player_ifc()->unk_387();
        face_target = the_actor->player_ifc()->unk_388();

        // the pole's middle, and its side
        vector3d pole_end;
        vector3d pole_start;
        retail::sub_91BC00((i32*)pole, (f32*)&pole_end);
        retail::sub_91BA80((i32*)pole, (f32*)&pole_start);

        pole_point = (pole_start + pole_end) * 0.5f;

        retail::sub_91BA80((i32*)pole, (f32*)&pole_start);
        retail::sub_91BC00((i32*)pole, (f32*)&pole_end);

        vector3d along = unitized(pole_end - pole_start);

        side = unitized(cross(along, treyarch::references::unk_00e6808c.get()));
    } else if (retail::sub_602850((u32*)the_actor)) {
        auto* teleport = (teleporting_layer_inode*)the_actor->get_ai_core()->get_info_node(info_node_type_teleporting_layer);

        if (teleport) {
            if (entity* target = (entity*)teleport->unk_0b8.resolve()) {
                face_target = true;
                pole_point  = target->my_abs_po->get_position();
            }
        }
    }
}

// sub_5C3570
void als::combat_pole_swing_attach::set_destination_point() {
    destination_point = the_actor->my_abs_po->get_position();

    // the player's right prop hand onto the pole, the body following
    if (the_actor->has_ifc(entity_ifc_player)) {
        actor* body = the_actor->player_ifc()->my_actor;

        vector3d hand_position;
        retail::sub_62FA60((i32)body, retail::sub_62F880((u32*)body, (u32*)&references::bip01_prop_r_hand.get()), (f32*)&hand_position, 0); // conglomerate::get_member_abs_position

        destination_point = vector3d(destination_point.x + (pole_point.x - hand_position.x),
                                     destination_point.y + (pole_point.y - hand_position.y),
                                     destination_point.z + (pole_point.z - hand_position.z));

        destination_point_valid = true;
    }
}

// sub_5DB1B0
void als::combat_pole_swing_attach::set_desired_facing() {
    auto unitized = [](const vector3d &value) {
        math::vector4 direction(value.x, value.y, value.z, 0.0f);
        math::vector4 unit;

        retail::sub_401AA0((u64*)&unit, (f32*)&direction);

        return vector3d(unit[0], unit[1], unit[2]);
    };

    // level, from the pole toward param 145
    if (the_base->has_ext_param_been_set(145)) {
        facing   = the_base->get_external_vector(145) - pole_point;
        facing.y = 0.0f;
        facing   = unitized(facing);
    }

    // or along the pole's side, on the target's half
    if (!face_target) {
        if (side * facing < 0.0f)
            side = -side;

        facing = side;
    }

    desired_facing       = facing;
    desired_facing_valid = true;
}

// sub_5B42F0
void als::combat_pole_swing_attach::set_desired_up() {
    desired_up = treyarch::references::unk_00e6808c.get();
}

// sub_5AE960
mash::virtual_types_key als::sync_move_and_face::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AE970
bool als::sync_move_and_face::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (use_anim_only::get_virtual_type_key() == parent_class)
        return true;

    if (use_anim_only::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4D6250
i32 als::sync_move_and_face::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5DA810
void als::sync_move_and_face::activate(animation_logic_system* the_als_core, layer_id layer) {
    motion_compensator::activate(the_als_core, layer);

    tween_pos          = 0.0f;
    tween_orient       = 0.0f;
    sync_index_actor_a = nullptr;
    sync_index_a       = -2;
    sync_index_actor_b = nullptr;
    sync_index_b       = -2;
    lock_pos_a         = false;
    lock_pos_b         = false;
    lock_orient_a      = false;
    lock_orient_b      = false;

    // standing and physics stay allowed until the post says otherwise
    if (the_actor->has_ifc(entity_ifc_standing))
        ((standing_interface*)the_actor->my_ifc_storage->get_ifc(entity_ifc_standing))->unk_044 |= 0x4000;

    if (the_actor->has_ifc(entity_ifc_physical))
        the_actor->physical_ifc()->flags |= 0x800;

    i32 disable_collision = the_base->get_optional_pb_int(sync_disable_collision_id, 0, nullptr);

    ((void (__thiscall*)(entity*, bool, bool))the_actor->vtable[69])(the_actor, disable_collision == 0, true); // slot 69

    rel_slide_a = treyarch::references::unk_00fd4370.get();
    rel_slide_b = treyarch::references::unk_00fd4370.get();
}

// sub_5C1220
void als::sync_move_and_face::deactivate() {
    if (the_actor->has_ifc(entity_ifc_standing))
        ((standing_interface*)the_actor->my_ifc_storage->get_ifc(entity_ifc_standing))->unk_044 &= ~0x4000;

    if (the_actor->has_ifc(entity_ifc_physical))
        the_actor->physical_ifc()->flags &= ~0x800;

    ((void (__thiscall*)(entity*, bool, bool))the_actor->vtable[69])(the_actor, true, true); // slot 69
}

// sub_5EAE80
void als::sync_move_and_face::post_anim_action(f32 delta_t) {
    use_anim_only::post_anim_action(delta_t);

    // the player syncs with the actor behind its unk_467 too, sync anim flag or not
    bool has_partner = false;

    if (the_actor->has_ifc(entity_ifc_player)) {
        if (void* held = the_actor->player_ifc()->unk_467()) {
            arch_base_vhandle target = *(arch_base_vhandle*)((u8*)held + 4);

            if (target.resolve() && target.resolve()->is_an_actor())
                has_partner = true;
        }
    }

    if (!(the_actor->ext_flags & 0x20000) && !has_partner)
        return;

    actor* actor_a = nullptr;
    actor* actor_b = nullptr;

    if (!get_sync_actors(&actor_a, &actor_b))
        return;

    auto* als_a = (animation_logic_system*)actor_a->get_als();
    auto* als_b = (animation_logic_system*)actor_b->get_als();

    state_machine_interface* layer_a = als_a ? als_a->get_als_layer(als_a->curr_mocomp->get_layer_id()) : nullptr;
    state_machine_interface* layer_b = als_b ? als_b->get_als_layer(als_b->curr_mocomp->get_layer_id()) : nullptr;

    if (!layer_a && !layer_b)
        return;

    if (layer_a && layer_a->get_optional_pb_int(allow_sync_id, 1, nullptr) != 1)
        return;

    if (layer_b && layer_b->get_optional_pb_int(allow_sync_id, 1, nullptr) != 1)
        return;

    // each side's tween times, b's over a's and the specific ones over sync_tween_time
    if (tween_pos < 1.0f) {
        f32 tween_time = 0.5f;

        if (layer_a)
            tween_time = layer_a->get_optional_pb_float(sync_tween_time_id, tween_time, nullptr);

        if (layer_b)
            tween_time = layer_b->get_optional_pb_float(sync_tween_time_id, tween_time, nullptr);

        if (layer_a)
            tween_time = layer_a->get_optional_pb_float(sync_tween_time_pos_id, tween_time, nullptr);

        if (layer_b)
            tween_time = layer_b->get_optional_pb_float(sync_tween_time_pos_id, tween_time, nullptr);

        if (tween_time < 0.0001f)
            tween_pos = 1.0f;
        else {
            if (tween_time < 0.1f)
                tween_time = 0.1f;

            tween_pos = (f32)((f64)delta_t / (f64)tween_time + (f64)tween_pos);

            if (tween_pos > 1.0f)
                tween_pos = 1.0f;
        }
    }

    if (tween_orient < 1.0f) {
        f32 tween_time = 0.5f;

        if (layer_a)
            tween_time = layer_a->get_optional_pb_float(sync_tween_time_id, tween_time, nullptr);

        if (layer_b)
            tween_time = layer_b->get_optional_pb_float(sync_tween_time_id, tween_time, nullptr);

        if (layer_a)
            tween_time = layer_a->get_optional_pb_float(sync_tween_time_orient_id, tween_time, nullptr);

        if (layer_b)
            tween_time = layer_b->get_optional_pb_float(sync_tween_time_orient_id, tween_time, nullptr);

        if (tween_time < 0.0001f)
            tween_orient = 1.0f;
        else {
            if (tween_time < 0.1f)
                tween_time = 0.1f;

            tween_orient = (f32)((f64)delta_t / (f64)tween_time + (f64)tween_orient);

            if (tween_orient > 1.0f)
                tween_orient = 1.0f;
        }
    }

    // a side out of the sync mocomps stays put
    bool unsynced_a = !als_a || !als_a->curr_mocomp || !als_a->curr_mocomp->is_synced();
    bool unsynced_b = !als_b || !als_b->curr_mocomp || !als_b->curr_mocomp->is_synced();

    bool locked_a = unsynced_a || !layer_a || layer_a->get_optional_pb_int(sync_lock_id, 0, nullptr);
    bool locked_b = unsynced_b || !layer_b || layer_b->get_optional_pb_int(sync_lock_id, 0, nullptr);

    lock_pos_a    = locked_a || !layer_a || layer_a->get_optional_pb_int(sync_lock_pos_id, 0, nullptr);
    lock_pos_b    = locked_b || !layer_b || layer_b->get_optional_pb_int(sync_lock_pos_id, 0, nullptr);
    lock_orient_a = locked_a || !layer_a || layer_a->get_optional_pb_int(sync_lock_orient_id, 0, nullptr);
    lock_orient_b = locked_b || !layer_b || layer_b->get_optional_pb_int(sync_lock_orient_id, 0, nullptr);

    // what each side's state lets it keep; physics follows standing unless sync_allow_physics is set
    bool allow_standing_a = false;
    bool allow_physics_a  = false;
    bool obey_signals_a   = false;

    if ((actor_a->ext_flags & 0x20000) && layer_a) {
        allow_standing_a = layer_a->get_optional_pb_int(sync_allow_standing_id, 1, nullptr) != 0;
        allow_physics_a  = allow_standing_a;

        if (layer_a->get_optional_pb_int(sync_allow_physics_id, -1, nullptr) > 0)
            allow_physics_a = true;

        obey_signals_a = layer_a->get_optional_pb_int(obey_move_and_face_signals_id, 0, nullptr) != 0;
    }

    if (actor_a->has_ifc(entity_ifc_standing)) {
        auto* standing = (standing_interface*)actor_a->my_ifc_storage->get_ifc(entity_ifc_standing);

        if (allow_standing_a)
            standing->unk_044 |= 0x4000;
        else
            standing->unk_044 &= ~0x4000;
    }

    if (actor_a->has_ifc(entity_ifc_physical)) {
        physical_interface* physical = actor_a->physical_ifc();

        if (allow_physics_a)
            physical->flags |= 0x800;
        else
            physical->flags &= ~0x800;
    }

    bool allow_standing_b = false;
    bool allow_physics_b  = false;
    bool obey_signals_b   = false;

    if ((actor_b->ext_flags & 0x20000) && layer_b) {
        allow_standing_b = layer_b->get_optional_pb_int(sync_allow_standing_id, 1, nullptr) != 0;
        allow_physics_b  = allow_standing_b;

        if (layer_b->get_optional_pb_int(sync_allow_physics_id, -1, nullptr) > 0)
            allow_physics_b = true;

        obey_signals_b = layer_b->get_optional_pb_int(obey_move_and_face_signals_id, 0, nullptr) != 0;
    }

    if (actor_b->has_ifc(entity_ifc_standing)) {
        auto* standing = (standing_interface*)actor_b->my_ifc_storage->get_ifc(entity_ifc_standing);

        if (allow_standing_b)
            standing->unk_044 |= 0x4000;
        else
            standing->unk_044 &= ~0x4000;
    }

    if (actor_b->has_ifc(entity_ifc_physical)) {
        physical_interface* physical = actor_b->physical_ifc();

        if (allow_physics_b)
            physical->flags |= 0x800;
        else
            physical->flags &= ~0x800;
    }

    // a side obeying the move and face signals stops the sync once it's past TRANSLATE_END
    bool keep_syncing = true;

    if (obey_signals_a) {
        bool exists = false;
        f32  time   = layer_a->get_time_to_signal(references::translate_end.read(), &exists, false, nullptr);

        if (exists && time <= 0.0f)
            keep_syncing = false;

        layer_a->get_time_to_signal(references::rotate_end.read(), &exists, false, nullptr);
    }

    if (obey_signals_b) {
        bool exists = false;
        f32  time   = layer_b->get_time_to_signal(references::translate_end.read(), &exists, false, nullptr);

        if (exists && time <= 0.0f)
            keep_syncing = false;

        layer_b->get_time_to_signal(references::rotate_end.read(), &exists, false, nullptr);
    }

    if (!keep_syncing)
        return;

    reorient_to_vertical(actor_a, layer_a, actor_b, layer_b);

    po po_a;
    po po_b;
    retail::sub_4015B0((u64*)&po_a);
    retail::sub_4015B0((u64*)&po_b);

    get_sync_bone_po(actor_a, &po_a, actor_b, &po_b);

    // the bones' positions, apart from their orientations
    vector3d pos_a = po_a.get_position();

    retail::sub_424900((u64*)&po_a, (u32*)&treyarch::references::unk_00fd4370.get()); // po::set_position
    retail::sub_9595B0((i32)&po_a);

    vector3d pos_b = po_b.get_position();

    retail::sub_424900((u64*)&po_b, (u32*)&treyarch::references::unk_00fd4370.get());
    retail::sub_9595B0((i32)&po_b);

    apply_sync_rotate_angles(layer_a, &po_a, layer_b, &po_b);

    apply_axis_slides(actor_a, layer_a, &pos_a, &po_a, actor_b, layer_b, &pos_b, &po_b);

    // each side eases the other over, the whole tween when it's locked itself and half otherwise
    if (!lock_pos_b || !lock_orient_b) {
        f32 pos_weight    = lock_pos_b    ? 0.0f : (lock_pos_a    ? tween_pos    : tween_pos * 0.5f);
        f32 orient_weight = lock_orient_b ? 0.0f : (lock_orient_a ? tween_orient : tween_orient * 0.5f);

        sync_entities(delta_t, actor_a, &po_a, &pos_a, actor_b, &po_b, &pos_b, pos_weight, orient_weight, lock_pos_b, lock_orient_b);
    }

    if (!lock_pos_a || !lock_orient_a) {
        f32 pos_weight    = lock_pos_a    ? 0.0f : (lock_pos_b    ? tween_pos    : tween_pos * 0.5f);
        f32 orient_weight = lock_orient_a ? 0.0f : (lock_orient_b ? tween_orient : tween_orient * 0.5f);

        sync_entities(delta_t, actor_b, &po_b, &pos_b, actor_a, &po_a, &pos_a, pos_weight, orient_weight, lock_pos_a, lock_orient_a);
    }
}

// sub_667F80
bool als::sync_move_and_face::is_synced() const {
    return true;
}

// sub_667F80
bool als::sync_move_and_face::force_reactivate() const {
    return true;
}

// sub_5E31A0
bool als::sync_move_and_face::get_sync_actors(actor** actor_a, actor** actor_b) {
    if (!actor_a || !actor_b)
        return false;

    *actor_a = nullptr;
    *actor_b = nullptr;

    animation_controller* actor_nal_ctrl = the_actor->anim_ctrl;

    // following a master, the master's actor leads
    if (actor_nal_ctrl->has_master()) {
        *actor_a = actor_nal_ctrl->get_master()->get_actor();
        *actor_b = the_actor;
    } else if (actor_nal_ctrl->has_slaves()) {
        for (animation_controller** slave = actor_nal_ctrl->state->slaves.begin(); slave != actor_nal_ctrl->state->slaves.end(); ++slave) {
            actor*                  slave_actor = (*slave)->get_actor();
            animation_logic_system* slave_als   = (animation_logic_system*)slave_actor->get_als();

            if (!slave_als || !slave_als->curr_mocomp || slave_als->curr_mocomp->get_virtual_type_key() != get_virtual_type_key()) {
                *actor_a = the_actor;
                *actor_b = slave_actor;

                break;
            }
        }
    }

    // carrying or carried, the other side leads
    if (!*actor_a && !*actor_b && the_actor->get_ai_core()) {
        auto* carry = (std_carry_inode*)the_actor->get_ai_core()->get_info_node(info_node_type_std_carry);

        if (carry && carry->unk_030) {
            *actor_a = (actor*)retail::sub_4C3A70((u32*)carry->unk_030); // info_node: its core's actor
            *actor_b = the_actor;
        }
    }

    // the player and the actor behind its unk_467
    if (!*actor_a && !*actor_b && retail::sub_601C50((u32*)the_actor)) {
        if (void* held = the_actor->player_ifc()->unk_467()) {
            arch_base_vhandle target = *(arch_base_vhandle*)((u8*)held + 4);

            if (target.resolve() && target.resolve()->is_an_actor()) {
                *actor_a = (actor*)target.resolve();
                *actor_b = the_actor;
            }
        }
    }

    return *actor_a && *actor_b;
}

// sub_5B3B40
void als::sync_move_and_face::get_sync_bone_po(actor* actor_a, po* po_a, actor* actor_b, po* po_b) {
    bool found = sync_index_a != -2 && sync_index_b != -2;

    // found for this pair, or for it the other way round
    if (!found || sync_index_actor_a != actor_a || sync_index_actor_b != actor_b) {
        if (found && sync_index_actor_a == actor_b && sync_index_actor_b == actor_a) {
            i16 index_a = sync_index_a;

            sync_index_a       = sync_index_b;
            sync_index_b       = index_a;
            sync_index_actor_a = actor_a;
            sync_index_actor_b = actor_b;
        } else {
            sync_index_actor_a = actor_a;
            sync_index_actor_b = actor_b;

            compute_best_sync_bones(actor_a, actor_b, &sync_index_a, &sync_index_b);
        }
    }

    if (sync_index_a >= 0)
        retail::sub_62FC10((i32)actor_a, sync_index_a, (u64*)po_a, 0); // conglomerate::get_member_abs_po
    else
        retail::sub_583280((u64*)po_a, (u64*)actor_a->my_abs_po); // po copy

    if (sync_index_b >= 0)
        retail::sub_62FC10((i32)actor_b, sync_index_b, (u64*)po_b, 0);
    else
        retail::sub_583280((u64*)po_b, (u64*)actor_b->my_abs_po);
}

// sub_5C1290
void als::sync_move_and_face::apply_sync_rotate_angles(state_machine_interface* layer_a, po* po_a, state_machine_interface* layer_b, po* po_b) {
    if (layer_b) {
        if (i32 angle = layer_b->get_optional_pb_int(sync_rotate_angle_id, 0, nullptr))
            retail::sub_5B7400((f32*)po_a, (i32)&po_a->get_y_facing(), (f32)((f32)angle * 0.017453292519943295)); // po: turns about the axis
    }

    if (layer_a) {
        if (i32 angle = layer_a->get_optional_pb_int(sync_rotate_angle_id, 0, nullptr))
            retail::sub_5B7400((f32*)po_b, (i32)&po_b->get_y_facing(), (f32)((f32)angle * 0.017453292519943295));
    }
}

// sub_5C1350
void als::sync_move_and_face::reorient_to_vertical(actor* actor_a, state_machine_interface* layer_a, actor* actor_b, state_machine_interface* layer_b) {
    // turns the po about the axis from its up to the blended one, keeping its position
    auto reorient = [](actor* target, f32 speed) {
        po       abs_po   = *target->my_abs_po;
        vector3d position = abs_po.get_position();
        vector3d up       = abs_po.get_y_facing();
        vector3d blended  = up * (1.0f - speed) + treyarch::references::unk_00e6808c.get() * speed;

        retail::sub_402050((f32*)&blended);

        if (blended.length2() > 0.0001f) {
            f32 angle = math::acos(up * blended);

            if (angle > 0.01f || angle < -0.01f) {
                vector3d axis = cross(abs_po.get_y_facing(), blended);

                retail::sub_402050((f32*)&axis);

                // without an axis between them, about the facing
                if (axis.length2() < 0.01f)
                    axis = abs_po.get_z_facing();

                po rotation;
                retail::sub_4015B0((u64*)&rotation);

                angle = -angle;

                math::vector4 axis_dir(axis.x, axis.y, axis.z, 0.0f);
                retail::sub_40A750((u64*)&rotation, (f32*)&axis_dir, &angle);
                retail::sub_44E230((u64*)&abs_po, (f32*)&abs_po, (f32*)&rotation);
            }
        }

        retail::sub_424900((u64*)&abs_po, (u32*)&position); // po::set_position
        retail::sub_627180((i32)target, (f32*)&abs_po);     // entity_base::set_abs_po
    };

    // a's speed comes from b's layer, as retail has it
    if (actor_a && layer_a) {
        bool exists = false;
        f32  speed  = layer_b->get_optional_pb_float(reorient_to_vertical_speed_id, 1.0f, &exists);

        if (exists && speed < 0.9999f)
            reorient(actor_a, speed);
    }

    if (actor_b && layer_b) {
        bool exists = false;
        f32  speed  = layer_b->get_optional_pb_float(reorient_to_vertical_speed_id, 1.0f, &exists);

        if (exists && speed < 0.9999f)
            reorient(actor_b, speed);
    }
}

// sub_5B31B0
void als::sync_move_and_face::apply_axis_slides(actor*                   actor_a,
                                                state_machine_interface* layer_a,
                                                vector3d*                pos_a,
                                                po*                      po_a,
                                                actor*                   actor_b,
                                                state_machine_interface* layer_b,
                                                vector3d*                pos_b,
                                                po*                      po_b) {

    auto clamped = [](f32 value, f32 low, f32 high) {
        if (low > value)
            return low;

        if (value > high)
            return high;

        return value;
    };

    if (layer_b->get_optional_pb_int(sync_allow_axis_slide_id, 0, nullptr)) {
        if (rel_slide_a == treyarch::references::unk_00fd4370.get()) {
            vector3d offset = actor_b->my_abs_po->get_position() - actor_a->my_abs_po->get_position();

            rel_slide_a.x = po_a->get_x_facing() * offset;
            rel_slide_a.y = po_a->get_y_facing() * offset;
            rel_slide_a.z = po_a->get_z_facing() * offset;
        }

        vector3d slide_max = layer_b->get_optional_pb_vector3d(sync_rel_slide_max_id, treyarch::references::unk_00fd4370.get(), nullptr);
        vector3d slide_min = layer_b->get_optional_pb_vector3d(sync_rel_slide_min_id, treyarch::references::unk_00fd4370.get(), nullptr);

        *pos_a += po_a->get_x_facing() * clamped(rel_slide_a.x, slide_min.x, slide_max.x);
        *pos_a += po_a->get_y_facing() * clamped(rel_slide_a.y, slide_min.y, slide_max.y);
        *pos_a += po_a->get_z_facing() * clamped(rel_slide_a.z, slide_min.z, slide_max.z);
    }

    if (layer_a->get_optional_pb_int(sync_allow_axis_slide_id, 0, nullptr)) {
        if (rel_slide_b == treyarch::references::unk_00fd4370.get()) {
            vector3d offset = actor_a->my_abs_po->get_position() - actor_b->my_abs_po->get_position();

            rel_slide_b.x = po_b->get_x_facing() * offset;
            rel_slide_b.y = po_b->get_y_facing() * offset;
            rel_slide_b.z = po_b->get_z_facing() * offset;
        }

        vector3d slide_max = layer_a->get_optional_pb_vector3d(sync_rel_slide_max_id, treyarch::references::unk_00fd4370.get(), nullptr);
        vector3d slide_min = layer_a->get_optional_pb_vector3d(sync_rel_slide_min_id, treyarch::references::unk_00fd4370.get(), nullptr);

        *pos_b += po_b->get_x_facing() * clamped(rel_slide_b.x, slide_min.x, slide_max.x);
        *pos_b += po_b->get_y_facing() * clamped(rel_slide_b.y, slide_min.y, slide_max.y);
        *pos_b += po_b->get_z_facing() * clamped(rel_slide_b.z, slide_min.z, slide_max.z);
    }
}

// sub_5B38C0
void als::sync_move_and_face::compute_best_sync_bones(actor* actor_a, actor* actor_b, i16* index_a, i16* index_b) {
    i16 best_index_a = -2;
    i16 best_index_b = -2;
    f32 best_dist2   = 3.4028235e38f;

    for (i32 a = 0; a < 10; ++a) {
        vector3d pos_a;
        i16      bone_a = get_sync_bone_index_and_pos(sync_bone_ids[a], actor_a, &pos_a);

        if (a > 0 && bone_a == -1)
            break;

        for (i32 b = 0; b < 10; ++b) {
            if (best_index_b == -1)
                break;

            vector3d pos_b;
            i16      bone_b = get_sync_bone_index_and_pos(sync_bone_ids[b], actor_b, &pos_b);

            if (b > 0 && bone_b == -1)
                break;

            f32 dist2 = (pos_b - pos_a).length2();

            if (best_dist2 > dist2) {
                best_index_a = bone_a;
                best_index_b = bone_b;
                best_dist2   = dist2;
            }

            if (bone_b == -1)
                break;
        }

        if (bone_a == -1)
            break;
    }

    *index_a = best_index_a;
    *index_b = best_index_b;
}

// inlined @ sub_5B38C0
i16 als::sync_move_and_face::get_sync_bone_index_and_pos(const string_hash &bone, actor* the_actor, vector3d* pos) {
    if (the_actor->is_a_conglomerate()) {
        i16 index = retail::sub_62F880((u32*)the_actor, (u32*)&bone); // conglomerate::get_member_index

        if (index >= 0) {
            retail::sub_62FA60((i32)the_actor, index, (f32*)pos, 0); // conglomerate::get_member_abs_position

            return index;
        }
    }

    *pos = the_actor->my_abs_po->get_position();

    return -1;
}

// sub_5C1AE0
void als::sync_move_and_face::sync_entities(f32       delta_t,
                                            actor*    actor_a,
                                            po*       po_a,
                                            vector3d* pos_a,
                                            actor*    actor_b,
                                            po*       po_b,
                                            vector3d* pos_b,
                                            f32       pos_weight,
                                            f32       orient_weight,
                                            bool      lock_pos,
                                            bool      lock_orient) {

    vector3d position_b = *pos_b;

    if (!lock_pos && pos_weight > 0.0001f) {
        vector3d move     = (*pos_a - position_b) * pos_weight;
        vector3d moved_to = actor_b->my_abs_po->get_position() + move;

        retail::sub_6271F0((u32*)actor_b, (i32)&moved_to); // entity_base::set_abs_position

        actor_b->set_frame_delta_trans(move, delta_t);

        position_b += move;
    }

    if (!lock_orient && orient_weight > 0.0001f) {
        po target = *po_a;

        // the bones' rotations blended, a's whole from a weight of 1
        if (orient_weight < 0.0001f)
            target = *po_b;
        else if (orient_weight < 1.0f) {
            math::vector4 rotation;

            retail::sub_5AABF0((i32)&target, (u64*)&rotation); // po: its rotation, x first
            quaternion to(rotation[3], rotation[0], rotation[1], rotation[2]);

            retail::sub_5AABF0((i32)po_b, (u64*)&rotation);
            quaternion from(rotation[3], rotation[0], rotation[1], rotation[2]);

            quaternion blended;
            retail::sub_48E070((f32*)&blended, (f32*)&from, (f32*)&to, orient_weight); // slerp

            target.get_matrix().x.w = 0.0f;
            target.get_matrix().y.w = 0.0f;
            target.get_matrix().z.w = 0.0f;
            target.get_matrix().w.w = 1.0f;

            retail::sub_48DFC0((f32*)&blended, (u64*)&target); // quaternion::to_matrix
            retail::sub_9595B0((i32)&target);
        }

        po relative;
        retail::sub_4015B0((u64*)&relative);

        po inverse;
        retail::sub_48E2C0((f32*)po_b, (u64*)&inverse); // po inverse
        retail::sub_44E230((u64*)&relative, (f32*)&inverse, (f32*)&target);

        // b's po about its bone, turned with it
        po       moved        = *actor_b->my_abs_po;
        vector3d old_position = moved.get_position();
        vector3d about_bone   = old_position - position_b;

        moved.get_matrix().w = vector4d(about_bone.x, about_bone.y, about_bone.z, 1.0f);

        retail::sub_44E230((u64*)&moved, (f32*)&moved, (f32*)&relative);

        if (lock_pos)
            moved.get_matrix().w = vector4d(old_position.x, old_position.y, old_position.z, 1.0f);
        else {
            vector3d position = moved.get_position() + position_b;

            moved.get_matrix().w = vector4d(position.x, position.y, position.z, 1.0f);

            actor_b->set_frame_delta_trans(position - actor_b->my_abs_po->get_position(), delta_t);
        }

        retail::sub_627180((i32)actor_b, (f32*)&moved); // entity_base::set_abs_po
    }

    if (actor_b->has_ifc(entity_ifc_physical) && !(actor_b->physical_ifc()->flags & 0x800))
        actor_b->physical_ifc()->cancel_all_velocity(treyarch::references::unk_00fd4370.get(), true);
}

// sub_5C2120
void als::sync_move_and_face::sync_actors(actor* actor_a,
                                          bool   lock_orient_a,
                                          bool   lock_pos_a,
                                          actor* actor_b,
                                          bool   lock_orient_b,
                                          bool   lock_pos_b,
                                          f32    pos_tween,
                                          f32    orient_tween,
                                          f32    delta_t) {

    if (delta_t < 0.0f)
        delta_t = treyarch::references::g_world_ptr.read()->time_mgr.time_inc;

    po po_a;
    po po_b;
    retail::sub_4015B0((u64*)&po_a);
    retail::sub_4015B0((u64*)&po_b);

    i16 index_a;
    i16 index_b;
    compute_best_sync_bones(actor_a, actor_b, &index_a, &index_b);

    if (index_a >= 0)
        retail::sub_62FC10((i32)actor_a, index_a, (u64*)&po_a, 0); // conglomerate::get_member_abs_po
    else
        po_a = *actor_a->my_abs_po;

    if (index_b >= 0)
        retail::sub_62FC10((i32)actor_b, index_b, (u64*)&po_b, 0);
    else
        po_b = *actor_b->my_abs_po;

    // the bones' positions, apart from their orientations
    const vector3d &zero = treyarch::references::unk_00fd4370.get();

    vector3d pos_a = po_a.get_position();

    po_a.get_matrix().w = vector4d(zero.x, zero.y, zero.z, 1.0f);
    retail::sub_9595B0((i32)&po_a);

    vector3d pos_b = po_b.get_position();

    po_b.get_matrix().w = vector4d(zero.x, zero.y, zero.z, 1.0f);
    retail::sub_9595B0((i32)&po_b);

    if (!lock_pos_b || !lock_orient_b) {
        f32 pos_weight    = lock_pos_b    ? 0.0f : (lock_pos_a    ? pos_tween    : pos_tween * 0.5f);
        f32 orient_weight = lock_orient_b ? 0.0f : (lock_orient_a ? orient_tween : orient_tween * 0.5f);

        sync_entities(delta_t, actor_a, &po_a, &pos_a, actor_b, &po_b, &pos_b, pos_weight, orient_weight, lock_pos_b, lock_orient_b);
    }

    if (!lock_pos_a || !lock_orient_a) {
        f32 pos_weight    = lock_pos_a    ? 0.0f : (lock_pos_b    ? pos_tween    : pos_tween * 0.5f);
        f32 orient_weight = lock_orient_a ? 0.0f : (lock_orient_b ? orient_tween : orient_tween * 0.5f);

        sync_entities(delta_t, actor_b, &po_b, &pos_b, actor_a, &po_a, &pos_a, pos_weight, orient_weight, lock_pos_a, lock_orient_a);
    }
}

// sub_5AE9A0
als::momentum_sync_move_and_face::momentum_sync_move_and_face() : params_read(false),
                                                                  has_hit(false),
                                                                  stick_to_collision(false),
                                                                  no_reorient_to_vertical(false),
                                                                  apply_slide(false) {}

// sub_719650
als::momentum_sync_move_and_face::momentum_sync_move_and_face(mash::generating_vtable_version so_do_nothing) : sync_move_and_face(so_do_nothing),
                                                                                                               params_read(false),
                                                                                                               has_hit(false),
                                                                                                               stick_to_collision(false),
                                                                                                               no_reorient_to_vertical(false),
                                                                                                               apply_slide(false) {}

// sub_5AE9E0
mash::virtual_types_key als::momentum_sync_move_and_face::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5AE9F0
bool als::momentum_sync_move_and_face::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (sync_move_and_face::get_virtual_type_key() == parent_class)
        return true;

    if (sync_move_and_face::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AEAC0
i32 als::momentum_sync_move_and_face::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5DA940
void als::momentum_sync_move_and_face::activate(animation_logic_system* the_als_core, layer_id layer) {
    sync_move_and_face::activate(the_als_core, layer);

    read_params(0.033333335f);
}

// sub_5C2480
void als::momentum_sync_move_and_face::deactivate() {
    sync_move_and_face::deactivate();
}

// sub_5C2CA0
void als::momentum_sync_move_and_face::pre_anim_action(f32 delta_t) {
    if (the_als->did_transition_succeed())
        read_params(delta_t);

    params_read = false;
}

// sub_5EB9D0
void als::momentum_sync_move_and_face::post_anim_action(f32 delta_t) {
    read_hit(delta_t);
    update_slide_axes(delta_t);

    actor* actor_a = nullptr;
    actor* actor_b = nullptr;

    if (get_sync_actors(&actor_a, &actor_b)) {
        anim_time = the_base->get_current_anim_time();

        // turned in place, a for bit 1 and b for bit 2
        if (apply_turn) {
            po turned = get_turned_po(delta_t, actor_a);

            if (apply_turn & 1) {
                const vector3d &position = actor_a->my_abs_po->get_position();

                turned.get_matrix().w = vector4d(position.x, position.y, position.z, 1.0f);

                retail::sub_627180((i32)actor_a, (f32*)&turned); // entity_base::set_abs_po
            }

            if (apply_turn & 2) {
                const vector3d &position = actor_b->my_abs_po->get_position();

                turned.get_matrix().w = vector4d(position.x, position.y, position.z, 1.0f);

                retail::sub_627180((i32)actor_b, (f32*)&turned);
            }
        }

        if (stick_to_collision && has_hit) {
            po po_a = *actor_a->my_abs_po;
            po po_b = *actor_b->my_abs_po;

            // a locked side creeps onto the hit point at stick_speed_min
            if (lock_pos_a) {
                f32      max_step = stick_speed_min * delta_t;
                vector3d step     = hit_point - po_a.get_position();

                if ((f32)retail::sub_402020((f32*)&step) > max_step) {
                    retail::sub_402050((f32*)&step);

                    step *= max_step;
                }

                vector3d position = po_a.get_position() + step;

                retail::sub_424900((u64*)&po_a, (u32*)&position); // po::set_position
                retail::sub_627180((i32)actor_a, (f32*)&po_a);

                actor_a->set_frame_delta_trans(step, delta_t);
            }

            if (lock_pos_b) {
                f32      max_step = stick_speed_min * delta_t;
                vector3d step     = hit_point - po_b.get_position();

                if ((f32)retail::sub_402020((f32*)&step) > max_step) {
                    retail::sub_402050((f32*)&step);

                    step *= max_step;
                }

                vector3d position = po_b.get_position() + step;

                retail::sub_424900((u64*)&po_b, (u32*)&position);
                retail::sub_627180((i32)actor_b, (f32*)&po_b);

                actor_b->set_frame_delta_trans(step, delta_t);
            }
        } else if (apply_slide) {
            po po_a = *actor_a->my_abs_po;
            po po_b = *actor_b->my_abs_po;

            // both slide along the actor's facing
            up        = the_actor->my_abs_po->get_y_facing();
            slide_dir = the_actor->my_abs_po->get_z_facing();

            vector3d move = get_slide_move(delta_t);

            vector3d position_a = po_a.get_position() + move;
            vector3d position_b = po_b.get_position() + move;

            po_a.get_matrix().w = vector4d(position_a.x, position_a.y, position_a.z, 1.0f);
            po_b.get_matrix().w = vector4d(position_b.x, position_b.y, position_b.z, 1.0f);

            retail::sub_627180((i32)actor_a, (f32*)&po_a);
            actor_a->set_frame_delta_trans(move, delta_t);

            retail::sub_627180((i32)actor_b, (f32*)&po_b);
            actor_b->set_frame_delta_trans(move, delta_t);
        }
    }

    sync_move_and_face::post_anim_action(delta_t);
}

// sub_5B3D30
void als::momentum_sync_move_and_face::read_params(f32) {
    if (params_read)
        return;

    params_read = true;

    read_signal_times();

    up        = the_actor->my_abs_po->get_y_facing();
    slide_dir = the_actor->my_abs_po->get_z_facing();

    stick_speed_min = the_base->get_optional_pb_float(stick_speed_min_id, 20.0f, nullptr);
    slide_speed_min = the_base->get_optional_pb_float(slide_speed_min_id, 0.0f, nullptr);
    slide_speed_max = the_base->get_optional_pb_float(slide_speed_max_id, 0.0f, nullptr);
    turn_rate       = the_base->get_optional_pb_float(turn_rate_id, -1.0f, nullptr);

    if (turn_rate >= 0.0f)
        turn_rate = (f32)((f64)turn_rate * 0.017453292519943295);

    apply_slide             = the_base->get_optional_pb_int(apply_slide_id, 0, nullptr) != 0;
    apply_turn              = the_base->get_optional_pb_int(apply_turn_id, 0, nullptr);
    apply_turn_constant     = the_base->get_optional_pb_int(apply_turn_constant_id, 0, nullptr) != 0;
    preserve_y_facing       = the_base->get_optional_pb_int(preserve_y_facing_id, 0, nullptr) != 0;
    no_reorient_to_vertical = the_base->get_optional_pb_int(no_reorient_to_vertical_id, 0, nullptr) != 0;
    reorient_blend          = the_base->get_optional_pb_float(no_reorient_to_vertical_id, 1.0f, nullptr);
    stick_to_collision      = the_base->get_optional_pb_int(stick_to_collision_id, 0, nullptr) != 0;
    hit_height_min          = the_base->get_optional_pb_float(hit_height_min_id, 2.0f, nullptr);
    hit_norm_offset         = the_base->get_optional_pb_float(hit_norm_offset_id, 0.25f, nullptr);

    slide_speed = slide_speed_max;
}

// sub_5B3C00
void als::momentum_sync_move_and_face::read_signal_times() {
    // a missing begin is the start, a missing end the anim's end
    move_begin_time = the_base->get_time_to_signal(references::move_begin.read(), nullptr, false, nullptr);

    if (move_begin_time < 0.0f)
        move_begin_time = 0.0f;

    move_end_time = the_base->get_time_to_signal(references::move_end.read(), nullptr, false, nullptr);

    if (move_end_time < 0.0f)
        move_end_time = the_base->get_duration();

    rotate_begin_time = the_base->get_time_to_signal(references::rotate_begin.read(), nullptr, false, nullptr);

    if (rotate_begin_time < 0.0f)
        rotate_begin_time = 0.0f;

    rotate_end_time = the_base->get_time_to_signal(references::rotate_end.read(), nullptr, false, nullptr);

    if (rotate_end_time < 0.0f)
        rotate_end_time = the_base->get_duration();
}

// sub_5F0A00
void als::momentum_sync_move_and_face::read_hit(f32) {
    // param 120 over 0.0001 reports one: param 143 its normal, 145 its point, raised off the surface
    if (the_base->has_ext_param_been_set(120) && the_base->get_external_float(120) > 0.0001f) {
        has_hit    = true;
        hit_normal = the_base->get_external_vector(143);
        hit_point  = the_base->get_external_vector(145);
        hit_point += hit_normal * hit_norm_offset;

        // and up by hit_height_min when internal param 196 is under it
        if (hit_height_min > the_als->the_cache.get_internal_float(the_als, the_base, 196))
            hit_point += treyarch::references::unk_00e6808c.get() * hit_height_min;
    }
}

// sub_5DA970
void als::momentum_sync_move_and_face::update_slide_axes(f32) {
    if (!preserve_y_facing && !no_reorient_to_vertical) {
        if (reorient_blend > 0.9999f)
            up = treyarch::references::unk_00e6808c.get();
        else
            up = up * (1.0f - reorient_blend) + treyarch::references::unk_00e6808c.get() * reorient_blend;
    }

    // into the wall when stuck to a hit, else param 143 when set
    if (stick_to_collision && has_hit)
        slide_dir = -hit_normal;
    else if (the_base->has_ext_param_been_set(143))
        slide_dir = the_base->get_external_vector(143);

    slide_dir = slide_dir - up * (f32)(up * slide_dir);
}

// sub_5C2490
po als::momentum_sync_move_and_face::get_turned_po(f32 delta_t, actor* target) {
    using get_turned_po_t = po* (__thiscall*)(momentum_sync_move_and_face*, po*, f32, actor*);

    po turned;

    ((get_turned_po_t)retail::sub_5C2490)(this, &turned, delta_t, target);

    return turned;
}

// sub_5B4060
vector3d als::momentum_sync_move_and_face::get_slide_move(f32 delta_t) {
    f32 span     = move_end_time - move_begin_time;
    f32 progress = 0.0f;

    if (span < 0.0001f) {
        if (anim_time > move_begin_time)
            progress = 1.0f;
    } else {
        progress = (f32)(((f64)move_end_time - (f64)anim_time) / (f64)span);

        if (0.0f > progress)
            progress = 0.0f;
        else if (progress > 1.0f)
            progress = 1.0f;
    }

    slide_speed = (f32)(((f64)slide_speed_max - (f64)slide_speed_min) * (f64)progress + (f64)slide_speed_min);

    vector3d velocity = slide_dir * slide_speed;

    // still outside the move signals
    if (move_begin_time > anim_time || anim_time > move_end_time)
        velocity = treyarch::references::unk_00fd4370.get();

    return velocity * delta_t;
}
