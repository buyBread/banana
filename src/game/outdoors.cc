#include "game/outdoors.hh"
#include "treyarch/mash/mash_info.hh"

using namespace treyarch;

// sub_7C3240
void outdoors_unk_004::destruct_mashed_class() {
    unk_00.destruct_mashed_class();
    unk_10.destruct_mashed_class();
    unk_20.destruct_mashed_class();

    for (gradient &part : unk_30)
        part.destruct_mashed_class();
}

// sub_7DAE40
void outdoors_unk_004::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    unk_00.unmash(mash_info, this, buffer);
    unk_10.unmash(mash_info, this, buffer);
    unk_20.unmash(mash_info, this, buffer);

    for (gradient &part : unk_30)
        part.unmash(mash_info, this, buffer);
}

// sub_7C3100
void celestial_body::destruct_mashed_class() {
    color               .destruct_mashed_class();
    intensity           .destruct_mashed_class();
    mesh                .destruct_mashed_class();
    scale_by_time_of_day.destruct_mashed_class();
    position            .destruct_mashed_class();
}

// sub_7DADB0
void celestial_body::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    color               .unmash(mash_info, this, buffer);
    intensity           .unmash(mash_info, this, buffer);
    mesh                .unmash(mash_info, this, buffer);
    scale_by_time_of_day.unmash(mash_info, this, buffer);
    position            .unmash(mash_info, this, buffer);
}

// sub_7C31E0
void cloud_colors::destruct_mashed_class() {
    lit_colors   .destruct_mashed_class();
    shadow_colors.destruct_mashed_class();
}

// sub_7C30A0
void sky_prop::destruct_mashed_class() {
    mesh    .destruct_mashed_class();
    position.destruct_mashed_class();
}

// sub_7DEC30
void outdoors::construct_mashed_class() {
    self = nullptr;

    for (outdoors_unk_004 &part : unk_004)
        part.construct_mashed_class();

    sun                    .construct_mashed_class();
    moon                   .construct_mashed_class();
    cloud_colors           .construct_mashed_class();
    ground_color           .construct_mashed_class();
    skylight_color         .construct_mashed_class();
    zenith_color           .construct_mashed_class();
    horizon_color          .construct_mashed_class();
    streetlight_color      .construct_mashed_class();
    horizon_mad            .construct_mashed_class();
    skyprops               .construct_mashed_class();
    current_day_sky_color  .construct_mashed_class();
    current_night_sky_color.construct_mashed_class();
}

// sub_7DCD40
void outdoors::destruct_mashed_class() {
    for (outdoors_unk_004 &part : unk_004)
        part.destruct_mashed_class();

    sun                    .destruct_mashed_class();
    moon                   .destruct_mashed_class();
    cloud_colors           .destruct_mashed_class();
    ground_color           .destruct_mashed_class();
    skylight_color         .destruct_mashed_class();
    zenith_color           .destruct_mashed_class();
    horizon_color          .destruct_mashed_class();
    streetlight_color      .destruct_mashed_class();
    horizon_mad            .destruct_mashed_class();
    skyprops               .destruct_mashed_class();
    current_day_sky_color  .destruct_mashed_class();
    current_night_sky_color.destruct_mashed_class();
}

// sub_7DE190
void outdoors::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    for (outdoors_unk_004 &part : unk_004)
        part.unmash(mash_info, this, buffer);

    sun                    .unmash(mash_info, this, buffer);
    moon                   .unmash(mash_info, this, buffer);
    cloud_colors           .unmash(mash_info, this, buffer);
    ground_color           .unmash(mash_info, this, buffer);
    skylight_color         .unmash(mash_info, this, buffer);
    zenith_color           .unmash(mash_info, this, buffer);
    horizon_color          .unmash(mash_info, this, buffer);
    streetlight_color      .unmash(mash_info, this, buffer);
    horizon_mad            .unmash(mash_info, this, buffer);
    skyprops               .unmash(mash_info, this, buffer);
    current_day_sky_color  .unmash(mash_info, this, buffer);
    current_night_sky_color.unmash(mash_info, this, buffer);
}
