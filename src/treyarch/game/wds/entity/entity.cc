#include "retail.hh"
#include "treyarch/game/wds/entity/entity.hh"

using namespace treyarch;

// sub_612D30
region* entity::get_primary_region() {
    entity* owner = this;

    // flag 0x8000 entities borrow the regions of the first parent without it
    if (owner->unk_014 & 0x8000) {
        do {
            owner = (entity*)retail::sub_601910((u32*)owner);

            if (!owner)
                return nullptr;
        } while (owner->unk_014 & 0x8000);
    }

    if (owner->regions.size())
        return owner->regions.data_[0];

    return nullptr;
}
