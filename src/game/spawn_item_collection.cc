#include "game/spawn_item_collection.hh"
#include "treyarch/mash/mash_info.hh"

using namespace treyarch;

// inlined @ sub_93EF60
void spawn_item_collection_unk_38::construct_mashed_class() {
    name      .construct_mashed_class();
    expression.construct_mashed_class();

    if (condition)
        mash::mash_info_struct::construct_class(condition);
}

// inlined @ sub_928E50
void spawn_item_collection_unk_38::destruct_mashed_class() {
    name      .destruct_mashed_class();
    expression.destruct_mashed_class();

    if (condition) {
        condition->destruct_mashed_class();
        condition = nullptr;
    }
}

// sub_934A70
void spawn_item_collection_unk_38::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    name      .unmash(mash_info, this, buffer);
    expression.unmash(mash_info, this, buffer);

    u8 class_mashed;
    mash_info->read_from_buffer(mash::SHARED_BUFFER, class_mashed);

    if (class_mashed == mash::member_class_mashed)
        mash_info->unmash_class(condition, this, buffer);
    else
        condition = nullptr;
}

// inlined @ sub_94CC90
void spawn_item_collection_unk_4c::construct_mashed_class() {
    name          .construct_mashed_class();
    expression    .construct_mashed_class();
    no_spawn_areas.construct_mashed_class();

    if (condition)
        mash::mash_info_struct::construct_class(condition);
}

// inlined @ sub_93F370
void spawn_item_collection_unk_4c::destruct_mashed_class() {
    name          .destruct_mashed_class();
    expression    .destruct_mashed_class();
    no_spawn_areas.destruct_mashed_class();

    if (condition) {
        condition->destruct_mashed_class();
        condition = nullptr;
    }
}

// sub_93D960
void spawn_item_collection_unk_4c::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    name          .unmash(mash_info, this, buffer);
    expression    .unmash(mash_info, this, buffer);
    no_spawn_areas.unmash(mash_info, this, buffer);

    u8 class_mashed;
    mash_info->read_from_buffer(mash::SHARED_BUFFER, class_mashed);

    if (class_mashed == mash::member_class_mashed)
        mash_info->unmash_class(condition, this, buffer);
    else
        condition = nullptr;
}

// sub_952370
void spawn_item_collection::construct_mashed_class() {
    self = nullptr;

    name   .construct_mashed_class();
    changes.construct_mashed_class();
    unk_24 .construct_mashed_class();
    unk_38 .construct_mashed_class();
    unk_4c .construct_mashed_class();
}

// sub_9503F0
void spawn_item_collection::destruct_mashed_class() {
    name   .destruct_mashed_class();
    changes.destruct_mashed_class();
    unk_24 .destruct_mashed_class();
    unk_38 .destruct_mashed_class();
    unk_4c .destruct_mashed_class();
}

// sub_953E10
void spawn_item_collection::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    name   .unmash(mash_info, this, buffer);
    changes.unmash(mash_info, this, buffer);
    unk_24 .unmash(mash_info, this, buffer);
    unk_38 .unmash(mash_info, this, buffer);
    unk_4c .unmash(mash_info, this, buffer);
}
