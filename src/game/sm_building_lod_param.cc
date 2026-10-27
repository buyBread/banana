#include "game/sm_building_lod_param.hh"
#include "treyarch/mash/mash_info.hh"

using namespace treyarch;

// sub_7DB480
void sm_building_lod_param::construct_mashed_class() {
    self = nullptr;

    diffuse_edge             .construct_mashed_class();
    shininess_edge           .construct_mashed_class();
    diffuse_field            .construct_mashed_class();
    shininess_field          .construct_mashed_class();
    environment_map          .construct_mashed_class();
    adjustment_layers_library.construct_mashed_class();
}

// sub_7CB3D0
void sm_building_lod_param::destruct_mashed_class() {
    diffuse_edge             .destruct_mashed_class();
    shininess_edge           .destruct_mashed_class();
    diffuse_field            .destruct_mashed_class();
    shininess_field          .destruct_mashed_class();
    environment_map          .destruct_mashed_class();
    adjustment_layers_library.destruct_mashed_class();
}

// sub_7D9AD0
void sm_building_lod_param::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    diffuse_edge             .unmash(mash_info, this, buffer);
    shininess_edge           .unmash(mash_info, this, buffer);
    diffuse_field            .unmash(mash_info, this, buffer);
    shininess_field          .unmash(mash_info, this, buffer);
    environment_map          .unmash(mash_info, this, buffer);
    adjustment_layers_library.unmash(mash_info, this, buffer);
}
