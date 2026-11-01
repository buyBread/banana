#include "retail.hh"
#include "game/app.hh"
#include "game/frontend/finger_of_god.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/igo/igo_3d_camera_widget.hh"
#include "game/frontend/igo/igo_3d_face_button_system.hh"
#include "game/frontend/igo/igo_3d_loading_screen.hh"
#include "game/frontend/igo/igo_3d_nav_button_bar.hh"
#include "game/frontend/igo/igo_3d_scrapbook.hh"
#include "game/game.hh"
#include "game/mission/mission_manager.hh"
#include "game/script/script_access.hh"
#include "game/ui_character_info_list.hh"
#include "game/wds/entity/entity.hh"
#include "game/wds/references.hh"
#include "game/wds/world_dynamics_system.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mm/mm.hh"

using namespace treyarch;

// sub_7152D0
void ui_frontend::update(f32 time_inc) {
    retail::sub_6CC810();

    unk_01c.update(time_inc, false, false);
    unk_068.update(time_inc, false, false);

    retail::sub_715020((u32*)&unk_01c, (u32*)&unk_068); // acts on either button's press

    if (death_dialog_deferred) {
        on_player_faded_out();

        return;
    }

    if (unk_0b4) {
        igo_3d_pauseless_dialog_widget* pauseless = references::frontend.get().igo->pauseless_dialog;

        if (pauseless && !pauseless->is_shown()) {
            if (pauseless->unk_073()) {
                if (pauseless->get_selected_button() == 1) {
                    mission_manager::inst()->terminate_script(false, false);

                    unk_0b4 = false;

                    return;
                }

                if (pauseless->get_selected_button() == 2) {
                    ui_frontend* igo = references::frontend.get().igo;

                    if (void* widget = igo->unknown_widget_174) {
                        ((void (__thiscall*)(void*))(*(void***)widget)[54])(widget); // slot 54

                        unk_0b4 = false;

                        return;
                    }

                    igo->scrapbook->activate(false);
                }
            }

            unk_0b4 = false;

            return;
        }
    }

    if (death_dialog_shown) {
        igo_3d_pauseless_dialog_widget* pauseless = references::frontend.get().igo->pauseless_dialog;

        if (pauseless && !pauseless->is_shown()) {
            death_dialog_shown = false;

            retail::sub_70AAF0(); // ends the mission and resets the city

            return;
        }
    }

    if (entity_tracker_mgr)
        retail::sub_6FC230((u32*)entity_tracker_mgr, time_inc);

    vtable->unk_014(this, time_inc);

    retail::sub_6D9450((u32*)this, time_inc);

    if (radar_widget)
        retail::sub_6DFC70((u32*)radar_widget, time_inc);

    if (letterbox_widget)
        letterbox_widget->update(time_inc);

    if (special_meter)
        special_meter->update(time_inc);

    if (unknown_widget_0bc)
        unknown_widget_0bc->update(time_inc);

    if (unknown_widget_0c0)
        unknown_widget_0c0->update(time_inc);

    if (ped_warning)
        retail::sub_6B54D0((i32)ped_warning, time_inc);

    if (unknown_widget_0c8)
        unknown_widget_0c8->update(time_inc);

    if (unknown_widget_0cc)
        unknown_widget_0cc->update(time_inc);

    if (buddy_summon)
        buddy_summon->update(time_inc);

    if (widget_group_1a4)
        retail::sub_6B56D0((i32)widget_group_1a4, time_inc);

    if (enemy_health_manager)
        retail::sub_6F26C0((i32)enemy_health_manager, time_inc);

    if (unknown_widget_1a8)
        unknown_widget_1a8->update(time_inc);

    if (ally_health_widget)
        ally_health_widget->update(time_inc);

    if (objective_text)
        objective_text->update(time_inc);

    if (hint_text)
        hint_text->update(time_inc);

    if (combo_meter)
        combo_meter->update(time_inc);

    if (hero_health)
        hero_health->update(time_inc);

    if (hero_portrait)
        hero_portrait->update(time_inc);

    if (references::optional_widget_group_enabled.read()) {
        for (u32 i = 0; i < optional_widgets.size(); ++i) {
            if (optional_widgets[i])
                optional_widgets[i]->update(time_inc);
        }

        if (optional_widget)
            optional_widget->update(time_inc);
    }

    if (mission_specific_meter)
        mission_specific_meter->update(time_inc);

    if (spidey_distance)
        spidey_distance->update(time_inc);

    if (boss_meter_system)
        retail::sub_6D0950((u32*)boss_meter_system, time_inc);

    if (pauseless_dialog && !scrapbook->is_active())
        pauseless_dialog->update(time_inc);

    if (district_marker)
        district_marker->update(time_inc);

    if (unlock_message)
        unlock_message->update(time_inc);

    if (combo_move_tracker)
        combo_move_tracker->update(time_inc);

    if (combo_counter_message)
        combo_counter_message->update(time_inc);

    if (hint_text_message)
        hint_text_message->update(time_inc);

    if (mem_card_message)
        mem_card_message->update(time_inc);

    if (timer_widget)
        timer_widget->update(time_inc);

    if (radar_widget)
        radar_widget->update(time_inc);

    if (agility_test)
        agility_test->update(time_inc);

    if (nav_button_bar)
        nav_button_bar->unk_004(time_inc);

    if (modal_save_dialog)
        modal_save_dialog->update(time_inc);

    if (generic_modal_dialog)
        generic_modal_dialog->update(time_inc);

    if (controller_disconnect_dialog)
        controller_disconnect_dialog->update(time_inc);

    if (autosave_indicator)
        autosave_indicator->update(time_inc);

    if (face_button_system)
        face_button_system->unk_025(time_inc);

    if (scrapbook)
        scrapbook->unk_025(time_inc);

    if (zoom_map)
        zoom_map->unk_026(time_inc);

    if (loading_screen)
        loading_screen->update(time_inc);

    if (saving_or_loading_screen)
        saving_or_loading_screen->update(time_inc);

    if (unknown_widget_138)
        unknown_widget_138->update(time_inc);

    if (unknown_widget_13c)
        unknown_widget_13c->update(time_inc);

    if (unknown_widget_140)
        unknown_widget_140->update(time_inc);

    if (unknown_widget_14c)
        unknown_widget_14c->update(time_inc);

    if (unknown_widget_188)
        unknown_widget_188->update(time_inc);

    if (text_debug_widget)
        text_debug_widget->update(time_inc);

    if (camera_widget)
        camera_widget->update(time_inc);

    if (conversation_menu_system)
        conversation_menu_system->update(time_inc);

    if (unified_announcement) {
        if (references::unk_0102d00c.read() != 1.0f) {
            igo_3d_text* text  = unified_announcement->unk_150;
            f32          width = text ? text->get_width() : 0.0f;

            if (width != 470.0)
                retail::sub_68F460((u32**)unified_announcement, 470.0f); // sets the text's width
        }

        unified_announcement->update(time_inc);
    }

    if (dialog) {
        dialog->update(time_inc);

        if (!dialog->shown && !dialog->unfreeze_delay) {
            delete dialog;

            dialog = nullptr;
        }
    }

    retail::sub_6E73B0((u32*)this, time_inc); // the script widgets
    retail::sub_6B17B0(this, time_inc);       // the background effects
}

