#include "treyarch/amalga/descriptor/resource_handler.hh"
#include "treyarch/amalga/empty_resource_handler.hh"
#include "treyarch/amalga/merged_apk/resource_handler.hh"
#include "treyarch/amalga/merged_singular_apk_resource_handler.hh"
#include "treyarch/amalga/native_resource_handlers.hh"
#include "treyarch/amalga/script/resource_handler.hh"
#include "treyarch/amalga/sin/resource_handler.hh"
#include "treyarch/amalga/stall/resource_handler.hh"
#include "treyarch/amalga/terrain_types/resource_handler.hh"
#include "treyarch/amalga/textfile/resource_handler.hh"
#include "treyarch/amalga/worldly_pack_slot.hh"

using namespace treyarch;

// inlined @ sub_73A710
template<typename T>
void amalga::worldly_pack_slot::create_resource_handler(e_resource_type type) {
    resource_handler_base* &handler = resource_handlers[(u32)type];

    handler            = new T;
    handler->pack_slot = this;
    handler->type      = type;
}

// sub_73A710
amalga::worldly_pack_slot::worldly_pack_slot() {
    vtable = &references::worldly_pack_slot_vtable.get();

    create_resource_handler<empty_resource_handler>(e_resource_type::error);
    create_resource_handler<level_descriptors_resource_handler>(e_resource_type::descriptor);
    create_resource_handler<sin_resource_handler>(e_resource_type::sin);
    create_resource_handler<text_resource_handler>(e_resource_type::textfile);
    create_resource_handler<terrain_types_resource_handler>(e_resource_type::terrain_types);
    create_resource_handler<mashable_resource_handler<localized_string_table>>(e_resource_type::language);
    create_resource_handler<mashable_resource_handler<district_graph_container>>(e_resource_type::district_graph);
    create_resource_handler<mashable_resource_handler<speedtree_collection>>(e_resource_type::speedtree_coll);
    create_resource_handler<mashable_resource_handler<speedtree_collection_reference>>(e_resource_type::speedtree_coll_refs);
    create_resource_handler<script_resource_handler>(e_resource_type::script);
    create_resource_handler<script_var_container_resource_handler>(e_resource_type::script_gv);
    create_resource_handler<script_var_container_resource_handler>(e_resource_type::script_sv);
    create_resource_handler<mashable_resource_handler<gb_map>>(e_resource_type::GenBlockMap);
    create_resource_handler<mashable_resource_handler<region_traffic_light_info>>(e_resource_type::traffic_light_info);
    create_resource_handler<merged_singular_apk_resource_handler>(e_resource_type::texture);
    create_resource_handler<merged_singular_apk_resource_handler>(e_resource_type::fxbinary);
    create_resource_handler<merged_singular_apk_resource_handler>(e_resource_type::material);
    create_resource_handler<merged_singular_apk_resource_handler>(e_resource_type::mesh);
    create_resource_handler<merged_singular_apk_resource_handler>(e_resource_type::ngl_skeleton);
    create_resource_handler<merged_singular_apk_resource_handler>(e_resource_type::nal_skeleton);
    create_resource_handler<merged_singular_apk_resource_handler>(e_resource_type::ngl_font);
    create_resource_handler<merged_singular_apk_resource_handler>(e_resource_type::animation);
    create_resource_handler<merged_singular_apk_resource_handler>(e_resource_type::scene_animation);
    create_resource_handler<merged_singular_apk_resource_handler>(e_resource_type::morph);
    create_resource_handler<merged_singular_apk_resource_handler>(e_resource_type::efx);
    create_resource_handler<merged_apk_resource_handler>(e_resource_type::merged_apk);
    create_resource_handler<singular_apk_resource_handler>(e_resource_type::nal_skeleton_map);
    create_resource_handler<singular_apk_resource_handler>(e_resource_type::convex_hull_mesh);
    create_resource_handler<mashable_resource_handler<vehicle_model_depot>>(e_resource_type::vehicle_model_depot);
    create_resource_handler<mashable_resource_handler<Tilemap>>(e_resource_type::TILEMAP);
    create_resource_handler<mashable_resource_handler<Environment>>(e_resource_type::ENVIRONMENT);
    create_resource_handler<mashable_resource_handler<LightBehavior>>(e_resource_type::LIGHTBEHAVIOR);
    create_resource_handler<mashable_resource_handler<GraphicsOptions>>(e_resource_type::GRAPHICSOPTIONS);
    create_resource_handler<mashable_resource_handler<EffectContainer>>(e_resource_type::EFFECTCONTAINER);
    create_resource_handler<mashable_resource_handler<SMDecalMaterial>>(e_resource_type::SMDECALMATERIAL);
    create_resource_handler<mashable_resource_handler<PhatPalette>>(e_resource_type::PHATPALETTE);
    create_resource_handler<mashable_resource_handler<CowbellFile>>(e_resource_type::COWBELL);
    create_resource_handler<mashable_resource_handler<Meshmap>>(e_resource_type::MESHMAP);
    create_resource_handler<mashable_resource_handler<CloudTweak>>(e_resource_type::CLOUDTWEAK);
    create_resource_handler<mashable_resource_handler<CityLODs>>(e_resource_type::CITYLODS);
    create_resource_handler<mashable_resource_handler<SMRoadParam>>(e_resource_type::SMROADPARAM);
    create_resource_handler<mashable_resource_handler<AllBuildings>>(e_resource_type::ALLBUILDINGS);
    create_resource_handler<mashable_resource_handler<IGO3DWidgetParameters>>(e_resource_type::UIP3DW);
    create_resource_handler<mashable_resource_handler<IGO3DTextParameters>>(e_resource_type::UIP3DT);
    create_resource_handler<mashable_resource_handler<SMDecalParam>>(e_resource_type::SMDECALPARAM);
    create_resource_handler<mashable_resource_handler<AdjustmentLayersLibrary>>(e_resource_type::ADJUSTMENTLAYERSLIBRARY);
    create_resource_handler<mashable_resource_handler<Outdoors>>(e_resource_type::OUTDOORS);
    create_resource_handler<mashable_resource_handler<daySkyColor>>(e_resource_type::DAYSKYCOLOR);
    create_resource_handler<mashable_resource_handler<nightSkyColor>>(e_resource_type::NIGHTSKYCOLOR);
    create_resource_handler<mashable_resource_handler<PreLight>>(e_resource_type::PRELIGHT);
    create_resource_handler<mashable_resource_handler<SMBuildingLODParam>>(e_resource_type::SMBUILDINGLODPARAM);
    create_resource_handler<mashable_resource_handler<SMRoadLODParam>>(e_resource_type::SMROADLODPARAM);
    create_resource_handler<mashable_resource_handler<SMRetrofitParam>>(e_resource_type::SMRETROFITPARAM);
    create_resource_handler<mashable_resource_handler<region_spawn_interface>>(e_resource_type::JSON_REGIONINFO);
    create_resource_handler<mashable_resource_handler<meter_data_file>>(e_resource_type::JSON_METERDATA);
    create_resource_handler<mashable_resource_handler<meter_behavior_file>>(e_resource_type::JSON_METERBEHAVIOR);
    create_resource_handler<mashable_resource_handler<combo_move_type_file>>(e_resource_type::JSON_COMBOMOVEINFO);
    create_resource_handler<mashable_resource_handler<quest_descriptions>>(e_resource_type::JSON_QUESTDESC);
    create_resource_handler<mashable_resource_handler<quest_list>>(e_resource_type::JSON_QUESTLIST);
    create_resource_handler<mashable_resource_handler<conversation>>(e_resource_type::JSON_CONVERSATION);
    create_resource_handler<mashable_resource_handler<spawn_table_interface>>(e_resource_type::JSON_SPAWNTABLE);
    create_resource_handler<mashable_resource_handler<spawn_item_collection>>(e_resource_type::JSON_SPAWNPROGRESSION);
    create_resource_handler<mashable_resource_handler<spawn_point_list_collection>>(e_resource_type::JSON_SPAWNPOINTS);
    create_resource_handler<mashable_resource_handler<neighborhood_interface>>(e_resource_type::JSON_NEIGHBORHOODINFO);
    create_resource_handler<mashable_resource_handler<default_values>>(e_resource_type::JSON_DEF_VALUES);
    create_resource_handler<mashable_resource_handler<progression_messages>>(e_resource_type::JSON_PROGRESSMESSAGES);
    create_resource_handler<mashable_resource_handler<static_spawn_item_collection>>(e_resource_type::JSON_SPAWNITEMS);
    create_resource_handler<mashable_resource_handler<rpg_upgrades>>(e_resource_type::JSON_UPGRADES);
    create_resource_handler<mashable_resource_handler<sound_map>>(e_resource_type::JSON_SOUNDMAP);
    create_resource_handler<mashable_resource_handler<ui_character_info_list>>(e_resource_type::JSON_UICHARINFO);
    create_resource_handler<mashable_resource_handler<summon_progression>>(e_resource_type::JSON_SUMMONPROGRESSION);
    create_resource_handler<mashable_resource_handler<environment_progression>>(e_resource_type::JSON_ENVIRONMENTPROGRESSION);
    create_resource_handler<mashable_resource_handler<render_lod_mgr>>(e_resource_type::JSON_RENDERLOD);
    create_resource_handler<mashable_resource_handler<VisemeStream>>(e_resource_type::viseme_stream);
    create_resource_handler<mashable_resource_handler<static_colgeom_root>>(e_resource_type::static_colgeom);
    create_resource_handler<mashable_resource_handler<lego_map_root_node>>(e_resource_type::lego_map);
    create_resource_handler<mashable_resource_handler<ai_pf_region_graph>>(e_resource_type::scn_quad_path);
    create_resource_handler<mashable_resource_handler<path_graph>>(e_resource_type::path);
    create_resource_handler<mashable_resource_handler<environment_box_set>>(e_resource_type::scn_env_box);
    create_resource_handler<mashable_resource_handler<cosmetic_environments_set>>(e_resource_type::cosmetic_environ);
    create_resource_handler<mashable_resource_handler<sm3_light>>(e_resource_type::sm3_light);
    create_resource_handler<stall_resource_handler>(e_resource_type::stall);
    create_resource_handler<empty_resource_handler>(e_resource_type::slim);
    create_resource_handler<slim_list_handler>(e_resource_type::slim_list);
    create_resource_handler<mashable_resource_handler<als::animation_logic_system_shared>>(e_resource_type::als_file);
    create_resource_handler<mashable_resource_handler<state_graph>>(e_resource_type::ai_state_graph);
    create_resource_handler<mashable_resource_handler<core_ai_resource>>(e_resource_type::base_ai);
    create_resource_handler<mashable_resource_handler<region_audio_box_set>>(e_resource_type::scn_audio_box);
    create_resource_handler<box_trigger_resource_handler>(e_resource_type::box_trigger_info);
    create_resource_handler<scene_entity_container_resource_handler>(e_resource_type::scn_entity);
    create_resource_handler<anchor_collection_resource_handler>(e_resource_type::anchor_coll);
    create_resource_handler<mashable_resource_handler<fx_cache>>(e_resource_type::fx_cache);
    create_resource_handler<empty_resource_handler>(e_resource_type::entity);
    create_resource_handler<mashable_resource_handler<cut_scene>>(e_resource_type::cut_scene);
    create_resource_handler<empty_resource_handler>(e_resource_type::collision_mesh);
    create_resource_handler<empty_resource_handler>(e_resource_type::packfile);
    create_resource_handler<empty_resource_handler>(e_resource_type::speedtree_spt);
    create_resource_handler<empty_resource_handler>(e_resource_type::speedtree_seed);
    create_resource_handler<empty_resource_handler>(e_resource_type::speedtree_wind);
    create_resource_handler<mashable_resource_handler<sound_toc>>(e_resource_type::sound_toc);
    create_resource_handler<mashable_resource_handler<sound_index>>(e_resource_type::sound_index);
    create_resource_handler<ise_library_handler>(e_resource_type::ise_library);
    create_resource_handler<vertex_shader_handler>(e_resource_type::vertex_shader);
    create_resource_handler<pixel_shader_handler>(e_resource_type::pixel_shader);

    entity_instances = new worldly_entity_instances;

    clear_slot();
}

// sub_72E360
void amalga::worldly_pack_slot::clear_slot() {
    resource_pack_slot::clear_slot();

    vtable->clear_pack(this);
}
