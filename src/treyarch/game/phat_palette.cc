#include "treyarch/game/phat_palette.hh"
#include "treyarch/shared/mash/mash_info.hh"

using namespace treyarch;

// sub_7D9730
void phat_palette::construct_mashed_class() {
    self = nullptr;

    palettized_output.construct_mashed_class();
    palette          .construct_mashed_class();
}

// sub_7C9F40
void phat_palette::destruct_mashed_class() {
    palettized_output.destruct_mashed_class();
    palette          .destruct_mashed_class();
}

// sub_7D9780
void phat_palette::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    palettized_output.unmash(mash_info, this, buffer);
    palette          .unmash(mash_info, this, buffer);
}
