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
