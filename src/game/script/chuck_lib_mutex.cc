#include "retail.hh"
#include "game/script/chuck_lib_mutex.hh"

using namespace treyarch;

bool slf__create_mutex__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8424C0)(this, stack);
}

bool slf__destroy_mutex__mutex__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876840)(this, stack);
}

bool slf__get_global_data_mutex__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_842520)(this, stack);
}

bool slf__is_cut_scene_playing__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_842480)(this, stack);
}

bool slf__mutex__lock__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876880)(this, stack);
}

bool slf__mutex__unlock__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8768A0)(this, stack);
}

bool slf__mutex__lock_required__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}
