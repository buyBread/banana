#include "game/lego_map.hh"

using namespace treyarch;

// sub_7AAA90
void lego_map_root_node::construct_mashed_class() {
    self = nullptr;

    tilemaps       .construct_mashed_class();
    lego_mesh_names.construct_mashed_class();
    unk_40         .construct_mashed_class();
}

// sub_7A6B20
void lego_map_root_node::destruct_mashed_class() {
    tilemaps       .destruct_mashed_class();
    lego_mesh_names.destruct_mashed_class();
    unk_40         .destruct_mashed_class();
}

// sub_7A71D0
void lego_map_root_node::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    tilemaps       .unmash(mash_info, this, buffer);
    lego_mesh_names.unmash(mash_info, this, buffer);
    unk_40         .unmash(mash_info, this, buffer);

    custom_unmash(mash_info, this, buffer);
}

// sub_785B70
void lego_map_root_node::custom_unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    legos = (lego*)mash_info->read_from_buffer(buffer, (i32)(0x20 * num_regular_legos + 0x90 * num_building_legos), 8);
    grid  = (proximity_map*)mash_info->read_from_buffer(buffer, (i32)grid_size, 8);

    mash_info->align_buffer(buffer, 8);
}
