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
