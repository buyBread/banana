#include "game/sound_map.hh"
#include "treyarch/mash/mash_info.hh"

using namespace treyarch;

// sub_905670
void sound_map::construct_mashed_class() {
    self = nullptr;

    entries.construct_mashed_class();
}

// sub_905430
void sound_map::destruct_mashed_class() {
    entries.destruct_mashed_class();
}

// sub_9056F0
void sound_map::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    entries.unmash(mash_info, this, buffer);
}
