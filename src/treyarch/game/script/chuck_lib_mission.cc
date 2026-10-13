#include "retail.hh"
#include "treyarch/chuck/vm/script_executable.hh"
#include "treyarch/chuck/vm/script_object.hh"
#include "treyarch/chuck/vm/vm_thread.hh"
#include "treyarch/game/mission/mission_manager.hh"
#include "treyarch/game/script/chuck_lib_mission.hh"

using namespace treyarch;

bool slf__terminate_script__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8570B0)(this, stack);
}

bool slf__set_loading_hint_stage__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_857100)(this, stack);
}

bool slf__get_running_script_index__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E140)(this, stack);
}

bool slf__get_mission_index__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_857120)(this, stack);
}

bool slf__get_total_times_script_has_been_successfully_completed__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E170)(this, stack);
}

bool slf__mission_icon_punched__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_857160)(this, stack);
}

bool slf__is_mission_eligible__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8571C0)(this, stack);
}

bool slf__get_mission_caption_index__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_857210)(this, stack);
}

// sub_857280
bool slf__setup_mission_instance_parameters__mission_district_instance_base__t::operator()(chuck::vm::vm_stack &stack) {
    auto* parameters = (chuck::vm::script_instance*)stack.pop_uint();

    if (mission_manager::inst()->setup_mission_instance_parameters(parameters))
        return true;

    // suspends the calling executable
    ++stack.thread->inst->parent->parent->suspend_count;

    return false;
}

bool slf__is_mission_running__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E1A0)(this, stack);
}

bool slf__get_registry_value__str__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8572C0)(this, stack);
}

bool slf__get_registry_value__uint__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_857370)(this, stack);
}

bool slf__set_registry_value__uint__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_857420)(this, stack);
}

bool slf__get_registry_value__uint__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8574B0)(this, stack);
}

bool slf__save_checkpoint__vector3d__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_857570)(this, stack);
}

bool slf__get_checkpoint__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E1F0)(this, stack);
}

bool slf__get_overrride_checkpoint__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E230)(this, stack);
}

bool slf__is_debug_force_script_running__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841EB0)(this, stack);
}

bool slf__is_debug_force_script_retried__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_841EB0)(this, stack);
}

bool slf__game_time_advance__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8575B0)(this, stack);
}

bool slf__set_time_of_day_rate__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8575E0)(this, stack);
}

bool slf__game_time_get_seconds_since_midnight__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E280)(this, stack);
}

bool slf__internal_mission_manager_set_current_act__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_857620)(this, stack);
}

bool slf__mission_manager_set_is_quitable__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_857640)(this, stack);
}

bool slf__mission_manager_is_act_transitioning__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E2B0)(this, stack);
}

bool slf__mission_manager_get_current_act__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E310)(this, stack);
}

bool slf__mission_manager_get_current_act_internal__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E340)(this, stack);
}

bool slf__set_mission_meta_text__widget_3d__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_857680)(this, stack);
}

bool slf__set_mission_finished_screen_has_appeared__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8576B0)(this, stack);
}

bool slf__handle_finished_dialog_selection__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_823B80)(this, stack);
}

bool slf__hide_blackscreen_after_unload__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_857710)(this, stack);
}

bool slf__get_current_key_position__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E370)(this, stack);
}

bool slf__test__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_823B90)(this, stack);
}

bool slf__register_quest_giver__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_857840)(this, stack);
}

bool slf__unregister_quest_giver__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_857870)(this, stack);
}

bool slf__quest_event__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8578A0)(this, stack);
}

bool slf__give_quest__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_857910)(this, stack);
}

bool slf__attach_conversation__entity__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_857980)(this, stack);
}

bool slf__attach_quest_conversation__entity__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_857A00)(this, stack);
}

bool slf__completed_quest_count__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E3A0)(this, stack);
}

bool slf__get_completed_quest__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_857A80)(this, stack);
}

bool slf__available_quest_count__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E3D0)(this, stack);
}

bool slf__get_available_quest__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_857B10)(this, stack);
}

bool slf__get_questgiver_name__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_857BA0)(this, stack);
}

bool slf__get_questgiver_loc__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_857C50)(this, stack);
}

bool slf__city_life_failed_message__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_857F60)(this, stack);
}

bool slf__do_conversation_wait__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_857CD0)(this, stack);
}

bool slf__do_dialog_wait__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E400)(this, stack);
}

bool slf__update_progression__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_823BB0)(this, stack);
}

bool slf__get_progression_level__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E4E0)(this, stack);
}

bool slf__city_life_complete_message__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_857D70)(this, stack);
}

bool slf__city_life_xp_message__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_857EC0)(this, stack);
}

bool slf__city_life_in_progress_message__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_857F60)(this, stack);
}

bool slf__has_succeeded_city_life__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E590)(this, stack);
}

bool slf__set_max_active_spawn_items__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_857FD0)(this, stack);
}

bool slf__update_point_of_interest__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858030)(this, stack);
}

bool slf__set_spawn_placement_figures__num__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858090)(this, stack);
}

bool slf__push_spawn_item_during_mission__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8580E0)(this, stack);
}

bool slf__pop_spawn_item_during_mission__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858130)(this, stack);
}

bool slf__register_entity_with_spawn_manager__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858180)(this, stack);
}

bool slf__unregister_entity_with_spawn_manager__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858280)(this, stack);
}

bool slf__begin_populate_mission_with_entities__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E5C0)(this, stack);
}

