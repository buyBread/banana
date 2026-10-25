#include <cmath>
#include <new>

#include "retail.hh"
#include "treyarch/game/als/animation_logic_system.hh"
#include "treyarch/game/als/mocomp.hh"
#include "treyarch/game/als/state_machine.hh"
#include "treyarch/game/animation/animation_controller.hh"
#include "treyarch/game/wds/entity/actor.hh"
#include "treyarch/game/wds/entity/interface/physical_interface.hh"
#include "treyarch/game/wds/entity/interface/player_interface.hh"
#include "treyarch/game/wds/entity/interface/standing_interface.hh"
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
bool als::motion_compensator::unk_015() const {
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
// inlined @ sub_5C5BE0
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
