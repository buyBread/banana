#include <cstring>

#include "retail.hh"
#include "treyarch/game/arch_base.hh"
#include "treyarch/game/game.hh"
#include "treyarch/game/script/chuck_lib_controller.hh"
#include "treyarch/shared/hash/string_hash.hh"

using namespace treyarch;

// sub_83F4A0
u32 slc_script_controller_t::find_instance(const mash::string &name) const {
    if (!std::strncmp(name.c_str(), "NULL", 0xFFFF) || std::strncmp(name.c_str(), "CONTROLLER_1", 0xFFFF))
        return 0;

    return references::script_controller.read()->my_handle.value;
}
