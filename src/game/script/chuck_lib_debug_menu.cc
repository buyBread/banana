#include "retail.hh"
#include "game/script/chuck_lib_debug_menu.hh"

using namespace treyarch;

bool slf__debug_menu__set_name__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622F0)(this, stack);
}

bool slf__debug_menu__get_name__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8636F0)(this, stack);
}

bool slf__debug_menu__set_sort_mode__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622F0)(this, stack);
}

bool slf__debug_menu__destroy_contents__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__debug_menu__create_submenu__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_863680)(this, stack);
}

bool slf__debug_menu__destroy_submenu__debug_menu__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622F0)(this, stack);
}

bool slf__debug_menu__create_entry__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_863680)(this, stack);
}

bool slf__debug_menu__create_entry__str__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8636A0)(this, stack);
}

bool slf__debug_menu__destroy_entry__debug_menu_entry__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622F0)(this, stack);
}

bool slf__debug_menu__is_valid__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8636C0)(this, stack);
}

bool slf__debug_menu_entry__set_name__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622F0)(this, stack);
}

bool slf__debug_menu_entry__get_id__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_863730)(this, stack);
}

bool slf__debug_menu_entry__get_name__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8636F0)(this, stack);
}

bool slf__debug_menu_entry__get_parent_menu__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_863710)(this, stack);
}

bool slf__debug_menu_entry__set_handler__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622F0)(this, stack);
}

bool slf__debug_menu_entry__get_value__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_863730)(this, stack);
}

bool slf__debug_menu_entry__set_id__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622F0)(this, stack);
}

bool slf__debug_menu_entry__get_handler__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8636F0)(this, stack);
}

bool slf__debug_menu_entry__set_value_type__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622F0)(this, stack);
}

bool slf__debug_menu_entry__get_min_value__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_863730)(this, stack);
}

bool slf__debug_menu_entry__set_value__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622F0)(this, stack);
}

bool slf__debug_menu_entry__get_value_type__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_863730)(this, stack);
}

bool slf__debug_menu_entry__set_max_value__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622F0)(this, stack);
}

bool slf__debug_menu_entry__get_step_size__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_863730)(this, stack);
}

bool slf__debug_menu_entry__set_min_value__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622F0)(this, stack);
}

bool slf__debug_menu_entry__get_max_value__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_863730)(this, stack);
}

bool slf__debug_menu_entry__set_step_scale__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622F0)(this, stack);
}

bool slf__debug_menu_entry__is_valid__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8636C0)(this, stack);
}

bool slf__debug_menu_entry__set_step_size__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622F0)(this, stack);
}

bool slf__debug_menu_entry__get_step_scale__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_863730)(this, stack);
}
