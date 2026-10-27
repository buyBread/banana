#include "game/sound_index.hh"
#include "treyarch/mash/mash_info.hh"

using namespace treyarch;

// sub_902980
void sound_index::construct_mashed_class() {
    self = nullptr;

    entries.construct_mashed_class();

    references::sound_index.write(this);
}

// sub_901A60
void sound_index::destruct_mashed_class() {
    references::sound_index.write(nullptr);

    entries.destruct_mashed_class();
}

// sub_902DD0
void sound_index::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    entries.unmash(mash_info, this, buffer);
}
