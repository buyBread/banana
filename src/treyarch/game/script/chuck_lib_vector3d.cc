#include "retail.hh"
#include "treyarch/game/script/chuck_lib_vector3d.hh"

using namespace treyarch;

bool slf__dot__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85A230)(this, stack);
}

bool slf__cross__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85A260)(this, stack);
}

bool slf__normal__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85A310)(this, stack);
}

bool slf__distance3d__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85A3B0)(this, stack);
}

bool slf__distance3d_sqr__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85A460)(this, stack);
}

bool slf__angle_between__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85A510)(this, stack);
}

bool slf__rotate_vector__vector3d__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85A540)(this, stack);
}

bool slf__rotate_vector__vector3d__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85A720)(this, stack);
}

bool slf__reflect_vector__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85A8E0)(this, stack);
}

bool slf__closest_point_segment__vector3d__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85AAA0)(this, stack);
}

bool slf__vector3d__vector3d__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860000)(this, stack);
}

bool slf__vector3d__operator_plus__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_859C90)(this, stack);
}

bool slf__vector3d__operator_minus__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_859D10)(this, stack);
}

bool slf__vector3d__operator_multiply__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_859D90)(this, stack);
}

bool slf__vector3d__operator_divide__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_859E10)(this, stack);
}

bool slf__vector3d__operator_equals_equals__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_859E90)(this, stack);
}

bool slf__vector3d__operator_not_equals__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_859F20)(this, stack);
}

bool slf__vector3d__length__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_859FB0)(this, stack);
}

bool slf__vector3d__length2__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85A020)(this, stack);
}

bool slf__vector3d__xz_length2__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85A090)(this, stack);
}

bool slf__vector3d__xy_norm__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85A0E0)(this, stack);
}

bool slf__vector3d__xz_norm__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85A140)(this, stack);
}

bool slf__vector3d__x__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85A1A0)(this, stack);
}

bool slf__vector3d__y__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85A1D0)(this, stack);
}

bool slf__vector3d__z__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85A200)(this, stack);
}
