#include "retail.hh"
#include "game/script/chuck_lib_ai.hh"

using namespace treyarch;

bool slf__get_camera_y_facing__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83F9A0)(this, stack);
}

bool slf__get_camera_z_facing__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_842000)(this, stack);
}

bool slf__create_generic_event_callback__arch_base__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867E00)(this, stack);
}

bool slf__get_best_surface_pos__vector3d__num__line_info__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867FB0)(this, stack);
}

bool slf__random_attack_set___frame_advance__random_attack_set__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_868E80)(this, stack);
}

bool slf__random_attack_set___frame_advance_array__dynamic_array__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_868EA0)(this, stack);
}

bool slf__random_attack_set___request_attack_index__random_attack_set__dynamic_array__dynamic_array__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_868F40)(this, stack);
}

bool slf__entity__enable_tether__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_866A90)(this, stack);
}

bool slf__entity__disable_tether__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_866AE0)(this, stack);
}

bool slf__entity__get_tether_center__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_866B20)(this, stack);
}

bool slf__entity__get_tether_radius__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_866C00)(this, stack);
}

bool slf__entity__is_tether_active__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_866C70)(this, stack);
}

bool slf__cpu_combat_movement__get_tether_center__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_866CE0)(this, stack);
}

bool slf__cpu_combat_movement__get_tether_radius__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_866D50)(this, stack);
}

bool slf__cpu_combat_movement__is_tether_active__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_866DA0)(this, stack);
}

bool slf__cpu_combat_movement__clip_to_tether__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_866E00)(this, stack);
}

bool slf__cpu_combat_movement__get_pie_target__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_866E70)(this, stack);
}

bool slf__cpu_combat_movement__get_pie_center__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_866EC0)(this, stack);
}

bool slf__cpu_combat_movement__get_pie_normal__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_866F30)(this, stack);
}

bool slf__cpu_combat_movement__get_pie_facing__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_866FA0)(this, stack);
}

bool slf__cpu_combat_movement__get_new_pie_pos__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867010)(this, stack);
}

bool slf__cpu_combat_movement__pos_inside_pie_slice__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867090)(this, stack);
}

bool slf__cpu_combat_movement__inside_pie_slice__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867100)(this, stack);
}

bool slf__cpu_combat_movement__get_pie_min_dist__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8671A0)(this, stack);
}

bool slf__cpu_combat_movement__get_pie_max_dist__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8671F0)(this, stack);
}

bool slf__cpu_combat_movement__get_pie_ring__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867240)(this, stack);
}

bool slf__cpu_combat_movement__get_pie_num_rings__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867290)(this, stack);
}

bool slf__cpu_combat_movement__get_pie_slice__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8672F0)(this, stack);
}

bool slf__cpu_combat_movement__get_pie_num_slices__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867340)(this, stack);
}

bool slf__cpu_combat_movement__get_pie_buffer_arc_pct__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8673A0)(this, stack);
}

bool slf__cpu_combat_movement__is_moving__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8673F0)(this, stack);
}

bool slf__cpu_combat_movement__get_movement_dest__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867430)(this, stack);
}

bool slf__cpu_combat_movement__strafe_to_position__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8674A0)(this, stack);
}

bool slf__cpu_combat_movement__goto_position__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8675B0)(this, stack);
}

bool slf__cpu_combat_movement__stop_movement__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8676C0)(this, stack);
}

bool slf__cpu_combat_movement__set_facing_point__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8676E0)(this, stack);
}

bool slf__cpu_combat_movement__set_facing_pie_center__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867710)(this, stack);
}

bool slf__cpu_combat_movement__rappel_allowed__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867750)(this, stack);
}

bool slf__cpu_combat_movement__rappel_is_firing__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8677E0)(this, stack);
}

bool slf__cpu_combat_movement__rappel_is_engaged__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867880)(this, stack);
}

bool slf__cpu_combat_movement__rappel_fire__vector3d__entity__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867920)(this, stack);
}

bool slf__cpu_combat_movement__rappel_attach__vector3d__entity__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867A20)(this, stack);
}

bool slf__cpu_combat_movement__rappel_disengage__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867AF0)(this, stack);
}

bool slf__cpu_combat_movement__rappel_set_speed__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867B50)(this, stack);
}

bool slf__cpu_combat_movement__rappel_set_rope_length__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867BA0)(this, stack);
}

bool slf__cpu_combat_movement__rappel_get_constraint__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867C00)(this, stack);
}

bool slf__cpu_combat_movement__rappel_get_speed__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867CB0)(this, stack);
}

bool slf__cpu_combat_movement__rappel_set_rope_pivot__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867D40)(this, stack);
}

bool slf__generic_event_callback__add__uint__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867E90)(this, stack);
}

bool slf__generic_event_callback__remove__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867EE0)(this, stack);
}

bool slf__generic_event_callback__clear__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867F30)(this, stack);
}

bool slf__generic_event_callback__raised__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867F50)(this, stack);
}

bool slf__entity__ai_puppet_lock_access__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_868030)(this, stack);
}

bool slf__entity__ai_puppet_unlock_access__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8680A0)(this, stack);
}

bool slf__entity__ai_set_mutation_scale__str__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_868110)(this, stack);
}

bool slf__entity__ai_set_mutation_scale_anim__str__vector3d__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_868280)(this, stack);
}

bool slf__entity__ai_stop_mutation_scale_anim__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8683F0)(this, stack);
}

bool slf__entity__ai_get_mutation_scale__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_868490)(this, stack);
}

bool slf__entity__ai_set_mutation_trans__str__vector3d__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_868580)(this, stack);
}

bool slf__entity__ai_set_mutation_trans_anim__str__vector3d__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_868720)(this, stack);
}

bool slf__entity__ai_stop_mutation_trans_anim__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8688A0)(this, stack);
}

bool slf__entity__ai_get_mutation_current_trans__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_868940)(this, stack);
}

bool slf__entity__ai_set_mutation_trans_abs_position__str__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_868A30)(this, stack);
}

bool slf__entity__ai_is_mutation_trans_affect_children__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_868AF0)(this, stack);
}

bool slf__entity__push_ai_disable__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_868BB0)(this, stack);
}

bool slf__entity__pop_ai_disable__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_868C10)(this, stack);
}
