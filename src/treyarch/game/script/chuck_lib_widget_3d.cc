#include "retail.hh"
#include "treyarch/game/script/chuck_lib_widget_3d.hh"

using namespace treyarch;

bool slf__create_3d_widget__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8604B0)(this, stack);
}

bool slf__create_3d_widget_from_params__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860580)(this, stack);
}

bool slf__create_3d_mission_text_widget__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860650)(this, stack);
}

bool slf__create_3d_hint_text_widget__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8607B0)(this, stack);
}

bool slf__create_3d_text_widget_from_params__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860910)(this, stack);
}

bool slf__destroy_all_3d_mission_text_widgets__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8241E0)(this, stack);
}

bool slf__hide_all_3d_mission_text_widgets__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824210)(this, stack);
}

bool slf__destroy_all_3d_hint_text_widgets__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824220)(this, stack);
}

bool slf__hide_all_3d_hint_text_widgets__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824250)(this, stack);
}

bool slf__create_3d_button_widget__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860A20)(this, stack);
}

bool slf__destroy_3d_widget__widget_3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860AB0)(this, stack);
}

bool slf__destroy_all_3d_widgets__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824260)(this, stack);
}

bool slf__move_ui_camera_to_marker__widget_3d__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860AF0)(this, stack);
}

bool slf__move_ui_camera_to_home__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860B70)(this, stack);
}

bool slf__enable_health_meter__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860BD0)(this, stack);
}

bool slf__enable_radar__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860C20)(this, stack);
}

bool slf__enable_boss_meter__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860C70)(this, stack);
}

bool slf__disable_boss_meter__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824290)(this, stack);
}

bool slf__add_boss_meter__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860CB0)(this, stack);
}

bool slf__add_boss_meter__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860D00)(this, stack);
}

bool slf__remove_boss_meter__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860D40)(this, stack);
}

bool slf__remove_all_boss_meters__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824290)(this, stack);
}

bool slf__get_boss_meter_health_value__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860D70)(this, stack);
}

bool slf__set_boss_meter_health_value__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860DB0)(this, stack);
}

bool slf__clear_mission_objectives__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8242B0)(this, stack);
}

bool slf__add_mission_objective__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860DE0)(this, stack);
}

bool slf__add_mission_objective__num__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860EF0)(this, stack);
}

bool slf__add_mission_objective_debug__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860FF0)(this, stack);
}

bool slf__clear_3d_dialog_text__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824360)(this, stack);
}

bool slf__add_3d_dialog_text__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861030)(this, stack);
}

bool slf__add_3d_dialog_text__num__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861140)(this, stack);
}

bool slf__add_3d_dialog_text_debug__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860FF0)(this, stack);
}

bool slf__add_3d_objective_text__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861240)(this, stack);
}

bool slf__add_3d_objective_text__num__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861350)(this, stack);
}

bool slf__add_3d_objective_text_debug__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860FF0)(this, stack);
}

bool slf__bring_up_3d_dialog__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861450)(this, stack);
}

bool slf__set_3d_dialog_title__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861570)(this, stack);
}

bool slf__set_3d_dialog_title_debug__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__show_game_button__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861610)(this, stack);
}

bool slf__remove_all_game_buttons__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8243B0)(this, stack);
}

bool slf__remove_game_button__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861690)(this, stack);
}

bool slf__play_game_button_anim__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8616C0)(this, stack);
}

bool slf__game_button_anim_playing__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861700)(this, stack);
}

bool slf__set_game_button_pos__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861770)(this, stack);
}

bool slf__set_game_button_pos__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8617B0)(this, stack);
}

bool slf__get_2d_ui_pos_from_world_pos__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8617F0)(this, stack);
}

bool slf__set_game_button_scale__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861B00)(this, stack);
}

bool slf__set_game_button_alpha__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861B30)(this, stack);
}

bool slf__show_X_over_button__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861B60)(this, stack);
}

bool slf__is_X_over_button_shown__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861BB0)(this, stack);
}

bool slf__load_debug_3d_widget__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__set_debug_3d_widget_pos__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622F0)(this, stack);
}

bool slf__get_debug_3d_widget_x__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841EB0)(this, stack);
}

