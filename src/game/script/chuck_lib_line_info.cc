#include "retail.hh"
#include "game/script/chuck_lib_line_info.hh"

using namespace treyarch;

bool slf__create_line_info__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85AB30)(this, stack);
}

bool slf__destroy_line_info__line_info__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85ABF0)(this, stack);
}

bool slf__line_info__check_collision__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85AC40)(this, stack);
}

bool slf__line_info__check_sphere_collision__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85AEE0)(this, stack);
}

bool slf__line_info__clear_collision__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85B0A0)(this, stack);
}

bool slf__line_info__set_start_pos__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85B0C0)(this, stack);
}

bool slf__line_info__get_start_pos__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85B0F0)(this, stack);
}

bool slf__line_info__set_end_pos__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85B140)(this, stack);
}

bool slf__line_info__get_end_pos__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85B170)(this, stack);
}

bool slf__line_info__get_hit_pos__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85B1C0)(this, stack);
}

bool slf__line_info__get_hit_normal__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85B210)(this, stack);
}

bool slf__line_info__get_hit_entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85B260)(this, stack);
}

bool slf__line_info__did_collide__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85B2D0)(this, stack);
}

bool slf__line_info__debug_render__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622F0)(this, stack);
}
