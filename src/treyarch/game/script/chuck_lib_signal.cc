#include "retail.hh"
#include "treyarch/game/script/chuck_lib_signal.hh"

using namespace treyarch;

bool slf__wait__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_856500)(this, stack);
}

bool slf__create_beam__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E0F0)(this, stack);
}

bool slf__wait__arch_base__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_856600)(this, stack);
}

bool slf__wait_timeout__arch_base__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_856710)(this, stack);
}

bool slf__wait_a_frame__arch_base__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_856890)(this, stack);
}

bool slf__arch_base__clear_callbacks__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_856470)(this, stack);
}

bool slf__arch_base__clear_callback__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_856490)(this, stack);
}

bool slf__arch_base__clear_callback__uint__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8564D0)(this, stack);
}