// sub_6884F0
void ui_frontend::release_hero_freezes() {
    while (hero_freezes > 0) {
        references::game.read()->freeze_hero(false);
        --hero_freezes;
    }
}

// sub_70A8B0
void ui_frontend::fade_blackscreen(bool on, f32 time, bool silent) {
    if (!loading_screen)
        return;

    if (on) {
        if (time >= 0.1f)
            loading_screen->blackscreen_on(time, false, false, silent, false, 3.0f);

        retail::sub_77BAB0((u32*)references::g_world_ptr.read()->the_terrain);

        ui_frontend* igo = references::frontend.get().igo;

        references::spider_senses_on.write(0);
        igo->background_effect_mode = 0;
        retail::sub_6B17B0(igo, 0.0f); // the background effect update

        if (references::g_world_ptr.read()->get_hero_ptr())
            references::g_world_ptr.read()->get_hero_ptr()->raise_event(references::senses_exited.read());

        ++hero_freezes;
        references::game.read()->freeze_hero(true);

        return;
    }

    loading_screen->blackscreen_off(time, false);

    if (references::game.read()->game_paused)
        retail::sub_97BB70((i32)references::game.read()); // game::unpause_action

    release_hero_freezes();
}

// sub_6F1F90
void ui_frontend::register_player_callbacks(entity* hero) {
    if (player_callbacks_registered)
        return;

    faded_out_callback_id = hero->add_callback(references::faded_out.read(), player_faded_out_callback, this, false);

    player_callbacks_registered = true;
}

// sub_6E73A0
void ui_frontend::player_faded_out_callback(event*, arch_base_vhandle, void* parameters) {
    ((ui_frontend*)parameters)->on_player_faded_out();
}

