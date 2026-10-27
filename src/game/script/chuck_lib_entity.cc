#include <cstring>

#include "retail.hh"
#include "game/arch_base.hh"
#include "game/script/chuck_lib_entity.hh"
#include "treyarch/mash/string_hash.hh"

using namespace treyarch;

// sub_83DAE0
u32 slc_entity_t::find_instance(const mash::string &name) const {
    if (!std::strncmp(name.c_str(), "NULL", 0xFFFF))
        return 0;

    string_hash hash;

    hash.initialize(mash::ALLOCATED, name.c_str());

    auto* entity = (arch_base*)retail::sub_640AE0(&hash.source_hash_code, 28, true); // entity_handle_manager::find_entity

    if (!entity)
        return 0;

    return entity->my_handle.value;
}

bool slf__hero__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83DB50)(this, stack);
}

bool slf__chase_cam__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83DBA0)(this, stack);
}

bool slf__create_devopt_entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84AA40)(this, stack);
}

bool slf__create_entity__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84AEA0)(this, stack);
}

bool slf__create_entity__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84B040)(this, stack);
}

bool slf__create_preregistered_entity__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84B160)(this, stack);
}

bool slf__create_preregistered_entity__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84B300)(this, stack);
}

bool slf__create_next_preregistered_debug_obstacle__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83DDF0)(this, stack);
}

bool slf__find_entity__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84B420)(this, stack);
}

bool slf__find_entity__uint__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84B490)(this, stack);
}

bool slf__entity_exists__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84B4F0)(this, stack);
}

bool slf__destroy_entity__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84B560)(this, stack);
}

bool slf__entity_has_entity_tracker__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84B8C0)(this, stack);
}

bool slf__entity_get_entity_tracker__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84B910)(this, stack);
}

bool slf__enable_entity_fading__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84BBA0)(this, stack);
}

bool slf__script_get_entities_of_type__num__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_880970)(this, stack);
}

bool slf__script_get_sorted_entities_of_type__num__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_880AB0)(this, stack);
}

bool slf__crawl_attach_to_surface__entity__line_info__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850550)(this, stack);
}

bool slf__entity__rvb_set_needs_repathfind__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_843680)(this, stack);
}

bool slf__entity__rvb_snapshot_behavior__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8436A0)(this, stack);
}

bool slf__entity__rvb_reset_behavior__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8436C0)(this, stack);
}

bool slf__entity__rvb_restart_timer__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8436E0)(this, stack);
}

bool slf__entity__rvb_set_target__num__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_843730)(this, stack);
}

bool slf__entity__rvb_set_target__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_843780)(this, stack);
}

bool slf__entity__rvb_has_target__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8437C0)(this, stack);
}

bool slf__entity__rvb_get_target_entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_843830)(this, stack);
}

bool slf__entity__rvb_get_target_vector__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_843890)(this, stack);
}

bool slf__entity__rvb_disable_tether__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_843910)(this, stack);
}

bool slf__entity__rvb_clear_target__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_843940)(this, stack);
}

bool slf__entity__rvb_set_machine__num__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_843970)(this, stack);
}

bool slf__entity__rvb_clear_machine__num__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_843A00)(this, stack);
}

bool slf__entity__rvb_add_patrol_waypoint__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_843A80)(this, stack);
}

bool slf__entity__rvb_add_patrol_waypoint__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_843AD0)(this, stack);
}

bool slf__entity__rvb_add_patrol_waypoint__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_843B20)(this, stack);
}

bool slf__entity__rvb_add_patrol_waypoint__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_843B70)(this, stack);
}

bool slf__entity__rvb_add_patrol_waypoint__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_843BB0)(this, stack);
}

bool slf__entity__rvb_add_patrol_waypoint__entity__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_843BF0)(this, stack);
}

bool slf__entity__rvb_clear_patrol_waypoints__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_843C30)(this, stack);
}

bool slf__entity__rvb_get_patrol_waypoint_count__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_843C50)(this, stack);
}

bool slf__entity__rvb_set_flag__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_843CB0)(this, stack);
}

bool slf__entity__rvb_get_flag__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_843CF0)(this, stack);
}

