#pragma once

#include "vc8/list.hh"
#include "game/frontend/conversation_menu_system.hh"
#include "game/frontend/dialog_box.hh"
#include "game/frontend/entity_tracker.hh"
#include "game/frontend/igo/igo_3d_agility_test.hh"
#include "game/frontend/igo/igo_3d_boss_meter_system.hh"
#include "game/frontend/igo/igo_3d_buddy_summon_widget.hh"
#include "game/frontend/igo/igo_3d_button_tips.hh"
#include "game/frontend/igo/igo_3d_enemy_health_manager.hh"
#include "game/frontend/igo/igo_3d_generic_meter_widget.hh"
#include "game/frontend/igo/igo_3d_letterbox.hh"
#include "game/frontend/igo/igo_3d_loading_screen.hh"
#include "game/frontend/igo/igo_3d_message_box_widget.hh"
#include "game/frontend/igo/igo_3d_objective_text_widget.hh"
#include "game/frontend/igo/igo_3d_pauseless_dialog_widget.hh"
#include "game/frontend/igo/igo_3d_ped_warning_widget.hh"
#include "game/frontend/igo/igo_3d_radar_widget.hh"
#include "game/frontend/igo/igo_3d_script_widget.hh"
#include "game/frontend/igo/igo_3d_spidey_distance_widget.hh"
#include "game/frontend/igo/igo_3d_timer_widget.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "game/frontend/igo/igo_3d_zoom_map.hh"
#include "game/frontend/igo/igo_generic_decay_timer.hh"
#include "game/frontend/igo/igo_widget_group.hh"
#include "treyarch/ngl/quad/quad.hh"
#include "treyarch/ngl/shaders/pcuv/render_node.hh"
#include "game/container/legacy_list.hh"
#include "game/math/algebra.hh"
#include "treyarch/math/matrix.hh"
#include "treyarch/mash/string_hash.hh"
#include "game/mutex.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_camera_widget;
    class igo_3d_face_button_system;
    class igo_3d_nav_button_bar;
    class igo_3d_scrapbook;

    class ui_frontend;

    using ui_frontend_method         = void (__thiscall*)(ui_frontend* self);
    using ui_frontend_advance_method = void (__thiscall*)(ui_frontend* self, f32 time_inc);
    using ui_frontend_show_method    = void (__thiscall*)(ui_frontend* self, bool shown);

    struct ui_frontend_vtable {
        void*                      reserved_000[6];
        ui_frontend_advance_method update; // KSPS IGOUpdate, SM3 IGOFrontEnd::Update; see game::frame_advance_game_overlays
        void*                      reserved_01c[2];
        ui_frontend_method         draw;
        ui_frontend_method         draw_startup;
        ui_frontend_method         draw_quad_list;
        ui_frontend_method         draw_pass_1;
        ui_frontend_method         draw_pass_2;
        void*                      reserved_038[2];
        ui_frontend_show_method    set_meters_shown;
        ui_frontend_show_method    set_radar_shown;
    };

    using igo_3d_script_widget_list = vc8::list<igo_3d_script_widget*>;

    // what the ui light natives read and write; pc's light lookup never finds one
    struct ui_light {
        f32 near_dist;
        f32 far_dist;
        f32 x;
        f32 y;
    };

    namespace references {
        inline util::memory_reference<u8> optional_widget_group_enabled { 0x00BB1EA9 };

        // guards UIFrontEnd's three script-widget lists
        inline util::memory_reference<engine_recursive_lock> script_widget_lock { 0x0102F430 };

        // the camera axes draw_background_texture measures, (1, 0, 0) and (0, 1, 0) in retail data
        inline util::memory_reference<vector3d> background_texture_u_axis { 0x00E73660 };
        inline util::memory_reference<vector3d> background_texture_v_axis { 0x00E7366C };

        // background mode 4 sets it; nothing reads it
        inline util::memory_reference<u32> unk_0102fe9c { 0x0102FE9C };

        // spiderman_is_spider_senses_on is true while it's set
        inline util::memory_reference<u8> spider_senses_on { 0x0102CDB5 };

        // 0 in retail data and never written; letterbox keeps the height only at exactly 1
        inline util::memory_reference<f32> unk_0102d00c { 0x0102D00C };

        inline util::memory_reference<string_hash> senses_exited { 0x0102C48C };
    } // references

    // the ally meter at ui_frontend +0x1ac
    class ally_health_widget_t : public igo_3d_widget {

    public:
        u8                reserved_150[0x48];
        f32               value;
        f32               drop_from;          // the value a drop started from
        bool              dropping;           // a drop latches drop_from, a rise or a drop to 0 resets it
        u8                reserved_1a1[0x1F];
        arch_base_vhandle entity;
    };

    ASSERT_OFFSETOF(ally_health_widget_t, value,     0x198);
    ASSERT_OFFSETOF(ally_health_widget_t, drop_from, 0x19C);
    ASSERT_OFFSETOF(ally_health_widget_t, dropping,  0x1A0);
    ASSERT_OFFSETOF(ally_health_widget_t, entity,    0x1C0);

    class ui_frontend {

    public:
        ui_frontend_vtable*                 vtable;
        u8                                  reserved_004[0xB4];
        igo_3d_widget*                      special_meter;
        igo_3d_widget*                      unknown_widget_0bc;
        igo_3d_widget*                      unknown_widget_0c0;
        igo_3d_ped_warning_widget*          ped_warning;
        igo_3d_widget*                      unknown_widget_0c8;
        igo_3d_widget*                      unknown_widget_0cc;
        igo_3d_buddy_summon_widget*         buddy_summon;
        igo_3d_widget*                      combo_meter;
        igo_3d_widget*                      hero_health;
        igo_3d_widget*                      hero_portrait;
        igo_3d_spidey_distance_widget*      spidey_distance;
        igo_3d_boss_meter_system*           boss_meter_system;
        igo_3d_pauseless_dialog_widget*     pauseless_dialog;
        igo_3d_widget*                      district_marker;
        igo_3d_widget*                      unlock_message;
        igo_3d_widget*                      combo_move_tracker;
        igo_3d_widget*                      combo_counter_message;
        igo_3d_widget*                      hint_text_message;
        igo_3d_widget*                      mem_card_message;
        igo_3d_timer_widget*                timer_widget;
        igo_3d_radar_widget*                radar_widget;
        igo_3d_nav_button_bar*              nav_button_bar;
        igo_3d_widget*                      modal_save_dialog;
        igo_3d_widget*                      generic_modal_dialog;
        igo_3d_agility_test*                agility_test;
        igo_3d_widget*                      autosave_indicator;
        igo_3d_widget*                      controller_disconnect_dialog;
        igo_3d_face_button_system*          face_button_system;
        igo_3d_scrapbook*                   scrapbook;
        igo_3d_zoom_map*                    zoom_map;
        igo_3d_loading_screen*              loading_screen;
        igo_3d_widget*                      saving_or_loading_screen;
        igo_3d_widget*                      unknown_widget_138;
        igo_3d_widget*                      unknown_widget_13c;
        igo_3d_widget*                      unknown_widget_140;
        igo_3d_message_box_widget*          unified_announcement;
        igo_3d_message_box_widget*          unified_announcement_alias;
        igo_3d_message_box_widget*          unknown_widget_14c;
        igo_3d_objective_text_widget*       objective_text;
        igo_3d_hint_text_widget*            hint_text;
        igo_3d_letterbox*                   letterbox_widget;
        igo_3d_button_tips*                 button_tips;
        treyarch::conversation_menu_system* conversation_menu_system;
        dialog_box*                         dialog;
        u8                                  conversation_state_168;
        u8                                  reserved_169[0x03];
        entity_tracker_manager*             entity_tracker_mgr;
        igo_3d_widget*                      unknown_widget_170;
        void*                               unknown_widget_174;
        u8                                  reserved_178[0x04];
        igo_3d_widget*                      startup_widget_17c;
        igo_3d_widget*                      startup_widget_180;
        igo_3d_widget*                      startup_widget_184;
        igo_3d_widget*                      unknown_widget_188;
        igo_3d_camera_widget*               camera_widget;
        u8                                  reserved_190[0x04];
        igo_3d_widget**                     optional_widgets_begin;
        igo_3d_widget**                     optional_widgets_end;
        igo_3d_widget**                     optional_widgets_capacity;
        igo_3d_widget*                      optional_widget;
        igo_widget_group*                   widget_group_1a4;
        igo_3d_widget*                      unknown_widget_1a8;
        ally_health_widget_t*               ally_health_widget;     // only ever null on pc
        igo_3d_enemy_health_manager*        enemy_health_manager;
        igo_3d_generic_meter_widget*        mission_specific_meter;
        void*                               ui_lights[4][3];
        void*                               ui_environments[4];
        void*                               current_ui_environment;
        igo_3d_widget*                      text_debug_widget;
        u8                                  quad_list_state_200;
        u8                                  reserved_201[0x03];
        container::legacy_list<ngl::quad*>  quad_list;
        math::mat43                         quad_transform;
        ngl::shaders::pcuv::pcuv_material*  flat_material;
        igo_3d_script_widget_list           script_widgets;         // create_3d_widget, _button_widget, _widget_from_params, _text_widget_from_params
        igo_3d_script_widget_list           mission_text_widgets;   // create_3d_mission_text_widget
        igo_3d_script_widget_list           hint_text_widgets;      // create_3d_hint_text_widget
        u8                                  scene_draw_state_278;
        u8                                  reserved_279[0x03];
        u32                                 background_effect_mode;     // the RenderBackgroundEffects switch
        ngl::texture*                       background_texture;         // modes 7-9
        igo_generic_decay_timer             background_effect_fade;     // its value is the strength of modes 2, 3, 8, 9
        igo_generic_decay_timer             unk_2a0;
        u8                                  reserved_2bc[0x18];
        u32                                 widget_suppression_conditions;
        vc8::list<igo_3d_widget*>           widgets; // drawn unless their suppression mask matches

        void draw();
        void draw_startup();
        void draw_quad_list();
        void clear_quad_list();
        i32 select_ui_environment_index();
        i32 update_widget_suppression_conditions();

        void draw_widgets();
        void draw_script_widgets();
        void draw_mission_text_widget();
        void draw_hint_text_widget();
        void draw_interface_disabled_script_widgets(bool interface_disabled);

        void set_dialog_box(dialog_box* box, bool freeze_hero);

        void set_generic_meter_visible(bool visible);
        void set_generic_meter_value  (f32 value);
        void set_generic_meter_label  (mash::string* label); // finalizes the label

        void destroy_fog(finger_of_god* fog); // deletes it and drops it from the radar

        void letterbox(f32 time, f32 height); // leaves the senses first when the height is above 0.0001

        void slide_in_ally_health_widget();
        void slide_out_ally_health_widget();
        void set_ally_health_meter_value(f32 value);
        void set_ally_health_icon(i32 icon);
        void set_ally_entity(arch_base_vhandle entity);

        void set_boss_enabled(bool enabled);
        void slide_in_boss_health_widget();
        void slide_out_boss_health_widget();
        void set_boss_health_icon(i32 icon); // also marks the manager's icon as set
        void set_boss_entity(arch_base_vhandle entity);
        void set_grunt_enabled(bool enabled);

        // listed and its entity still there; a listed widget whose entity is gone gets destroyed
        bool is_script_widget_valid(igo_3d_script_widget* widget);

        void destroy_script_widget      (igo_3d_script_widget* widget); // whichever list has it
        bool destroy_mission_text_widget(igo_3d_script_widget* widget); // false when unlisted or the instance ids differ
        bool destroy_hint_text_widget   (igo_3d_script_widget* widget); // false when unlisted or the instance ids differ

        void destroy_all_script_widgets(); // all three lists
        void destroy_all_mission_text_widgets();
        void destroy_all_hint_text_widgets();
        void hide_all_mission_text_widgets();
        void hide_all_hint_text_widgets();

        // inlined @ sub_6D85F0
        static igo_3d_script_widget* find_script_widget(const igo_3d_script_widget_list &list, igo_3d_script_widget* widget) {
            for (auto* position = list.begin(); position != list.end(); position = position->next) {
                if (position->value == widget)
                    return position->value;
            }

            return nullptr;
        }

        void render_background_effects();
        void draw_background_texture(f32 strength);

        static void draw_background_overlay(f32 strength, u32 unused);
    };

    ASSERT_SIZEOF  (ui_frontend_vtable,                   0x48);
    ASSERT_OFFSETOF(ui_frontend_vtable, update,           0x18);
    ASSERT_OFFSETOF(ui_frontend_vtable, draw,             0x24);
    ASSERT_OFFSETOF(ui_frontend_vtable, draw_startup,     0x28);
    ASSERT_OFFSETOF(ui_frontend_vtable, draw_quad_list,   0x2C);
    ASSERT_OFFSETOF(ui_frontend_vtable, draw_pass_1,      0x30);
    ASSERT_OFFSETOF(ui_frontend_vtable, draw_pass_2,      0x34);
    ASSERT_OFFSETOF(ui_frontend_vtable, set_meters_shown, 0x40);
    ASSERT_OFFSETOF(ui_frontend_vtable, set_radar_shown,  0x44);

    ASSERT_OFFSETOF(ui_frontend, special_meter,                 0x0B8);
    ASSERT_OFFSETOF(ui_frontend, pauseless_dialog,              0x0E8);
    ASSERT_OFFSETOF(ui_frontend, scrapbook,                     0x128);
    ASSERT_OFFSETOF(ui_frontend, zoom_map,                      0x12C);
    ASSERT_OFFSETOF(ui_frontend, loading_screen,                0x130);
    ASSERT_OFFSETOF(ui_frontend, letterbox_widget,              0x158);
    ASSERT_OFFSETOF(ui_frontend, button_tips,                   0x15C);
    ASSERT_OFFSETOF(ui_frontend, unified_announcement,          0x144);
    ASSERT_OFFSETOF(ui_frontend, conversation_menu_system,      0x160);
    ASSERT_OFFSETOF(ui_frontend, dialog,                        0x164);
    ASSERT_OFFSETOF(ui_frontend, entity_tracker_mgr,            0x16C);
    ASSERT_OFFSETOF(ui_frontend, unknown_widget_174,            0x174);
    ASSERT_OFFSETOF(ui_frontend, startup_widget_17c,            0x17C);
    ASSERT_OFFSETOF(ui_frontend, camera_widget,                 0x18C);
    ASSERT_OFFSETOF(ui_frontend, optional_widgets_begin,        0x194);
    ASSERT_OFFSETOF(ui_frontend, ui_lights,                     0x1B8);
    ASSERT_OFFSETOF(ui_frontend, ui_environments,               0x1E8);
    ASSERT_OFFSETOF(ui_frontend, current_ui_environment,        0x1F8);
    ASSERT_OFFSETOF(ui_frontend, text_debug_widget,             0x1FC);
    ASSERT_OFFSETOF(ui_frontend, quad_list_state_200,           0x200);
    ASSERT_OFFSETOF(ui_frontend, quad_list,                     0x204);
    ASSERT_OFFSETOF(ui_frontend, quad_transform,                0x210);
    ASSERT_OFFSETOF(ui_frontend, flat_material,                 0x250);
    ASSERT_OFFSETOF(ui_frontend, script_widgets,                0x254);
    ASSERT_OFFSETOF(ui_frontend, mission_text_widgets,          0x260);
    ASSERT_OFFSETOF(ui_frontend, hint_text_widgets,             0x26C);
    ASSERT_OFFSETOF(ui_frontend, scene_draw_state_278,          0x278);
    ASSERT_OFFSETOF(ui_frontend, background_effect_mode,        0x27C);
    ASSERT_OFFSETOF(ui_frontend, background_texture,            0x280);
    ASSERT_OFFSETOF(ui_frontend, background_effect_fade,        0x284);
    ASSERT_OFFSETOF(ui_frontend, unk_2a0,                       0x2A0);
    ASSERT_OFFSETOF(ui_frontend, widget_suppression_conditions, 0x2D4);
    ASSERT_OFFSETOF(ui_frontend, widgets,                       0x2D8);
} // treyarch
