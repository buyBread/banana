#include "game/viseme_stream.hh"
#include "treyarch/mash/mash_info.hh"

using namespace treyarch;

// sub_5A85C0
void viseme_stream::construct_mashed_class() {
    self = nullptr;

    weights.construct_mashed_class();
}

// sub_5A82B0
void viseme_stream::destruct_mashed_class() {
    weights.destruct_mashed_class();
}

// sub_5A8BF0
void viseme_stream::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    weights.unmash(mash_info, this, buffer);
}
