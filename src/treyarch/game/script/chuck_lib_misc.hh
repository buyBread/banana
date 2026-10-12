#pragma once

#include "treyarch/chuck/script_library/script_library_function.hh"
#include "treyarch/chuck/vm/vm_stack.hh"
#include "util/types.hh"

namespace treyarch {
    class slf__toggle_switchable_connectors__uint__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__has_substring__str__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__to_string_hash__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__clear_area_of_entities__vector3d__vector3d__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__change_environment_box_env_file__uint__uint__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__is_camera_lr_inverted__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__is_camera_ud_inverted__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_current_view_cam_x_facing__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_current_view_cam_y_facing__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_current_view_cam_z_facing__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_current_view_cam_pos__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__enable_marky_cam__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__is_marky_cam_enabled__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_marky_fov__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__enable_animated_cam__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__disable_animated_cam__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__is_animated_cam_enabled__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_animated_fov__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_desired_fov__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__disable_marky_cam__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__inhibit_animated_cam__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__freeze_hero__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_blur__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__suspend_all_ai__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__unsuspend_all_ai__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__is_dialog_active__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__is_conversation_active__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__enable_pause__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__enable_interface__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__enable_pois__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__map_debug_menu_to_select_button__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__delay__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__dilated_delay__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__random__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__float_random__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__ZERO_TO_ONE__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__PLUS_MINUS_ONE__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_time_inc__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_dilated_time_inc__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__stop_vibration__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__apply_radius_damage__vector3d__num__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__apply_radius_damage_ignore_hero__vector3d__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__apply_radius_damage__vector3d__num__num__num__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__apply_radius_damage_ignore_hero__vector3d__num__num__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__los_check__vector3d__vector3d__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__find_district_for_point__vector3d__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__find_innermost_district__vector3d__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__collide_segment_with_hollow_sphere__vector3d__vector3d__vector3d__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__col_check__vector3d__vector3d__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_col_hit_pos__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_col_hit_norm__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_col_hit_ent__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_global_time_dilation__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_global_time_dilation__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__wait_set_global_time_dilation__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__sqrt__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__sin__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__cos__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__acos__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__sin_cos__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_int_num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_script_user_num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_script_user_num__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_script_user_vector__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_script_user_vector__num__vector3d__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_script_user_str__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_script_user_str__num__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_ini_flag__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_ini_num__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_ini_flag__str__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_ini_num__str__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_hires_real_world_timer__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_player_type__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__malor__vector3d__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__malor_no_locking__vector3d__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__is_in_district__vector3d__uint__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__is_sphere_visible__vector3d__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__is_sphere_on_screen__vector3d__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__calc_launch_vector__vector3d__vector3d__num__entity__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_all_instances_thread_count__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_all_execs_thread_count__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__is_interior__uint__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_global_sunlight__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_light_position__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_light_position__str__vector3d__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_light_direction__str__vector3d__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_light_direction__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_light_intensity__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_light_intensity__str__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__light_orbit__str__vector3d__num__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_light_color__str__vector3d__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_light_color__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__enable_bloom__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_sandman_wetness__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__find_good_spot__entity__vector3d__num__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__find_good_spot__dynamic_array__vector3d__num__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__combat_check_for_dodge__entity__entity__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__combat_check_for_block__entity__entity__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_path_graph_num_nodes__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_path_graph_closest_node__str__vector3d__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_path_graph_node_pos__str__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_path_graph_node_num_edges__str__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_path_graph_node_other_node__str__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__find_nearest_position_on_path_graph__str__vector3d__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__profile_begin__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__profile_end__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;
    };

    class slf__keep_sphere_in_world__vector3d__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__apply_radius_damage_adv__vector3d__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__find_throwable_manip_obj__vector3d__num__num__dynamic_array__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__find_thrown_manip_obj__vector3d__num__num__dynamic_array__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__find_electrical_swing_objects__vector3d__num__num__dynamic_array__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__profiler_enable__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__profiler_display__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__profiler_set_thread__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__profiler_set_refresh_rate__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__profiler_set_parenting__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__profiler_set_sort__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__profiler_set_number_type__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__profiler_set_reset_counter__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__profiler_set_filter__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_teams_love__str__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_teams_hate__str__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__add_to_hate_list__str__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__remove_from_hate_list__str__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__add_to_friend_list__str__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__remove_from_friend_list__str__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };
} // treyarch