bool slf__entity__rvb_clear_flag__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_843D70)(this, stack);
}

bool slf__entity__rvb_set_var__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_843DB0)(this, stack);
}

bool slf__entity__rvb_get_var__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_843E00)(this, stack);
}

bool slf__entity__rvb_set_vector__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_843E80)(this, stack);
}

bool slf__entity__rvb_get_vector__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_843EE0)(this, stack);
}

bool slf__entity__rvb_set_hash__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_843FB0)(this, stack);
}

bool slf__entity__is_rvb_character__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_844010)(this, stack);
}

bool slf__entity__rvb_is_crawling__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_844070)(this, stack);
}

bool slf__entity__rvb_set_blocked_attack_types__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8440D0)(this, stack);
}

bool slf__entity__rvb_set_dodged_attack_types__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_844100)(this, stack);
}

bool slf__entity__rvb_enter_car__entity__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_844130)(this, stack);
}

bool slf__entity__rvb_exit_car__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8441A0)(this, stack);
}

bool slf__entity__rvb_enable_tether__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8441F0)(this, stack);
}

bool slf__entity__rvb_enable_combat_tether__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_844230)(this, stack);
}

bool slf__entity__rvb_enable_tether__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8442A0)(this, stack);
}

bool slf__entity__rvb_enable_combat_tether__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8442F0)(this, stack);
}

bool slf__entity__rvb_set_speed_modifier__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_844360)(this, stack);
}

bool slf__entity__rvb_get_animation_coordinator__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_844390)(this, stack);
}

bool slf__entity__get_visual_center__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8443F0)(this, stack);
}

bool slf__entity__get_visual_radius__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_844490)(this, stack);
}

bool slf__entity__get_abs_position__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_844500)(this, stack);
}

bool slf__entity__compute_sector__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_844570)(this, stack);
}

bool slf__entity__get_facing__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8445C0)(this, stack);
}

bool slf__entity__get_x_facing__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_844660)(this, stack);
}

bool slf__entity__get_y_facing__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8446D0)(this, stack);
}

bool slf__entity__get_z_facing__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_844740)(this, stack);
}

bool slf__entity__get_rel_x_facing__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8447B0)(this, stack);
}

bool slf__entity__get_rel_y_facing__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_844820)(this, stack);
}

bool slf__entity__get_rel_z_facing__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_844890)(this, stack);
}

bool slf__entity__get_last_anchor__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_844900)(this, stack);
}

bool slf__entity__get_rel_position__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8449A0)(this, stack);
}

bool slf__entity__set_parent__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_844A10)(this, stack);
}

bool slf__entity__set_parent_rel__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_844B10)(this, stack);
}

bool slf__entity__get_vehicle__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_844BA0)(this, stack);
}

bool slf__entity__get_parent__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_844C40)(this, stack);
}

bool slf__entity__get_conglom_owner__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_844C80)(this, stack);
}

bool slf__entity__set_rel_position__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_879C80)(this, stack);
}

bool slf__entity__move_rel_position__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_879D10)(this, stack);
}

bool slf__entity__set_velocity__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_844CD0)(this, stack);
}

bool slf__entity__set_abs_velocity__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_844E70)(this, stack);
}

bool slf__entity__look_at__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_844EF0)(this, stack);
}

bool slf__entity__set_xz_facing__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_844F70)(this, stack);
}

bool slf__entity__set_facing__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_845230)(this, stack);
}

bool slf__entity__set_abs_xz_facing__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8454B0)(this, stack);
}

bool slf__entity__set_po_facing__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8456D0)(this, stack);
}

bool slf__entity__set_rel_po_facing__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8458B0)(this, stack);
}

bool slf__entity__apply_continuous_rotation__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_845A90)(this, stack);
}

bool slf__entity__wait_rotate__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_845AE0)(this, stack);
}

bool slf__entity__wait_rotate_WCS__vector3d__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_845EF0)(this, stack);
}

bool slf__entity__wait_translate__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8462C0)(this, stack);
}

bool slf__entity__wait_translate_with_compute_sector__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8466A0)(this, stack);
}

bool slf__entity__wait_translate_abs__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_846AA0)(this, stack);
}

