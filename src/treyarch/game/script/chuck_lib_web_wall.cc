#include <cstring>

#include "retail.hh"
#include "treyarch/game/arch_base.hh"
#include "treyarch/game/script/chuck_lib_web_wall.hh"
#include "treyarch/shared/hash/string_hash.hh"

using namespace treyarch;

// sub_842570
u32 slc_web_wall_t::find_instance(const mash::string &name) const {
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

bool slf__create_web_wall__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8425F0)(this, stack);
}

bool slf__web_wall__clear__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8789E0)(this, stack);
}

bool slf__web_wall__fire__entity__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_878A00)(this, stack);
}

bool slf__web_wall__fire__vector3d__vector3d__vector3d__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_878AA0)(this, stack);
}

bool slf__web_wall__set_material__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_878B90)(this, stack);
}

bool slf__web_wall__is_active__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_878CB0)(this, stack);
}

bool slf__web_wall__is_building__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_878D30)(this, stack);
}

bool slf__web_wall__get_build_point__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_878DB0)(this, stack);
}
