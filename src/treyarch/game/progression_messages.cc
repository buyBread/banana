#include "treyarch/game/progression_messages.hh"
#include "treyarch/shared/mash/mash_info.hh"

using namespace treyarch;

// sub_934F50
void progression_messages_unk_04::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    expression.unmash(mash_info, this, buffer);

    u8 class_mashed;
    mash_info->read_from_buffer(mash::SHARED_BUFFER, class_mashed);

    if (class_mashed == mash::member_class_mashed)
        mash_info->unmash_class(condition, this, buffer);
    else
        condition = nullptr;
}

// sub_94C550
void progression_messages::construct_mashed_class() {
    self = nullptr;

    messages.construct_mashed_class();
}

// sub_934FC0
void progression_messages::destruct_mashed_class() {
    messages.destruct_mashed_class();
}

// sub_94C570
void progression_messages::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    messages.unmash(mash_info, this, buffer);
}
