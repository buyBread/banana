#include "treyarch/game/adjustment_layers_library.hh"
#include "treyarch/shared/mash/mash_info.hh"

using namespace treyarch;

// sub_7D10F0
void adjustment_layers_library::construct_mashed_class() {
    self = nullptr;

    layers.construct_mashed_class();
}

// sub_7D1110
void adjustment_layers_library::destruct_mashed_class() {
    layers.destruct_mashed_class();
}

// sub_7DA050
void adjustment_layers_library::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    layers.unmash(mash_info, this, buffer);
}