// sub_6E70D0
void ui_frontend::on_player_faded_out() {
    if (death_dialog_shown)
        return;

    mission_manager* missions = mission_manager::inst();

    if (missions->get_state() == mission_manager_state_start_mission_failed_dialog)
        return;

    if (missions->get_state() == mission_manager_state_running_mission_failed_dialog)
        return;

    if (retail::sub_97E210((u32*)missions)) // the show_failed_dialog flag
        return;

    if (retail::sub_967DF0()) {
        death_dialog_deferred = true;

        return;
    }

    death_dialog_deferred = false;

    if (retail::sub_97E3D0((u32*)missions)) { // the twin of mission_manager::is_mission_running
        missions->terminate_script(false, false);

        return;
    }

    {
        f32 detail_count = 0.0f;

        script_arg count;
        count.type  = 1;
        count.value = &detail_count;

        string_hash clean_up_random_details;
        clean_up_random_details.initialize(mash::ALLOCATED, "clean_up_random_details(num)");

        script::call_function(nullptr,
                              clean_up_random_details,
                              count,
                              script_arg(),
                              script_arg(),
                              script_arg(),
                              script_arg(),
                              script_arg(),
                              script_arg(),
                              script_arg());
    }

    references::frontend.get().igo->loading_screen->blackscreen_off(0.0f, false);

    references::frontend.get().igo->pauseless_dialog->clear();
    references::frontend.get().igo->pauseless_dialog->set_text("");
    references::frontend.get().igo->pauseless_dialog->set_title_by_id(3234);

    retail::sub_6A2280((u32*)references::frontend.get().igo->pauseless_dialog, 410); // button 2 text
    retail::sub_68E950((i32)references::frontend.get().igo->pauseless_dialog, 1);    // shows button 2
    retail::sub_68E920((i32)references::frontend.get().igo->pauseless_dialog, 0);    // hides button 1

    references::frontend.get().igo->pauseless_dialog->igo_3d_dialog_widget::bring_up(2);

    death_dialog_shown = true;
}

// sub_688B40
i32 ui_frontend::select_ui_environment_index() {
    if (scrapbook->is_active())
        return 1;

    if (pauseless_dialog->is_shown())
        return 3;

    return references::frontend.get().igo->zoom_map->is_open_or_zooming() ? 2 : 0;
}

// sub_689400
i32 ui_frontend::update_widget_suppression_conditions() {
    widget_suppression_conditions = 0;

    game* current_game = references::game.read();

    if (current_game->game_paused)
        widget_suppression_conditions = suppress_when_paused;

    if (current_game->disable_interface)
        widget_suppression_conditions |= suppress_when_interface_disabled;

    ui_frontend* current_igo = references::frontend.get().igo;

    if (current_igo->zoom_map->is_open_or_zooming())
        widget_suppression_conditions |= suppress_when_zoom_map_open;

    if (current_igo->loading_screen && current_igo->loading_screen->is_shown())
        widget_suppression_conditions |= suppress_when_loading_screen;

    if (current_igo->letterbox_widget && current_igo->letterbox_widget->is_shown())
        widget_suppression_conditions |= suppress_when_letterbox_visible;

    u8 visible = current_igo->pauseless_dialog->is_shown();

    if (visible)
        widget_suppression_conditions |= suppress_when_pauseless_dialog;

    return visible;
}

// sub_689500
void ui_frontend::set_generic_meter_visible(bool visible) {
    if (mission_specific_meter)
        mission_specific_meter->set_shown(visible);
}

// sub_689520
void ui_frontend::set_generic_meter_value(f32 value) {
    if (mission_specific_meter)
        mission_specific_meter->set_value(value);
}

// sub_698E90
void ui_frontend::set_dialog_box(dialog_box* box, bool freeze_hero) { // deletes the one before and shows the new one
    if (dialog)
        delete dialog;

    dialog = box;
    box->show(freeze_hero);
}

// sub_717230
void ui_frontend::rebuild_input_device_widgets() {
    if (scrapbook)
        delete scrapbook;

    void* allocation = mm::allocate(sizeof(igo_3d_scrapbook));
    scrapbook = allocation ? (igo_3d_scrapbook*)retail::sub_715C70((u32*)allocation) : nullptr;

    if (scrapbook) {
        scrapbook->stamp_source_vtable();
        scrapbook->unk_024();
    }

    if (startup_widget_184)
        retail::sub_7107A0((i32)startup_widget_184);

    if (pauseless_dialog) {
        delete pauseless_dialog;

        allocation       = mm::allocate(sizeof(igo_3d_pauseless_dialog_widget));
        pauseless_dialog = allocation ? (igo_3d_pauseless_dialog_widget*)retail::sub_70D830((u32*)allocation) : nullptr;

        if (pauseless_dialog)
            pauseless_dialog->init();
    }

    if (conversation_menu_system) {
        delete conversation_menu_system;

        allocation               = mm::allocate(sizeof(treyarch::conversation_menu_system));
        conversation_menu_system = allocation ? (treyarch::conversation_menu_system*)retail::sub_70B6D0((i32)allocation) : nullptr;

        if (conversation_menu_system) {
            conversation_menu_system->stamp_source_vtables();
            conversation_menu_system->init();
        }
    }

    if (startup_widget_17c)
        retail::sub_6AFE00((u32**)startup_widget_17c);

    if (dialog)
        retail::sub_70C1F0((u32*)dialog);
}

