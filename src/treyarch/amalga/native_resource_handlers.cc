#include "retail.hh"
#include "treyarch/amalga/native_resource_handlers.hh"

using namespace treyarch;

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
