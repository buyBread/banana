#include "treyarch/game/summon_progression.hh"
#include "treyarch/shared/mash/mash_info.hh"
#include "treyarch/shared/memory/heap.hh"

using namespace treyarch;

// sub_934D90
void summon_level::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    unk_00.unmash(mash_info, this, buffer);
    unk_0c.unmash(mash_info, this, buffer);

    u8 class_mashed;
    mash_info->read_from_buffer(mash::SHARED_BUFFER, class_mashed);

    if (class_mashed == mash::member_class_mashed)
        mash_info->unmash_class(condition, this, buffer);
    else
        condition = nullptr;
}

// sub_9564D0
void summon_progression::construct_mashed_class() {
    self = nullptr;

    changes.construct_mashed_class();
    summons.construct_mashed_class();

    summons_by_name = nullptr;
}

// sub_9550E0
void summon_progression::destruct_mashed_class() {
    if (summons_by_name) {
        summons_by_name->~map();

        memory::heap::free(summons_by_name);
    }

    changes.destruct_mashed_class();
    summons.destruct_mashed_class();
}

// sub_955150
void summon_progression::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    changes.unmash(mash_info, this, buffer);
    summons.unmash(mash_info, this, buffer);
}
