#include "retail.hh"
#include "treyarch/amalga/native_resource_handlers.hh"

using namespace treyarch;

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

template<> i32 amalga::mashable_resource_handler<viseme_stream>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
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
