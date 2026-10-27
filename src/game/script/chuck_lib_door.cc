#include "retail.hh"
#include "game/script/chuck_lib_door.hh"

using namespace treyarch;

bool slf__door_animation_completed__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85C7C0)(this, stack);
}

bool slf__lock_door__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85C820)(this, stack);
}

bool slf__force_door_opened__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85C880)(this, stack);
}

bool slf__force_door_closed__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85C8D0)(this, stack);
}

bool slf__is_door_allow_any_char__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85C920)(this, stack);
}

bool slf__set_door_allow_any_char__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85C9C0)(this, stack);
}

bool slf__is_door_ignore_hero__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85CA20)(this, stack);
}

bool slf__set_door_ignore_hero__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85CAC0)(this, stack);
}

bool slf__door_contains_hero__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85CB20)(this, stack);
}

bool slf__is_door_locked__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85CBB0)(this, stack);
}

bool slf__make_door_always_camera_collide__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85CC40)(this, stack);
}
