#include <cstring>

#include "game/spawn_point_list_collection.hh"
#include "treyarch/mash/mash_info.hh"

using namespace treyarch;

// sub_927C70
spawn_data_base* spawn_point::find_data(const mash::string &name) {
    for (spawn_data_base** it = data.begin(); it != data.end(); ++it)
        if (!std::strncmp((*it)->name.c_str(), name.c_str(), mash::string::max_length))
            return *it;

    return nullptr;
}

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
