#pragma once

#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/math/algebra/vector3d.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace als {
    class animation_logic_system;
    class state_machine;

    using param_id = u32;

    // ids below 161 are set by the game, the rest are computed by the als itself; retail's names, each range ending where the next starts
    enum external_float_parameter_types : u32 {
        param_desired_speed,
        param_desired_speed_pct,
        param_turn_rate,
        param_turn_rate_multiplier,
        param_turn_rate_accel_time,
        param_wallsprint_trans_type,
        param_crawl_type,
        param_crawl_transition_type,
        param_crawl_approach_angle,
        param_crawl_approach_vert_velocity,
        param_crawl_corner_angle,
        param_jump_type,
        param_combat_desired_branch,
        param_combat_desired_move,
        param_combat_num_targets,
        param_combat_next_num_targets,
        param_combat_same_primary_target,
        param_combat_moves_with_same_primary_target,
        param_combat_target_angle,
        param_combat_target_facing_angle,
        param_combat_target_up_angle,
        param_combat_target_tilt_angle,
        param_combat_secondary_target_angle,
        param_combat_active,
        param_combat_button_held,
        param_combat_button_held_through_state,
        param_combat_script_desired_move,
        param_combat_dodge_type,
        param_combat_block_type,
        param_combat_hit_target,
        param_combat_chain_size,
        param_combat_secondary_chain_size,
        param_combat_attack_applied,
        param_combat_attack_missed,
        param_combat_prev_attack_missed,
        param_combat_attack_dodged,
        param_combat_prev_attack_dodged,
        param_combat_attack_blocked,
        param_combat_prev_attack_blocked,
        param_combat_attack_connected,
        param_combat_prev_attack_connected,
        param_combat_primary_target_stunned,
        param_combat_next_primary_target_stunned,
        param_combat_strength_test_status,
        param_combat_primary_target_type,
        param_combat_primary_is_enemy,
        param_combat_next_primary_target_type,
        param_combat_next_primary_target_seat,
        param_combat_next_target_angle,
        param_combat_next_secondary_target_angle,
        param_combat_next_target_stick_angle,
        param_combat_recovery_time,
        param_combat_primary_target_allow_grapple,
        param_combat_next_primary_target_allow_grapple,
        param_combat_primary_target_allow_beatdown,
        param_combat_next_primary_target_allow_beatdown,
        param_combat_primary_target_allow_momentum,
        param_combat_next_primary_target_allow_moment,
        param_combat_primary_target_allow_old_web_fir,
        param_combat_primary_target_allow_friendly_we,
        param_combat_first_target_hit_this_frame,
        param_combat_target_in_blacksuit_mode,
        param_combat_target_swinging,
        param_combat_target_swing_attack,
        param_webcast_approach_unresolved,
        param_webcast_approach_junction,
        param_webcast_approach_hit_normal,
        param_webcast_approach_hit_strong,
        param_webcast_approach_fail_block,
        param_webcast_approach_fail_counter,
        param_webcast_approach_input_branch,
        param_webcast_approach_dir,
        param_webcast_approach_counter_pct,
        param_webcast_approach_disable_sync,
        param_combat_target_state,
        param_combat_target_airborne,
        param_webcast_hit_chain_number,
        param_webcast_crit_chain_number,
        param_dash_active,
        param_strength_test_status,
        param_hit_react_level,
        param_air_hit_react_level,
        param_wounded_stun_duration,
        param_wounded_knockdown_duration,
        param_knockback_scale,
        param_knockup_scale,
        param_knockdown_scale,
        param_air_knockback_scale,
        param_air_knockup_scale,
        param_air_knockdown_scale,
        param_hit_react_direction,
        param_wall_grind_dir,
        param_to_wall_angle,
        param_fire_stop,
        param_ai_custom_param_a,
        param_desired_distance,
        param_in_combat_mode,
        param_use_branch_result,
        param_ledge_height,
        param_wallrun_deviation,
        param_phys_prim_force_scale,
        param_phys_prim_bone,
        param_phys_prim_force_torque_mul,
        param_phys_prim_force_spin_only,
        param_disable_mocomp_movement,
        param_disable_mocomp_facing,
        param_meta_linear_blend_amount,
        param_meta_linear_blend_amount_2,
        param_meta_linear_blend_amount_3,
        param_meta_linear_blend_amount_4,
        param_meta_linear_blend_amount_5,
        param_swing_boost_timer,
        param_change_swing_direction,
        param_fall_speed,
        param_pole_swing_speed,
        param_pole_swing_angle,
        param_pole_swing_jump_type,
        param_pole_swing_orientation,
        param_wall_sprint_jump_type,
        param_jump_direction,
        param_had_wall_collision,
        param_collision_lateral_angle,
        param_collision_lateral_speed,
        param_collision_vertical_angle,
        param_collision_vertical_speed,
        param_player_desires_blacksuit_mode,
        param_player_in_blacksuit_mode,
        param_player_in_combat_mode,
        param_player_in_focus_mode,
        param_player_is_webclimbing,
        param_player_is_throwing,
        param_player_is_evading,
        param_surface_transition_type,
        param_entity_attachment_side,
        param_counter_combat_type,
        param_ai_rvb_capture_mode,
        param_rvb_idle_track_target,
        param_obstacle_avoid_type,
        param_slim_ready_stance_request,
        param_on_non_walkable_terrain,
        param_wall_transition_direction,
        num_external_float_params
    };

    enum external_vector_parameter_types : u32 {
        param_attach_normal = num_external_float_params,
        param_desired_up_vector,
        param_desired_forward_vector,
        param_desired_movement_dir,
        param_desired_destination,
        param_attach_point,
        param_attack_direction,
        param_aim_direction,
        param_phys_prim_force_dir,
        param_phys_prim_force_point,
        param_combat_primary_target_delta,
        param_combat_primary_target_delta_norm,
        param_combat_next_primary_target_delta,
        param_combat_next_primary_target_delta_norm,
        param_hop_object_rel_collision_point,
        num_external_vector_params
    };

    enum external_hash_parameter_types : u32 {
        param_combat_primary_target_id = num_external_vector_params,
        param_combat_primary_target_var_id,
        param_combat_next_primary_target_id,
        param_combat_primary_target_skeleton,
        param_hop_object_skeleton_type,
        num_external_hash_params
    };

    enum internal_float_parameter_types : u32 {
        param_current_time_in_seconds = num_external_hash_params,
        param_current_anim_time_in_seconds,
        param_current_normalized_time,
        param_current_anim_normalized_time,
        param_aim_pitch_angle,
        param_aim_yaw_angle,
        param_frames_from_end_of_anim,
        param_jump_charge,
        param_active_jump_charge,
        param_damage_stunned,
        param_damage_hit_point_percent,
        param_damage_armor_percent,
        param_swing_time,
        param_swing_to_anchor_angle,
        param_swing_to_anchor_length,
        param_swing_angle,
        param_swing_side_angle,
        param_swing_hand,
        param_swing_entry_speed,
        param_swing_exit_speed,
        param_trick_window,
        param_max_trick_combo,
        param_blacksuit_swing_window,
        param_swing_level_rating,
        param_swing_angular_rotatation_speed,
        param_is_carrying,
        param_is_being_carried,
        param_smoothed_speed,
        param_smoothed_lateral_speed,
        param_smoothed_vertical_speed,
        param_strafe_angle,
        param_strafe_y_angle,
        param_strafing_mode,
        param_elevation,
        param_height_above_ground,
        param_height_above_floor,
        param_height_above_wall,
        param_floor_offset,
        param_attack_angle,
        param_computed_distance,
        param_desired_angle,
        param_desired_dir,
        param_phys_stable,
        param_phys_active,
        param_phys_allowed,
        param_effectively_standing,
        param_ai_locomotion_mode,
        param_ai_flight_bank,
        param_ai_flight_pitch,
        param_ai_pie_ring,
        param_ai_idle_type,
        param_ai_walk_run_cycle,
        param_ai_walk_item,
        param_ai_car_seat,
        param_ai_car_combat_engaged,
        param_ai_rappel_engaged,
        param_ai_rappel_speed,
        param_random_zero_to_one,
        param_random_plus_minus_one,
        param_on_fire,
        param_in_water,
        param_wading_water,
        param_sync_master_changed_state,
        param_coordinator_changed_state,
        param_ai_cover_type,
        param_ai_roll_type_2,
        param_ai_roll_type_4,
        param_rage_level,
        param_is_visible,
        param_distance_to_camera,
        param_distance_to_hero,
        param_pos_to_camera_facing_dot_product,
        param_pos_to_camera_facing_angle,
        param_combo_next_move_switch,
        param_combo_restart_switch,
        param_nav_exit_switch,
        param_block_exit_switch,
        param_early_exit_switch,
        param_is_informant_scared,
        param_is_informant_dropped,
        param_can_super_recover,
        param_can_recover_to_wall,
        param_can_recover_to_rooftop,
        param_ai_rvb_capture_mode_internal,
        param_ai_rvb_pod_off_base,
        param_is_in_stars_mode,
        param_is_pod_converting_symbiote,
        param_pod_conversion_type,
        param_ai_rvb_frenzy_mode,
        param_target_mini_head_damage_level,
        param_mv_mini_head_index,
        param_is_riding_rhino,
        param_rvb_target_in_sync_anim,
        param_rvb_target_is_carried,
        num_internal_float_params
    };

    enum internal_vector_parameter_types : u32 {
        param_current_up_vector = num_internal_float_params,
        param_current_right_vector,
        param_current_forward_vector,
        param_current_velocity_vector,
        param_pelvis_facing,
        param_last_collision_normal,
        num_internal_vector_params
    };

    enum internal_hash_parameter_types : u32 {
        param_entity_id = num_internal_vector_params,
        param_variant_id,
        param_als_state,
        param_als_prev_state,
        param_als_category,
        param_als_prev_category,
        param_coordinator_als_state,
        param_coordinator_als_prev_state,
        param_coordinator_als_category,
        param_coordinator_als_prev_category,
        param_r_hand_weapon,
        param_l_hand_weapon,
        param_ai_carry_type,
        param_ai_car_skeleton_name,
        num_internal_hash_params
    };

    // what a vector parameter is reduced to when a single number is wanted; retail's debug names
    enum e_vector_component : u32 {
        component_x = 0,
        component_y,
        component_z,
        component_norm_x,
        component_norm_y,
        component_norm_z,
        component_length,
        component_length_sq,
        component_xz_length,
        component_xz_length_sq,
        component_y_length
    };

    f32 get_vector_component(const vector3d &vec, e_vector_component component);

    // dir's angle round from forward toward side, in degrees within 180 either way
    f32 compute_angle_between_vectors(const vector3d &forward, const vector3d &side, const vector3d &dir);

    namespace references {
        // "BIP01 PELVIS" and "BIP01 HEAD", hashed by the CRT initializers
        inline util::memory_reference<string_hash> bip01_pelvis { 0x010034A8 };
        inline util::memory_reference<string_hash> bip01_head   { 0x01003A40 };

        // the switch signals, hashed from their names by the CRT initializers
        inline util::memory_reference<string_hash> combo_next_move_switch_time { 0x0102C634 };
        inline util::memory_reference<string_hash> combo_restart_switch_time   { 0x0102C39C };
        inline util::memory_reference<string_hash> nav_exit_switch_time        { 0x0102C53C };
        inline util::memory_reference<string_hash> block_exit_switch_time      { 0x0102C4AC };
        inline util::memory_reference<string_hash> early_exit_switch_time      { 0x0102C63C };
    } // references

    // what a request carries to the state machine
    struct parameter_list {
        vector3d    vector_params[15]; // ids 141..155
        string_hash hash_params[5];    // ids 156..160
        f32         float_params[141]; // ids 0..140
        bool        vector_set[15];
        bool        hash_set[5];
        bool        float_set[141];
        u8          pad_39d[0x03];

        parameter_list();

        bool has_param_been_set(param_id param) const;

        // inlined @ sub_5ADD30
        string_hash get_hash(param_id param) const {
            return hash_params[param - num_external_vector_params];
        }
    };

    // the computed parameters, filled the first time a frame asks for them
    struct internal_parameter_cache {
        vector3d    vectors[6];
        string_hash hashes[14];
        f32         floats[94];
        bool        vector_set[6];
        bool        hash_set[14];
        bool        float_set[94];
        u8          pad_26a[0x02];

        internal_parameter_cache();

        void invalidate_per_layer();

        f32         get_internal_float           (animation_logic_system* the_als, state_machine* the_sm, param_id param);
        vector3d    get_internal_vector          (animation_logic_system* the_als, state_machine* the_sm, param_id param);
        f32         get_internal_vector_component(animation_logic_system* the_als, state_machine* the_sm, param_id param, e_vector_component component);
        string_hash get_internal_hash            (animation_logic_system* the_als, state_machine* the_sm, param_id param);

        f32         calculate_internal_float (animation_logic_system* the_als, state_machine* the_sm, param_id param);
        vector3d    calculate_internal_vector(animation_logic_system* the_als, state_machine* the_sm, param_id param);
        string_hash calculate_internal_hash  (animation_logic_system* the_als, state_machine* the_sm, param_id param);
    };

    ASSERT_SIZEOF  (parameter_list,               0x3A0);
    ASSERT_OFFSETOF(parameter_list, hash_params,  0x0B4);
    ASSERT_OFFSETOF(parameter_list, float_params, 0x0C8);
    ASSERT_OFFSETOF(parameter_list, vector_set,   0x2FC);
    ASSERT_OFFSETOF(parameter_list, hash_set,     0x30B);
    ASSERT_OFFSETOF(parameter_list, float_set,    0x310);

    ASSERT_SIZEOF  (internal_parameter_cache,             0x26C);
    ASSERT_OFFSETOF(internal_parameter_cache, hashes,     0x048);
    ASSERT_OFFSETOF(internal_parameter_cache, floats,     0x080);
    ASSERT_OFFSETOF(internal_parameter_cache, vector_set, 0x1F8);
    ASSERT_OFFSETOF(internal_parameter_cache, hash_set,   0x1FE);
    ASSERT_OFFSETOF(internal_parameter_cache, float_set,  0x20C);
}} // treyarch::als
