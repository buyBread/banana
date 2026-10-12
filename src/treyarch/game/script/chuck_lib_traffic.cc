#include "retail.hh"
#include "treyarch/game/script/chuck_lib_traffic.hh"

using namespace treyarch;

bool slf__enable_traffic__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8696B0)(this, stack);
}

bool slf__is_traffic_enabled__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83FAD0)(this, stack);
}

bool slf__vehicle_nogo_zone__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869710)(this, stack);
}

bool slf__kill_vehicle_nogo_zone__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824B60)(this, stack);
}

bool slf__find_crossfire_attackline__vector3d__vector3d__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8819A0)(this, stack);
}

bool slf__ai_vehicle_get_trickline__entity__dynamic_array__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_881DB0)(this, stack);
}

bool slf__find_particular_lane__vector3d__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869740)(this, stack);
}

bool slf__find_spot_on_road__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8697A0)(this, stack);
}

bool slf__find_particular_intersection__vector3d__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869840)(this, stack);
}

bool slf__find_nearest_point_on_road_middle__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8698F0)(this, stack);
}

bool slf__find_nearest_point_on_road__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8699A0)(this, stack);
}

bool slf__find_aircraft_attackline__vector3d__num__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_881F40)(this, stack);
}

bool slf__find_road_intersections__vector3d__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_882570)(this, stack);
}

bool slf__find_new_trickline__vector3d__vector3d__num__num__dynamic_array__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_882640)(this, stack);
}

bool slf__create_vehicle__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869A40)(this, stack);
}

bool slf__place_vehicle__entity__vector3d__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869AE0)(this, stack);
}

bool slf__place_vehicle_for_goto__entity__vector3d__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869BA0)(this, stack);
}

bool slf__find_vehicle_spawn_point__vector3d__vector3d__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869C20)(this, stack);
}

bool slf__set_traffic_density__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869D40)(this, stack);
}

bool slf__enforce_traffic_density__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824B70)(this, stack);
}

bool slf__enforce_traffic_density_offscreen__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869D90)(this, stack);
}

bool slf__get_traffic_density__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83FB00)(this, stack);
}

bool slf__get_num_traffic_vehicles__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83FB40)(this, stack);
}

bool slf__clear_traffic__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824B80)(this, stack);
}

bool slf__force_clear_traffic__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824B90)(this, stack);
}

bool slf__clear_traffic__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869DD0)(this, stack);
}

bool slf__force_clear_traffic__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869E10)(this, stack);
}

bool slf__get_vehicle_models__str__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622F0)(this, stack);
}

bool slf__set_vehicle_script_ownership__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869E50)(this, stack);
}

bool slf__spawn_traffic_minigame__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_854A30)(this, stack);
}

bool slf__spawn_traffic_minigame_on_vehicle__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}
