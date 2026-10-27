#include <cstring>

#include "retail.hh"
#include "game/arch_base.hh"
#include "game/script/chuck_lib_entity2.hh"
#include "treyarch/mash/string_hash.hh"

using namespace treyarch;

// sub_83DAE0
u32 slc_player_entity_t::find_instance(const mash::string &name) const {
    if (!std::strncmp(name.c_str(), "NULL", 0xFFFF))
        return 0;

    string_hash hash;

    hash.initialize(mash::ALLOCATED, name.c_str());

    auto* entity = (arch_base*)retail::sub_640AE0(&hash.source_hash_code, 28, true); // entity_handle_manager::find_entity

    if (!entity)
        return 0;

    return entity->my_handle.value;
}

bool slf__rvb_find_entities_in_radius__dynamic_array__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_882BD0)(this, stack);
}

bool slf__rvb_find_cop_patrol_dude__vector3d__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87A650)(this, stack);
}

bool slf__find_entities_in_radius_LEGACY__dynamic_array__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_882D10)(this, stack);
}

bool slf__get_entities_in_pack__str__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86FFA0)(this, stack);
}

bool slf__find_best_prop_entity__vector3d__vector3d__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_870840)(this, stack);
}

bool slf__find_best_damageable_entity__entity__vector3d__vector3d__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_870940)(this, stack);
}

bool slf__find_best_damageable_entity__entity__vector3d__vector3d__num__num__num__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_883000)(this, stack);
}

bool slf__find_best_manip_obj__vector3d__vector3d__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_870A90)(this, stack);
}

bool slf__entity__set_path_graph__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_869FC0)(this, stack);
}

bool slf__entity__ai_can_carry__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86A0A0)(this, stack);
}

bool slf__entity__ai_carry_pickup__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86A1B0)(this, stack);
}

bool slf__entity__ai_carry_putdown__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86A2A0)(this, stack);
}

bool slf__entity__ai_is_carrying__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86A3B0)(this, stack);
}

bool slf__entity__ai_is_carried__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86A470)(this, stack);
}

bool slf__entity__set_ai_subclass__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86A520)(this, stack);
}

bool slf__entity__set_render_scale__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86A560)(this, stack);
}

bool slf__entity__wait_change_render_scale__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86A5A0)(this, stack);
}

bool slf__entity__set_render_color__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86A840)(this, stack);
}

bool slf__entity__set_render_alpha__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86A930)(this, stack);
}

bool slf__entity__get_render_color__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86A9E0)(this, stack);
}

bool slf__entity__get_render_scale__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86AAD0)(this, stack);
}

bool slf__entity__get_render_alpha__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86AB60)(this, stack);
}

bool slf__entity__wait_change_render_color__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86ABF0)(this, stack);
}

bool slf__entity__set_scale__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86AF70)(this, stack);
}

bool slf__player_entity__get_player_num__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86B180)(this, stack);
}

bool slf__player_entity__set_player_num__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86B240)(this, stack);
}

bool slf__player_entity__get_player_vec__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86B2A0)(this, stack);
}

bool slf__player_entity__is_button_enabled__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86B360)(this, stack);
}

bool slf__player_entity__set_button_enabled__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86B400)(this, stack);
}

bool slf__player_entity__set_buttons_enabled__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86B470)(this, stack);
}

bool slf__player_entity__set_swing_override_anchor__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86B4F0)(this, stack);
}

bool slf__player_entity__set_zip_override_anchor__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86B540)(this, stack);
}

bool slf__player_entity__clear_buttons__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86B5B0)(this, stack);
}

bool slf__player_entity__clear_controller__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86B600)(this, stack);
}

bool slf__player_entity__set_override_mode__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86B640)(this, stack);
}

bool slf__player_entity__override_button__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86B6B0)(this, stack);
}

bool slf__player_entity__override_command__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86B710)(this, stack);
}

