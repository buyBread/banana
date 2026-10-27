#include <cstring>

#include "retail.hh"
#include "game/arch_base.hh"
#include "game/script/chuck_lib_water_box.hh"
#include "treyarch/mash/string_hash.hh"

using namespace treyarch;

// sub_83DAE0
u32 slc_water_box_t::find_instance(const mash::string &name) const {
    if (!std::strncmp(name.c_str(), "NULL", 0xFFFF))
        return 0;

    string_hash hash;

    hash.initialize(mash::ALLOCATED, name.c_str());

    auto* entity = (arch_base*)retail::sub_640AE0(&hash.source_hash_code, 28, true); // entity_handle_manager::find_entity

    if (!entity)
        return 0;

    return entity->my_handle.value;
}

bool slf__find_water_box__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_878070)(this, stack);
}

bool slf__water_box__set_rel_position__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877F80)(this, stack);
}

bool slf__water_box__get_water_height__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877FB0)(this, stack);
}

bool slf__water_box__get_water_type__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_878010)(this, stack);
}

bool slf__water_box__contains__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8780E0)(this, stack);
}

bool slf__water_box__surface_contains__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_878170)(this, stack);
}
