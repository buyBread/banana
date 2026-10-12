#include "retail.hh"
#include "treyarch/game/script/chuck_lib_widget.hh"

using namespace treyarch;

bool slf__get_timer_widget__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83EC10)(this, stack);
}

bool slf__is_blackscreen__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83EC40)(this, stack);
}

bool slf__blackscreen_on__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E280)(this, stack);
}

bool slf__blackscreen_on_from_cutscene__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E2C0)(this, stack);
}

bool slf__blackscreen_on_from_cutscene_no_freeze__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E300)(this, stack);
}

bool slf__blackscreen_off__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E340)(this, stack);
}

bool slf__letterbox__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E380)(this, stack);
}

bool slf__add_tip__entity__str__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E3B0)(this, stack);
}

bool slf__remove_tip__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E410)(this, stack);
}

bool slf__activate_stealth_meter__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E440)(this, stack);
}

bool slf__activate_follow_meter__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E490)(this, stack);
}

bool slf__activate_distance_widget__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E4D0)(this, stack);
}

bool slf__activate_distance_widget__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_823CA0)(this, stack);
}

bool slf__set_distance_widget_min_distance__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E510)(this, stack);
}

bool slf__set_distance_widget_max_distance__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E540)(this, stack);
}

bool slf__flip_distance_widget_orientation_with_hot_cold__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E570)(this, stack);
}

bool slf__update_distance_widget__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E5B0)(this, stack);
}

bool slf__deactivate_distance_widget__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_823CC0)(this, stack);
}

bool slf__set_distance_bar_near_tint__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E5D0)(this, stack);
}

bool slf__set_distance_bar_mid_tint__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E5F0)(this, stack);
}

bool slf__set_distance_bar_far_tint__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E610)(this, stack);
}

bool slf__reset_distance_bar_tints__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_823D00)(this, stack);
}

bool slf__start_exitable_state__num__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622F0)(this, stack);
}

bool slf__set_mission_text__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E630)(this, stack);
}

bool slf__set_mission_text_debug__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869EB0)(this, stack);
}

bool slf__remove_map_poi__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E910)(this, stack);
}

bool slf__add_zoom_map_poi__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E950)(this, stack);
}

bool slf__set_zoom_map_poi_shown__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E9B0)(this, stack);
}

bool slf__add_zoom_map_safe_zone_marker__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E9F0)(this, stack);
}

bool slf__add_zoom_map_safe_zone_marker__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85EA40)(this, stack);
}

bool slf__remove_zoom_map_safe_zone_marker__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E910)(this, stack);
}

bool slf__set_zoom_map_safe_zone_marker_shown__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E9B0)(this, stack);
}

bool slf__set_zoom_map_safe_zone_marker_state__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85EA80)(this, stack);
}

bool slf__enable_mini_map__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85EAB0)(this, stack);
}

bool slf__disable_zoom_map__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85EAF0)(this, stack);
}

bool slf__bring_up_zoom_map__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85EB40)(this, stack);
}

bool slf__kill_zoom_map__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_823D20)(this, stack);
}

bool slf__is_zoom_map_up__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83EC80)(this, stack);
}

bool slf__snap_to_zoom_map_point__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85EB90)(this, stack);
}

bool slf__set_zoom_map_help_text__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85EBC0)(this, stack);
}

bool slf__feature_current_zoom_poi__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85ECC0)(this, stack);
}

bool slf__is_a_zoom_poi_featured__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83ECD0)(this, stack);
}

bool slf__disable_zoom_map_movement__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85ED00)(this, stack);
}

bool slf__bring_up_dialog_box_debug__str__num__num__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85EDF0)(this, stack);
}

bool slf__begin_movie__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85EE20)(this, stack);
}

bool slf__offscreen_indicator_add__entity__num__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85EE40)(this, stack);
}

bool slf__set_zoom_map_poi_meta_title__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85EE60)(this, stack);
}

bool slf__set_zoom_map_poi_meta_text__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85EF00)(this, stack);
}

bool slf__slide_in_ally_health_widget__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824100)(this, stack);
}

bool slf__slide_out_ally_health_widget__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824120)(this, stack);
}

bool slf__set_ally_health_icon__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F010)(this, stack);
}

bool slf__set_ally_health_meter_value__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F040)(this, stack);
}

bool slf__set_ally_health_icon__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F070)(this, stack);
}

bool slf__clear_ally_health_widget__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83ED30)(this, stack);
}

bool slf__slide_in_boss_health_widget__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824140)(this, stack);
}

bool slf__slide_out_boss_health_widget__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_824170)(this, stack);
}

bool slf__set_boss_health_icon__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F0B0)(this, stack);
}

bool slf__set_boss_health_icon__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F0E0)(this, stack);
}

bool slf__clear_boss_health_widget__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83ED60)(this, stack);
}

bool slf__disable_grunt_widget__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8241A0)(this, stack);
}

bool slf__enable_grunt_widget__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8241C0)(this, stack);
}

bool slf__enable_summon_power_mode__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85F120)(this, stack);
}

bool slf__timer_widget__is_valid__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85DD30)(this, stack);
}

bool slf__timer_widget__get_time_modifier__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85DD80)(this, stack);
}

bool slf__timer_widget__set_time_modifier__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85DDD0)(this, stack);
}

bool slf__timer_widget__get_time_left__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85DDF0)(this, stack);
}

bool slf__timer_widget__is_timer_halted__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85DE40)(this, stack);
}

bool slf__timer_widget__set_time_left__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85DEA0)(this, stack);
}

bool slf__timer_widget__freeze__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85DEC0)(this, stack);
}

bool slf__timer_widget__run__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85DEE0)(this, stack);
}

bool slf__timer_widget__show_timer__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85DF00)(this, stack);
}

bool slf__timer_widget__show_timer__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85DF20)(this, stack);
}

bool slf__timer_widget__show_timer_corner__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85DF50)(this, stack);
}

bool slf__timer_widget__hide_timer__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85DF80)(this, stack);
}

bool slf__timer_widget__inc_time_left__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85DFA0)(this, stack);
}

bool slf__timer_widget__count_up__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85DFC0)(this, stack);
}

bool slf__timer_widget__count_down__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85DFE0)(this, stack);
}

bool slf__timer_widget__turn_green__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E000)(this, stack);
}

bool slf__timer_widget__turn_green__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E040)(this, stack);
}

bool slf__timer_widget__turn_yellow__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E080)(this, stack);
}

bool slf__timer_widget__turn_yellow__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E0C0)(this, stack);
}

bool slf__timer_widget__turn_red__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E100)(this, stack);
}

bool slf__timer_widget__turn_red__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E140)(this, stack);
}

bool slf__timer_widget__turn_flashing__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E180)(this, stack);
}

bool slf__timer_widget__run_when_hidden__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E1A0)(this, stack);
}

bool slf__timer_widget__display_hundredths__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E1E0)(this, stack);
}

bool slf__timer_widget__display_thousandths__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E220)(this, stack);
}

bool slf__timer_widget__set_z__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_85E260)(this, stack);
}
