#include "retail.hh"
#include "treyarch/app/app.hh"
#include "treyarch/game/frontend/frontend_manager.hh"
#include "treyarch/game/frontend/igo/igo_3d_camera_widget.hh"
#include "treyarch/game/frontend/igo/igo_3d_face_button_system.hh"
#include "treyarch/game/frontend/igo/igo_3d_loading_screen.hh"
#include "treyarch/game/frontend/igo/igo_3d_nav_button_bar.hh"
#include "treyarch/game/frontend/igo/igo_3d_scrapbook.hh"
#include "treyarch/game/frontend/ui_frontend_projection.hh"
#include "treyarch/game/game.hh"
#include "treyarch/game/mission/mission_manager.hh"
#include "treyarch/ngl/lighting/context.hh"
#include "treyarch/ngl/lighting/references.hh"
#include "treyarch/ngl/scene/lifecycle.hh"
#include "treyarch/ngl/scene/matrices.hh"
#include "treyarch/ngl/scene/references.hh"
#include "treyarch/ngl/scene/scene.hh"

using namespace treyarch;

// sub_6B15D0
void ui_frontend::draw_startup() {
    if (!startup_widget_17c)
        return;

    retail::sub_698F40((i32)this);
    scene_draw_state_278 = 1;

    ngl::lighting::light_context* previous_context = ngl::lighting::references::current_context.read();
    ngl::lighting::light_context* context = ngl::lighting::create_context();
    ngl::lighting::select_context(context);

    ngl::scene* scene = ngl::list_begin_scene(ngl::scene_parameter_defaults);
    ngl::set_scene_name("UIFrontEnd::DrawStartup");
    ngl::set_clear_flags(0);

    const igo_startup_projection &projection = references::startup_projection.get();

    if (projection.orthographic) {
        ngl::set_ortho_parameters(projection.ortho_width,
                                  projection.ortho_height,
                                  projection.near_plane,
                                  projection.far_plane);
    } else {
        ngl::set_perspective_parameters(projection.field_of_view,
                                        projection.near_plane,
                                        projection.far_plane);
    }

    i32 environment_index = select_ui_environment_index();
    current_ui_environment = (void*)retail::sub_7D6D60(ui_environments[environment_index], 0);

    matrix4x4 identity;
    identity.identity();
    ngl::set_world_to_view_matrix(&identity);

    ngl::lighting::set_scene_context(context, ngl::references::current_scene.read());
    ngl::set_z_test_enable(true);
    ngl::set_z_write_enable(true);
    ngl::set_clear_flags(6);
    ngl::validate_matrices(scene);

    vtable->draw_pass_1(this);
    update_widget_suppression_conditions();

    startup_widget_17c->draw();
    if (startup_widget_180)
        startup_widget_180->draw();
    if (startup_widget_184)
        startup_widget_184->draw();

    ngl::list_end_scene();
    ngl::lighting::select_context(previous_context);

    current_ui_environment = nullptr;
}

