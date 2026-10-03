#include "retail.hh"
#include "treyarch/amalga/resource_partition.hh"

using namespace treyarch;

// sub_739D30
amalga::resource_pack_slot* amalga::resource_partition::get_pack_slot(string_hash pack_name) {
    for (u32 index = 0; index < pack_slots.size(); ++index) {
        resource_pack_slot* slot = pack_slots[index];

        if (slot->slot_state != slot_state_ready)
            continue;

        u32 slot_name_hash;

        if (*retail::sub_5FD120((u32*)&slot->pack_name, &slot_name_hash) == pack_name.source_hash_code)
            return slot;
    }

    return nullptr;
}
