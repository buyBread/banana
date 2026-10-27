#include "retail.hh"
#include "game/script/chuck_lib_spiderman.hh"

using namespace treyarch;

bool slf__spiderman_stick_dir_in_world_coords__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_840610)(this, stack);
}

bool slf__spiderman_camera_autocorrect__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_872610)(this, stack);
}

bool slf__spiderman_camera_manual_pitch__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_872650)(this, stack);
}

bool slf__spiderman_camera_manual_look__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_872690)(this, stack);
}

bool slf__spiderman_camera_manual_look_3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8726D0)(this, stack);
}

bool slf__spiderman_camera_manual_look_at_target__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_872710)(this, stack);
}

bool slf__spiderman_camera_manual_look_at_target_3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_872750)(this, stack);
}

bool slf__spiderman_hide_web_dangles__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_840660)(this, stack);
}

bool slf__spiderman_is_crawling__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_840690)(this, stack);
}

bool slf__spiderman_is_on_wall__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8406E0)(this, stack);
}

bool slf__spiderman_is_on_ceiling__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_840730)(this, stack);
}

bool slf__spiderman_is_on_ground__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_840780)(this, stack);
}

bool slf__spiderman_is_truly_on_the_ground__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8407D0)(this, stack);
}

bool slf__spiderman_is_swinging__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8408B0)(this, stack);
}

bool slf__spiderman_is_running__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_840900)(this, stack);
}

bool slf__spiderman_is_falling__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_840970)(this, stack);
}

bool slf__spiderman_is_jumping__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8409C0)(this, stack);
}

bool slf__spiderman_is_sprinting__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_840A10)(this, stack);
}

bool slf__spiderman_is_wallsprinting__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_840A80)(this, stack);
}

bool slf__spiderman_is_sprint_crawling__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_840AD0)(this, stack);
}

bool slf__spiderman_is_in_wallmode__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_840B40)(this, stack);
}

bool slf__spiderman_is_in_webcast_approach__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_840B90)(this, stack);
}

bool slf__spiderman_is_in_webcast_approach_critical_hit_range__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_840BE0)(this, stack);
}

bool slf__spiderman_is_in_approach_counter__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_840C30)(this, stack);
}

bool slf__spiderman_is_in_webcast_approach_normal_hit_range__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_840C80)(this, stack);
}

bool slf__spiderman_approach_norm_to_crit_percent__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_840CD0)(this, stack);
}

bool slf__spiderman_is_in_webcast_approach_early_range__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_840D10)(this, stack);
}

bool slf__spiderman_is_in_webcast_approach_counter_range__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_840D60)(this, stack);
}

bool slf__spiderman_is_webcast_blocked__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_840DB0)(this, stack);
}

bool slf__spiderman_is_webcast_normal_hit__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_840E00)(this, stack);
}

bool slf__spiderman_is_webcast_crit__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_840E80)(this, stack);
}

bool slf__spiderman_is_webcast_crit_no_range_protection__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_840F00)(this, stack);
}

bool slf__spiderman_is_webcast_countered__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_840F50)(this, stack);
}

bool slf__spiderman_is_webcast_unresolved__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_840FA0)(this, stack);
}

bool slf__spiderman_is_webcast_at_junction__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_840FF0)(this, stack);
}

bool slf__spiderman_is_webcast_X_branch__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841040)(this, stack);
}

bool slf__spiderman_is_webcast_Y_branch__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841090)(this, stack);
}

bool slf__spiderman_is_webcast_B_branch__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8410E0)(this, stack);
}

bool slf__spiderman_is_webcasting_entity__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_872790)(this, stack);
}

bool slf__spiderman_lock_suit__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_872810)(this, stack);
}

bool slf__spiderman_is_suit_mode_locked__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841130)(this, stack);
}

bool slf__spiderman_mode_lock_suit__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8728A0)(this, stack);
}

bool slf__spiderman_break_web__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8411B0)(this, stack);
}

bool slf__spiderman_set_rpg_progress__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_872910)(this, stack);
}

bool slf__spiderman_set_desired_mode__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_872950)(this, stack);
}

bool slf__spiderman_get_mode__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8411F0)(this, stack);
}

bool slf__spiderman_lock_mode__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_872990)(this, stack);
}

bool slf__spiderman_is_mode_locked__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841230)(this, stack);
}

bool slf__spiderman_lock_spider_reflexes_on__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841280)(this, stack);
}

bool slf__spiderman_lock_spider_reflexes_off__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8412C0)(this, stack);
}

bool slf__spiderman_unlock_spider_reflexes__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841300)(this, stack);
}

bool slf__spiderman_is_spider_reflexes_on__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841340)(this, stack);
}

bool slf__spiderman_lock_spider_senses_on__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8729E0)(this, stack);
}

bool slf__spiderman_lock_spider_senses_off__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_872A30)(this, stack);
}

bool slf__spiderman_unlock_spider_senses__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_872A80)(this, stack);
}

bool slf__spiderman_is_spider_senses_on__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8413D0)(this, stack);
}

