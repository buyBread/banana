#include "retail.hh"
#include "game/app.hh"
#include "game/cutscene/cut_scene_toa_helper.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/igo/igo_3d_camera_widget.hh"
#include "game/frontend/igo/igo_3d_face_button_system.hh"
#include "game/frontend/igo/igo_3d_loading_screen.hh"
#include "game/frontend/igo/igo_3d_nav_button_bar.hh"
#include "game/frontend/igo/igo_3d_scrapbook.hh"
#include "game/frontend/ui_frontend_projection.hh"
#include "game/game.hh"
#include "game/mission/mission_manager.hh"
#include "game/quest_manager.hh"
#include "game/region_spawn_manager.hh"
#include "treyarch/ngl/lighting/lighting.hh"
#include "treyarch/ngl/scene/scene.hh"

using namespace treyarch;

// sub_6B15D0
void ui_frontend::draw_startup() {
    if (!startup_widget_17c)
        return;

    render_background_effects();
    scene_draw_state_278 = 1;

    ngl::lighting::light_context* previous_context = ngl::lighting::references::build_light_context.read();
    ngl::lighting::light_context* context = ngl::lighting::create_light_context();
    ngl::lighting::select_build_light_context(context);

    ngl::scene* scene = ngl::list_begin_scene(ngl::scene_parameter_defaults);
    ngl::set_scene_name("UIFrontEnd::DrawStartup");
    ngl::set_clear_flags(0);

    const igo_startup_projection &projection = references::startup_projection.get();

    if (projection.orthographic) {
        ngl::set_ortho_matrix(projection.ortho_width,
                              projection.ortho_height,
                              projection.near_plane,
                              projection.far_plane);
    } else {
        ngl::set_perspective_matrix(projection.field_of_view,
                                    projection.near_plane,
                                    projection.far_plane);
    }

    i32 environment_index = select_ui_environment_index();
    current_ui_environment = (void*)retail::sub_7D6D60(ui_environments[environment_index], 0);

    math::rot_tran_mat43 identity = math::identity_mat33();
    ngl::set_world_to_view_matrix(identity);

    ngl::lighting::set_light_context(context, ngl::references::build_scene.read());
    ngl::set_z_test_enable(true);
    ngl::set_z_write_enable(true);
    ngl::set_clear_flags(6);
    ngl::validate_matrices(scene);

    vtable->draw_pass_1(this);
    update_widget_suppression_conditions();

    startup_widget_17c->render();
    if (startup_widget_180)
        startup_widget_180->render();
    if (startup_widget_184)
        startup_widget_184->render();

    ngl::list_end_scene();
    ngl::lighting::select_build_light_context(previous_context);

    current_ui_environment = nullptr;
}

