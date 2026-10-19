#include "retail.hh"
#include "treyarch/amalga/native_resource_handlers.hh"

using namespace treyarch;

template<> i32 amalga::mashable_resource_handler<als::animation_logic_system_shared>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_7611C0)(this, behavior, descriptor, time_limit);
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

i32 amalga::ise_library_handler::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_75E6D0)(this, behavior, descriptor, time_limit);
}

i32 amalga::vertex_shader_handler::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_767E10)(this, behavior, descriptor, time_limit);
}

i32 amalga::pixel_shader_handler::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_767EA0)(this, behavior, descriptor, time_limit);
}
