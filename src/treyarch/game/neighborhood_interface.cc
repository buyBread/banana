#include "treyarch/game/neighborhood_interface.hh"
#include "treyarch/shared/mash/mash_info.hh"

using namespace treyarch;

// inlined @ sub_94CD50
void neighborhood_interface_unk_04::construct_mashed_class() {
    expression        .construct_mashed_class();
    unk_1c            .construct_mashed_class();
    meta_neighborhoods.construct_mashed_class();
    neighborhoods     .construct_mashed_class();

    if (condition)
        mash::mash_info_struct::construct_class(condition);
}

// inlined @ sub_941400
void neighborhood_interface_unk_04::destruct_mashed_class() {
    expression        .destruct_mashed_class();
    unk_1c            .destruct_mashed_class();
    meta_neighborhoods.destruct_mashed_class();
    neighborhoods     .destruct_mashed_class();

    if (condition) {
        condition->destruct_mashed_class();
        condition = nullptr;
    }
}

// sub_93DB00
void neighborhood_interface_unk_04::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    expression        .unmash(mash_info, this, buffer);
    unk_1c            .unmash(mash_info, this, buffer);
    meta_neighborhoods.unmash(mash_info, this, buffer);
    neighborhoods     .unmash(mash_info, this, buffer);

    u8 class_mashed;
    mash_info->read_from_buffer(mash::SHARED_BUFFER, class_mashed);

    if (class_mashed == mash::member_class_mashed)
        mash_info->unmash_class(condition, this, buffer);
    else
        condition = nullptr;
}

// sub_9524B0
void neighborhood_interface::construct_mashed_class() {
    self = nullptr;

    unk_04.construct_mashed_class();
}

// sub_9509A0
void neighborhood_interface::destruct_mashed_class() {
    unk_04.destruct_mashed_class();
}

// sub_9524E0
void neighborhood_interface::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    unk_04.unmash(mash_info, this, buffer);
}
