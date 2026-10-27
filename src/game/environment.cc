#include "game/environment.hh"
#include "treyarch/mash/mash_info.hh"

using namespace treyarch;

// sub_7C4BE0
void fog::construct_mashed_class() {
    position0     .construct_mashed_class();
    position1     .construct_mashed_class();
    fogs_per_meter.construct_mashed_class();
    maximum_fogs  .construct_mashed_class();
    color         .construct_mashed_class();
    depth_start   .construct_mashed_class();
    depth_end     .construct_mashed_class();
    depth_power   .construct_mashed_class();
    depth_opacity .construct_mashed_class();
    depth_overide .construct_mashed_class();
}

// sub_7C4C40
void fog::destruct_mashed_class() {
    position0     .destruct_mashed_class();
    position1     .destruct_mashed_class();
    fogs_per_meter.destruct_mashed_class();
    maximum_fogs  .destruct_mashed_class();
    color         .destruct_mashed_class();
    depth_start   .destruct_mashed_class();
    depth_end     .destruct_mashed_class();
    depth_power   .destruct_mashed_class();
    depth_opacity .destruct_mashed_class();
    depth_overide .destruct_mashed_class();
}

// sub_7DAF00
void fog::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    position0     .unmash(mash_info, this, buffer);
    position1     .unmash(mash_info, this, buffer);
    fogs_per_meter.unmash(mash_info, this, buffer);
    maximum_fogs  .unmash(mash_info, this, buffer);
    color         .unmash(mash_info, this, buffer);
    depth_start   .unmash(mash_info, this, buffer);
    depth_end     .unmash(mash_info, this, buffer);
    depth_power   .unmash(mash_info, this, buffer);
    depth_opacity .unmash(mash_info, this, buffer);
    depth_overide .unmash(mash_info, this, buffer);
}

// sub_7C4E20
void environment_unk_148::construct_mashed_class() {
    for (mash::vector_basic<f32> &list : unk_010)
        list.construct_mashed_class();

    for (gradient &colors : unk_110)
        colors.construct_mashed_class();

    for (mash::vector_basic<f32> &list : unk_140)
        list.construct_mashed_class();
}

// sub_7C4F10
void environment_unk_148::destruct_mashed_class() {
    for (mash::vector_basic<f32> &list : unk_010)
        list.destruct_mashed_class();

    for (gradient &colors : unk_110)
        colors.destruct_mashed_class();

    for (mash::vector_basic<f32> &list : unk_140)
        list.destruct_mashed_class();
}

// sub_7DB000
void environment_unk_148::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    for (mash::vector_basic<f32> &list : unk_010)
        list.unmash(mash_info, this, buffer);

    for (gradient &colors : unk_110)
        colors.unmash(mash_info, this, buffer);

    for (mash::vector_basic<f32> &list : unk_140)
        list.unmash(mash_info, this, buffer);
}

// sub_7DE310
void environment::construct_mashed_class() {
    self = nullptr;

    ambient                           .construct_mashed_class();
    adjustment                        .construct_mashed_class();
    material_adjustment               .construct_mashed_class();
    character_adjustment              .construct_mashed_class();
    character_ambient_light_adjustment.construct_mashed_class();
    character_diffuse_light_adjustment.construct_mashed_class();
    outdoors                          .construct_mashed_class();
    light                             .construct_mashed_class();
    blur                              .construct_mashed_class();
    fog                               .construct_mashed_class();

    for (environment_unk_148 &part : unk_148)
        part.construct_mashed_class();

    alternates.construct_mashed_class();
}

// sub_7DCF10
void environment::destruct_mashed_class() {
    ambient                           .destruct_mashed_class();
    adjustment                        .destruct_mashed_class();
    material_adjustment               .destruct_mashed_class();
    character_adjustment              .destruct_mashed_class();
    character_ambient_light_adjustment.destruct_mashed_class();
    character_diffuse_light_adjustment.destruct_mashed_class();
    outdoors                          .destruct_mashed_class();
    light                             .destruct_mashed_class();
    blur                              .destruct_mashed_class();
    fog                               .destruct_mashed_class();

    for (environment_unk_148 &part : unk_148)
        part.destruct_mashed_class();

    alternates.destruct_mashed_class();
}

// sub_7DD630
void environment::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    ambient                           .unmash(mash_info, this, buffer);
    adjustment                        .unmash(mash_info, this, buffer);
    material_adjustment               .unmash(mash_info, this, buffer);
    character_adjustment              .unmash(mash_info, this, buffer);
    character_ambient_light_adjustment.unmash(mash_info, this, buffer);
    character_diffuse_light_adjustment.unmash(mash_info, this, buffer);
    outdoors                          .unmash(mash_info, this, buffer);
    light                             .unmash(mash_info, this, buffer);
    blur                              .unmash(mash_info, this, buffer);
    fog                               .unmash(mash_info, this, buffer);

    for (environment_unk_148 &part : unk_148)
        part.unmash(mash_info, this, buffer);

    alternates.unmash(mash_info, this, buffer);
}
