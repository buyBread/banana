#include "game/cosmetic_environments_set.hh"
#include "treyarch/mm/heap.hh"

using namespace treyarch;

// sub_7A5360
void cosmetic_environments_set::construct_mashed_class() {
    self = nullptr;

    environment_names.construct_mashed_class();
}

// sub_7DC610
void cosmetic_environments_set::destruct_mashed_class() {
    if (environments) {
        environments->~vector();

        memory::heap::free(environments);
    }

    environment_names.destruct_mashed_class();
}

// sub_7D93F0
void cosmetic_environments_set::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    environment_names.unmash(mash_info, this, buffer);
}
