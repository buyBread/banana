#include <new>

#include "game/environment_progression.hh"
#include "treyarch/mash/mash_info.hh"
#include "treyarch/mm/mm.hh"

using namespace treyarch;

// sub_7FB0D0
void environment_progression_stage::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    name      .unmash(mash_info, this, buffer);
    expression.unmash(mash_info, this, buffer);

    u8 class_mashed;
    mash_info->read_from_buffer(mash::SHARED_BUFFER, class_mashed);

    if (class_mashed == mash::member_class_mashed)
        mash_info->unmash_class(condition, this, buffer);
    else
        condition = nullptr;
}

// sub_801D40
void environment_progression::construct_mashed_class() {
    self = nullptr;

    stages.construct_mashed_class();
}

// sub_7FB150
void environment_progression::destruct_mashed_class() {
    stages.destruct_mashed_class();
}

// sub_801D70
void environment_progression::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    stages.unmash(mash_info, this, buffer);
}

// sub_7EB4B0
environment_progression_state::environment_progression_state() : progression(nullptr),
                                                                 unk_04(0),
                                                                 unk_08(-1),
                                                                 progression_level(0.0f) {}

// sub_428F60
void environment_progression_state::create_inst() {
    void* allocation = mm::allocate(sizeof(environment_progression_state));

    references::environment_progression_state.write
        (allocation ? new (allocation) environment_progression_state() : nullptr);
}
