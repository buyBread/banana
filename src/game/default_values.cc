#include "game/default_values.hh"
#include "treyarch/mash/mash_info.hh"

using namespace treyarch;

// sub_4CD1F0
void default_values::construct_mashed_class() {
    self = nullptr;

    my_param_block.construct_mashed_class();
}

// sub_4CBD10
void default_values::destruct_mashed_class() {
    my_param_block.destruct_mashed_class();
}

// sub_4CD940
void default_values::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    my_param_block.unmash(mash_info, this, buffer);
}
