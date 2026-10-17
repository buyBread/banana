#include "treyarch/game/sm_retrofit_param.hh"
#include "treyarch/shared/mash/mash_info.hh"

using namespace treyarch;

// sub_7DB520
void sm_retrofit_param::construct_mashed_class() {
    self = nullptr;

    adjustment_layers_library.construct_mashed_class();
}

// sub_7CB4A0
void sm_retrofit_param::destruct_mashed_class() {
    adjustment_layers_library.destruct_mashed_class();
}

// sub_7D9B80
void sm_retrofit_param::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    adjustment_layers_library.unmash(mash_info, this, buffer);
}
