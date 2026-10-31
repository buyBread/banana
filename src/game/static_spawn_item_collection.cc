#include <cstring>

#include "game/static_spawn_item_collection.hh"
#include "treyarch/mash/mash_info.hh"

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

// sub_927CD0
spawn_data_base* static_spawn_item::find_data(const mash::string &name) {
    for (spawn_data_base** it = data.begin(); it != data.end(); ++it)
        if (!std::strncmp((*it)->name.c_str(), name.c_str(), mash::string::max_length))
            return *it;

    return nullptr;
}