bool slf__entity__get_rel_velocity__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_846EE0)(this, stack);
}

bool slf__entity__force_region__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8470B0)(this, stack);
}

bool slf__entity__force_current_region__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_847140)(this, stack);
}

bool slf__entity__unforce_regions__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8471B0)(this, stack);
}

bool slf__entity__operator_equals_equals__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_847230)(this, stack);
}

bool slf__entity__operator_not_equals__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_847270)(this, stack);
}

bool slf__entity__set_damage_locked__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852680)(this, stack);
}

bool slf__entity__is_invulnerable__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8472B0)(this, stack);
}

bool slf__entity__is_immune_to_death_from_damage__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_847330)(this, stack);
}

bool slf__entity__enable_push_away_force_on_damage__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8473D0)(this, stack);
}

bool slf__entity__enable_die_on_fall_damage__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_847430)(this, stack);
}

bool slf__entity__set_invulnerable__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8474A0)(this, stack);
}

bool slf__entity__set_targetting__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_847570)(this, stack);
}

bool slf__entity__set_beamable__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8475E0)(this, stack);
}

bool slf__entity__set_ai_los_block__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_847650)(this, stack);
}

bool slf__entity__set_shadow_casting__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8476C0)(this, stack);
}

bool slf__entity__is_visible__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_847710)(this, stack);
}

bool slf__entity__set_visible__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8477A0)(this, stack);
}

bool slf__entity__set_physical__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_847840)(this, stack);
}

bool slf__entity__is_auto_activate_prop__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8478A0)(this, stack);
}

bool slf__entity__set_distance_clip__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_847920)(this, stack);
}

bool slf__entity__set_distance_clip_far__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_847990)(this, stack);
}

bool slf__entity__is_hanging_from_pole__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_847A00)(this, stack);
}

bool slf__entity__enable_pain_sounds__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_847AB0)(this, stack);
}

bool slf__entity__apply_web_damage__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_847B30)(this, stack);
}

bool slf__entity__apply_web_damage_with_directed_react__num__num__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_847C30)(this, stack);
}

bool slf__entity__apply_damage__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_847E20)(this, stack);
}

bool slf__entity__apply_damage_with_directed_react__num__num__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_847F00)(this, stack);
}

bool slf__entity__apply_damage_with_react__num__num__num__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8480F0)(this, stack);
}

bool slf__entity__apply_damage_with_react_and_scales__num__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_848380)(this, stack);
}

bool slf__entity__apply_damage_with_react_at_pos__num__num__num__vector3d__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_848800)(this, stack);
}

bool slf__entity__apply_damage_with_react_and_scales_at_pos__num__vector3d__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_848A10)(this, stack);
}

bool slf__entity__apply_damage_with_react_and_scales_at_pos_no_physics__num__vector3d__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_848E20)(this, stack);
}

bool slf__entity__apply_melee_damage__num__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_849230)(this, stack);
}

bool slf__entity__apply_explosive_damage__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_849440)(this, stack);
}

bool slf__entity__attach_to_wall__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_849710)(this, stack);
}

bool slf__entity__attach_to_wall__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_849820)(this, stack);
}

bool slf__entity__apply_explosive_damage__num__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8498D0)(this, stack);
}

bool slf__entity__get_last_entity_that_damaged_me__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_849B90)(this, stack);
}

bool slf__entity__set_damage_scale__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_849C00)(this, stack);
}

bool slf__entity__set_damage_scale__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_849C50)(this, stack);
}

bool slf__entity__get_damage_scale__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_849C90)(this, stack);
}

bool slf__entity__was_occluded_last_frame__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_849D10)(this, stack);
}

bool slf__entity__snap_to__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_849DB0)(this, stack);
}

bool slf__entity__abs_snap_to__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_849F90)(this, stack);
}

bool slf__entity__camera_set_target__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84A170)(this, stack);
}

bool slf__entity__camera_get_target__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84A1E0)(this, stack);
}

bool slf__entity__camera_set_roll__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84A290)(this, stack);
}

bool slf__entity__wait_camera_set_roll__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84A300)(this, stack);
}

