#include "game/tilemap.hh"
#include "treyarch/mash/mash_info.hh"

using namespace treyarch;

// sub_7DAC80
void tilemap::construct_mashed_class() {
    self = nullptr;

    texture                  .construct_mashed_class();
    ambient_occlusion        .construct_mashed_class();
    normal                   .construct_mashed_class();
    interior                 .construct_mashed_class();
    meshmap                  .construct_mashed_class();
    diffuse                  .construct_mashed_class();
    shininess                .construct_mashed_class();
    micronoise               .construct_mashed_class();
    grunge                   .construct_mashed_class();
    adjustment_layers_library.construct_mashed_class();
    low_lod_diffuse_map      .construct_mashed_class();
    low_lod_shiny_map        .construct_mashed_class();
}

// sub_7C2F20
void tilemap::destruct_mashed_class() {
    texture                  .destruct_mashed_class();
    ambient_occlusion        .destruct_mashed_class();
    normal                   .destruct_mashed_class();
    interior                 .destruct_mashed_class();
    meshmap                  .destruct_mashed_class();
    diffuse                  .destruct_mashed_class();
    shininess                .destruct_mashed_class();
    micronoise               .destruct_mashed_class();
    grunge                   .destruct_mashed_class();
    adjustment_layers_library.destruct_mashed_class();
    low_lod_diffuse_map      .destruct_mashed_class();
    low_lod_shiny_map        .destruct_mashed_class();
}

// sub_7D92A0
void tilemap::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    texture                  .unmash(mash_info, this, buffer);
    ambient_occlusion        .unmash(mash_info, this, buffer);
    normal                   .unmash(mash_info, this, buffer);
    interior                 .unmash(mash_info, this, buffer);
    meshmap                  .unmash(mash_info, this, buffer);
    diffuse                  .unmash(mash_info, this, buffer);
    shininess                .unmash(mash_info, this, buffer);
    micronoise               .unmash(mash_info, this, buffer);
    grunge                   .unmash(mash_info, this, buffer);
    adjustment_layers_library.unmash(mash_info, this, buffer);
    low_lod_diffuse_map      .unmash(mash_info, this, buffer);
    low_lod_shiny_map        .unmash(mash_info, this, buffer);
}
