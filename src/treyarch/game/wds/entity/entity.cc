#include "retail.hh"
#include "treyarch/game/wds/entity/entity.hh"

using namespace treyarch;

// sub_612D30
region* entity::get_primary_region() {
    entity* owner = this;

    // flag 0x8000 entities borrow the regions of the first parent without it
    if (owner->flags & 0x8000) {
        do {
            owner = (entity*)retail::sub_601910((u32*)owner);

            if (!owner)
                return nullptr;
        } while (owner->flags & 0x8000);
    }

    if (owner->regions.size())
        return owner->regions.data_[0];

    return nullptr;
}

// sub_6267E0
generic_interface* entity::find_ifc_by_name(string_hash name) {
    if (!my_ifc_storage)
        return nullptr;

    return my_ifc_storage->find_ifc_by_name(name);
}
