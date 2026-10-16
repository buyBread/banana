#pragma once

#include "treyarch/amalga/mashable_resource_handler.hh"
#include "treyarch/amalga/resource_handler.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

// handlers that still forward to retail; each one moves out as its type gets owned (yay!)
namespace treyarch {
    class phat_palette;
    class cowbell_file;
    class meshmap;
    class cloud_tweak;
    class city_lods;
    class sm_road_param;
    class all_buildings;
    class igo_3d_widget_parameters;
    class igo_3d_text_parameters;
    class sm_decal_param;
    class adjustment_layers_library;
    class outdoors;
    class day_sky_color;
    class night_sky_color;
    class pre_light;
    class sm_building_lod_param;
    class sm_road_lod_param;
    class sm_retrofit_param;
    class region_spawn_interface;
    class meter_data_file;
    class meter_behavior_file;
    class combo_move_type_file;
    class quest_descriptions;
    class quest_list;
    class conversation;
    class spawn_table_interface;
    class spawn_item_collection;
    class spawn_point_list_collection;
    class neighborhood_interface;
    class default_values;
    class progression_messages;
    class static_spawn_item_collection;
    class rpg_upgrades;
    class sound_map;
    class ui_character_info_list;
    class summon_progression;
    class environment_progression;
    class render_lod_mgr;
    class viseme_stream;
    class static_colgeom_root;
    struct lego_map_root_node;
    class ai_pf_region_graph;
    class path_graph;
    class environment_box_set;
    class cosmetic_environments_set;
    class sm3_light;
    class state_graph;
    class core_ai_resource;
    class region_audio_box_set;
    class fx_cache;
    class cut_scene;
    class sound_toc;
    class sound_index;

    namespace als {
        class animation_logic_system_shared;
    } // als

    namespace amalga {
        template<> i32 mashable_resource_handler<phat_palette>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<cowbell_file>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<meshmap>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<cloud_tweak>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<city_lods>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<sm_road_param>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<all_buildings>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<igo_3d_widget_parameters>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<igo_3d_text_parameters>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<sm_decal_param>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<adjustment_layers_library>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<outdoors>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<day_sky_color>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<night_sky_color>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<pre_light>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<sm_building_lod_param>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<sm_road_lod_param>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<sm_retrofit_param>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<region_spawn_interface>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<meter_data_file>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<meter_behavior_file>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<combo_move_type_file>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<quest_descriptions>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<quest_list>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<conversation>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<spawn_table_interface>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<spawn_item_collection>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<spawn_point_list_collection>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<neighborhood_interface>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<default_values>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<progression_messages>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<static_spawn_item_collection>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<rpg_upgrades>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<sound_map>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<ui_character_info_list>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<summon_progression>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<environment_progression>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<render_lod_mgr>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<viseme_stream>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<static_colgeom_root>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<lego_map_root_node>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<ai_pf_region_graph>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<path_graph>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<environment_box_set>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<cosmetic_environments_set>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<sm3_light>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<als::animation_logic_system_shared>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<state_graph>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<core_ai_resource>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<region_audio_box_set>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<fx_cache>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<cut_scene>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<sound_toc>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<sound_index>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);

        struct slim_list_handler : resource_handler_base {
            i32 handle_resource(e_behavior           behavior,
                                resource_descriptor* descriptor,
                                limited_timer*       time_limit) override;
        };

        struct box_trigger_resource_handler : resource_handler_base {
            i32 handle_resource(e_behavior           behavior,
                                resource_descriptor* descriptor,
                                limited_timer*       time_limit) override;
        };

        struct scene_entity_container_resource_handler : resource_handler_base {
            i32 unk_14;
            i32 unk_18;

            // sub_72E330
            void pre_handle_resources(e_behavior) override {
                unk_14 = 0;
                unk_18 = 0;
            }

            i32 handle_resource(e_behavior           behavior,
                                resource_descriptor* descriptor,
                                limited_timer*       time_limit) override;
        };

        struct anchor_collection_resource_handler : resource_handler_base {
            i32 unk_14;
            i32 unk_18;
            i32 unk_1C;

            // sub_72E340
            void pre_handle_resources(e_behavior) override {
                unk_14 = -1;
                unk_18 = 0;
            }

            i32 handle_resource(e_behavior           behavior,
                                resource_descriptor* descriptor,
                                limited_timer*       time_limit) override;
        };

        struct ise_library_handler : resource_handler_base {
            i32 handle_resource(e_behavior           behavior,
                                resource_descriptor* descriptor,
                                limited_timer*       time_limit) override;
        };

        struct vertex_shader_handler : resource_handler_base {
            i32 handle_resource(e_behavior           behavior,
                                resource_descriptor* descriptor,
                                limited_timer*       time_limit) override;
        };

        struct pixel_shader_handler : resource_handler_base {
            i32 handle_resource(e_behavior           behavior,
                                resource_descriptor* descriptor,
                                limited_timer*       time_limit) override;
        };

        ASSERT_SIZEOF  (scene_entity_container_resource_handler,         0x1C);
        ASSERT_OFFSETOF(scene_entity_container_resource_handler, unk_14, 0x14);
        ASSERT_OFFSETOF(scene_entity_container_resource_handler, unk_18, 0x18);

        ASSERT_SIZEOF  (anchor_collection_resource_handler,         0x20);
        ASSERT_OFFSETOF(anchor_collection_resource_handler, unk_14, 0x14);
        ASSERT_OFFSETOF(anchor_collection_resource_handler, unk_18, 0x18);
        ASSERT_OFFSETOF(anchor_collection_resource_handler, unk_1C, 0x1C);
    } // amalga
} // treyarch
