#include <bit>

#include "game/wds/entity/interface/generic_interface.hh"
#include "game/wds/entity/interface_storage.hh"

using namespace treyarch;

// sub_402CC0
generic_interface* interface_storage::get_ifc(e_entity_ifc index) {
    ifc_lock.acquire();

    generic_interface* ifc = interfaces.data[std::popcount(ifc_mask & ((1u << index) - 1))];

    ifc_lock.release();

    return ifc;
}

// sub_621640
generic_interface* interface_storage::find_ifc_by_name(string_hash name) {
    ifc_lock.acquire();

    generic_interface* found = nullptr;

    for (generic_interface* ifc : interfaces) {
        if (ifc->get_ifc_name() == name) {
            found = ifc;
            break;
        }
    }

    ifc_lock.release();

    return found;
}