bool slf__entity__camera_orbit__vector3d__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84A500)(this, stack);
}

bool slf__entity__get_member__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84A590)(this, stack);
}

bool slf__entity__has_member__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84A640)(this, stack);
}

bool slf__entity__get_member_for_linking__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84A6E0)(this, stack);
}

bool slf__entity__get_trigger_member_for_linking__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84A790)(this, stack);
}

bool slf__entity__has_member_for_linking__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84A850)(this, stack);
}

bool slf__entity__get_member_abs_position__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84A8F0)(this, stack);
}

bool slf__entity__get_target_member__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84A9B0)(this, stack);
}

bool slf__entity__entity__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84ABE0)(this, stack);
}

bool slf__entity__entity__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84AD80)(this, stack);
}

bool slf__entity__suspend__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84B5B0)(this, stack);
}

bool slf__entity__is_suspended__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84B610)(this, stack);
}

bool slf__entity__disable_collisions__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84B690)(this, stack);
}

bool slf__entity__enable_collisions__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84B6D0)(this, stack);
}

bool slf__entity__add_collision_ignorance__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84B710)(this, stack);
}

bool slf__entity__remove_collision_ignorance__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84B770)(this, stack);
}

bool slf__entity__unsuspend__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84B7D0)(this, stack);
}

bool slf__entity__get_district_id__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84B830)(this, stack);
}

bool slf__entity__als_play_animation_by_category__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84B950)(this, stack);
}

bool slf__entity__als_play_animation_by_filename__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84BAB0)(this, stack);
}

bool slf__entity__update_member_visible__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84BBD0)(this, stack);
}

bool slf__entity__update_member_color__str__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84BCC0)(this, stack);
}

bool slf__entity__wait_update_member_color__str__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84BE70)(this, stack);
}

bool slf__entity__particle_generator_set_scalar__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84C2F0)(this, stack);
}

bool slf__entity__get_enx_script_instance__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84C310)(this, stack);
}

bool slf__entity__get_enx_script_object_name__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84C3A0)(this, stack);
}

bool slf__entity__get_enx_script_exec_name__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84C410)(this, stack);
}

bool slf__entity__is_enx_script_related__uint__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84C490)(this, stack);
}

bool slf__entity__get_ai_param_float__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84C540)(this, stack);
}

bool slf__entity__get_ai_param_float_variance__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84C620)(this, stack);
}

bool slf__entity__get_ai_param_entity__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84C740)(this, stack);
}

bool slf__entity__get_ai_param_int__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84C840)(this, stack);
}

bool slf__entity__get_ai_param_str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84C910)(this, stack);
}

bool slf__entity__get_ai_param_hash__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84C9F0)(this, stack);
}

bool slf__entity__get_ai_param_vector3d__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84CAD0)(this, stack);
}

bool slf__entity__get_optional_ai_param_entity__str__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84CBD0)(this, stack);
}

bool slf__entity__get_optional_ai_param_float__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84CCD0)(this, stack);
}

bool slf__entity__get_optional_ai_param_int__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84CDC0)(this, stack);
}

bool slf__entity__get_optional_ai_param_str__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84CEA0)(this, stack);
}

bool slf__entity__get_optional_ai_param_hash__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84CF60)(this, stack);
}

bool slf__entity__get_optional_ai_param_vector3d__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84D0D0)(this, stack);
}

bool slf__entity__get_optional_ai_param_float_variance__str__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84D1E0)(this, stack);
}

bool slf__entity__set_ai_param_entity__str__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84D3C0)(this, stack);
}

bool slf__entity__set_ai_param_float__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84D470)(this, stack);
}

bool slf__entity__set_ai_param_float_variance__str__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84D520)(this, stack);
}

bool slf__entity__set_ai_param_int__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84D600)(this, stack);
}

bool slf__entity__set_ai_param_str__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84D6B0)(this, stack);
}

bool slf__entity__set_ai_param_hash__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84D760)(this, stack);
}

bool slf__entity__set_ai_param_hash__str__uint__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84D830)(this, stack);
}

bool slf__entity__set_ai_param_vector3d__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84D900)(this, stack);
}

