#include "treyarch/game/ui_character_info_list.hh"
#include "treyarch/shared/mash/mash_info.hh"

using namespace treyarch;

// sub_6EFF10
void ui_character_info_list::construct_mashed_class() {
    self = nullptr;

    characters.construct_mashed_class();
}

// sub_6E15C0
void ui_character_info_list::destruct_mashed_class() {
    characters.destruct_mashed_class();
}

// sub_6F7670
void ui_character_info_list::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    characters.unmash(mash_info, this, buffer);
}
