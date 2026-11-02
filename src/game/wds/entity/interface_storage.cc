#include <bit>

#include "game/wds/entity/interface/generic_interface.hh"
#include "game/wds/entity/entity.hh"
#include "game/wds/entity/interface_storage.hh"

using namespace treyarch;

// sub_402CC0
generic_interface* interface_storage::get_ifc(e_entity_ifc index) {
    ifc_lock.acquire();

    generic_interface* ifc = interfaces.array[std::popcount(ifc_mask & ((1u << index) - 1))];

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

// sub_620DE0
void interface_storage::set_owner(entity_base* owner) { // interfaces the mash image didn't build are skipped
    engine_lock_scope lock(&ifc_lock);

    for (generic_interface* ifc : interfaces) {
        if (ifc->dynamic)
            continue;

        switch (ifc->get_base_type()) {
            case entity_ifc_entity_base:
            case entity_ifc_entity:
            case entity_ifc_actor:
            case entity_ifc_conglomerate:
                ((entity_interface*)ifc)->set_owner((entity*)owner); // slot 18
                break;

            default:
                break;
        }
    }
}
