#include "retail.hh"
#include "game/script/chuck_lib_misc.hh"

using namespace treyarch;

bool slf__toggle_switchable_connectors__uint__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8737D0)(this, stack);
}

bool slf__has_substring__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_873860)(this, stack);
}

bool slf__to_string_hash__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_873900)(this, stack);
}

bool slf__clear_area_of_entities__vector3d__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87F120)(this, stack);
}

bool slf__change_environment_box_env_file__uint__uint__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_873940)(this, stack);
}

bool slf__is_camera_lr_inverted__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841F40)(this, stack);
}

bool slf__is_camera_ud_inverted__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841F80)(this, stack);
}

bool slf__get_current_view_cam_x_facing__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841FC0)(this, stack);
}

bool slf__get_current_view_cam_y_facing__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83F9A0)(this, stack);
}

bool slf__get_current_view_cam_z_facing__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_842000)(this, stack);
}

bool slf__get_current_view_cam_pos__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_842040)(this, stack);
}

bool slf__enable_marky_cam__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8739E0)(this, stack);
}

bool slf__is_marky_cam_enabled__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_842080)(this, stack);
}

bool slf__set_marky_fov__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_873A30)(this, stack);
}

bool slf__enable_animated_cam__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_873A80)(this, stack);
}

bool slf__disable_animated_cam__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_873BE0)(this, stack);
}

bool slf__is_animated_cam_enabled__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8420C0)(this, stack);
}

bool slf__set_animated_fov__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_873AF0)(this, stack);
}

bool slf__set_desired_fov__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_873B40)(this, stack);
}

bool slf__disable_marky_cam__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_873BE0)(this, stack);
}

bool slf__inhibit_animated_cam__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_873C30)(this, stack);
}

bool slf__freeze_hero__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_873C70)(this, stack);
}

bool slf__set_blur__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_873CD0)(this, stack);
}

bool slf__suspend_all_ai__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824CB0)(this, stack);
}

bool slf__unsuspend_all_ai__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824CD0)(this, stack);
}

bool slf__is_dialog_active__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_842100)(this, stack);
}

bool slf__is_conversation_active__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_842180)(this, stack);
}

bool slf__enable_pause__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_873CF0)(this, stack);
}

bool slf__enable_interface__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_873D30)(this, stack);
}

bool slf__enable_pois__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_873D70)(this, stack);
}

bool slf__map_debug_menu_to_select_button__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__delay__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_873DB0)(this, stack);
}

bool slf__dilated_delay__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_873E30)(this, stack);
}

bool slf__random__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_873F30)(this, stack);
}

bool slf__float_random__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_873F70)(this, stack);
}

bool slf__ZERO_TO_ONE__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8421C0)(this, stack);
}

bool slf__PLUS_MINUS_ONE__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_842200)(this, stack);
}

bool slf__get_time_inc__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_842250)(this, stack);
}

bool slf__get_dilated_time_inc__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_842280)(this, stack);
}

bool slf__stop_vibration__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8422C0)(this, stack);
}

bool slf__apply_radius_damage__vector3d__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_873FD0)(this, stack);
}

bool slf__apply_radius_damage_ignore_hero__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_874030)(this, stack);
}

bool slf__apply_radius_damage__vector3d__num__num__num__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_874070)(this, stack);
}

bool slf__apply_radius_damage_ignore_hero__vector3d__num__num__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8740F0)(this, stack);
}

bool slf__los_check__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_874160)(this, stack);
}

bool slf__find_district_for_point__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_874290)(this, stack);
}

bool slf__find_innermost_district__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8742E0)(this, stack);
}

bool slf__collide_segment_with_hollow_sphere__vector3d__vector3d__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_874330)(this, stack);
}

bool slf__col_check__vector3d__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8743C0)(this, stack);
}

bool slf__get_col_hit_pos__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8422E0)(this, stack);
}

bool slf__get_col_hit_norm__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_842320)(this, stack);
}

bool slf__get_col_hit_ent__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_842360)(this, stack);
}

bool slf__get_global_time_dilation__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_842380)(this, stack);
}

bool slf__set_global_time_dilation__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8745D0)(this, stack);
}

bool slf__wait_set_global_time_dilation__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_874600)(this, stack);
}

bool slf__sqrt__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_874750)(this, stack);
}

bool slf__sin__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_874780)(this, stack);
}

bool slf__cos__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_874850)(this, stack);
}

bool slf__acos__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8748D0)(this, stack);
}

bool slf__sin_cos__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_874910)(this, stack);
}

