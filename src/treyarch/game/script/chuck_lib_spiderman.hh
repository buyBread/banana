#pragma once

#include "treyarch/chuck/script_library/script_library_function.hh"
#include "treyarch/chuck/vm/vm_stack.hh"
#include "util/types.hh"

namespace treyarch {
    class slf__spiderman_stick_dir_in_world_coords__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_camera_autocorrect__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_camera_manual_pitch__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_camera_manual_look__vector3d__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_camera_manual_look_3d__vector3d__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_camera_manual_look_at_target__vector3d__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_camera_manual_look_at_target_3d__vector3d__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_hide_web_dangles__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_is_crawling__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_is_on_wall__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_is_on_ceiling__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_is_on_ground__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_is_truly_on_the_ground__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_is_swinging__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_is_running__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_is_falling__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_is_jumping__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_is_sprinting__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_is_wallsprinting__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_is_sprint_crawling__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_is_in_wallmode__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_is_in_webcast_approach__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_is_in_webcast_approach_critical_hit_range__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_is_in_approach_counter__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_is_in_webcast_approach_normal_hit_range__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_approach_norm_to_crit_percent__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_is_in_webcast_approach_early_range__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_is_in_webcast_approach_counter_range__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_is_webcast_blocked__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_is_webcast_normal_hit__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_is_webcast_crit__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_is_webcast_crit_no_range_protection__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_is_webcast_countered__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_is_webcast_unresolved__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_is_webcast_at_junction__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_is_webcast_X_branch__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_is_webcast_Y_branch__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_is_webcast_B_branch__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_is_webcasting_entity__entity__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_lock_suit__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_is_suit_mode_locked__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_mode_lock_suit__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_break_web__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_set_rpg_progress__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_set_desired_mode__num__vector3d__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_get_mode__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_lock_mode__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_is_mode_locked__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_lock_spider_reflexes_on__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_lock_spider_reflexes_off__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_unlock_spider_reflexes__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_is_spider_reflexes_on__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_lock_spider_senses_on__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_lock_spider_senses_off__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_unlock_spider_senses__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_is_spider_senses_on__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_spider_reflexes_depletion_rate__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_spider_reflexes_recharge_rate__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_spider_reflexes_max_recharge_percent__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_spider_reflexes_min_engage_percent__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_spider_reflexes_spiderman_time_dilation__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_spider_reflexes_spiderman_time_dilation__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_spider_reflexes_world_time_dilation__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_spider_reflexes_world_time_dilation__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_camera_get_z_facing__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__trigger_threat_ping__entity__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__trigger_attack_indicator__entity__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__trigger_attack_indicator__entity__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_spiderman_zlock_entity__entity__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__clear_spiderman_zlock_entity__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_spiderman_zlock_entity__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_spiderman_target__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_num_spiderman_engagers__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_num_spiderman_reservists__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_spiderman_zlock_control__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__is_valid_zlock_target__entity__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_force_evade__vector3d__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_get_block_strength__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_get_swing_anchor_pos__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_get_swing_angle__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_get_swing_side_angle__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_get_swing_to_anchor_angle__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_get_pole_swing_speed__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_get_pole_swing_boost_level__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_find_best_pole__vector3d__num__num__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_find_best_wall_hang_spot__vector3d__num__num__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_find_best_web_subdue_position__vector3d__vector3d__num__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_inc_combo_counter__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_set_combo_counter__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_get_combo_counter__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_set_combo_stat_achieved__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_get_last_combo_hit_count__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_get_prop_to_throw__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_get_prop_to_throw_attach_point__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_set_thrown_props_explode_on_entity_collide__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__spiderman_set_noone_attack_me_time__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__is_in_RvB_sloppy_manual_dodge__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__is_in_RvB_tight_manual_dodge__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__is_in_rvb_black_mode__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__is_in_focus_mode__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_alignment_points__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_alignment_percent__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_points_awarded_by_type__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__award_red_alignment_points_by_type__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__award_black_alignment_points_by_type__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_red_xp__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_red_xp_percent__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__award_red_xp__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_black_xp__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_black_xp_percent__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__award_black_xp__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_unified_xp__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_unified_xp_percent__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__award_unified_xp__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__create_proportional_orbs__num__entity__vector3d__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__award_xp_based_on_suit__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_glass_house_level__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };
} // treyarch