bool slf__entity__get_ai_idle_type__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84D9B0)(this, stack);
}

bool slf__entity__set_ai_idle_type__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84DA10)(this, stack);
}

bool slf__entity__has_inode__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84DA50)(this, stack);
}

bool slf__entity__get_inode_param_float__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84DAE0)(this, stack);
}

bool slf__entity__get_inode_param_float_variance__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84DB60)(this, stack);
}

bool slf__entity__get_inode_param_int__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84DC30)(this, stack);
}

bool slf__entity__get_inode_param_entity__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84DCB0)(this, stack);
}

bool slf__entity__get_inode_param_str__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84DD30)(this, stack);
}

bool slf__entity__get_inode_param_hash__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84DDB0)(this, stack);
}

bool slf__entity__get_inode_param_string_hash__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84DE40)(this, stack);
}

bool slf__entity__get_inode_param_vector3d__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84DEC0)(this, stack);
}

bool slf__entity__get_optional_inode_param_float__str__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84DF50)(this, stack);
}

bool slf__entity__get_optional_inode_param_int__str__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84DFE0)(this, stack);
}

bool slf__entity__get_optional_inode_param_entity__str__str__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84E060)(this, stack);
}

bool slf__entity__get_optional_inode_param_str__str__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84E0E0)(this, stack);
}

bool slf__entity__get_optional_inode_param_hash__str__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84E160)(this, stack);
}

bool slf__entity__get_optional_inode_param_string_hash__str__str__uint__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84E220)(this, stack);
}

bool slf__entity__get_optional_inode_param_vector3d__str__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84E2D0)(this, stack);
}

bool slf__entity__set_inode_param_float__str__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84E360)(this, stack);
}

bool slf__entity__set_inode_param_float_variance__str__str__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84E3C0)(this, stack);
}

bool slf__entity__set_inode_param_int__str__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84E440)(this, stack);
}

bool slf__entity__set_inode_param_entity__str__str__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84E4A0)(this, stack);
}

bool slf__entity__set_inode_param_str__str__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84E500)(this, stack);
}

bool slf__entity__set_inode_param_hash__str__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84E560)(this, stack);
}

bool slf__entity__set_inode_param_hash__str__str__uint__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84E5E0)(this, stack);
}

bool slf__entity__set_inode_param_vector3d__str__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84E660)(this, stack);
}

bool slf__entity__push_ai_base_machine_immediately__entity__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84E6C0)(this, stack);
}

bool slf__entity__pop_ai_base_machine_immediately__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84E840)(this, stack);
}

bool slf__entity__change_locomotion_machine__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84E9A0)(this, stack);
}

bool slf__entity__change_locomotion_category__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84EBB0)(this, stack);
}

bool slf__entity__ai_break_morale_force_cower__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84ECF0)(this, stack);
}

bool slf__entity__ai_break_morale_force_flee__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84ED70)(this, stack);
}

bool slf__entity__ai_break_morale_force_flee__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84EE00)(this, stack);
}

bool slf__entity__get_ai_base_machine_state_name__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84EEB0)(this, stack);
}

bool slf__entity__is_enemy__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84EF10)(this, stack);
}

bool slf__entity__is_enemy_of_team__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84EFC0)(this, stack);
}

bool slf__entity__is_friend__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84F060)(this, stack);
}

bool slf__entity__is_friend_of_team__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84F110)(this, stack);
}

bool slf__entity__ai_has_clear_line_of_fire__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84F1B0)(this, stack);
}

bool slf__entity__has_als_category__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84F550)(this, stack);
}

bool slf__entity__get_als_category_name__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84F610)(this, stack);
}

bool slf__entity__get_als_state_name__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84F6C0)(this, stack);
}

bool slf__entity__force_strength_test__entity__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84F770)(this, stack);
}

bool slf__entity__begin_strength_test__entity__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84F840)(this, stack);
}

bool slf__entity__begin_strength_test__entity__num__num__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_880770)(this, stack);
}

bool slf__entity__begin_strength_test_trigger__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84F990)(this, stack);
}

bool slf__entity__run_strength_test__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84FA60)(this, stack);
}

