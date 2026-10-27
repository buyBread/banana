#include "game/cowbell_file.hh"
#include "treyarch/mash/mash_info.hh"

using namespace treyarch;

// sub_7D78B0
void cowbell_file::construct_mashed_class() {
    self = nullptr;

    if (cowbell)
        cowbell = (treyarch::cowbell*)mash::mash_virtual_base::construct_class_helper(cowbell);
}

// sub_7C9F90
void cowbell_file::destruct_mashed_class() {
    if (cowbell) {
        cowbell->destruct_mashed_class();
        cowbell = nullptr;
    }
}

// sub_7D9490
void cowbell_file::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    u8 class_mashed;
    mash_info->read_from_buffer(mash::SHARED_BUFFER, class_mashed);

    if (class_mashed == mash::member_class_mashed)
        mash_info->unmash_class(cowbell, this, buffer);
    else
        cowbell = nullptr;
}
