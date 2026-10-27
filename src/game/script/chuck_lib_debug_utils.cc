#include "retail.hh"
#include "game/script/chuck_lib_debug_utils.hh"

using namespace treyarch;

bool slf__debug_show_triggers__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622F0)(this, stack);
}

bool slf__assert__num__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622F0)(this, stack);
}

bool slf__debug_show_trigger__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622F0)(this, stack);
}

bool slf__copy_to_clipboard__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__add_debug_sphere__vector3d__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_854A40)(this, stack);
}

bool slf__post_message__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622F0)(this, stack);
}

bool slf__DEBUG_to_string__uint__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8636F0)(this, stack);
}

bool slf__add_debug_sphere__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_854A30)(this, stack);
}

bool slf__add_debug_line__vector3d__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_854A50)(this, stack);
}

bool slf__add_debug_line__vector3d__vector3d__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_854A60)(this, stack);
}

bool slf__add_3d_debug_str__vector3d__vector3d__num__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_854A70)(this, stack);
}

bool slf__add_to_console__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__console_exec__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__add_recordable_line__vector3d__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_854A50)(this, stack);
}

bool slf__add_2d_debug_str__vector3d__vector3d__num__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_854A70)(this, stack);
}

bool slf__add_recordable_sphere__vector3d__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_854A40)(this, stack);
}

bool slf__add_2d_recordable_str__vector3d__vector3d__num__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_854A70)(this, stack);
}

bool slf__add_3d_recordable_str__vector3d__vector3d__num__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_854A70)(this, stack);
}
