#include "game/quest_list.hh"
#include "treyarch/mash/mash_info.hh"

using namespace treyarch;

// inlined @ sub_7FFC70
void quest_unlock::construct_mashed_class() {
    expression.construct_mashed_class();
    quest     .construct_mashed_class();
    unk_18    .construct_mashed_class();

    if (condition)
        mash::mash_info_struct::construct_class(condition);
}

// inlined @ sub_7F8980
void quest_unlock::destruct_mashed_class() {
    expression.destruct_mashed_class();
    quest     .destruct_mashed_class();
    unk_18    .destruct_mashed_class();

    if (condition) {
        condition->destruct_mashed_class();
        condition = nullptr;
    }
}

// sub_7FAF90
void quest_unlock::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    expression.unmash(mash_info, this, buffer);
    quest     .unmash(mash_info, this, buffer);
    unk_18    .unmash(mash_info, this, buffer);

    u8 class_mashed;
    mash_info->read_from_buffer(mash::SHARED_BUFFER, class_mashed);

    if (class_mashed == mash::member_class_mashed)
        mash_info->unmash_class(condition, this, buffer);
    else
        condition = nullptr;
}

// sub_7FE270
void quest_giver::construct_mashed_class() {
    locations     .construct_mashed_class();
    name          .construct_mashed_class();
    localized_name.construct_mashed_class();
    unk_30        .construct_mashed_class();
    unk_3c        .construct_mashed_class();
    unk_48        .construct_mashed_class();
    unk_58        .construct_mashed_class();
    greeting      .construct_mashed_class();
    unk_70        .construct_mashed_class();
    unk_7c        .construct_mashed_class();

    unk_94 = 0;
}

// inlined @ sub_8011A0
void quest_giver::destruct_mashed_class() {
    locations     .destruct_mashed_class();
    name          .destruct_mashed_class();
    localized_name.destruct_mashed_class();
    unk_30        .destruct_mashed_class();
    unk_3c        .destruct_mashed_class();
    unk_48        .destruct_mashed_class();
    unk_58        .destruct_mashed_class();
    greeting      .destruct_mashed_class();
    unk_70        .destruct_mashed_class();
    unk_7c        .destruct_mashed_class();
}

// sub_7FF5B0
void quest_giver::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    locations     .unmash(mash_info, this, buffer);
    name          .unmash(mash_info, this, buffer);
    localized_name.unmash(mash_info, this, buffer);
    unk_30        .unmash(mash_info, this, buffer);
    unk_3c        .unmash(mash_info, this, buffer);
    unk_48        .unmash(mash_info, this, buffer);
    unk_58        .unmash(mash_info, this, buffer);
    greeting      .unmash(mash_info, this, buffer);
    unk_70        .unmash(mash_info, this, buffer);
    unk_7c        .unmash(mash_info, this, buffer);
}

// sub_802410
void quest_list::construct_mashed_class() {
    self = nullptr;

    unlocks     .construct_mashed_class();
    descriptions.construct_mashed_class();
    givers      .construct_mashed_class();
    unk_40      .construct_mashed_class();
    unk_50      .construct_mashed_class();
}

// sub_802170
void quest_list::destruct_mashed_class() {
    unlocks     .destruct_mashed_class();
    descriptions.destruct_mashed_class();
    givers      .destruct_mashed_class();
    unk_40      .destruct_mashed_class();
    unk_50      .destruct_mashed_class();
}

// sub_8024D0
void quest_list::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    unlocks     .unmash(mash_info, this, buffer);
    descriptions.unmash(mash_info, this, buffer);
    givers      .unmash(mash_info, this, buffer);
    unk_40      .unmash(mash_info, this, buffer);
    unk_50      .unmash(mash_info, this, buffer);
}
