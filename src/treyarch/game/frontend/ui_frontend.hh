#pragma once

#include "treyarch/game/frontend/conversation_menu_system.hh"
#include "treyarch/game/frontend/igo/igo_3d_boss_meter_system.hh"
#include "treyarch/game/frontend/igo/igo_3d_enemy_health_manager.hh"
#include "treyarch/game/frontend/igo/igo_3d_ped_warning_widget.hh"
#include "treyarch/game/frontend/igo/igo_3d_script_widget.hh"
#include "treyarch/game/frontend/igo/igo_3d_widget.hh"
#include "treyarch/game/frontend/igo/igo_3d_zoom_map.hh"
#include "treyarch/game/frontend/igo/igo_widget_group.hh"
#include "treyarch/game/frontend/ui_object_manager.hh"
#include "treyarch/ngl/quad/quad.hh"
#include "treyarch/ngl/shaders/pcuv/render_node.hh"
#include "treyarch/shared/container/legacy_list.hh"
#include "treyarch/shared/dinkumware/list.hh"
#include "treyarch/shared/math/types/matrix4x4.hh"
#include "treyarch/shared/mutex.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    class igo_3d_camera_widget;
    class igo_3d_face_button_system;
    class igo_3d_loading_screen;
    class igo_3d_nav_button_bar;
    class igo_3d_scrapbook;

    class ui_frontend;

    using ui_frontend_method         = void (__thiscall*)(ui_frontend* self);
    using ui_frontend_advance_method = void (__thiscall*)(ui_frontend* self, f32 time_inc);

    struct ui_frontend_vtable {
        void*                      reserved_000[6];
        ui_frontend_advance_method update; // KSPS IGOUpdate, SM3 IGOFrontEnd::Update; see game::frame_advance_game_overlays
        void*                      reserved_01c[2];
        ui_frontend_method         draw;
        ui_frontend_method         draw_startup;
        ui_frontend_method         draw_quad_list;
        ui_frontend_method         draw_pass_1;
        ui_frontend_method         draw_pass_2;
    };

    using igo_3d_script_widget_list = dinkumware::list<igo_3d_script_widget*>;

    namespace references {
        inline util::memory_reference<u8> optional_widget_group_enabled { 0x00BB1EA9 };

        // guards UIFrontEnd's three script-widget lists
        inline util::memory_reference<engine_recursive_lock> script_widget_lock { 0x0102F430 };

        // the camera axes draw_background_texture measures, (1, 0, 0) and (0, 1, 0) in retail data
        inline util::memory_reference<vector3> background_texture_u_axis { 0x00E73660 };
        inline util::memory_reference<vector3> background_texture_v_axis { 0x00E7366C };

        // background mode 4 sets it; nothing reads it
        inline util::memory_reference<u32> unk_0102fe9c { 0x0102FE9C };
    } // references

    class ui_frontend {

    public:
        ui_frontend_vtable*                 vtable;
        u8                                  reserved_004[0xB4];
        igo_3d_widget*                      special_meter;
        igo_3d_drawable*                    unknown_widget_0bc;
        igo_3d_drawable*                    unknown_widget_0c0;
        igo_3d_ped_warning_widget*          ped_warning;
        igo_3d_drawable*                    unknown_widget_0c8;
        igo_3d_drawable*                    unknown_widget_0cc;
        igo_3d_widget*                      buddy_summon;
        igo_3d_widget*                      combo_meter;
        igo_3d_widget*                      hero_health;
        igo_3d_widget*                      hero_portrait;
        igo_3d_widget*                      spidey_distance;
        igo_3d_boss_meter_system*           boss_meter_system;
        igo_3d_widget*                      pauseless_dialog;
        igo_3d_widget*                      district_marker;
        igo_3d_widget*                      unlock_message;
        igo_3d_widget*                      combo_move_tracker;
        igo_3d_widget*                      combo_counter_message;
        igo_3d_widget*                      hint_text_message;
        igo_3d_widget*                      mem_card_message;
        igo_3d_widget*                      timer_widget;
        igo_3d_widget*                      radar_widget;
        igo_3d_nav_button_bar*              nav_button_bar;
        igo_3d_widget*                      modal_save_dialog;
        igo_3d_widget*                      generic_modal_dialog;
        igo_3d_widget*                      agility_test;
        igo_3d_widget*                      autosave_indicator;
        igo_3d_widget*                      controller_disconnect_dialog;
        igo_3d_face_button_system*          face_button_system;
        igo_3d_scrapbook*                   scrapbook;
        igo_3d_zoom_map*                    zoom_map;
        igo_3d_loading_screen*              loading_screen;
        igo_3d_widget*                      saving_or_loading_screen;
        igo_3d_drawable*                    unknown_widget_138;
        igo_3d_drawable*                    unknown_widget_13c;
        igo_3d_drawable*                    unknown_widget_140;
        igo_3d_widget*                      unified_announcement;
        igo_3d_widget*                      unified_announcement_alias;
        igo_3d_drawable*                    unknown_widget_14c;
        igo_3d_widget*                      objective_text;
        igo_3d_widget*                      hint_text;
        igo_3d_widget*                      letterbox;
        igo_3d_widget*                      button_tips;
        treyarch::conversation_menu_system* conversation_menu_system;
        void*                               widget_owner_164;
        u8                                  conversation_state_168;
        u8                                  reserved_169[0x03];
        treyarch::ui_object_manager*        ui_object_manager;
        u8                                  reserved_170[0x04];
        void*                               unknown_widget_174;
        u8                                  reserved_178[0x04];
        igo_3d_drawable*                    startup_widget_17c;
        igo_3d_drawable*                    startup_widget_180;
        igo_3d_drawable*                    startup_widget_184;
        igo_3d_drawable*                    unknown_widget_188;
        igo_3d_camera_widget*               camera_widget;
        u8                                  reserved_190[0x04];
        igo_3d_drawable**                   optional_widgets_begin;
        igo_3d_drawable**                   optional_widgets_end;
        igo_3d_drawable**                   optional_widgets_capacity;
        igo_3d_drawable*                    optional_widget;
        igo_widget_group*                   widget_group_1a4;
        igo_3d_drawable*                    unknown_widget_1a8;
        igo_3d_drawable*                    unknown_widget_1ac;
        igo_3d_enemy_health_manager*        enemy_health_manager;
        igo_3d_widget*                      mission_specific_meter;
        void*                               ui_lights[4][3];
        void*                               ui_environments[4];
        void*                               current_ui_environment;
        igo_3d_widget*                      text_debug_widget;
        u8                                  quad_list_state_200;
        u8                                  reserved_201[0x03];
        container::legacy_list<ngl::quad*>  quad_list;
        matrix4x4                           quad_transform;
        ngl::shaders::pcuv::pcuv_material*  flat_material;
        igo_3d_script_widget_list           script_widgets;         // create_3d_widget, _button_widget, _widget_from_params, _text_widget_from_params
        igo_3d_script_widget_list           mission_text_widgets;   // create_3d_mission_text_widget
        igo_3d_script_widget_list           hint_text_widgets;      // create_3d_hint_text_widget
        u8                                  scene_draw_state_278;
        u8                                  reserved_279[0x03];
        u32                                 background_effect_mode;     // the RenderBackgroundEffects switch
        ngl::texture*                       background_texture;         // modes 7-9
        u8                                  reserved_284[0x08];         // an IGOGenericDecayTimer starts here
        f32                                 background_effect_strength; // that timer's first float; modes 2, 3, 8, 9
        u8                                  reserved_290[0x44];
        u32                                 widget_suppression_conditions;
        dinkumware::list<igo_3d_widget*>    widgets; // drawn unless their suppression mask matches

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

        void render_background_effects();
        void draw_background_texture(f32 strength);

        static void draw_background_overlay(f32 strength, u32 unused);

        bool blocks_world_rendering() const {
            return zoom_map->blocks_world_rendering();
        }
    };

    ASSERT_SIZEOF  (ui_frontend_vtable,                 0x38);
    ASSERT_OFFSETOF(ui_frontend_vtable, update,         0x18);
    ASSERT_OFFSETOF(ui_frontend_vtable, draw,           0x24);
    ASSERT_OFFSETOF(ui_frontend_vtable, draw_startup,   0x28);
    ASSERT_OFFSETOF(ui_frontend_vtable, draw_quad_list, 0x2C);
    ASSERT_OFFSETOF(ui_frontend_vtable, draw_pass_1,    0x30);
    ASSERT_OFFSETOF(ui_frontend_vtable, draw_pass_2,    0x34);

    ASSERT_OFFSETOF(ui_frontend, special_meter,                 0x0B8);
    ASSERT_OFFSETOF(ui_frontend, pauseless_dialog,              0x0E8);
    ASSERT_OFFSETOF(ui_frontend, scrapbook,                     0x128);
    ASSERT_OFFSETOF(ui_frontend, zoom_map,                      0x12C);
    ASSERT_OFFSETOF(ui_frontend, loading_screen,                0x130);
    ASSERT_OFFSETOF(ui_frontend, letterbox,                     0x158);
    ASSERT_OFFSETOF(ui_frontend, button_tips,                   0x15C);
    ASSERT_OFFSETOF(ui_frontend, conversation_menu_system,      0x160);
    ASSERT_OFFSETOF(ui_frontend, ui_object_manager,             0x16C);
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
    ASSERT_OFFSETOF(ui_frontend, background_effect_strength,    0x28C);
    ASSERT_OFFSETOF(ui_frontend, widget_suppression_conditions, 0x2D4);
    ASSERT_OFFSETOF(ui_frontend, widgets,                       0x2D8);
} // treyarch
