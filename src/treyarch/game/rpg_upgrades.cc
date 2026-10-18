#include "treyarch/game/rpg_upgrades.hh"
#include "treyarch/shared/mash/mash_info.hh"

using namespace treyarch;

// sub_927F70
void rpg_upgrade::destruct_mashed_class() {
    name        .destruct_mashed_class();
    title       .destruct_mashed_class();
    description .destruct_mashed_class();
    instructions.destruct_mashed_class();
    unk_30      .destruct_mashed_class();
    unk_3c      .destruct_mashed_class();
    unk_5c      .destruct_mashed_class();
    quest       .destruct_mashed_class();
}

// sub_934FE0
void rpg_upgrade::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    name        .unmash(mash_info, this, buffer);
    title       .unmash(mash_info, this, buffer);
    description .unmash(mash_info, this, buffer);
    instructions.unmash(mash_info, this, buffer);
    unk_30      .unmash(mash_info, this, buffer);
    unk_3c      .unmash(mash_info, this, buffer);
    unk_5c      .unmash(mash_info, this, buffer);
    quest       .unmash(mash_info, this, buffer);
}

// sub_9405C0
void rpg_upgrades::construct_mashed_class() {
    self = nullptr;

    upgrades.construct_mashed_class();
    unk_18  .construct_mashed_class();
}

// sub_93DBB0
void rpg_upgrades::destruct_mashed_class() {
    upgrades.destruct_mashed_class();
    unk_18  .destruct_mashed_class();
}

// sub_94C5B0
void rpg_upgrades::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    upgrades.unmash(mash_info, this, buffer);
    unk_18  .unmash(mash_info, this, buffer);
}
