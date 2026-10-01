#include <bit>

#include "treyarch/game/wds/entity/interface_storage.hh"

using namespace treyarch;

// sub_402CC0
generic_interface* interface_storage::get_ifc(e_entity_ifc index) {
    ifc_lock.acquire();

    generic_interface* ifc = interfaces.data[std::popcount(ifc_mask & ((1u << index) - 1))];

    ifc_lock.release();

    return ifc;
}