bool slf__entity__begin_slingshot_point_test__entity__vector3d__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84FB40)(this, stack);
}

bool slf__entity__is_in_reflexes__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84FCD0)(this, stack);
}

bool slf__entity__is_in_senses__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84FD20)(this, stack);
}

bool slf__entity__is_sync_anim__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84FD70)(this, stack);
}

bool slf__entity__get_als_time_to_end_of_anim__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84FDD0)(this, stack);
}

bool slf__entity__get_als_current_anim_time__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84FE50)(this, stack);
}

bool slf__entity__get_als_current_normalized_time__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84FED0)(this, stack);
}

bool slf__entity__set_als_param_float__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84FF50)(this, stack);
}

bool slf__entity__set_als_param_vector3d__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84FFD0)(this, stack);
}

bool slf__entity__get_als_data_float__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850050)(this, stack);
}

bool slf__entity__get_als_data_int__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850150)(this, stack);
}

bool slf__entity__get_als_data_vector3d__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850250)(this, stack);
}

bool slf__entity__get_als_data_string__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850390)(this, stack);
}

bool slf__entity__has_ai_core__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8504E0)(this, stack);
}

bool slf__entity__crawl_attach_to_surface__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8505E0)(this, stack);
}

bool slf__entity__crawl_release_from_surface__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850990)(this, stack);
}

bool slf__entity__hates_spiderman__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850A60)(this, stack);
}

bool slf__entity__set_detectable__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850AE0)(this, stack);
}

bool slf__entity__set_spidey_sense_glow_color__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850B40)(this, stack);
}

bool slf__entity__clear_spidey_sense_glow_info__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850BA0)(this, stack);
}

bool slf__entity__set_target_icon__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850BE0)(this, stack);
}

bool slf__entity__clear_target_icon__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850C20)(this, stack);
}

bool slf__entity__is_an_actor__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850C50)(this, stack);
}

bool slf__entity__is_a_rocket__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850CB0)(this, stack);
}

bool slf__entity__is_a_vehicle__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850D30)(this, stack);
}

bool slf__entity__is_a_placeholder_vehicle__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850DA0)(this, stack);
}

bool slf__entity__set_vehicle_model__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850E10)(this, stack);
}

bool slf__entity__damage_vehicle_part__uint__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850E60)(this, stack);
}

bool slf__entity__damage_all_vehicle_parts__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850EC0)(this, stack);
}

bool slf__entity__repair_vehicle_part__uint__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850F00)(this, stack);
}

bool slf__entity__repair_all_vehicle_parts__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850F60)(this, stack);
}

bool slf__entity__get_vehicle_parts__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86FF80)(this, stack);
}

bool slf__entity__set_vehicle_mass__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850FA0)(this, stack);
}

bool slf__entity__set_vehicle_self_damage__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850FE0)(this, stack);
}

bool slf__entity__set_vehicle_other_damage__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851040)(this, stack);
}

bool slf__entity__get_vehicle_mass__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8510A0)(this, stack);
}

bool slf__entity__get_vehicle_dimensions__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851110)(this, stack);
}

bool slf__entity__get_vehicle_velocity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8511A0)(this, stack);
}

bool slf__entity__get_vehicle_angular_velocity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851240)(this, stack);
}

bool slf__entity__set_max_vehicle_speed__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8512E0)(this, stack);
}

bool slf__entity__get_max_vehicle_speed__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851320)(this, stack);
}

bool slf__entity__set_vehicle_flags__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851390)(this, stack);
}

bool slf__entity__get_vehicle_flags__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851400)(this, stack);
}

bool slf__entity__make_vehicle_clown_car__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851480)(this, stack);
}

bool slf__entity__set_vehicle_purpose__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8514E0)(this, stack);
}

bool slf__entity__set_vehicle_purpose__num__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851520)(this, stack);
}

bool slf__entity__set_vehicle_purpose__num__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851570)(this, stack);
}

bool slf__entity__set_vehicle_purpose__num__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8516F0)(this, stack);
}

bool slf__entity__set_vehicle_purpose__num__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851790)(this, stack);
}

bool slf__entity__set_vehicle_purpose__num__dynamic_array__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87EA20)(this, stack);
}