bool slf__set_spider_reflexes_depletion_rate__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_872AD0)(this, stack);
}

bool slf__set_spider_reflexes_recharge_rate__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_872B10)(this, stack);
}

bool slf__set_spider_reflexes_max_recharge_percent__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_872B50)(this, stack);
}

bool slf__set_spider_reflexes_min_engage_percent__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_872B90)(this, stack);
}

bool slf__set_spider_reflexes_spiderman_time_dilation__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_872BD0)(this, stack);
}

bool slf__get_spider_reflexes_spiderman_time_dilation__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841440)(this, stack);
}

bool slf__set_spider_reflexes_world_time_dilation__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_872C10)(this, stack);
}

bool slf__get_spider_reflexes_world_time_dilation__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8414A0)(this, stack);
}

bool slf__spiderman_camera_get_z_facing__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841500)(this, stack);
}

bool slf__trigger_threat_ping__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_872C50)(this, stack);
}

bool slf__trigger_attack_indicator__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_872C90)(this, stack);
}

bool slf__trigger_attack_indicator__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_872CD0)(this, stack);
}

bool slf__set_spiderman_zlock_entity__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_872D20)(this, stack);
}

bool slf__clear_spiderman_zlock_entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841540)(this, stack);
}

bool slf__get_spiderman_zlock_entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841580)(this, stack);
}

bool slf__get_spiderman_target__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8415C0)(this, stack);
}

bool slf__get_num_spiderman_engagers__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8416F0)(this, stack);
}

bool slf__get_num_spiderman_reservists__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841770)(this, stack);
}

bool slf__set_spiderman_zlock_control__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_872D60)(this, stack);
}

bool slf__is_valid_zlock_target__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_872DB0)(this, stack);
}

bool slf__spiderman_force_evade__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_872E30)(this, stack);
}

bool slf__spiderman_get_block_strength__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8417F0)(this, stack);
}

bool slf__spiderman_get_swing_anchor_pos__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841860)(this, stack);
}

bool slf__spiderman_get_swing_angle__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8418F0)(this, stack);
}

bool slf__spiderman_get_swing_side_angle__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841930)(this, stack);
}

bool slf__spiderman_get_swing_to_anchor_angle__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841970)(this, stack);
}

bool slf__spiderman_get_pole_swing_speed__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8419B0)(this, stack);
}

bool slf__spiderman_get_pole_swing_boost_level__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841AA0)(this, stack);
}

bool slf__spiderman_find_best_pole__vector3d__num__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_872E80)(this, stack);
}

bool slf__spiderman_find_best_wall_hang_spot__vector3d__num__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_872FE0)(this, stack);
}

bool slf__spiderman_find_best_web_subdue_position__vector3d__vector3d__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_873050)(this, stack);
}

bool slf__spiderman_inc_combo_counter__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8730C0)(this, stack);
}

bool slf__spiderman_set_combo_counter__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_873100)(this, stack);
}

bool slf__spiderman_get_combo_counter__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841B80)(this, stack);
}

bool slf__spiderman_set_combo_stat_achieved__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_873140)(this, stack);
}

bool slf__spiderman_get_last_combo_hit_count__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841BC0)(this, stack);
}

bool slf__spiderman_get_prop_to_throw__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841C00)(this, stack);
}

bool slf__spiderman_get_prop_to_throw_attach_point__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841C40)(this, stack);
}

bool slf__spiderman_set_thrown_props_explode_on_entity_collide__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_873180)(this, stack);
}

bool slf__spiderman_set_noone_attack_me_time__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8731D0)(this, stack);
}

bool slf__is_in_RvB_sloppy_manual_dodge__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841C80)(this, stack);
}

bool slf__is_in_RvB_tight_manual_dodge__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841CF0)(this, stack);
}

bool slf__is_in_rvb_black_mode__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841D60)(this, stack);
}

bool slf__is_in_focus_mode__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841DD0)(this, stack);
}

bool slf__get_alignment_points__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841E50)(this, stack);
}

bool slf__get_alignment_percent__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841E80)(this, stack);
}

bool slf__get_points_awarded_by_type__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_873210)(this, stack);
}

bool slf__award_red_alignment_points_by_type__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_873240)(this, stack);
}

bool slf__award_black_alignment_points_by_type__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_873290)(this, stack);
}

bool slf__get_red_xp__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841EB0)(this, stack);
}

bool slf__get_red_xp_percent__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841EB0)(this, stack);
}

bool slf__award_red_xp__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__get_black_xp__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841EB0)(this, stack);
}

bool slf__get_black_xp_percent__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841EB0)(this, stack);
}

bool slf__award_black_xp__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__get_unified_xp__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841EB0)(this, stack);
}

bool slf__get_unified_xp_percent__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841EB0)(this, stack);
}

bool slf__award_unified_xp__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8732E0)(this, stack);
}

bool slf__create_proportional_orbs__num__entity__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_873320)(this, stack);
}

bool slf__award_xp_based_on_suit__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__set_glass_house_level__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8734E0)(this, stack);
}
