#include "retail.hh"
#include "treyarch/game/speedtree/collection.hh"
#include "treyarch/game/speedtree/manager.hh"
#include "treyarch/shared/mash/mash_info.hh"

using namespace treyarch;

// sub_7DC990
void speedtree_collection::construct_mashed_class() {
    self = nullptr;

    unk_04    .construct_mashed_class();
    placements.construct_mashed_class();

    speed_tree_manager &manager = references::speed_tree_manager.get();

    for (i32 index = 0; index < manager.entry_count; ++index) {
        for (speed_tree_wrapper* wrapper : manager.entries[index].wrappers)
            retail::sub_7BE970((u32*)wrapper); // rebuilds its region billboards from these placements
    }
}

// sub_7DB590
void speedtree_collection::destruct_mashed_class() {
    unk_04    .destruct_mashed_class();
    placements.destruct_mashed_class();
}

// sub_7DD260
void speedtree_collection::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    unk_04    .unmash(mash_info, this, buffer);
    placements.unmash(mash_info, this, buffer);
}
