#include "game/light_behavior.hh"
#include "treyarch/mash/mash_info.hh"

using namespace treyarch;

// sub_7D78B0
void light_behavior::construct_mashed_class() {
    self = nullptr;

    if (flicker)
        flicker = (treyarch::flicker*)mash::mash_virtual_base::construct_class_helper(flicker);
}

// sub_7C9F90
void light_behavior::destruct_mashed_class() {
    if (flicker) {
        flicker->destruct_mashed_class();
        flicker = nullptr;
    }
}

// sub_7D9490
void light_behavior::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    u8 class_mashed;
    mash_info->read_from_buffer(mash::SHARED_BUFFER, class_mashed);

    if (class_mashed == mash::member_class_mashed)
        mash_info->unmash_class(flicker, this, buffer);
    else
        flicker = nullptr;
}