bool slf__player_entity__set_button__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86B770)(this, stack);
}

bool slf__player_entity__set_axis__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86B7D0)(this, stack);
}

bool slf__player_entity__set_command__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86B840)(this, stack);
}

bool slf__player_entity__set_axis_enabled__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86B8B0)(this, stack);
}

bool slf__player_entity__set_axis_positive_enabled__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86B920)(this, stack);
}

bool slf__player_entity__set_axis_negative_enabled__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86B9C0)(this, stack);
}

bool slf__player_entity__set_axis_direction_enabled__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86BA60)(this, stack);
}

bool slf__entity__get_custom_ent__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86BB00)(this, stack);
}

bool slf__entity__set_custom_ent__str__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86BBF0)(this, stack);
}

bool slf__entity__has_entity_in_custom_list__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86BC60)(this, stack);
}

bool slf__entity__add_entity_to_custom_entity_list__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86BCF0)(this, stack);
}

bool slf__entity__remove_entity_from_custom_entity_list__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86BD40)(this, stack);
}

bool slf__entity__get_first_entity_from_custom_entity_list__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86BD90)(this, stack);
}

bool slf__entity__get_custom_entity_list_count__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86BE20)(this, stack);
}

bool slf__entity__get_custom_num__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86BEB0)(this, stack);
}

bool slf__entity__get_optional_custom_num__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86BF60)(this, stack);
}

bool slf__entity__set_custom_num__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86C020)(this, stack);
}

bool slf__entity__get_custom_vec__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86C090)(this, stack);
}

bool slf__entity__get_optional_custom_vec__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86C170)(this, stack);
}

bool slf__entity__set_custom_vec__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86C230)(this, stack);
}

bool slf__entity__get_custom_str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86C2A0)(this, stack);
}

bool slf__entity__get_optional_custom_str__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86C380)(this, stack);
}

bool slf__entity__set_custom_str__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86C450)(this, stack);
}

bool slf__entity__get_hit_points__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86C4C0)(this, stack);
}

bool slf__entity__set_hit_points__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86C550)(this, stack);
}

bool slf__entity__set_hit_points_with_signal__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86C5B0)(this, stack);
}

bool slf__entity__get_max_hit_points__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86C610)(this, stack);
}

bool slf__entity__get_damage_dealt__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86C6A0)(this, stack);
}

bool slf__entity__set_damage_dealt__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86C730)(this, stack);
}

bool slf__entity__set_web_subdue_recover_duration__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86C790)(this, stack);
}

bool slf__entity__cancel_web_subdue__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86C7E0)(this, stack);
}

bool slf__entity__get_web_subdued__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86C830)(this, stack);
}

bool slf__entity__get_web_hit_points__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86C8F0)(this, stack);
}

bool slf__entity__set_web_hit_points__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86C9A0)(this, stack);
}

bool slf__entity__get_max_web_hit_points__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86C9F0)(this, stack);
}

bool slf__entity__set_armor_points__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86CAA0)(this, stack);
}

bool slf__entity__set_max_armor_points__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87A5B0)(this, stack);
}

bool slf__entity__get_armor_points__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86CAF0)(this, stack);
}

bool slf__entity__get_max_armor_points__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86CB80)(this, stack);
}

bool slf__entity__set_max_hit_points__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87A600)(this, stack);
}

bool slf__entity__set_damage_modifier__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86CC10)(this, stack);
}

bool slf__entity__set_damage_lock_threshold__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86CC60)(this, stack);
}

bool slf__entity__get_damage_lock_threshold__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86CCB0)(this, stack);
}

bool slf__entity__set_immune_to_death_from_damage__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86CD40)(this, stack);
}

bool slf__entity__get_immune_to_death_from_damage__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86CDC0)(this, stack);
}

bool slf__entity__get_stun_hit_points__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86CE90)(this, stack);
}

bool slf__entity__set_stun_hit_points__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86CF40)(this, stack);
}

