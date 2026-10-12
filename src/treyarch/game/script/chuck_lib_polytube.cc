#include <cstring>

#include "retail.hh"
#include "treyarch/game/arch_base.hh"
#include "treyarch/game/script/chuck_lib_polytube.hh"
#include "treyarch/shared/hash/string_hash.hh"

using namespace treyarch;

// sub_842570
u32 slc_polytube_t::find_instance(const mash::string &name) const {
    if (!std::strncmp(name.c_str(), "NULL", 0xFFFF))
        return 0;

    string_hash hash;

    hash.initialize(mash::ALLOCATED, name.c_str());

    auto* entity = (arch_base*)retail::sub_640AE0(&hash.source_hash_code, 28, true); // entity_handle_manager::find_entity

    // slot 77
    if (entity)
        ((void (__thiscall*)(arch_base*))entity->vtable[77])(entity);

    return entity->my_handle.value;
}

bool slf__polytube__reverse_tentacle__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_863760)(this, stack);
}

bool slf__polytube__reserve_control_pts__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87E4F0)(this, stack);
}

bool slf__polytube__extend_to_have_widths_and_normals__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_863780)(this, stack);
}

bool slf__polytube__initialize_ends_closed__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8637A0)(this, stack);
}

bool slf__polytube__add_control_pt__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8637C0)(this, stack);
}

bool slf__polytube__set_control_pt__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8639F0)(this, stack);
}

bool slf__polytube__get_control_pt__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_863C30)(this, stack);
}

bool slf__polytube__set_additive__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_863D90)(this, stack);
}

bool slf__polytube__get_num_control_pts__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_863DE0)(this, stack);
}

bool slf__polytube__build__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_863E70)(this, stack);
}

bool slf__polytube__set_material__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_863EE0)(this, stack);
}

bool slf__polytube__set_material_dynamic__str__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_864000)(this, stack);
}

bool slf__polytube__set_tube_radius__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8640D0)(this, stack);
}

bool slf__polytube__set_num_sides__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_864120)(this, stack);
}

bool slf__polytube__set_tiles_per_meter__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_864160)(this, stack);
}

bool slf__polytube__set_max_length__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_864190)(this, stack);
}

bool slf__polytube__v_animate__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8641C0)(this, stack);
}

bool slf__polytube__simulate_slack__vector3d__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_864200)(this, stack);
}

bool slf__polytube__create_tentacle__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_864240)(this, stack);
}

bool slf__polytube__destroy_tentacle__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_864260)(this, stack);
}

bool slf__polytube__tentacle_set_collide__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_864280)(this, stack);
}

bool slf__polytube__tentacle_set_end_position__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8642D0)(this, stack);
}

bool slf__polytube__tentacle_get_end_position__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_864300)(this, stack);
}

bool slf__polytube__tentacle_get_num_positions__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_864380)(this, stack);
}

bool slf__polytube__tentacle_set_position__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8643E0)(this, stack);
}

bool slf__polytube__tentacle_get_position__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_864420)(this, stack);
}

bool slf__polytube__tentacle_create_line__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8644B0)(this, stack);
}

bool slf__polytube__set_code_blend__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_864510)(this, stack);
}

bool slf__polytube__tentacle_kill_engine__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_864550)(this, stack);
}

bool slf__polytube__tentacle_engine_running__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8645B0)(this, stack);
}

bool slf__polytube__tentacle_add_strike_engine__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_864640)(this, stack);
}

bool slf__polytube__tentacle_add_swirl_strike_engine__vector3d__num__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_864730)(this, stack);
}

bool slf__polytube__tentacle_add_dangle_engine__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_864830)(this, stack);
}

bool slf__polytube__tentacle_add_follow_engine__num__num__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_864940)(this, stack);
}

bool slf__polytube__tentacle_add_slack_engine__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_864A50)(this, stack);
}

bool slf__polytube__tentacle_set_engine_attach_ent__num__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_864B80)(this, stack);
}

bool slf__polytube__tentacle_set_engine_attach_ent__num__entity__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_864C10)(this, stack);
}

bool slf__polytube__tentacle_set_engine_target__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_864CB0)(this, stack);
}

bool slf__polytube__tentacle_get_engine_length__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_864D70)(this, stack);
}

bool slf__polytube__tentacle_set_engine_length__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_864E30)(this, stack);
}

bool slf__polytube__tentacle_get_engine_slack_percent__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_864ED0)(this, stack);
}

bool slf__polytube__tentacle_set_engine_slack_percent__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_864F80)(this, stack);
}

bool slf__polytube__tentacle_get_engine_drag__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_865010)(this, stack);
}

bool slf__polytube__tentacle_set_engine_drag__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8650E0)(this, stack);
}

bool slf__polytube__tentacle_get_engine_elasticity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_865180)(this, stack);
}

bool slf__polytube__tentacle_set_engine_elasticity__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_865220)(this, stack);
}

bool slf__polytube__tentacle_get_engine_gravity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_865290)(this, stack);
}

bool slf__polytube__tentacle_set_engine_gravity__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_865380)(this, stack);
}

bool slf__polytube__get_spline_pt__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_865420)(this, stack);
}

bool slf__polytube__tentacle_web_set_up__entity__uint__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_865620)(this, stack);
}
