#include "treyarch/game/static_spawn_item_collection.hh"
#include "treyarch/shared/mash/mash_info.hh"

using namespace treyarch;

// sub_933C70
void static_spawn_item_collection::construct_mashed_class() {
    self = nullptr;

    spawn_items.construct_mashed_class();
}

// sub_934C80
void static_spawn_item_collection::destruct_mashed_class() {
    spawn_items.destruct_mashed_class();
}

// sub_93DA00
void static_spawn_item_collection::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    spawn_items.unmash(mash_info, this, buffer);
}
