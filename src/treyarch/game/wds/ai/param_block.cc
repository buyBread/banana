#include "treyarch/game/wds/ai/param_block.hh"
#include "treyarch/shared/mash/mash_info.hh"
#include "treyarch/shared/memory/heap.hh"

using namespace treyarch;

// sub_443E30
void param_block::construct_mashed_class() {
    // sub_58CA60
    lock.owner = 0;
    lock.state = 0;
    lock.depth = 0;

    if (params)
        params->construct_mashed_class();

    unk_018     = 0;
    owns_params = false;
}

// sub_4425C0
void param_block::destruct_mashed_class() {
    if (params && owns_params) {
        params->~vector();

        memory::heap::free(params);

        params = nullptr;
    }

    if (params) {
        params->destruct_mashed_class();

        params = nullptr;
    }
}

// sub_442610
void param_block::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    u8 class_mashed;
    mash_info->read_from_buffer(mash::SHARED_BUFFER, class_mashed);

    if (class_mashed == mash::member_class_mashed)
        mash_info->unmash_class(params, this, buffer);
    else
        params = nullptr;
}
