#include "retail.hh"
#include "treyarch/game/script/chuck_lib_switch.hh"

using namespace treyarch;

bool slf__entity__create_switch_obj_interface__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8721A0)(this, stack);
}

bool slf__entity__switch_set_no_use__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8721E0)(this, stack);
}

bool slf__entity__switch_set_usable__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_872220)(this, stack);
}

bool slf__entity__switch_get_state__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_872260)(this, stack);
}

bool slf__entity__switch_set_enabled__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8722D0)(this, stack);
}

bool slf__entity__switch_set_state__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_872330)(this, stack);
}

bool slf__entity__switch_set_web_targetable__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_872390)(this, stack);
}

bool slf__entity__switch_set_web_target_only__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8723F0)(this, stack);
}

bool slf__entity__switch_set_alarm__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_872450)(this, stack);
}

bool slf__entity__switch_set_single_use__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8724B0)(this, stack);
}

bool slf__entity__switch_set_ignore_hero__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_872510)(this, stack);
}

bool slf__entity__switch_set_flick_animation__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_872570)(this, stack);
}

bool slf__entity__switch_set_radius_mod__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8725D0)(this, stack);
}