bool slf__get_debug_3d_widget_y__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841EB0)(this, stack);
}

bool slf__set_debug_3d_widget_scale__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__get_debug_3d_widget_scale__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841EB0)(this, stack);
}

bool slf__set_debug_3d_widget_tilt__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__get_debug_3d_widget_tilt__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841EB0)(this, stack);
}

bool slf__set_debug_3d_widget_spin__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__get_debug_3d_widget_spin__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841EB0)(this, stack);
}

bool slf__set_debug_3d_widget_rotation__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__get_debug_3d_widget_rotation__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841EB0)(this, stack);
}

bool slf__get_debug_3d_widget_entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83F550)(this, stack);
}

bool slf__debug_3d_widget_play_anim__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__debug_3d_widget_set_cowbell_variant__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__debug_3d_widget_add_ui_parameter_files_to_menu__debug_menu__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__get_ui_camera_entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83ED90)(this, stack);
}

bool slf__ui_camera_play_anim__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861C20)(this, stack);
}

bool slf__ui_camera_move_to_marker__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861C70)(this, stack);
}

bool slf__ui_camera_reset_starting_anim_pos__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8243D0)(this, stack);
}

bool slf__ui_camera_set_edit_mode__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861CF0)(this, stack);
}

bool slf__ui_camera_inc_x_pos__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824400)(this, stack);
}

bool slf__ui_camera_dec_x_pos__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824420)(this, stack);
}

bool slf__ui_camera_inc_y_pos__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824440)(this, stack);
}

bool slf__ui_camera_dec_y_pos__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824460)(this, stack);
}

bool slf__ui_camera_inc_z_pos__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824480)(this, stack);
}

bool slf__ui_camera_dec_z_pos__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8244A0)(this, stack);
}

bool slf__ui_camera_inc_tilt__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8244C0)(this, stack);
}

bool slf__ui_camera_dec_tilt__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8244E0)(this, stack);
}

bool slf__ui_camera_inc_spin__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824500)(this, stack);
}

bool slf__ui_camera_dec_spin__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824520)(this, stack);
}

bool slf__ui_camera_inc_rotation__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824540)(this, stack);
}

bool slf__ui_camera_dec_rotation__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824560)(this, stack);
}

bool slf__ui_camera_reset_pos__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861D40)(this, stack);
}

bool slf__add_ui_light__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83EDE0)(this, stack);
}

bool slf__ui_light_exists__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861D80)(this, stack);
}

bool slf__get_ui_light_x__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861DE0)(this, stack);
}

bool slf__get_ui_light_y__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861E40)(this, stack);
}

bool slf__get_ui_light_near_dist__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861EA0)(this, stack);
}

bool slf__get_ui_light_far_dist__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861F00)(this, stack);
}

bool slf__set_ui_light_x__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861F60)(this, stack);
}

bool slf__set_ui_light_y__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861F90)(this, stack);
}

bool slf__set_ui_light_near_dist__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861FC0)(this, stack);
}

bool slf__set_ui_light_far_dist__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_861FF0)(this, stack);
}

bool slf__set_ui_light_widget_index__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862020)(this, stack);
}

bool slf__set_debug_3d_text_marker__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__set_debug_3d_text_str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__set_debug_show_3d_text_regions__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__set_debug_child_widget__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622F0)(this, stack);
}

bool slf__reattach_child_widget__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__set_cutscene_texture__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862040)(this, stack);
}

bool slf__display_cutscene__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8620B0)(this, stack);
}

bool slf__hide_cutscene__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824580)(this, stack);
}

bool slf__is_displaying_cutscene__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83EE10)(this, stack);
}

bool slf__send_hide_cutscene_quad_signal__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824590)(this, stack);
}

bool slf__display_ped_warning__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8245A0)(this, stack);
}

bool slf__hide_ped_warning__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8245B0)(this, stack);
}

bool slf__display_ped_warning_with_duration__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8620E0)(this, stack);
}

bool slf__create_meter__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862110)(this, stack);
}

bool slf__create_meter__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8621F0)(this, stack);
}

bool slf__create_meter_with_label_placement__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862270)(this, stack);
}

bool slf__set_meter_value__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622D0)(this, stack);
}