bool slf__entity__get_vehicle_purpose__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8518F0)(this, stack);
}

bool slf__entity__get_vehicle_purpose_entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851990)(this, stack);
}

bool slf__entity__get_vehicle_purpose_pos__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851A30)(this, stack);
}

bool slf__entity__get_vehicle_purpose_radius__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851AC0)(this, stack);
}

bool slf__entity__is_vehicle_purpose_done__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851B60)(this, stack);
}

bool slf__entity__set_vehicle_steering__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87C190)(this, stack);
}

bool slf__entity__get_vehicle_steering__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87C220)(this, stack);
}

bool slf__entity__set_vehicle_gas_pedal__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87C2D0)(this, stack);
}

bool slf__entity__get_vehicle_gas_pedal__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87C360)(this, stack);
}

bool slf__entity__set_vehicle_brake_pedal__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87C410)(this, stack);
}

bool slf__entity__get_vehicle_brake_pedal__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87C4A0)(this, stack);
}

bool slf__entity__set_vehicle_power_brake__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87C550)(this, stack);
}

bool slf__entity__get_vehicle_power_brake__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87C5F0)(this, stack);
}

bool slf__entity__set_vehicle_reverse__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87C6A0)(this, stack);
}

bool slf__entity__get_vehicle_reverse__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87C740)(this, stack);
}

bool slf__entity__enable_vehicle_physics__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851C10)(this, stack);
}

bool slf__entity__is_vehicle_physics_enabled__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851C70)(this, stack);
}

bool slf__entity__set_vehicle_condition__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851CD0)(this, stack);
}

bool slf__entity__get_vehicle_condition__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851D10)(this, stack);
}

bool slf__entity__can_vehicle_be_stolen__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851D60)(this, stack);
}

bool slf__entity__set_vehicle_can_be_stolen__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851E00)(this, stack);
}

bool slf__entity__set_vehicle_sleeps_on_stop__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851E60)(this, stack);
}

bool slf__entity__weapon_fire_as_gun__entity__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851ED0)(this, stack);
}

bool slf__entity__weapon_fire_as_rocket__entity__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851F20)(this, stack);
}

bool slf__entity__weapon_fire_as_grenade__entity__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851F70)(this, stack);
}

bool slf__entity__rocket_redirect__entity__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851FC0)(this, stack);
}

bool slf__entity__rocket_redirect_dir__entity__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852020)(this, stack);
}

bool slf__entity__rocket_disable_guidance__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852080)(this, stack);
}

bool slf__entity__force_ai_enter_car__entity__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8520C0)(this, stack);
}

bool slf__entity__force_ai_exit_car__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8521B0)(this, stack);
}

bool slf__entity__get_car_seat__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852270)(this, stack);
}

bool slf__entity__is_seat_taken__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8522D0)(this, stack);
}

bool slf__entity__is_car_occupied__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852380)(this, stack);
}

bool slf__entity__vehicle_disabled_escape_enabled__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852410)(this, stack);
}

bool slf__entity__vehicle_eject_kills_occupants__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852480)(this, stack);
}

bool slf__entity__set_vehicle_occupant_damage_tolerance_percentage__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8524F0)(this, stack);
}

bool slf__entity__get_actor_in_seat__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852560)(this, stack);
}

bool slf__entity__get_tentacle__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8525E0)(this, stack);
}

bool slf__entity__set_attack_damage_locked__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852680)(this, stack);
}

bool slf__entity__is_a_variant__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8526C0)(this, stack);
}

bool slf__entity__get_variant_name__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852740)(this, stack);
}

bool slf__entity__get_variant_weight__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8527D0)(this, stack);
}

bool slf__entity__get_num_variants__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852850)(this, stack);
}

bool slf__entity__get_max_variant_texture__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8528C0)(this, stack);
}

bool slf__entity__get_min_variant_texture__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852940)(this, stack);
}

bool slf__entity__has_variant__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8529C0)(this, stack);
}

bool slf__entity__find_variant__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852A90)(this, stack);
}

bool slf__entity__prepare_change_variant__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852B50)(this, stack);
}