bool slf__end_populate_mission_with_entities__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E5E0)(this, stack);
}

bool slf__force_populate_mission_cleanup__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E600)(this, stack);
}

bool slf__set_street_percent_override__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8582C0)(this, stack);
}

bool slf__get_entities__dynamic_array__num__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_880D60)(this, stack);
}

bool slf__get_spawn_position__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E630)(this, stack);
}

bool slf__has_optional_data__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858320)(this, stack);
}

bool slf__get_vector_data__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858390)(this, stack);
}

bool slf__get_str_data__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858440)(this, stack);
}

bool slf__get_num_data__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8584D0)(this, stack);
}

bool slf__get_vector_data_array__str__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_880EA0)(this, stack);
}

bool slf__get_str_data_array__str__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_880FB0)(this, stack);
}

bool slf__get_num_data_array__str__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8810A0)(this, stack);
}

bool slf__has_optional_data__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E6C0)(this, stack);
}

bool slf__add_no_spawn_zone__str__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858570)(this, stack);
}

bool slf__add_no_spawn_zone__str__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8585D0)(this, stack);
}

bool slf__remove_no_spawn_zone__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858670)(this, stack);
}

bool slf__is_point_within_no_spawn_zone__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8586B0)(this, stack);
}

bool slf__add_debris_no_spawn_zone__str__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858710)(this, stack);
}

bool slf__add_debris_no_spawn_zone__str__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858760)(this, stack);
}

bool slf__add_arbitrary_region__str__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8587F0)(this, stack);
}

bool slf__add_arbitrary_region__str__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858850)(this, stack);
}

bool slf__remove_arbitrary_region__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8588F0)(this, stack);
}

bool slf__is_point_within_arbitrary_region__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858930)(this, stack);
}

bool slf__has_persistence_data__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E730)(this, stack);
}

bool slf__start_persistence_data_save__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858990)(this, stack);
}

bool slf__save_persistence_vector3d__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8589D0)(this, stack);
}

bool slf__save_persistence_vector3d_array__str__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858A30)(this, stack);
}

bool slf__get_persistence_vector3d__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858AE0)(this, stack);
}

bool slf__get_persistence_vector3d_array__str__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_881190)(this, stack);
}

bool slf__save_persistence_str__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858B70)(this, stack);
}

bool slf__save_persistence_str_array__str__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858C00)(this, stack);
}

bool slf__get_persistence_str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858CB0)(this, stack);
}

bool slf__get_persistence_str_array__str__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_881250)(this, stack);
}

bool slf__save_persistence_num__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858D30)(this, stack);
}

bool slf__save_persistence_num_array__str__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858D80)(this, stack);
}

bool slf__get_persistence_num__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858E30)(this, stack);
}

bool slf__get_persistence_num_array__str__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_881310)(this, stack);
}

bool slf__end_persistance_data_save__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E780)(this, stack);
}

bool slf__force_despawn_city_life__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E7B0)(this, stack);
}

bool slf__toggle_override_spawn_distance_in_mission__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8622F0)(this, stack);
}

bool slf__toggle_override_spawn_pacing_in_mission__num__num__num__num__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858EB0)(this, stack);
}

bool slf__get_all_city_life_of_category__str__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_858F90)(this, stack);
}

bool slf__get_all_categories_on_city_life__num__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_859030)(this, stack);
}

bool slf__create_category_for_mission__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8590E0)(this, stack);
}

bool slf__register_important_city_life_entity__str__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_859130)(this, stack);
}

bool slf__get_important_city_life_entity__num__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_859180)(this, stack);
}

bool slf__is_valid_spawn__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_859210)(this, stack);
}

bool slf__save_safe_zone_wall_percentages__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_859280)(this, stack);
}

bool slf__get_safe_zone_wall_percentages__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_859310)(this, stack);
}

bool slf__set_safe_zone_wall_index__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8593A0)(this, stack);
}

bool slf__get_safe_zone_wall_index__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E7E0)(this, stack);
}

bool slf__end_summon__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E830)(this, stack);
}

bool slf__summon_deduct_meter_for_special_attack__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E850)(this, stack);
}

bool slf__mission_force_summon_availability__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_859400)(this, stack);
}

bool slf__can_summon__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E870)(this, stack);
}

bool slf__call_in_summon__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_859460)(this, stack);
}

bool slf__toggle_summon_ability_in_script__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8594A0)(this, stack);
}

bool slf__get_is_summon_ability_script_toggled__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E8C0)(this, stack);
}

bool slf__get_spawn_point_point_data__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8594E0)(this, stack);
}

bool slf__is_in_stars_mode__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E900)(this, stack);
}

bool slf__get_neighborhood_center__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E950)(this, stack);
}

bool slf__get_meta_neighborhood_center__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83E9D0)(this, stack);
}

bool slf__get_current_flagsets__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_859570)(this, stack);
}

bool slf__flag_always_on_for_despawn__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83EA50)(this, stack);
}

bool slf__get_spawn_position__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_859600)(this, stack);
}

bool slf__set_current_stored_summon_actor__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8596B0)(this, stack);
}

bool slf__despawn_and_lock_quest_giver_in_mission__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83EA80)(this, stack);
}

bool slf__set_collectibles_disabled__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8596E0)(this, stack);
}

bool slf__is_summon_summoned__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_859720)(this, stack);
}

bool slf__is_cross_neighborhood_spawn_active__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83EAA0)(this, stack);
}

bool slf__is_spidey_near_quest_giver__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83EB10)(this, stack);
}
