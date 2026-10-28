#include "retail.hh"
#include "game/wds/entity/entity.hh"

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

script_data_interface* entity::create_script_data_ifc() {
    return (script_data_interface*)retail::sub_64A8B0((i32*)this);
}

physical_interface* entity::create_physical_ifc() {
    return (physical_interface*)retail::sub_660CB0((i32*)this);
}

damage_interface* entity::create_damage_ifc() {
    return (damage_interface*)retail::sub_660B80((i32*)this);
}

time_interface* entity::create_time_ifc() {
    return (time_interface*)retail::sub_65ABE0((i32*)this);
}

// sub_602010
void entity::lock_found_entities() {
    references::found_entities_lock.get().acquire();

    references::found_entities_locked.write(true);
}

// sub_602030
void entity::unlock_found_entities() {
    references::found_entities_locked.write(false);

    references::found_entities_lock.get().release();
}