// sub_699210
void ui_frontend::set_generic_meter_label(mash::string* label) {
    auto set_label = (void (__thiscall*)(igo_3d_generic_meter_widget*, mash::string, bool))retail::sub_68C960;

    if (mission_specific_meter)
        set_label(mission_specific_meter, mash::string(*label), false);

    label->finalize(mash::ALLOCATED);
}

// sub_6E77C0
void ui_frontend::destroy_fog(finger_of_god* fog) {
    igo_3d_radar_widget* radar = radar_widget;

    if (!radar || !fog)
        return;

    finger_of_god** position = radar->fingers_of_god.begin();

    while (position != radar->fingers_of_god.end() && *position != fog)
        ++position;

    if (position == radar->fingers_of_god.end())
        return;

    delete *position;

    *position = nullptr;

    radar->fingers_of_god.erase(position);
}

// sub_6E7000
void ui_frontend::letterbox(f32 time, f32 height) {
    if (!letterbox_widget)
        return;

    if ((f64)height > 0.0001) {
        ui_frontend* igo = references::frontend.get().igo;

        references::spider_senses_on.write(0);
        igo->background_effect_mode = 0;
        retail::sub_6B17B0(igo, 0.0f); // the background effect update

        if (references::g_world_ptr.read()->get_hero_ptr())
            references::g_world_ptr.read()->get_hero_ptr()->raise_event(references::senses_exited.read());
    }

    if (references::unk_0102d00c.read() != 1.0f)
        height = (f32)(height * 0.5);

    letterbox_widget->letterbox(time, height);
}

// sub_6990D0
void ui_frontend::slide_in_ally_health_widget() {
    if (ally_health_widget)
        retail::sub_68CE10((u32*)ally_health_widget);
}

// sub_6990E0
void ui_frontend::slide_out_ally_health_widget() {
    if (ally_health_widget)
        retail::sub_68CEB0((u32*)ally_health_widget);
}

// sub_6990F0
void ui_frontend::set_ally_health_meter_value(f32 value) {
    ally_health_widget_t* widget = ally_health_widget;

    if (!widget)
        return;

    f32 previous = widget->value;

    widget->value = value;

    if (previous > value) {
        if (value != 0.0f) {
            if (!widget->dropping) {
                widget->dropping  = true;
                widget->drop_from = previous;
            }

            return;
        }
    } else if (!(value > previous))
        return;

    widget->dropping  = false;
    widget->drop_from = widget->value;
}

// sub_699170
void ui_frontend::set_ally_entity(arch_base_vhandle entity) {
    if (ally_health_widget)
        ally_health_widget->entity = entity;
}

// sub_6F21F0
void ui_frontend::set_ally_health_icon(i32 icon) {
    string_hash key;

    retail::sub_6EFF40((u32*)references::ui_character_info_map.read(), (u32*)&key, icon); // the key for the icon

    if (ally_health_widget)
        ((void (__thiscall*)(ally_health_widget_t*, string_hash))retail::sub_6EC1F0)(ally_health_widget, key); // set_icon
}

// sub_6991F0
void ui_frontend::set_boss_enabled(bool enabled) {
    if (enemy_health_manager)
        enemy_health_manager->set_boss_enabled(enabled);
}

// sub_699190
void ui_frontend::slide_in_boss_health_widget() {
    if (enemy_health_manager && enemy_health_manager->boss_widget) {
        enemy_health_manager->boss_widget->set_unk_1c0(true);
        enemy_health_manager->boss_widget->slide_in();
    }
}

// sub_6991C0
void ui_frontend::slide_out_boss_health_widget() {
    if (enemy_health_manager && enemy_health_manager->boss_widget) {
        enemy_health_manager->boss_widget->set_unk_1c0(false);
        enemy_health_manager->boss_widget->slide_out();
    }
}

// sub_6F2230
void ui_frontend::set_boss_health_icon(i32 icon) {
    string_hash key;

    retail::sub_6EFF40((u32*)references::ui_character_info_map.read(), (u32*)&key, icon); // the key for the icon

    igo_3d_enemy_health_manager* manager = enemy_health_manager;

    if (manager && manager->boss_widget) {
        ((void (__thiscall*)(igo_3d_enemy_health_widget*, string_hash))retail::sub_6EC1F0)(manager->boss_widget, key); // set_icon

        manager->boss_icon_set = true;
    }
}

// sub_6894C0
void ui_frontend::set_boss_entity(arch_base_vhandle entity) {
    if (enemy_health_manager)
        enemy_health_manager->boss_entity = entity;
}

// sub_6894E0
void ui_frontend::set_grunt_enabled(bool enabled) {
    if (enemy_health_manager)
        enemy_health_manager->grunt_enabled = enabled;
}