bool slf__entity__set_variant__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852B90)(this, stack);
}

bool slf__entity__set_variant_id__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852C40)(this, stack);
}

bool slf__entity__set_variant_textures__num__num__num__num__num__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852CB0)(this, stack);
}

bool slf__entity__set_variant_and_textures__str__num__num__num__num__num__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852D30)(this, stack);
}

bool slf__entity__set_random_variant__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852E20)(this, stack);
}

bool slf__entity__set_random_textures__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852E70)(this, stack);
}

bool slf__entity__set_simple_variant_mode__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852EC0)(this, stack);
}

bool slf__entity__get_simple_variant_mode__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852F70)(this, stack);
}

bool slf__entity__set_simple_variant__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852FF0)(this, stack);
}

bool slf__entity__get_simple_variant__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8530A0)(this, stack);
}

bool slf__entity__aim_head__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_853120)(this, stack);
}

bool slf__entity__aim_head_direct__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8531B0)(this, stack);
}

bool slf__entity__aim_head_and_eyes__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_853240)(this, stack);
}

bool slf__entity__aim_head_eyes_and_upper_body__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8532D0)(this, stack);
}

bool slf__entity__aim_head_and_upper_body__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_853360)(this, stack);
}

bool slf__entity__aim_head_and_upper_body_direct__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8533F0)(this, stack);
}

bool slf__entity__aim_left_arm__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_853480)(this, stack);
}

bool slf__entity__aim_right_arm__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_853510)(this, stack);
}

bool slf__entity__update_target_to_look_at__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8535A0)(this, stack);
}

bool slf__entity__physical_ifc_add_dynamic_pendulum__vector3d__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_853620)(this, stack);
}

bool slf__entity__physical_ifc_add_dynamic_pendulum__entity__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8537F0)(this, stack);
}

bool slf__entity__physical_ifc_remove_dynamic_pendulum__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8539C0)(this, stack);
}

bool slf__entity__physical_ifc_set_dynamic_pendulum_length__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_883960)(this, stack);
}

bool slf__entity__physical_ifc_get_dynamic_pendulum_length__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8839B0)(this, stack);
}

bool slf__entity__physical_ifc_set_dynamic_pendulum_attach_limb__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_883A20)(this, stack);
}

bool slf__entity__physical_ifc_get_dynamic_pendulum_attach_limb__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_883A70)(this, stack);
}

bool slf__entity__physical_ifc_set_dynamic_pendulum_maintain_rel_orient__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_883AF0)(this, stack);
}

bool slf__entity__physical_ifc_set_dynamic_pendulum_maintain_abs_orient__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_883B70)(this, stack);
}

bool slf__entity__set_puppet_slave_array__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_879DA0)(this, stack);
}

bool slf__entity__get_floor_offset__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_853A20)(this, stack);
}

bool slf__entity__set_floor_offset_override__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_853AB0)(this, stack);
}

bool slf__entity__can_be_web_yanked__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_853B00)(this, stack);
}

bool slf__entity__get_velocity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_853BD0)(this, stack);
}

bool slf__entity__set_crawlable__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_853DC0)(this, stack);
}

bool slf__entity__in_combat_state__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_853E10)(this, stack);
}

bool slf__entity__set_tentacle_end_target__uint__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_853EC0)(this, stack);
}

bool slf__entity__set_tentacle_default_pct__uint__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_853F00)(this, stack);
}

bool slf__entity__set_tentacle_effect__uint__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_853F40)(this, stack);
}

bool slf__entity__play_tentacle_effect__uint__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_853F90)(this, stack);
}

bool slf__entity__spawn_effect__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_853FE0)(this, stack);
}

bool slf__entity__spawn_effect_at_entity_position__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_854030)(this, stack);
}

bool slf__entity__spawn_effect_at_world_position__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8542E0)(this, stack);
}

bool slf__entity__spawn_looping_effect__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_854360)(this, stack);
}

bool slf__entity__stop_looping_effect__uint__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8543E0)(this, stack);
}

bool slf__entity__stop_all_effects__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_854410)(this, stack);
}

bool slf__entity__get_teleport_target_position__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_854450)(this, stack);
}