bool slf__entity__get_stun_restore_hit_points__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86CFA0)(this, stack);
}

bool slf__entity__set_stun_restore_hit_points__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86D050)(this, stack);
}

bool slf__entity__get_stun_recover_duration__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86D0B0)(this, stack);
}

bool slf__entity__set_stun_recover_duration__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86D160)(this, stack);
}

bool slf__entity__get_stun_recover_remaining__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86D1B0)(this, stack);
}

bool slf__entity__set_stun_recover_remaining__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86D260)(this, stack);
}

bool slf__entity__is_stunned__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86D2B0)(this, stack);
}

bool slf__entity__is_life_bar_hidden__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86D360)(this, stack);
}

bool slf__entity__set_life_bar_hidden__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86D410)(this, stack);
}

bool slf__entity__set_use_distance_based_invulnerability__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86D470)(this, stack);
}

bool slf__entity__set_webcast_level__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86D4F0)(this, stack);
}

bool slf__entity__set_webcast_style__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86D540)(this, stack);
}

bool slf__entity__get_manip_thrower__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86D590)(this, stack);
}

bool slf__entity__get_manip_throw_velocity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86D610)(this, stack);
}

bool slf__entity__set_webcast_enabled__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86D6A0)(this, stack);
}

bool slf__entity__set_webcast_sync_enabled__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86D720)(this, stack);
}

bool slf__entity__set_webcast_counter_chance__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86D780)(this, stack);
}

bool slf__entity__set_webcast_counter_scale__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86D7F0)(this, stack);
}

bool slf__entity__get_entity_force_attack_multiplier__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86D840)(this, stack);
}

bool slf__entity__set_entity_force_attack_multiplier__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86D8D0)(this, stack);
}

bool slf__entity__get_entity_force_react_multiplier__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86D920)(this, stack);
}

bool slf__entity__set_entity_force_react_multiplier__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86D9B0)(this, stack);
}

bool slf__entity__get_manip_damage__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86DA00)(this, stack);
}

bool slf__entity__manip_update_for_grab__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86DA90)(this, stack);
}

bool slf__entity__set_throwable__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86DB30)(this, stack);
}

bool slf__entity__is_throwable__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86DBD0)(this, stack);
}

bool slf__entity__set_always_upright__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86DCB0)(this, stack);
}

bool slf__entity__is_always_upright__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86DD20)(this, stack);
}

bool slf__entity__set_manip_allow_pickup__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86DDD0)(this, stack);
}

bool slf__entity__set_manip_allow_putdown__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86DE50)(this, stack);
}

bool slf__entity__get_manip_allow_pickup__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86DED0)(this, stack);
}

bool slf__entity__get_manip_allow_putdown__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86DF80)(this, stack);
}

bool slf__entity__set_manip_allow_auto_pickup__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86E030)(this, stack);
}

bool slf__entity__set_manip_allow_auto_putdown__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86E0A0)(this, stack);
}

bool slf__entity__get_manip_allow_auto_pickup__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86E110)(this, stack);
}

bool slf__entity__get_manip_allow_auto_putdown__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86E1D0)(this, stack);
}

bool slf__entity__set_allow_manage_standing__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86E290)(this, stack);
}

bool slf__entity__set_allow_swimming__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86E310)(this, stack);
}

bool slf__entity__set_mass__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86E390)(this, stack);
}

bool slf__entity__set_slide_factor__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86E3E0)(this, stack);
}

bool slf__entity__set_bounce_factor__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86E430)(this, stack);
}

bool slf__entity__set_bouncy__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86E480)(this, stack);
}

bool slf__entity__set_sticky__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86E500)(this, stack);
}

bool slf__entity__is_physics_enabled__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86E580)(this, stack);
}

bool slf__entity__set_physics_enabled__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86E600)(this, stack);
}

bool slf__entity__is_physics_suspended__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86E670)(this, stack);
}