bool slf__get_int_num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_874A00)(this, stack);
}

bool slf__get_script_user_num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_863730)(this, stack);
}

bool slf__set_script_user_num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622F0)(this, stack);
}

bool slf__get_script_user_vector__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_874A30)(this, stack);
}

bool slf__set_script_user_vector__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_854A30)(this, stack);
}

bool slf__get_script_user_str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8636F0)(this, stack);
}

bool slf__set_script_user_str__num__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622F0)(this, stack);
}

bool slf__get_ini_flag__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_874A80)(this, stack);
}

bool slf__get_ini_num__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_874AD0)(this, stack);
}

bool slf__set_ini_flag__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_874B40)(this, stack);
}

bool slf__set_ini_num__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_874B80)(this, stack);
}

bool slf__get_hires_real_world_timer__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8423E0)(this, stack);
}

bool slf__get_player_type__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_842410)(this, stack);
}

bool slf__malor__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_874BE0)(this, stack);
}

bool slf__malor_no_locking__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_874C50)(this, stack);
}

bool slf__is_in_district__vector3d__uint__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_874CC0)(this, stack);
}

bool slf__is_sphere_visible__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_874D50)(this, stack);
}

bool slf__is_sphere_on_screen__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_874DF0)(this, stack);
}

bool slf__calc_launch_vector__vector3d__vector3d__num__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_874E80)(this, stack);
}

bool slf__get_all_instances_thread_count__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8750D0)(this, stack);
}

bool slf__get_all_execs_thread_count__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87AA40)(this, stack);
}

bool slf__is_interior__uint__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_875120)(this, stack);
}

bool slf__set_global_sunlight__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__get_light_position__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87AC40)(this, stack);
}

bool slf__set_light_position__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87AD40)(this, stack);
}

bool slf__set_light_direction__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87AE10)(this, stack);
}

bool slf__get_light_direction__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87AEF0)(this, stack);
}

bool slf__get_light_intensity__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87B030)(this, stack);
}

bool slf__set_light_intensity__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87B100)(this, stack);
}

bool slf__light_orbit__str__vector3d__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87B1B0)(this, stack);
}

bool slf__set_light_color__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87B2F0)(this, stack);
}

bool slf__get_light_color__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87B3D0)(this, stack);
}

bool slf__enable_bloom__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_875190)(this, stack);
}

bool slf__set_sandman_wetness__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8751C0)(this, stack);
}

bool slf__find_good_spot__entity__vector3d__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87E1E0)(this, stack);
}

bool slf__find_good_spot__dynamic_array__vector3d__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8831A0)(this, stack);
}

bool slf__combat_check_for_dodge__entity__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8751E0)(this, stack);
}

bool slf__combat_check_for_block__entity__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_875350)(this, stack);
}

bool slf__get_path_graph_num_nodes__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8754C0)(this, stack);
}

bool slf__get_path_graph_closest_node__str__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8755C0)(this, stack);
}

bool slf__get_path_graph_node_pos__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_875760)(this, stack);
}

bool slf__get_path_graph_node_num_edges__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8758C0)(this, stack);
}

bool slf__get_path_graph_node_other_node__str__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_875A10)(this, stack);
}

bool slf__find_nearest_position_on_path_graph__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_875B90)(this, stack);
}

bool slf__profile_begin__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__keep_sphere_in_world__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_875D00)(this, stack);
}

bool slf__apply_radius_damage_adv__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87B4D0)(this, stack);
}

bool slf__find_throwable_manip_obj__vector3d__num__num__dynamic_array__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_883290)(this, stack);
}

bool slf__find_thrown_manip_obj__vector3d__num__num__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8833C0)(this, stack);
}

bool slf__find_electrical_swing_objects__vector3d__num__num__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8834A0)(this, stack);
}

bool slf__profiler_enable__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__profiler_display__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__profiler_set_thread__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__profiler_set_refresh_rate__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__profiler_set_parenting__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__profiler_set_sort__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__profiler_set_number_type__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__profiler_set_reset_counter__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__profiler_set_filter__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__set_teams_love__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_875DA0)(this, stack);
}

bool slf__set_teams_hate__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_875E50)(this, stack);
}

bool slf__add_to_hate_list__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_875F00)(this, stack);
}

bool slf__remove_from_hate_list__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_875FA0)(this, stack);
}

bool slf__add_to_friend_list__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876040)(this, stack);
}

bool slf__remove_from_friend_list__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8760E0)(this, stack);
}
