#include "game/sm3_light.hh"
#include "treyarch/mash/mash_info.hh"

using namespace treyarch;

// sub_7A3040
void sm3_light::construct_mashed_class() {
    self = nullptr;

    name    .construct_mashed_class();
    behavior.construct_mashed_class();
    gobo    .construct_mashed_class();
}

// sub_7987A0
void sm3_light::destruct_mashed_class() {
    name    .destruct_mashed_class();
    behavior.destruct_mashed_class();
    gobo    .destruct_mashed_class();
}

// sub_7A5200
void sm3_light::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    name    .unmash(mash_info, this, buffer);
    behavior.unmash(mash_info, this, buffer);
    gobo    .unmash(mash_info, this, buffer);
}
