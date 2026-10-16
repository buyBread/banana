#include "treyarch/game/sm_decal_material.hh"
#include "treyarch/shared/mash/mash_info.hh"

using namespace treyarch;

// sub_933C70
void sm_decal_material::construct_mashed_class() {
    self = nullptr;

    tweaks.construct_mashed_class();
}

// sub_934C80
void sm_decal_material::destruct_mashed_class() {
    tweaks.destruct_mashed_class();
}

// sub_7DC680
void sm_decal_material::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    tweaks.unmash(mash_info, this, buffer);
}
