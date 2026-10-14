#pragma once

#include "treyarch/amalga/mashable_resource_handler.hh"
#include "treyarch/amalga/resource_handler.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

// handlers that still forward to retail; each one moves out as its type gets owned (yay!)
namespace treyarch {
    class vehicle_model_depot;
    class Tilemap;
    class Environment;
    class LightBehavior;
    class GraphicsOptions;
    class EffectContainer;
    class SMDecalMaterial;
    class PhatPalette;
    class CowbellFile;
    class Meshmap;
    class CloudTweak;
    class CityLODs;
    class SMRoadParam;
    class AllBuildings;
    class IGO3DWidgetParameters;
    class IGO3DTextParameters;
    class SMDecalParam;
    class AdjustmentLayersLibrary;
    class Outdoors;
    class daySkyColor;
    class nightSkyColor;
    class PreLight;
    class SMBuildingLODParam;
    class SMRoadLODParam;
    class SMRetrofitParam;
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
    class VisemeStream;
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
        template<> i32 mashable_resource_handler<vehicle_model_depot>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<Tilemap>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<Environment>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<LightBehavior>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<GraphicsOptions>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<EffectContainer>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<SMDecalMaterial>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<PhatPalette>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<CowbellFile>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<Meshmap>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<CloudTweak>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<CityLODs>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<SMRoadParam>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<AllBuildings>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<IGO3DWidgetParameters>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<IGO3DTextParameters>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<SMDecalParam>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<AdjustmentLayersLibrary>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<Outdoors>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<daySkyColor>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<nightSkyColor>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<PreLight>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<SMBuildingLODParam>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<SMRoadLODParam>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
        template<> i32 mashable_resource_handler<SMRetrofitParam>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
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
        template<> i32 mashable_resource_handler<VisemeStream>::handle_resource(e_behavior, resource_descriptor*, limited_timer*);
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
