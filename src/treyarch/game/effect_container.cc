#include "treyarch/game/effect_container.hh"
#include "treyarch/shared/mash/mash_info.hh"

using namespace treyarch;

// sub_7DBF30
void effect_container::construct_mashed_class() {
    self = nullptr;

    light.construct_mashed_class();
}

// sub_7DD350
void effect_container::destruct_mashed_class() {
    light.destruct_mashed_class();
}

// sub_7DD370
void effect_container::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    light.unmash(mash_info, this, buffer);
}
