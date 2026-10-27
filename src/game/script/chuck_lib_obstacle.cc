#include "retail.hh"
#include "game/script/chuck_lib_obstacle.hh"

using namespace treyarch;

bool slf__create_stored_entity__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_878E10)(this, stack);
}

bool slf__create_stored_entity__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_878FC0)(this, stack);
}

bool slf__create_stored_preregistered_entity__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_879100)(this, stack);
}

bool slf__create_stored_preregistered_entity__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8792B0)(this, stack);
}

bool slf__create_driver__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8793F0)(this, stack);
}

bool slf__destroy_stored_entity__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_879580)(this, stack);
}

bool slf__convert_stored_entity_to_type__entity__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8795B0)(this, stack);
}

bool slf__convert_stored_entity_to_types__entity__dynamic_array__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860000)(this, stack);
}

bool slf__create_obstacle__vector3d__vector3d__vector3d__vector3d__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_879610)(this, stack);
}

bool slf__destroy_obstacle__obstacle__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_879690)(this, stack);
}
