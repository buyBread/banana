#include <cstring>

#include "retail.hh"
#include "treyarch/game/arch_base.hh"
#include "treyarch/game/script/chuck_lib_trigger.hh"
#include "treyarch/game/trigger_manager.hh"
#include "treyarch/shared/hash/string_hash.hh"

using namespace treyarch;

// sub_83E040
u32 slc_trigger_t::find_instance(const mash::string &name) const {
    string_hash hash;

    hash.initialize(mash::ALLOCATED, name.c_str());

    auto* trigger = (arch_base*)retail::sub_7456D0((u32*)trigger_manager::inst(), &hash.source_hash_code); // trigger_manager::find_instance

    return trigger->my_handle.value;
}
