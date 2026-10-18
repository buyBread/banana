#include "treyarch/game/spawn_point_list_collection.hh"
#include "treyarch/shared/mash/mash_info.hh"

using namespace treyarch;

// sub_956EA0
void spawn_point_list_collection::construct_mashed_class() {
    self = nullptr;

    lists.construct_mashed_class();
}

// sub_956350
void spawn_point_list_collection::destruct_mashed_class() {
    lists.destruct_mashed_class();
}

// sub_956720
void spawn_point_list_collection::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    lists.unmash(mash_info, this, buffer);
}