// sub_6F5A70
void ui_frontend::draw() {
    render_background_effects();
    scene_draw_state_278 = 1;

    ngl::lighting::light_context* previous_context = ngl::lighting::references::build_light_context.read();
    ngl::lighting::light_context* context = ngl::lighting::create_light_context();
    ngl::lighting::select_build_light_context(context);

    ngl::scene* pass_1 = ngl::list_begin_scene(ngl::scene_parameter_defaults);
    ngl::set_scene_name("UIFrontEnd::Draw (1)");
    ngl::set_clear_flags(0);

    igo_view_fov_cache &fov_cache = references::view_fov_cache.get();

    if (!(fov_cache.state & 1)) {
        fov_cache.state |= 1;

        fov_cache.field_of_view = references::game.read()->get_current_view_camera()->get_fov();
    }

    const igo_draw_projection &projection = references::draw_projection.get();

    if (projection.orthographic) {
        ngl::set_ortho_matrix(projection.ortho_width,
                              projection.ortho_height,
                              projection.near_plane,
                              projection.far_plane);
    } else {
        ngl::set_perspective_matrix(fov_cache.field_of_view,
                                    projection.near_plane,
                                    projection.far_plane);
    }

    i32 environment_index = select_ui_environment_index();
    current_ui_environment = (void*)retail::sub_7D6D60(ui_environments[environment_index], 0);

    if (camera_widget) {
        matrix4x4 camera_matrix_copy = camera_widget->camera_matrix;
        ngl::set_camera_matrix(*(const math::rot_tran_mat43*)&camera_matrix_copy);
        camera_widget->matrix_dirty = 1;
    } else {
        math::rot_tran_mat43 identity = math::identity_mat33();
        ngl::set_world_to_view_matrix(identity);
    }

    ngl::lighting::set_light_context(context, pass_1);
    ngl::set_z_test_enable(true);
    ngl::set_z_write_enable(true);
    ngl::set_clear_flags(6);
    ngl::validate_matrices(pass_1);

    vtable->draw_pass_1(this);
    update_widget_suppression_conditions();
    draw_widgets();

    ui_frontend* current_igo  = references::frontend.get().igo;
    game*        current_game = references::game.read();

    if (current_igo->loading_screen->is_shown()) {
        if (loading_screen)
            loading_screen->render();

        if (letterbox_widget && (loading_screen->color.a != 0xFF ||
                                 loading_screen->fade < 1.0f)) {

            letterbox_widget->render();
        }
    } else {
        if (!current_game->disable_interface) {
            if (!current_game->game_paused) {
                if (!current_igo->letterbox_widget || !current_igo->letterbox_widget->is_shown()) {
                    if (!cut_scene_toa_helper::get()->is_running()) {
                        if (!conversation_menu_system->is_conversation_active()) {
                            if (unknown_widget_0c0)
                                unknown_widget_0c0->render();
                            if (button_tips)
                                button_tips->render();
                        }

                        if (ped_warning)
                            ped_warning->draw();

                        if (unknown_widget_0c8)
                            unknown_widget_0c8->render();
                        if (unknown_widget_0cc)
                            unknown_widget_0cc->render();

                        if (widget_group_1a4)
                            widget_group_1a4->draw();

                        if (enemy_health_manager)
                            enemy_health_manager->draw();

                        if (unknown_widget_1a8)
                            unknown_widget_1a8->render();
                        if (ally_health_widget)
                            ally_health_widget->render();

                        if (references::optional_widget_group_enabled.read()) {
                            if (optional_widgets_begin) {
                                for (igo_3d_widget** widget = optional_widgets_begin;
                                     widget != optional_widgets_end; ++widget) {
                                    if (*widget)
                                        (*widget)->render();
                                }
                            }

                            if (optional_widget)
                                optional_widget->render();
                        }

                        if (spidey_distance)
                            spidey_distance->render();

                        if (boss_meter_system)
                            boss_meter_system->draw();

                        if (district_marker)
                            district_marker->render();
                        if (unlock_message)
                            unlock_message->render();
                        if (hint_text_message)
                            hint_text_message->render();
                        if (unknown_widget_138)
                            unknown_widget_138->render();

                        if (unknown_widget_13c && (!current_igo->pauseless_dialog ||
                                                   !current_igo->pauseless_dialog->is_shown())) {
                            
                            unknown_widget_13c->render();
                        }

                        if (unknown_widget_140)
                            unknown_widget_140->render();
                        if (unknown_widget_14c)
                            unknown_widget_14c->render();

                        if (!conversation_menu_system->is_conversation_active()) {
                            if (unknown_widget_0bc)
                                unknown_widget_0bc->render();
                            if (buddy_summon)
                                buddy_summon->render();
                            if (combo_meter)
                                combo_meter->render();
                            if (hero_health)
                                hero_health->render();
                            if (hero_portrait)
                                hero_portrait->render();
                            if (special_meter)
                                special_meter->render();
                            if (mission_specific_meter)
                                mission_specific_meter->render();
                            if (unified_announcement)
                                unified_announcement->render();
                        }
                    }
                }

                if (timer_widget)
                    timer_widget->render();
                if (agility_test)
                    agility_test->render();
            }

            if (unknown_widget_188)
                unknown_widget_188->render();
            if (text_debug_widget)
                text_debug_widget->render();

            if (!current_igo->conversation_state_168) {
                current_igo->entity_tracker_mgr->render();
                retail::sub_9818F0((i32)mission_manager::inst());
            }

            quest_manager::draw_text();

            if (references::region_spawns_enabled.read() && region_spawn_manager::inst())
                region_spawn_manager::draw_text();

            if (conversation_menu_system)
                conversation_menu_system->render();

            if (dialog)
                dialog->render();

            draw_script_widgets();

            if (scrapbook && !scrapbook->is_active()) {
                if (objective_text)
                    objective_text->render();
                if (hint_text)
                    hint_text->render();
            }
        }

        draw_interface_disabled_script_widgets(current_game->disable_interface);

        if (letterbox_widget && (!scrapbook || !scrapbook->is_active()))
            letterbox_widget->render();

        if (zoom_map)
            zoom_map->draw_map_overlay();

        if (scrapbook)
            scrapbook->draw_pages();

        if (pauseless_dialog && !scrapbook->is_active())
            pauseless_dialog->render();

        if (mem_card_message)
            mem_card_message->render();

        if (face_button_system)
            face_button_system->draw_buttons();

        if (saving_or_loading_screen)
            saving_or_loading_screen->render();
    }

    if (camera_widget)
        camera_widget->render();

    ngl::list_end_scene();

    if (!current_igo->loading_screen->is_shown() &&
        !current_game->disable_interface && !current_game->game_paused &&
        (!current_igo->letterbox_widget ||
         !current_igo->letterbox_widget->is_shown())) {

        if (!cut_scene_toa_helper::get()->is_running() && !conversation_menu_system->is_conversation_active())
            if (radar_widget)
                radar_widget->render();
    }

    ngl::scene* pass_2 = ngl::list_begin_scene(ngl::scene_parameter_defaults);
    ngl::set_scene_name("UIFrontEnd::Draw (2)");
    ngl::set_clear_flags(0);
    ngl::set_ortho_matrix(projection.ortho_width,
                          projection.ortho_height,
                          projection.near_plane,
                          projection.far_plane);

    math::rot_tran_mat43 identity = math::identity_mat33();
    ngl::set_world_to_view_matrix(identity);
    ngl::lighting::set_light_context(context, pass_1);
    ngl::set_z_test_enable(true);
    ngl::set_z_write_enable(true);
    ngl::set_clear_flags(0);

    vtable->draw_pass_2(this);
    ngl::validate_matrices(pass_2);

    if (nav_button_bar)
        nav_button_bar->draw_buttons();

    if (modal_save_dialog)
        modal_save_dialog->render();
    if (generic_modal_dialog)
        generic_modal_dialog->render();
    if (controller_disconnect_dialog)
        controller_disconnect_dialog->render();
    if (autosave_indicator)
        autosave_indicator->render();

    ngl::list_end_scene();
    ngl::lighting::select_build_light_context(previous_context);

    current_ui_environment = nullptr;
}
