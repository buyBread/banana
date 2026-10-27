#include "retail.hh"
#include "game/region_audio_box_set.hh"

using namespace treyarch;

// sub_903930
void audio_box_events::construct_mashed_class() {
    low_altitude_events      .construct_mashed_class();
    high_altitude_events     .construct_mashed_class();
    low_altitude_stop_events .construct_mashed_class();
    high_altitude_stop_events.construct_mashed_class();
    unk_1c                   .construct_mashed_class();
    unk_20                   .construct_mashed_class();
}

// sub_903930
void audio_box_events::destruct_mashed_class() {
    low_altitude_events      .destruct_mashed_class();
    high_altitude_events     .destruct_mashed_class();
    low_altitude_stop_events .destruct_mashed_class();
    high_altitude_stop_events.destruct_mashed_class();
    unk_1c                   .destruct_mashed_class();
    unk_20                   .destruct_mashed_class();
}

// inlined @ sub_905730
void audio_box_events::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    low_altitude_events      .unmash(mash_info, this, buffer);
    high_altitude_events     .unmash(mash_info, this, buffer);
    low_altitude_stop_events .unmash(mash_info, this, buffer);
    high_altitude_stop_events.unmash(mash_info, this, buffer);
    unk_1c                   .unmash(mash_info, this, buffer);
    unk_20                   .unmash(mash_info, this, buffer);
}

// inlined @ sub_905450
void audio_box::construct_mashed_class() {
    events.construct_mashed_class();
}

// inlined @ sub_905490
void audio_box::destruct_mashed_class() {
    events.destruct_mashed_class();
}

// inlined @ sub_905730
void audio_box::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    events.unmash(mash_info, this, buffer);
}

// inlined @ sub_905520
void region_audio_box_set_unk_4c::construct_mashed_class() {
    unk_00.construct_mashed_class();
}

// inlined @ sub_9056B0
void region_audio_box_set_unk_4c::destruct_mashed_class() {
    unk_00.destruct_mashed_class();
}

// inlined @ sub_905980
void region_audio_box_set_unk_4c::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    unk_00.unmash(mash_info, this, buffer);
}

// sub_905520
void region_audio_box_set::construct_mashed_class() {
    self = nullptr;

    obb_root   .construct_mashed_class();
    audio_boxes.construct_mashed_class();
    unk_4c     .construct_mashed_class();
}

// sub_9056B0
void region_audio_box_set::destruct_mashed_class() {
    // stops and unregisters every unk_4c game object first
    retail::sub_902C50(&unk_4c);

    obb_root   .destruct_mashed_class();
    audio_boxes.destruct_mashed_class();
    unk_4c     .destruct_mashed_class();
}

// sub_905980
void region_audio_box_set::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    obb_root   .unmash(mash_info, this, buffer);
    audio_boxes.unmash(mash_info, this, buffer);
    unk_4c     .unmash(mash_info, this, buffer);
}
