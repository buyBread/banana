#include "game/render_lod_mgr.hh"
#include "treyarch/mash/mash_info.hh"

using namespace treyarch;

// sub_7A5360
void render_lod_mgr::construct_mashed_class() {
    self = nullptr;

    entity_names.construct_mashed_class();
}

// sub_971540
void render_lod_mgr::destruct_mashed_class() {
    entity_names.destruct_mashed_class();
}

// sub_978240
void render_lod_mgr::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    entity_names.unmash(mash_info, this, buffer);
}
