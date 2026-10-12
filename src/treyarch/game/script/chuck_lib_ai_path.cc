#include "retail.hh"
#include "treyarch/game/script/chuck_lib_ai_path.hh"

using namespace treyarch;

bool slf__find_closest_point_on_a_path_to_point__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_873500)(this, stack);
}

bool slf__can_path_between_points__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_873560)(this, stack);
}

bool slf__find_attach_normal__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8735B0)(this, stack);
}

bool slf__create_ai_path__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841ED0)(this, stack);
}

bool slf__destroy_ai_path__ai_path__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_873640)(this, stack);
}

bool slf__ai_path__has_more_points__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_873670)(this, stack);
}

bool slf__ai_path__find_path_from_points__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8736D0)(this, stack);
}

bool slf__ai_path__get_next_point__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_873750)(this, stack);
}
