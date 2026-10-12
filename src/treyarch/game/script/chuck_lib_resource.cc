#include "retail.hh"
#include "treyarch/game/script/chuck_lib_resource.hh"

using namespace treyarch;

bool slf__push_pack__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87D860)(this, stack);
}

bool slf__pop_pack__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87DA50)(this, stack);
}

bool slf__is_pack_pushed__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87C8D0)(this, stack);
}

bool slf__is_pack_ready__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_866540)(this, stack);
}

bool slf__get_pack_size__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8665F0)(this, stack);
}

bool slf__get_available_stack_size__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_866660)(this, stack);
}

bool slf__unlock_district__uint__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_866700)(this, stack);
}

bool slf__lock_district__uint__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_866740)(this, stack);
}

bool slf__force_region__uint__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_866780)(this, stack);
}

bool slf__unforce_region__uint__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8667C0)(this, stack);
}

bool slf__unforce_all_regions__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824A00)(this, stack);
}

bool slf__is_district_loaded__uint__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_866800)(this, stack);
}

bool slf__is_any_act_pack_ready__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83F810)(this, stack);
}

bool slf__wait_for_streamer_to_reach_equilibrium__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83F850)(this, stack);
}

bool slf__purge_district__uint__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_866880)(this, stack);
}

bool slf__swap_hero_costume__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8668C0)(this, stack);
}

bool slf__rvb_is_same_surface_as_hero__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_866950)(this, stack);
}

bool slf__rvb_is_same_surface__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8669B0)(this, stack);
}

bool slf__get_point_on_same_navmesh_as_hero__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83F940)(this, stack);
}

bool slf__add_static_hospital__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_866A10)(this, stack);
}

bool slf__remove_static_hospital__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_866A30)(this, stack);
}

bool slf__get_closest_hospital__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_866A50)(this, stack);
}

bool slf__get_camera_pos__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_842040)(this, stack);
}

bool slf__get_camera_x_facing__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841FC0)(this, stack);
}
