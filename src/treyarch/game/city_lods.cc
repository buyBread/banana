#include "treyarch/game/city_lods.hh"
#include "treyarch/shared/mash/mash_info.hh"

using namespace treyarch;

// sub_7DEDF0
void city_lods::construct_mashed_class() {
    self = nullptr;

    districts.construct_mashed_class();
}

// sub_7DD1B0
void city_lods::destruct_mashed_class() {
    districts.destruct_mashed_class();
}

// sub_7DE910
void city_lods::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    districts.unmash(mash_info, this, buffer);
}
