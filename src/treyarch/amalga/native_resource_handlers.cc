#include "retail.hh"
#include "treyarch/amalga/native_resource_handlers.hh"

using namespace treyarch;

template<> i32 amalga::mashable_resource_handler<als::animation_logic_system_shared>::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_7611C0)(this, behavior, descriptor, time_limit);
}

i32 amalga::vertex_shader_handler::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_767E10)(this, behavior, descriptor, time_limit);
}

i32 amalga::pixel_shader_handler::handle_resource(e_behavior behavior, resource_descriptor* descriptor, limited_timer* time_limit) {
    return ((handle_resource_t)retail::sub_767EA0)(this, behavior, descriptor, time_limit);
}
