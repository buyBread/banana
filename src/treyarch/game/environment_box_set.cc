#include "treyarch/game/environment_box_set.hh"

using namespace treyarch;

// sub_7DC5F0
void environment_box_set::construct_mashed_class() {
    self = nullptr;

    root             .construct_mashed_class();
    environment_boxes.construct_mashed_class();
}

// sub_7DB260
void environment_box_set::destruct_mashed_class() {
    root             .destruct_mashed_class();
    environment_boxes.destruct_mashed_class();
}

// sub_7DD0C0
void environment_box_set::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    root             .unmash(mash_info, this, buffer);
    environment_boxes.unmash(mash_info, this, buffer);
}
