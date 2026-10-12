#pragma once

#include "treyarch/chuck/script_library/script_library_class.hh"
#include "treyarch/chuck/script_library/script_library_function.hh"
#include "treyarch/chuck/vm/vm_stack.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    class slc_widget_3d_t : public chuck::script_library::script_library_class {

    public:
        // inlined @ sub_825E80
        slc_widget_3d_t(const char* name, i32 requested_size, const char* requested_parent_name)
            : script_library_class(name, requested_size, requested_parent_name, false) {}
    };

    class slc_finger_of_god_t : public chuck::script_library::script_library_class {

    public:
        // inlined @ sub_825E80
        slc_finger_of_god_t(const char* name, i32 requested_size, const char* requested_parent_name)
            : script_library_class(name, requested_size, requested_parent_name, false) {}
    };

    class slf__create_3d_widget__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__create_3d_widget_from_params__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__create_3d_mission_text_widget__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__create_3d_hint_text_widget__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__create_3d_text_widget_from_params__str__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__destroy_all_3d_mission_text_widgets__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__hide_all_3d_mission_text_widgets__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__destroy_all_3d_hint_text_widgets__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__hide_all_3d_hint_text_widgets__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__create_3d_button_widget__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__destroy_3d_widget__widget_3d__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__destroy_all_3d_widgets__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__move_ui_camera_to_marker__widget_3d__str__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__move_ui_camera_to_home__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__enable_health_meter__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__enable_radar__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__enable_boss_meter__entity__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__disable_boss_meter__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__add_boss_meter__entity__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__add_boss_meter__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__remove_boss_meter__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__remove_all_boss_meters__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_boss_meter_health_value__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_boss_meter_health_value__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__clear_mission_objectives__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__add_mission_objective__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__add_mission_objective__num__dynamic_array__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__add_mission_objective_debug__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__clear_3d_dialog_text__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__add_3d_dialog_text__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__add_3d_dialog_text__num__dynamic_array__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__add_3d_dialog_text_debug__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__add_3d_objective_text__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__add_3d_objective_text__num__dynamic_array__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__add_3d_objective_text_debug__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__bring_up_3d_dialog__num__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_3d_dialog_title__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_3d_dialog_title_debug__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__show_game_button__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__remove_all_game_buttons__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__remove_game_button__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__play_game_button_anim__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__game_button_anim_playing__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_game_button_pos__num__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_game_button_pos__num__vector3d__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_2d_ui_pos_from_world_pos__vector3d__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_game_button_scale__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_game_button_alpha__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__show_X_over_button__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__is_X_over_button_shown__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__load_debug_3d_widget__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_debug_3d_widget_pos__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_debug_3d_widget_x__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_debug_3d_widget_y__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_debug_3d_widget_scale__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_debug_3d_widget_scale__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_debug_3d_widget_tilt__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_debug_3d_widget_tilt__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_debug_3d_widget_spin__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_debug_3d_widget_spin__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_debug_3d_widget_rotation__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_debug_3d_widget_rotation__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__inc_debug_3d_widget_alignment__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;
    };

    class slf__get_debug_3d_widget_entity__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__debug_3d_widget_play_anim__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__debug_3d_widget_reset_starting_pos__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;
    };

    class slf__debug_3d_widget_set_cowbell_variant__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__debug_3d_widget_copy_to_clipboard__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;
    };

    class slf__debug_3d_widget_add_ui_parameter_files_to_menu__debug_menu__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_ui_camera_entity__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__ui_camera_play_anim__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__ui_camera_move_to_marker__str__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__ui_camera_reset_starting_anim_pos__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__ui_camera_set_edit_mode__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__ui_camera_inc_x_pos__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__ui_camera_dec_x_pos__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__ui_camera_inc_y_pos__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__ui_camera_dec_y_pos__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__ui_camera_inc_z_pos__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__ui_camera_dec_z_pos__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__ui_camera_inc_tilt__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__ui_camera_dec_tilt__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__ui_camera_inc_spin__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__ui_camera_dec_spin__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__ui_camera_inc_rotation__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__ui_camera_dec_rotation__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__ui_camera_reset_pos__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__add_ui_light__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__ui_light_exists__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_ui_light_x__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_ui_light_y__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_ui_light_near_dist__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_ui_light_far_dist__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_ui_light_x__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_ui_light_y__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_ui_light_near_dist__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_ui_light_far_dist__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_ui_light_widget_index__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_debug_3d_text_marker__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_debug_3d_text_str__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__inc_debug_3d_text_x__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;
    };

    class slf__dec_debug_3d_text_x__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;
    };

    class slf__inc_debug_3d_text_y__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;
    };

    class slf__dec_debug_3d_text_y__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;
    };

    class slf__inc_debug_3d_text_scale__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;
    };

    class slf__dec_debug_3d_text_scale__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;
    };

    class slf__inc_debug_3d_text_width__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;
    };

    class slf__dec_debug_3d_text_width__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;
    };

    class slf__inc_debug_3d_text_height__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;
    };

    class slf__dec_debug_3d_text_height__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;
    };

    class slf__inc_debug_3d_text_valign__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;
    };

    class slf__inc_debug_3d_text_halign__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;
    };

    class slf__inc_debug_3d_text_font__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;
    };

    class slf__inc_debug_3d_text_color__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;
    };

    class slf__inc_debug_3d_text_wordwrap__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;
    };

    class slf__inc_debug_3d_text_test_case__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;
    };

    class slf__set_debug_show_3d_text_regions__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_debug_child_widget__str__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__inc_debug_child_scale__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;
    };

    class slf__dec_debug_child_scale__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;
    };

    class slf__reattach_child_widget__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_cutscene_texture__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__display_cutscene__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__hide_cutscene__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__is_displaying_cutscene__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__send_hide_cutscene_quad_signal__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__display_ped_warning__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__hide_ped_warning__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__display_ped_warning_with_duration__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__create_meter__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__create_meter__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__create_meter_with_label_placement__str__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_meter_value__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_meter_scale__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_meter_color__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_meter_pos__num__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__delete_meter__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_meter_visibility__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_meter_label__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__create_fog__vector3d__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__create_fog__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__destroy_fog__finger_of_god__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__terminate_script__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__terminate_script__num__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__format_string__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__show_mission_success_screen__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__close_3d_mission_info_screen__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__lock_minimap__vector3d__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__unlock_minimap__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__minimap_buildings_invisible__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__minimap_wallmode_disable__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_collectible_level__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__temporarily_purchase_upgrade__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__temporarily_purchase_all_upgrades__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__is_upgrade_purchased__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__suspend_instructional_text__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__clear_instructional_text_no_fade__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__rvb_clear_instructional_text__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__try_to_purchase_upgrade__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_upgrade_id__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_rvb_hint_text_debug__num__num__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__is_vibration_option_enabled__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__hide_rvb_hint_text__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_rvb_instructional_text__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__get_root_debug_menu__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__is_motion_sensor_option_enabled__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__clear_rvb_instructional_queue__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_rvb_instructional_text_debug__num__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__create_polytube__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__enumerate_resources_to_debug_menu__debug_menu_entry__num__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__clear_rvb_hint_queue__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__set_rvb_hint_text__num__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__is_valid__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_shown__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_position__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_position__vector3d__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_z__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_scale__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_scale_offset__vector3d__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_tilt__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_spin__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_rotation__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_max_timer__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_color__vector3d__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_continuous_tilt__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_continuous_spin__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_continuous_rotation__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_widescreen_alignment__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_uv_offset__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__fade_in__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__fade_out__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_alpha__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__play_anim__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__play_button_anim__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__is_anim_playing__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__get_text_id__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_text_shown__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_text__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_null_text__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_text_debug__num__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_text_pos__num__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_text_dimensions__num__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_text_scale__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_text_button_scale__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_text_valign__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_text_halign__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_text_font__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_text_color__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_text_color__num__vector3d__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_text_wordwrap__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_text_x_scale__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_text_character_gap__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_text_wordwrap_line_gap__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_text_shadow__num__vector3d__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_text_shadow_alpha__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_text_typewriter__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__is_text_done_updating__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__end_text_typewriter__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__get_child_id_from_name__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__get_text_id_from_name__str__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__widget_3d__set_persistent__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__finger_of_god__set_visibility__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__finger_of_god__set_active__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__finger_of_god__set_pos__vector3d__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__finger_of_god__set_radius__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__finger_of_god__deactive_fog__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__finger_of_god__set_poi_icon__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__finger_of_god__toggle_fog__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__finger_of_god__fog_requires_senses__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__finger_of_god__place_fog__vector3d__num__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__finger_of_god__place_fog_reappear__vector3d__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__finger_of_god__arrived_at_fog__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__finger_of_god__set_poi_active__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__finger_of_god__set_gta_widget_active__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__finger_of_god__set_gta_widget_inner_radius__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__finger_of_god__set_gta_widget_outer_radius__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__finger_of_god__set_gta_widget_pos_offset__vector3d__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    class slf__finger_of_god__set_gta_widget_render_scale__num__t : public chuck::script_library::script_library_function {

    public:
        using script_library_function::script_library_function;

        bool operator()(chuck::vm::vm_stack &stack) override;
    };

    namespace references {
        inline util::memory_reference<slc_widget_3d_t*>     slc_widget_3d     { 0x010F70D8 };
        inline util::memory_reference<slc_finger_of_god_t*> slc_finger_of_god { 0x010F7104 };
    } // references
} // treyarch