// sub_6F5A70
void ui_frontend::draw() {
    using camera_fov_method = f32 (__thiscall*)(camera* self);

    retail::sub_698F40((i32)this);
    scene_draw_state_278 = 1;

    ngl::lighting::light_context* previous_context = ngl::lighting::references::current_context.read();
    ngl::lighting::light_context* context = ngl::lighting::create_context();
    ngl::lighting::select_context(context);

    ngl::scene* pass_1 = ngl::list_begin_scene(ngl::scene_parameter_defaults);
    ngl::set_scene_name("UIFrontEnd::Draw (1)");
    ngl::set_clear_flags(0);

    igo_view_fov_cache &fov_cache = references::view_fov_cache.get();

    if (!(fov_cache.state & 1)) {
        fov_cache.state |= 1;

        camera* view_camera = references::game.read()->get_current_view_camera();
        camera_fov_method get_fov = (camera_fov_method)view_camera->vtable[0x28C / 4];
        fov_cache.field_of_view = get_fov(view_camera);
    }

    const igo_draw_projection &projection = references::draw_projection.get();

    if (projection.orthographic) {
        ngl::set_ortho_parameters(projection.ortho_width,
                                  projection.ortho_height,
                                  projection.near_plane,
                                  projection.far_plane);
    } else {
        ngl::set_perspective_parameters(fov_cache.field_of_view,
                                        projection.near_plane,
                                        projection.far_plane);
    }

    i32 environment_index = select_ui_environment_index();
    current_ui_environment = (void*)retail::sub_7D6D60(ui_environments[environment_index], 0);

    if (camera_widget) {
        matrix4x4 camera_matrix_copy = camera_widget->camera_matrix;
        ngl::set_camera_matrix(&camera_matrix_copy);
        camera_widget->matrix_dirty = 1;
    } else {
        matrix4x4 identity;
        identity.identity();
        ngl::set_world_to_view_matrix(&identity);
    }

    ngl::lighting::set_scene_context(context, pass_1);
    ngl::set_z_test_enable(true);
    ngl::set_z_write_enable(true);
    ngl::set_clear_flags(6);
    ngl::validate_matrices(pass_1);

    vtable->draw_pass_1(this);
    update_widget_suppression_conditions();
    retail::sub_6CD220((u32*)this);

    ui_frontend* current_igo  = references::frontend.get().igo;
    game*        current_game = references::game.read();

    if (current_igo->loading_screen->is_visible()) {
        if (loading_screen)
            loading_screen->draw();

        if (letterbox &&((loading_screen->color >> 24) != 0xFF ||
                          loading_screen->fade < 1.0f)) {

            letterbox->draw();
        }
    } else {
        if (!current_game->disable_interface) {
            if (!current_game->game_paused) {
                if (!current_igo->letterbox || !current_igo->letterbox->is_visible()) {
                    u32* state = retail::sub_809590();

                    if (!retail::sub_805390((u8*)state)) {
                        if (!retail::sub_68BF40((u8*)conversation_menu_system)) {
                            if (unknown_widget_0c0)
                                unknown_widget_0c0->draw();
                            if (button_tips)
                                button_tips->draw();
                        }

                        if (ped_warning)
                            retail::sub_6A1490((i32)ped_warning);

                        if (unknown_widget_0c8)
                            unknown_widget_0c8->draw();
                        if (unknown_widget_0cc)
                            unknown_widget_0cc->draw();

                        if (widget_group_1a4)
                            retail::sub_68CAA0((u32*)widget_group_1a4);

                        if (enemy_health_manager)
                            retail::sub_68CFF0((u32**)enemy_health_manager);

                        if (unknown_widget_1a8)
                            unknown_widget_1a8->draw();
                        if (unknown_widget_1ac)
                            unknown_widget_1ac->draw();

                        if (references::optional_widget_group_enabled.read()) {
                            if (optional_widgets_begin) {
                                for (igo_3d_drawable** widget = optional_widgets_begin;
                                     widget != optional_widgets_end; ++widget) {
                                    if (*widget)
                                        (*widget)->draw();
                                }
                            }

                            if (optional_widget)
                                optional_widget->draw();
                        }

                        if (spidey_distance)
                            spidey_distance->draw();

                        if (boss_meter_system)
                            retail::sub_6D0990((u32*)boss_meter_system);

                        if (district_marker)
                            district_marker->draw();
                        if (unlock_message)
                            unlock_message->draw();
                        if (hint_text_message)
                            hint_text_message->draw();
                        if (unknown_widget_138)
                            unknown_widget_138->draw();

                        if (unknown_widget_13c && (!current_igo->pauseless_dialog ||
                                                   !current_igo->pauseless_dialog->is_visible())) {
                            
                            unknown_widget_13c->draw();
                        }

                        if (unknown_widget_140)
                            unknown_widget_140->draw();
                        if (unknown_widget_14c)
                            unknown_widget_14c->draw();

                        if (!retail::sub_68BF40((u8*)conversation_menu_system)) {
                            if (unknown_widget_0bc)
                                unknown_widget_0bc->draw();
                            if (buddy_summon)
                                buddy_summon->draw();
                            if (combo_meter)
                                combo_meter->draw();
                            if (hero_health)
                                hero_health->draw();
                            if (hero_portrait)
                                hero_portrait->draw();
                            if (special_meter)
                                special_meter->draw();
                            if (mission_specific_meter)
                                mission_specific_meter->draw();
                            if (unified_announcement)
                                unified_announcement->draw();
                        }
                    }
                }

                if (timer_widget)
                    timer_widget->draw();
                if (agility_test)
                    agility_test->draw();
            }

            if (unknown_widget_188)
                unknown_widget_188->draw();
            if (text_debug_widget)
                text_debug_widget->draw();

            if (!current_igo->conversation_state_168) {
                retail::sub_6D2710((i32)current_igo->ui_object_manager);
                retail::sub_9818F0((i32)references::mission_manager.read());
            }

            retail::sub_7EA9D0();

            if (references::region_spawns_enabled.read() && references::region_spawn_manager.read())
                retail::sub_90F5C0();

            if (conversation_menu_system)
                ((igo_3d_widget*)((u8*)conversation_menu_system + 0x10))->draw();

            if (widget_owner_164)
                ((igo_3d_widget*)((u8*)widget_owner_164 + 0x10))->draw();

            retail::sub_6F1FD0((u32**)this);

            if (scrapbook && !scrapbook->is_active()) {
                if (objective_text)
                    objective_text->draw();
                if (hint_text)
                    hint_text->draw();
            }
        }

        retail::sub_6CD0E0((i32)this, current_game->disable_interface);

        if (letterbox && (!scrapbook || !scrapbook->is_active()))
            letterbox->draw();

        if (zoom_map)
            zoom_map->draw_map_overlay();

        if (scrapbook)
            scrapbook->draw_pages();

        if (pauseless_dialog && !scrapbook->is_active())
            pauseless_dialog->draw();

        if (mem_card_message)
            mem_card_message->draw();

        if (face_button_system)
            face_button_system->draw_buttons();

        if (saving_or_loading_screen)
            saving_or_loading_screen->draw();
    }

    if (camera_widget)
        camera_widget->draw();

    ngl::list_end_scene();

    if (!current_igo->loading_screen->is_visible() &&
        !current_game->disable_interface && !current_game->game_paused &&
        (!current_igo->letterbox ||
         !current_igo->letterbox->is_visible())) {

        u32* state = retail::sub_809590();

        if (!retail::sub_805390((u8*)state) && !retail::sub_68BF40((u8*)conversation_menu_system))
            if (radar_widget)
                radar_widget->draw();
    }

    ngl::scene* pass_2 = ngl::list_begin_scene(ngl::scene_parameter_defaults);
    ngl::set_scene_name("UIFrontEnd::Draw (2)");
    ngl::set_clear_flags(0);
    ngl::set_ortho_parameters(projection.ortho_width,
                              projection.ortho_height,
                              projection.near_plane,
                              projection.far_plane);

    matrix4x4 identity;
    identity.identity();
    ngl::set_world_to_view_matrix(&identity);
    ngl::lighting::set_scene_context(context, pass_1);
    ngl::set_z_test_enable(true);
    ngl::set_z_write_enable(true);
    ngl::set_clear_flags(0);

    vtable->draw_pass_2(this);
    ngl::validate_matrices(pass_2);

    if (nav_button_bar)
        nav_button_bar->draw_buttons();

    if (modal_save_dialog)
        modal_save_dialog->draw();
    if (generic_modal_dialog)
        generic_modal_dialog->draw();
    if (controller_disconnect_dialog)
        controller_disconnect_dialog->draw();
    if (autosave_indicator)
        autosave_indicator->draw();

    ngl::list_end_scene();
    ngl::lighting::select_context(previous_context);

    current_ui_environment = nullptr;
}