bool slf__set_meter_scale__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622F0)(this, stack);
}

bool slf__set_meter_color__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622F0)(this, stack);
}

bool slf__set_meter_pos__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860000)(this, stack);
}

bool slf__delete_meter__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862300)(this, stack);
}

bool slf__set_meter_visibility__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862320)(this, stack);
}

bool slf__set_meter_label__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862360)(this, stack);
}

bool slf__create_fog__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862430)(this, stack);
}

bool slf__create_fog__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83EE60)(this, stack);
}

bool slf__destroy_fog__finger_of_god__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8624C0)(this, stack);
}

bool slf__terminate_script__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862940)(this, stack);
}

bool slf__terminate_script__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862990)(this, stack);
}

bool slf__format_string__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8629E0)(this, stack);
}

bool slf__show_mission_success_screen__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862AE0)(this, stack);
}

bool slf__close_3d_mission_info_screen__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8245E0)(this, stack);
}

bool slf__lock_minimap__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862C90)(this, stack);
}

bool slf__unlock_minimap__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824600)(this, stack);
}

bool slf__minimap_buildings_invisible__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862CE0)(this, stack);
}

bool slf__minimap_wallmode_disable__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862D30)(this, stack);
}

bool slf__get_collectible_level__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83EED0)(this, stack);
}

bool slf__temporarily_purchase_upgrade__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862D80)(this, stack);
}

bool slf__temporarily_purchase_all_upgrades__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824620)(this, stack);
}

bool slf__is_upgrade_purchased__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862E40)(this, stack);
}

bool slf__suspend_instructional_text__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862E90)(this, stack);
}

bool slf__clear_instructional_text_no_fade__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824660)(this, stack);
}

bool slf__rvb_clear_instructional_text__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824680)(this, stack);
}

bool slf__try_to_purchase_upgrade__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862DB0)(this, stack);
}

bool slf__get_upgrade_id__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862DE0)(this, stack);
}

bool slf__set_rvb_hint_text_debug__num__num__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8632D0)(this, stack);
}

bool slf__is_vibration_option_enabled__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83F4F0)(this, stack);
}

bool slf__hide_rvb_hint_text__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8246A0)(this, stack);
}

bool slf__set_rvb_instructional_text__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862EE0)(this, stack);
}

bool slf__get_root_debug_menu__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83F550)(this, stack);
}

bool slf__is_motion_sensor_option_enabled__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841EB0)(this, stack);
}

bool slf__clear_rvb_instructional_queue__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824680)(this, stack);
}

bool slf__set_rvb_instructional_text_debug__num__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8630D0)(this, stack);
}

bool slf__create_polytube__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83F570)(this, stack);
}

bool slf__enumerate_resources_to_debug_menu__debug_menu_entry__num__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860000)(this, stack);
}

bool slf__clear_rvb_hint_queue__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8246A0)(this, stack);
}

bool slf__set_rvb_hint_text__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_863120)(this, stack);
}

bool slf__widget_3d__is_valid__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F1A0)(this, stack);
}

bool slf__widget_3d__set_shown__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F220)(this, stack);
}

bool slf__widget_3d__set_position__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F280)(this, stack);
}

bool slf__widget_3d__set_position__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F2C0)(this, stack);
}

bool slf__widget_3d__set_z__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F310)(this, stack);
}

bool slf__widget_3d__set_scale__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F370)(this, stack);
}

bool slf__widget_3d__set_scale_offset__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F3B0)(this, stack);
}

bool slf__widget_3d__set_tilt__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F400)(this, stack);
}

bool slf__widget_3d__set_spin__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F440)(this, stack);
}

bool slf__widget_3d__set_rotation__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F480)(this, stack);
}

bool slf__widget_3d__set_max_timer__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F4C0)(this, stack);
}

bool slf__widget_3d__set_color__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F500)(this, stack);
}

bool slf__widget_3d__set_continuous_tilt__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F550)(this, stack);
}

bool slf__widget_3d__set_continuous_spin__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F590)(this, stack);
}

bool slf__widget_3d__set_continuous_rotation__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F5D0)(this, stack);
}

bool slf__widget_3d__set_widescreen_alignment__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F610)(this, stack);
}

