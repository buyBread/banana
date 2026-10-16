#include "treyarch/game/all_buildings.hh"
#include "treyarch/shared/mash/mash_info.hh"

using namespace treyarch;

// sub_7DB540
void all_buildings::construct_mashed_class() {
    self = nullptr;

    unk_04.construct_mashed_class();
    unk_18.construct_mashed_class();
}

// sub_7D9BC0
void all_buildings::destruct_mashed_class() {
    unk_04.destruct_mashed_class();
    unk_18.destruct_mashed_class();
}

// sub_7DD1D0
void all_buildings::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    unk_04.unmash(mash_info, this, buffer);
    unk_18.unmash(mash_info, this, buffer);
}