bool slf__entity__set_physics_suspended__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86E700)(this, stack);
}

bool slf__entity__set_gravity_enabled__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86E770)(this, stack);
}

bool slf__entity__get_gravity_multiplier__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86E7E0)(this, stack);
}

bool slf__entity__set_gravity_multiplier__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86E870)(this, stack);
}

bool slf__entity__set_allow_biped_physics__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86E8C0)(this, stack);
}

bool slf__entity__set_angular_velocity__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86E940)(this, stack);
}

bool slf__entity__set_time_dilation__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86E9A0)(this, stack);
}

bool slf__entity__get_time_dilation__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86EA20)(this, stack);
}

bool slf__entity__set_time_mode__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86EAB0)(this, stack);
}

bool slf__entity__create_script_data_interface__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86EB80)(this, stack);
}

bool slf__entity__has_script_data_interface__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86EBC0)(this, stack);
}

bool slf__entity__create_physical_interface__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86EC10)(this, stack);
}

bool slf__entity__create_damage_interface__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86EC60)(this, stack);
}

bool slf__entity__has_damage_interface__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86ECA0)(this, stack);
}

bool slf__entity__has_player_interface__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86ECF0)(this, stack);
}

bool slf__entity__physical_ifc_apply_force__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86ED40)(this, stack);
}

bool slf__entity__physical_ifc_apply_force_at_pos__vector3d__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86EF80)(this, stack);
}

bool slf__entity__physical_ifc_is_effectively_standing__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86F1F0)(this, stack);
}

bool slf__entity__physical_ifc_manage_standing__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86F280)(this, stack);
}

bool slf__entity__physical_ifc_cancel_all_velocity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86F2F0)(this, stack);
}

bool slf__entity__physical_ifc_is_biped_physics_running__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86F330)(this, stack);
}

bool slf__entity__physical_ifc_is_prop_physics_running__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86F3D0)(this, stack);
}

bool slf__entity__physical_ifc_is_prop_physics_stable__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86F440)(this, stack);
}

bool slf__entity__physical_ifc_stop_biped_physics__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86F500)(this, stack);
}

bool slf__entity__physical_ifc_set_biped_physics_is_weapon__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86F550)(this, stack);
}

bool slf__entity__physical_ifc_set_pendulum__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87C980)(this, stack);
}

bool slf__entity__physical_ifc_set_pendulum__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87CB40)(this, stack);
}

bool slf__entity__physical_ifc_clear_pendulum__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87CCF0)(this, stack);
}

bool slf__entity__set_pendulum_attach_limb__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87CD60)(this, stack);
}

bool slf__entity__get_num_of_active_pendulums__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86F640)(this, stack);
}

bool slf__entity__enable_collisions__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86F6B0)(this, stack);
}

bool slf__entity__collisions_enabled__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86F710)(this, stack);
}

bool slf__entity__enable_character_collisions__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86F7A0)(this, stack);
}

bool slf__entity__character_collisions_enabled__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86F810)(this, stack);
}

bool slf__entity__get_name__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86F8B0)(this, stack);
}

bool slf__entity__set_special_target__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86F900)(this, stack);
}

bool slf__entity__set_allow_lockon__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86F950)(this, stack);
}

bool slf__entity__is_valid__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86F9A0)(this, stack);
}

bool slf__entity__is_alive__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86FA10)(this, stack);
}

bool slf__entity__set_immobile__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86FA60)(this, stack);
}

bool slf__entity__set_really_immobile__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86FAD0)(this, stack);
}

bool slf__entity__set_auto_prop_physics_activate__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86FB30)(this, stack);
}

bool slf__entity__set_actor_debug_text__str__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86FC10)(this, stack);
}

bool slf__entity__physical_ifc_start_prop_physics__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86FD40)(this, stack);
}

bool slf__entity__physical_ifc_start_prop_physics__vector3d__num__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86FE10)(this, stack);
}

