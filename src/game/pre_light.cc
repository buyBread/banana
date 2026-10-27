#include "game/pre_light.hh"
#include "treyarch/mash/mash_info.hh"

using namespace treyarch;

// sub_7D9C10
void pre_light::construct_mashed_class() {
    self = nullptr;

    horizon_map.construct_mashed_class();
    entries    .construct_mashed_class();
}

// sub_7CB510
void pre_light::destruct_mashed_class() {
    horizon_map.destruct_mashed_class();
    entries    .destruct_mashed_class();
}

// sub_7D9C50
void pre_light::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    horizon_map.unmash(mash_info, this, buffer);
    entries    .unmash(mash_info, this, buffer);
}
