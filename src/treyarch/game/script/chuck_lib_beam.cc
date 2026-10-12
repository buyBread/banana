#include <cstring>

#include "retail.hh"
#include "treyarch/game/arch_base.hh"
#include "treyarch/game/script/chuck_lib_beam.hh"
#include "treyarch/shared/hash/string_hash.hh"

using namespace treyarch;

// sub_83E080
u32 slc_beam_t::find_instance(const mash::string &name) const {
    if (!std::strncmp(name.c_str(), "NULL", 0xFFFF))
        return 0;

    string_hash hash;

    hash.initialize(mash::ALLOCATED, name.c_str());

    auto* entity = (arch_base*)retail::sub_640AE0(&hash.source_hash_code, 28, true); // entity_handle_manager::find_entity

    return entity->my_handle.value;
}

bool slf__beam__hit_world__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8569D0)(this, stack);
}

bool slf__beam__hit_hero__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_856A50)(this, stack);
}

bool slf__beam__get_impact_point__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_856AD0)(this, stack);
}

bool slf__beam__get_impact_normal__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_856B40)(this, stack);
}

bool slf__beam__set_thickness__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_856BB0)(this, stack);
}

bool slf__beam__set_max_length__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_856BE0)(this, stack);
}

bool slf__beam__set_color__num__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_856C10)(this, stack);
}

bool slf__beam__set_collide_hero__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_856C60)(this, stack);
}

bool slf__beam__set_clip_to_hero__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_856CB0)(this, stack);
}

bool slf__beam__set_collide_world__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_856D00)(this, stack);
}

bool slf__beam__set_clip_to_world__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_856D50)(this, stack);
}

bool slf__beam__set_world_entity_collision__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_856DA0)(this, stack);
}

bool slf__beam__set_collide_beamable__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_856DF0)(this, stack);
}

bool slf__beam__set_no_collision__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_856E40)(this, stack);
}

bool slf__beam__set_additive__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_856E70)(this, stack);
}

bool slf__beam__set_point_to_point__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_856EC0)(this, stack);
}

bool slf__beam__set_uv_anim__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_856EF0)(this, stack);
}

bool slf__beam__set_tiles_per_meter__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_856F80)(this, stack);
}

bool slf__beam__set_material__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_856FB0)(this, stack);
}