bool slf__entity__physical_ifc_stop_prop_physics__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86FF30)(this, stack);
}

bool slf__entity__debug_only_disable_weapon_interface__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86FF80)(this, stack);
}

bool slf__entity__get_animation_names__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86FF80)(this, stack);
}

bool slf__entity__get_weapon_names__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_882E00)(this, stack);
}

bool slf__entity__get_marker_names__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_882F40)(this, stack);
}

bool slf__entity__ai_enable_weapon__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86FFE0)(this, stack);
}

bool slf__entity__ai_weapon_available__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_870090)(this, stack);
}

bool slf__entity__ai_enable_all_weapons__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_870170)(this, stack);
}

bool slf__entity__holster_weapon__str__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_870220)(this, stack);
}

bool slf__entity__draw_weapon__str__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8702E0)(this, stack);
}

bool slf__entity__ai_weapon_sys_busy__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8703A0)(this, stack);
}

bool slf__entity__get_ai_weapon_state__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_870460)(this, stack);
}

bool slf__entity__fire_weapon__str__num__entity__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_870550)(this, stack);
}

bool slf__entity__get_weapon_entity__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_870610)(this, stack);
}

bool slf__entity__ai_reset_morale__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_870700)(this, stack);
}

bool slf__entity__ai_set_ignore_limbo__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_870770)(this, stack);
}

bool slf__entity__set_walkable__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8707F0)(this, stack);
}

bool slf__entity__physical_ifc_set_ballistic_target__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_870B80)(this, stack);
}

bool slf__entity__manip_obj_ifc_throw_it__entity__vector3d__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_870BC0)(this, stack);
}

bool slf__entity__manip_obj_ifc_throw_it_velocity__entity__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_870C30)(this, stack);
}

bool slf__entity__manip_obj_ifc_throw_it_velocity_and_rotation__entity__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_870C90)(this, stack);
}

bool slf__entity__manip_obj_ifc_throw_it_with_speed__entity__entity__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_870E30)(this, stack);
}

bool slf__entity__manip_obj_ifc_disarm__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_870EB0)(this, stack);
}

bool slf__entity__drop_manip_obj__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_870EF0)(this, stack);
}

bool slf__entity__orientation_floor__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_870FB0)(this, stack);
}

bool slf__entity__orientation_wall__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_871040)(this, stack);
}

bool slf__entity__orientation_ceiling__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8710F0)(this, stack);
}

bool slf__entity__orbit__vector3d__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_871190)(this, stack);
}

bool slf__entity__set_puppet_goto_destination__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_871300)(this, stack);
}

bool slf__entity__set_puppet_goto_destinations__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87A810)(this, stack);
}

bool slf__entity__puppet_set_movement_info__dynamic_array__num__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_871390)(this, stack);
}

bool slf__entity__puppet_setup_for_goto_complex__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_871890)(this, stack);
}

bool slf__entity__puppet_clear_callbacks__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_871AF0)(this, stack);
}

bool slf__entity__enable_zlock__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_871D90)(this, stack);
}

bool slf__entity__set_zlock_ranges__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_871E10)(this, stack);
}

bool slf__entity__reset_zlock_ranges__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_871E60)(this, stack);
}

bool slf__entity__zlock_ignore_los__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_871EA0)(this, stack);
}

bool slf__entity__set_camera_priority_boost__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_871F00)(this, stack);
}

bool slf__entity__set_entity_targeting_weight__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_871F40)(this, stack);
}

bool slf__entity__add_fakir_entity__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_871FC0)(this, stack);
}

bool slf__entity__add_fakir_array__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_872000)(this, stack);
}

bool slf__entity__remove_fakir_entity__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8720B0)(this, stack);
}

bool slf__entity__remove_fakir_array__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8720F0)(this, stack);
}

bool slf__entity__get_best_turret_target__num__num__num__num__vector3d__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87CDB0)(this, stack);
}
