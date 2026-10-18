#include "treyarch/game/progression_change.hh"
#include "treyarch/shared/mash/mash_info.hh"

using namespace treyarch;

// sub_939990
void progression_change::construct_mashed_class() {
    name      .construct_mashed_class();
    expression.construct_mashed_class();
    added     .construct_mashed_class();
    removed   .construct_mashed_class();

    if (condition)
        mash::mash_info_struct::construct_class(condition);
}

// inlined @ sub_941160
// inlined @ sub_941320
void progression_change::destruct_mashed_class() {
    name      .destruct_mashed_class();
    expression.destruct_mashed_class();
    added     .destruct_mashed_class();
    removed   .destruct_mashed_class();

    if (condition) {
        condition->destruct_mashed_class();
        condition = nullptr;
    }
}

// sub_940480
void progression_change::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    name      .unmash(mash_info, this, buffer);
    expression.unmash(mash_info, this, buffer);
    added     .unmash(mash_info, this, buffer);
    removed   .unmash(mash_info, this, buffer);

    u8 class_mashed;
    mash_info->read_from_buffer(mash::SHARED_BUFFER, class_mashed);

    if (class_mashed == mash::member_class_mashed)
        mash_info->unmash_class(condition, this, buffer);
    else
        condition = nullptr;
}
