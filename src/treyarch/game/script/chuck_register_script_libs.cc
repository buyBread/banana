#include "retail.hh"
#include "treyarch/chuck/script_library/script_library_class.hh"
#include "treyarch/game/script/chuck_lib_ai.hh"
#include "treyarch/game/script/chuck_lib_ai_path.hh"
#include "treyarch/game/script/chuck_lib_beam.hh"
#include "treyarch/game/script/chuck_lib_city_life_tracker.hh"
#include "treyarch/game/script/chuck_lib_controller.hh"
#include "treyarch/game/script/chuck_lib_debug_menu.hh"
#include "treyarch/game/script/chuck_lib_entity.hh"
#include "treyarch/game/script/chuck_lib_entity_tracker.hh"
#include "treyarch/game/script/chuck_lib_fight_group.hh"
#include "treyarch/game/script/chuck_lib_finger_of_god.hh"
#include "treyarch/game/script/chuck_lib_line_info.hh"
#include "treyarch/game/script/chuck_lib_mission_objective.hh"
#include "treyarch/game/script/chuck_lib_mutex.hh"
#include "treyarch/game/script/chuck_lib_obstacle.hh"
#include "treyarch/game/script/chuck_lib_player_entity.hh"
#include "treyarch/game/script/chuck_lib_polytube.hh"
#include "treyarch/game/script/chuck_lib_scene_anim.hh"
#include "treyarch/game/script/chuck_lib_signal.hh"
#include "treyarch/game/script/chuck_lib_sound_response.hh"
#include "treyarch/game/script/chuck_lib_trigger.hh"
#include "treyarch/game/script/chuck_lib_vector3d.hh"
#include "treyarch/game/script/chuck_lib_water_box.hh"
#include "treyarch/game/script/chuck_lib_web_wall.hh"
#include "treyarch/game/script/chuck_lib_widget.hh"
#include "treyarch/game/script/chuck_lib_widget_3d.hh"
#include "treyarch/game/script/chuck_register_script_libs.hh"

using namespace treyarch;

// sub_83D730
void treyarch::chuck_register_script_libs() {
    chuck_register_script_lib_classes();

    // global functions
    retail::sub_826740();
    retail::sub_8291D0();
    retail::sub_82BAC0();
    retail::sub_82E250();

    // class functions
    retail::sub_830900();
    retail::sub_833E10();
    retail::sub_837250();
    retail::sub_83A590();
}

// sub_825E80
void treyarch::chuck_register_script_lib_classes() {
    chuck::script_library::references::slc_global.write(new chuck::script_library::script_library_class("", 0, "", false));
    chuck::script_library::references::slc_num   .write(new chuck::script_library::slc_num_t("", 4, ""));
    chuck::script_library::references::slc_str   .write(new chuck::script_library::slc_str_t("", 4, ""));
    chuck::script_library::references::slc_uint  .write(new chuck::script_library::slc_uint_t("", 4, ""));

    references::slc_sound_response        .write(new slc_sound_response_t("", 4, ""));
    references::slc_beam                  .write(new slc_beam_t("", 4, ""));
    references::slc_entity                .write(new slc_entity_t("", 4, ""));
    references::slc_player_entity         .write(new slc_player_entity_t("", 4, ""));
    references::slc_entity_tracker        .write(new slc_entity_tracker_t("", 4, ""));
    references::slc_line_info             .write(new slc_line_info_t("", 4, ""));
    references::slc_polytube              .write(new slc_polytube_t("", 4, ""));
    references::slc_script_controller     .write(new slc_script_controller_t("", 4, ""));
    references::slc_arch_base             .write(new slc_arch_base_t("", 4, ""));
    references::slc_timer_widget          .write(new slc_timer_widget_t("", 4, ""));
    references::slc_trigger               .write(new slc_trigger_t("", 4, ""));
    references::slc_vector3d              .write(new slc_vector3d_t("", 12, ""));
    references::slc_cpu_combat_movement   .write(new slc_cpu_combat_movement_t("", 4, ""));
    references::slc_generic_event_callback.write(new slc_generic_event_callback_t("", 4, ""));
    references::slc_mutex                 .write(new slc_mutex_t("", 4, ""));
    references::slc_widget_3d             .write(new slc_widget_3d_t("", 4, ""));
    references::slc_water_box             .write(new slc_water_box_t("", 4, ""));
    references::slc_ai_path               .write(new slc_ai_path_t("", 4, ""));
    references::slc_cut_scene             .write(new slc_cut_scene_t("", 4, ""));
    references::slc_web_wall              .write(new slc_web_wall_t("", 4, ""));
    references::slc_obstacle              .write(new slc_obstacle_t("", 4, ""));
    references::slc_debug_menu            .write(new slc_debug_menu_t("", 4, ""));
    references::slc_debug_menu_entry      .write(new slc_debug_menu_entry_t("", 4, ""));
    references::slc_city_life_tracker     .write(new slc_city_life_tracker_t("", 4, ""));
    references::slc_mission_objective_data.write(new slc_mission_objective_data_t("", 4, ""));
    references::slc_mission_stage_data    .write(new slc_mission_stage_data_t("", 4, ""));
    references::slc_finger_of_god         .write(new slc_finger_of_god_t("", 4, ""));
    references::slc_fight_group           .write(new slc_fight_group_t("", 4, ""));
}
