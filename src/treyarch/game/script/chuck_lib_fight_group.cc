#include "retail.hh"
#include "treyarch/game/script/chuck_lib_fight_group.hh"

using namespace treyarch;

bool slf__activate_fight_group__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_878240)(this, stack);
}

bool slf__fight_group__pause_fight_group__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8782F0)(this, stack);
}

bool slf__fight_group__unpause_fight_group__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_878330)(this, stack);
}

bool slf__fight_group__release_fight_group__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_878370)(this, stack);
}

bool slf__fight_group__add_actor_to_fight_group__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8783B0)(this, stack);
}

bool slf__fight_group__set_actor_group_flags__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_878410)(this, stack);
}

bool slf__fight_group__clear_actor_group_flags__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_878470)(this, stack);
}

bool slf__fight_group__get_group_count__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8784D0)(this, stack);
}

bool slf__fight_group__rvb_set_flag__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_883770)(this, stack);
}

bool slf__fight_group__rvb_clear_flag__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8837D0)(this, stack);
}

bool slf__fight_group__rvb_get_flag__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_883830)(this, stack);
}

bool slf__fight_group__rvb_set_var__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_878570)(this, stack);
}

bool slf__fight_group__rvb_get_var__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8785D0)(this, stack);
}

bool slf__fight_group__rvb_set_target__num__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_878670)(this, stack);
}

bool slf__fight_group__rvb_set_target__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8786D0)(this, stack);
}

bool slf__fight_group__rvb_has_target__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_878730)(this, stack);
}

bool slf__fight_group__rvb_get_target_entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8787B0)(this, stack);
}

bool slf__fight_group__rvb_get_target_vector__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8788C0)(this, stack);
}

bool slf__fight_group__rvb_clear_target__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_878990)(this, stack);
}
