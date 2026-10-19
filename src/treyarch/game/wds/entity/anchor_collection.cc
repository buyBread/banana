#include "treyarch/game/wds/entity/anchor_collection.hh"

using namespace treyarch;

// sub_7D7FC0
void anchor_collection::construct_mashed_class() {
    self = nullptr;

    anchor_positions.construct_mashed_class();

    entity_instances = nullptr;
}

// sub_971540
void anchor_collection::destruct_mashed_class() {
    anchor_positions.destruct_mashed_class();
}

// sub_902DD0
void anchor_collection::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    anchor_positions.unmash(mash_info, this, buffer);
}
