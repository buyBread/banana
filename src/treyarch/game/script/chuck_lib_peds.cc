#include "retail.hh"
#include "treyarch/game/script/chuck_lib_peds.hh"

using namespace treyarch;

bool slf__zombie_set_zombies_enabled__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85B330)(this, stack);
}

bool slf__zombie_suspend__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85B370)(this, stack);
}

bool slf__peds_set_peds_enabled__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85B3B0)(this, stack);
}

bool slf__peds_get_peds_enabled__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83EB80)(this, stack);
}

bool slf__peds_set_max_peds__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85B3F0)(this, stack);
}

bool slf__peds_get_max_peds__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83EBB0)(this, stack);
}

bool slf__peds_set_ped_spawn_radius__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85B410)(this, stack);
}

bool slf__peds_set_ped_collide_radius__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85B430)(this, stack);
}

bool slf__peds_set_ped_animate_radius__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85B450)(this, stack);
}

bool slf__peds_set_ped_anim_move_radius__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85B470)(this, stack);
}

bool slf__peds_set_fake_ped_front_texture__num__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85B490)(this, stack);
}

bool slf__peds_set_fake_ped_top_texture__num__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85B580)(this, stack);
}

bool slf__peds_set_fake_ped_side_texture__num__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85B670)(this, stack);
}

bool slf__peds_set_max_fake_peds__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85B760)(this, stack);
}

bool slf__peds_set_fake_ped_tint__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85B780)(this, stack);
}

bool slf__set_fake_ped_spawn_rate__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85B7A0)(this, stack);
}

bool slf__set_min_fake_ped_radius__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85B7C0)(this, stack);
}

bool slf__set_max_fake_ped_radius_inner__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85B7E0)(this, stack);
}

bool slf__set_max_fake_ped_radius_outer__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85B800)(this, stack);
}

bool slf__set_max_fake_ped_height_outer__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85B820)(this, stack);
}

bool slf__get_max_fake_ped_height_outer__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83EBE0)(this, stack);
}

bool slf__peds_add_ped_entity__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85B840)(this, stack);
}

bool slf__peds_remove_ped_entity__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85B8B0)(this, stack);
}

bool slf__peds_safe_to_remove_ped_entity__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85B8F0)(this, stack);
}

bool slf__peds_add_vendor_entity__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85B970)(this, stack);
}

bool slf__peds_remove_vendor_entity__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85B9B0)(this, stack);
}

bool slf__peds_set_ped_value__entity__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85B9F0)(this, stack);
}

bool slf__peds_get_ped_value__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85BA40)(this, stack);
}

bool slf__peds_get_spawn_point_near_point__vector3d__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8813C0)(this, stack);
}

bool slf__peds_find_any_spawn_point__vector3d__vector3d__num__num__num__num__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_881500)(this, stack);
}

bool slf__peds_find_closest_spawn_point__vector3d__vector3d__num__num__num__num__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_881640)(this, stack);
}

bool slf__peds_find_ped_near__vector3d__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85BAB0)(this, stack);
}

bool slf__peds_find_peds_near__vector3d__num__num__num__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_881780)(this, stack);
}

bool slf__peds_clear_in_radius__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85BB00)(this, stack);
}

bool slf__peds_find_vehicle_near__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85BB20)(this, stack);
}

bool slf__peds_set_custom_idle__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85BB70)(this, stack);
}

bool slf__peds_set_custom_walk__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85BBD0)(this, stack);
}

bool slf__peds_set_wander_speed__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85BC10)(this, stack);
}

bool slf__peds_give_item__entity__entity__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85BC80)(this, stack);
}

bool slf__peds_give_item_to_ped__entity__entity__entity__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85BCD0)(this, stack);
}

bool slf__peds_get_item__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85BD30)(this, stack);
}

bool slf__peds_get_item_type__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85BD90)(this, stack);
}

bool slf__peds_drop_item__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85BE20)(this, stack);
}

bool slf__peds_setup_towing__entity__vector3d__entity__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85BE60)(this, stack);
}

bool slf__peds_set_resume_path__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85BEB0)(this, stack);
}

bool slf__peds_walk_pair__entity__entity__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85BF10)(this, stack);
}

bool slf__peds_walk_alone__entity__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85BF80)(this, stack);
}

bool slf__peds_get_behavior__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85BFE0)(this, stack);
}

bool slf__peds_is_on_extended_goto__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85C050)(this, stack);
}

bool slf__peds_extended_goto__entity__num__num__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85C0F0)(this, stack);
}

bool slf__spectacle_add_point_of_interest__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85C240)(this, stack);
}

bool slf__spectacle_add_point_of_interest__entity__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85C310)(this, stack);
}

bool slf__spectacle_add_shrinking_point_of_interest__vector3d__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85C420)(this, stack);
}

bool slf__spectacle_add_shrinking_point_of_interest__entity__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85C4F0)(this, stack);
}

bool slf__spectacle_remove_point_of_interest__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85C600)(this, stack);
}

bool slf__ped_enable_look_at__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85C690)(this, stack);
}

bool slf__find_variant_part_name__str__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85C720)(this, stack);
}
