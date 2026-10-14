#include "retail.hh"
#include "treyarch/amalga/native_resource_handlers.hh"

using namespace treyarch;

template<> i32 amalga::mashable_resource_handler<localized_string_table>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_75F7F0)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<district_graph_container>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_76E230)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<speedtree_collection>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_75F860)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<speedtree_collection_reference>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_766FB0)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<gb_map>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_75F9B0)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<region_traffic_light_info>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_75FA20)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<vehicle_model_depot>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_75FA90)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<Tilemap>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_75FB00)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<Environment>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_75FB70)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<LightBehavior>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_75FBE0)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<GraphicsOptions>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_75FC50)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<EffectContainer>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_75FCC0)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<SMDecalMaterial>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_75FD30)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<PhatPalette>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_75FDA0)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<CowbellFile>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_75FE10)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<Meshmap>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_75FE80)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<CloudTweak>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_75FEF0)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<CityLODs>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_75FF60)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<SMRoadParam>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_75FFD0)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<AllBuildings>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_760040)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<IGO3DWidgetParameters>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_7600B0)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<IGO3DTextParameters>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_760120)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<SMDecalParam>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_760190)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<AdjustmentLayersLibrary>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_760200)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<Outdoors>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_760270)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<daySkyColor>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_7602E0)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<nightSkyColor>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_760350)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<PreLight>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_7603C0)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<SMBuildingLODParam>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_760430)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<SMRoadLODParam>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_7604A0)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<SMRetrofitParam>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_760510)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<region_spawn_interface>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_760580)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<meter_data_file>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_7605F0)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<meter_behavior_file>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_760660)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<combo_move_type_file>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_7606D0)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<quest_descriptions>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_760740)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<quest_list>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_7607B0)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<conversation>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_760820)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<spawn_table_interface>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_760890)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<spawn_item_collection>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_760900)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<spawn_point_list_collection>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_760970)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<neighborhood_interface>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_7609E0)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<default_values>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_760A50)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<progression_messages>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_760AC0)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<static_spawn_item_collection>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_760B30)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<rpg_upgrades>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_760BA0)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<sound_map>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_760C10)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<ui_character_info_list>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_760C80)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<summon_progression>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_760CF0)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<environment_progression>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_760D60)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<render_lod_mgr>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_760DD0)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<VisemeStream>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_760E40)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<static_colgeom_root>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_760EB0)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<lego_map_root_node>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_760F20)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<ai_pf_region_graph>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_760F90)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<path_graph>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_761000)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<environment_box_set>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_761070)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<cosmetic_environments_set>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_7610E0)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<sm3_light>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_761150)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<als::animation_logic_system_shared>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_7611C0)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<state_graph>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_761230)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<core_ai_resource>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_7612A0)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<region_audio_box_set>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_761310)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<fx_cache>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_761380)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<cut_scene>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_7613F0)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<sound_toc>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_761460)(this, behavior, descriptor, time_limit);
}

template<> i32 amalga::mashable_resource_handler<sound_index>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_7614D0)(this, behavior, descriptor, time_limit);
}

i32 amalga::singular_apk_resource_handler::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_72E270)(this, behavior, descriptor, time_limit);
}

i32 amalga::slim_list_handler::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_76DA70)(this, behavior, descriptor, time_limit);
}

i32 amalga::box_trigger_resource_handler::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_764F30)(this, behavior, descriptor, time_limit);
}

i32 amalga::scene_entity_container_resource_handler::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_76E770)(this, behavior, descriptor, time_limit);
}

i32 amalga::anchor_collection_resource_handler::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_75E4F0)(this, behavior, descriptor, time_limit);
}

i32 amalga::ise_library_handler::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_75E6D0)(this, behavior, descriptor, time_limit);
}

i32 amalga::vertex_shader_handler::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_767E10)(this, behavior, descriptor, time_limit);
}

i32 amalga::pixel_shader_handler::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_767EA0)(this, behavior, descriptor, time_limit);
}
