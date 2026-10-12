#include "retail.hh"
#include "treyarch/game/script/chuck_lib_mission_objective.hh"

using namespace treyarch;

bool slf__create_stage_data__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8773A0)(this, stack);
}

bool slf__has_stage_data__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8773E0)(this, stack);
}

bool slf__retrieve_stage_data__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877420)(this, stack);
}

bool slf__get_mission_script_retry_count__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_842540)(this, stack);
}

bool slf__has_global_objective_data__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877480)(this, stack);
}

bool slf__create_global_objective_data__num__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8774C0)(this, stack);
}

bool slf__retrieve_global_objective_data__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877560)(this, stack);
}

bool slf__set_mission_name__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8775A0)(this, stack);
}

bool slf__set_mission_name_debug__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877650)(this, stack);
}

bool slf__set_mission_description__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8776D0)(this, stack);
}

bool slf__set_mission_description_debug__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877780)(this, stack);
}

bool slf__set_mission_success_string__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877800)(this, stack);
}

bool slf__set_mission_success_string_debug__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8778B0)(this, stack);
}

bool slf__set_mission_failure_string__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877930)(this, stack);
}

bool slf__set_mission_failure_string_debug__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8779E0)(this, stack);
}

bool slf__register_mission_objective__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877A60)(this, stack);
}

bool slf__register_mission_objective__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877AF0)(this, stack);
}

bool slf__unregister_mission_objective__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877BC0)(this, stack);
}

bool slf__mission_objective_is_registered__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877C40)(this, stack);
}

bool slf__mission_objective_actual__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877CC0)(this, stack);
}

bool slf__mission_objective_is_complete__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877D40)(this, stack);
}

bool slf__set_mission_objective_actual__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877DC0)(this, stack);
}

bool slf__set_mission_objective_complete__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877E50)(this, stack);
}

bool slf__set_mission_objective_complete__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877F00)(this, stack);
}

bool slf__mission_objective_data__create_child_data__num__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876970)(this, stack);
}

bool slf__mission_objective_data__retrieve_child_data__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877070)(this, stack);
}

bool slf__mission_objective_data__restart__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876A30)(this, stack);
}

bool slf__mission_objective_data__is_valid__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8770D0)(this, stack);
}

bool slf__mission_objective_data__is_failed__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877110)(this, stack);
}

bool slf__mission_objective_data__is_required__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876A60)(this, stack);
}

bool slf__mission_objective_data__is_activated__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877170)(this, stack);
}

bool slf__mission_objective_data__is_completed__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8771D0)(this, stack);
}

bool slf__mission_objective_data__is_autoactivated__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876AC0)(this, stack);
}

bool slf__mission_objective_data__is_autocompleted__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876B20)(this, stack);
}

bool slf__mission_objective_data__has_child_data__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876B80)(this, stack);
}

bool slf__mission_objective_data__get_id__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876BF0)(this, stack);
}

bool slf__mission_objective_data__get_actual_score__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876C50)(this, stack);
}

bool slf__mission_objective_data__get_target_score__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876CA0)(this, stack);
}

bool slf__mission_objective_data__set_failed__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877340)(this, stack);
}

bool slf__mission_objective_data__set_activated__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876CF0)(this, stack);
}

bool slf__mission_objective_data__set_completed__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876D20)(this, stack);
}

bool slf__mission_objective_data__set_required__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876D50)(this, stack);
}

bool slf__mission_objective_data__set_displayable__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876DA0)(this, stack);
}

bool slf__mission_objective_data__set_autoactivated__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876DF0)(this, stack);
}

bool slf__mission_objective_data__set_autocompleted__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876E40)(this, stack);
}

bool slf__mission_objective_data__set_description__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876E90)(this, stack);
}

bool slf__mission_objective_data__set_short_description__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876EC0)(this, stack);
}

bool slf__mission_objective_data__set_actual_score__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876EF0)(this, stack);
}

bool slf__mission_objective_data__init_tracking_scores__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876F20)(this, stack);
}

bool slf__mission_objective_data__set_awarded_points__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876F60)(this, stack);
}

bool slf__mission_stage_data__create_objective_data__num__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876FB0)(this, stack);
}

bool slf__mission_stage_data__retrieve_objective_data__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877070)(this, stack);
}

bool slf__mission_stage_data__restart__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876A30)(this, stack);
}

bool slf__mission_stage_data__is_valid__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8770D0)(this, stack);
}

bool slf__mission_stage_data__is_failed__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877110)(this, stack);
}

bool slf__mission_stage_data__is_activated__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877170)(this, stack);
}

bool slf__mission_stage_data__is_completed__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8771D0)(this, stack);
}

bool slf__mission_stage_data__has_objective_data__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876B80)(this, stack);
}

bool slf__mission_stage_data__get_id__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876BF0)(this, stack);
}

bool slf__mission_stage_data__get_restart_position__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877230)(this, stack);
}

bool slf__mission_stage_data__get_facing_direction__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8772A0)(this, stack);
}

bool slf__mission_stage_data__set_restart_data__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877310)(this, stack);
}

bool slf__mission_stage_data__set_failed__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877340)(this, stack);
}

bool slf__mission_stage_data__set_activated__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877360)(this, stack);
}

bool slf__mission_stage_data__set_completed__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877380)(this, stack);
}
