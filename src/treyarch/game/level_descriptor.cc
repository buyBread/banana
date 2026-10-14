#include "retail.hh"
#include "treyarch/app/app.hh"
#include "treyarch/game/level_descriptor.hh"
#include "treyarch/shared/mash/mash_info.hh"
#include "treyarch/shared/memory/heap.hh"

using namespace treyarch;

// sub_804E20
level_descriptor::~level_descriptor() {
    if (!references::pack_mode.read())
        return;

    if (blocks_in_this_level) {
        ((u8*)blocks_in_this_level)[0x0C] = 0;

        retail::sub_7FB170((i32**)blocks_in_this_level);
        retail::sub_7FEE10((i32**)blocks_in_this_level, 1); // deleting destructor
    }

    if (sounds_by_mission) {
        sounds_by_mission->~map();

        memory::heap::free(sounds_by_mission);
    }
}

// inlined @ sub_937950
void level_descriptor::construct_mashed_class() {
    level_name      .construct_mashed_class();
    path            .construct_mashed_class();
    short_level_name.construct_mashed_class();
}

// inlined @ sub_804EF0
void level_descriptor::destruct_mashed_class() {
    level_name      .destruct_mashed_class();
    path            .destruct_mashed_class();
    short_level_name.destruct_mashed_class();
}

// inlined @ sub_7FB980
void level_descriptor::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    level_name      .unmash(mash_info, this, buffer);
    path            .unmash(mash_info, this, buffer);
    short_level_name.unmash(mash_info, this, buffer);
}

// sub_7FEDF0
void level_descriptor_set::construct_mashed_class() {
    self = nullptr;

    descriptors.construct_mashed_class();
}

// sub_804F90
void level_descriptor_set::destruct_mashed_class() {
    descriptors.destruct_mashed_class();
}

// sub_7FFD50
void level_descriptor_set::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    descriptors.unmash(mash_info, this, buffer);
}
