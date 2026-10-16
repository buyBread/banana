#include "treyarch/game/cloud_tweak.hh"
#include "treyarch/shared/mash/mash_info.hh"

using namespace treyarch;

// sub_7DEDD0
void cloud_tweak::construct_mashed_class() {
    self = nullptr;

    layer.construct_mashed_class();
}

// sub_7DE8F0
void cloud_tweak::destruct_mashed_class() {
    layer.destruct_mashed_class();
}

// sub_7DF3E0
void cloud_tweak::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    layer.unmash(mash_info, this, buffer);
}
