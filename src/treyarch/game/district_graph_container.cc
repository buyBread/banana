#include "treyarch/game/district_graph_container.hh"
#include "treyarch/shared/mash/mash_info.hh"

using namespace treyarch;

// inlined @ sub_764A50
void dsg_region_container::construct_mashed_class() {
    name  .construct_mashed_class();
    unk_50.construct_mashed_class();
}

// inlined @ sub_75CCD0
void dsg_region_container::destruct_mashed_class() {
    name  .destruct_mashed_class();
    unk_50.destruct_mashed_class();
}

// inlined @ sub_7627E0
void dsg_region_container::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    name  .unmash(mash_info, this, buffer);
    unk_50.unmash(mash_info, this, buffer);
}

// inlined @ sub_76D640
void district_graph_container::construct_mashed_class() {
    self = nullptr;

    regions.construct_mashed_class();
    boxes  .construct_mashed_class();
}

// inlined @ sub_7675E0
void district_graph_container::destruct_mashed_class() {
    regions.destruct_mashed_class();
    boxes  .destruct_mashed_class();
}

// inlined @ sub_769550
void district_graph_container::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    regions.unmash(mash_info, this, buffer);
    boxes  .unmash(mash_info, this, buffer);
}