bool slf__widget_3d__set_uv_offset__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F650)(this, stack);
}

bool slf__widget_3d__fade_in__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F690)(this, stack);
}

bool slf__widget_3d__fade_out__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F6E0)(this, stack);
}

bool slf__widget_3d__set_alpha__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F730)(this, stack);
}

bool slf__widget_3d__play_anim__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F770)(this, stack);
}

bool slf__widget_3d__play_button_anim__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F840)(this, stack);
}

bool slf__widget_3d__is_anim_playing__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F8F0)(this, stack);
}

bool slf__widget_3d__get_text_id__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F960)(this, stack);
}

bool slf__widget_3d__set_text_shown__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85FA00)(this, stack);
}

bool slf__widget_3d__set_text__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85FA70)(this, stack);
}

bool slf__widget_3d__set_null_text__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85FD10)(this, stack);
}

bool slf__widget_3d__set_text_debug__num__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85FD70)(this, stack);
}

bool slf__widget_3d__set_text_pos__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85FDB0)(this, stack);
}

bool slf__widget_3d__set_text_dimensions__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85FE20)(this, stack);
}

bool slf__widget_3d__set_text_scale__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85FE70)(this, stack);
}

bool slf__widget_3d__set_text_button_scale__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85FEC0)(this, stack);
}

bool slf__widget_3d__set_text_valign__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85FF10)(this, stack);
}

bool slf__widget_3d__set_text_halign__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85FF60)(this, stack);
}

bool slf__widget_3d__set_text_font__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85FFB0)(this, stack);
}

bool slf__widget_3d__set_text_color__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860000)(this, stack);
}

bool slf__widget_3d__set_text_color__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860010)(this, stack);
}

bool slf__widget_3d__set_text_wordwrap__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860070)(this, stack);
}

bool slf__widget_3d__set_text_x_scale__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8600E0)(this, stack);
}

bool slf__widget_3d__set_text_character_gap__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860130)(this, stack);
}

bool slf__widget_3d__set_text_wordwrap_line_gap__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860180)(this, stack);
}

bool slf__widget_3d__set_text_shadow__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8601D0)(this, stack);
}

bool slf__widget_3d__set_text_shadow_alpha__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860240)(this, stack);
}

bool slf__widget_3d__set_text_typewriter__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8602B0)(this, stack);
}

bool slf__widget_3d__is_text_done_updating__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860310)(this, stack);
}

bool slf__widget_3d__end_text_typewriter__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8603A0)(this, stack);
}

bool slf__widget_3d__get_child_id_from_name__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8603E0)(this, stack);
}

bool slf__widget_3d__get_text_id_from_name__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8603E0)(this, stack);
}

bool slf__widget_3d__set_persistent__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_860460)(this, stack);
}

bool slf__finger_of_god__set_visibility__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862510)(this, stack);
}

bool slf__finger_of_god__set_active__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862560)(this, stack);
}

bool slf__finger_of_god__set_pos__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8625B0)(this, stack);
}

bool slf__finger_of_god__set_radius__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8625F0)(this, stack);
}

bool slf__finger_of_god__deactive_fog__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862620)(this, stack);
}

bool slf__finger_of_god__set_poi_icon__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862640)(this, stack);
}

bool slf__finger_of_god__toggle_fog__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862510)(this, stack);
}

bool slf__finger_of_god__fog_requires_senses__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862670)(this, stack);
}

bool slf__finger_of_god__place_fog__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8626C0)(this, stack);
}

bool slf__finger_of_god__place_fog_reappear__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862710)(this, stack);
}

bool slf__finger_of_god__arrived_at_fog__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862760)(this, stack);
}

bool slf__finger_of_god__set_poi_active__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862790)(this, stack);
}

bool slf__finger_of_god__set_gta_widget_active__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8627E0)(this, stack);
}

bool slf__finger_of_god__set_gta_widget_inner_radius__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862830)(this, stack);
}

bool slf__finger_of_god__set_gta_widget_outer_radius__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862870)(this, stack);
}

bool slf__finger_of_god__set_gta_widget_pos_offset__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8628B0)(this, stack);
}

bool slf__finger_of_god__set_gta_widget_render_scale__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_862900)(this, stack);
}
