#include "treyarch/game/box_trigger_info.hh"

using namespace treyarch;

// inlined @ sub_764A90
void box_trigger_info::construct_mashed_class() {
    ent_id.construct_mashed_class();
}

// inlined @ sub_749FF0
void box_trigger_info::destruct_mashed_class() {
    ent_id.destruct_mashed_class();
}

// inlined @ sub_764A90
void box_trigger_info::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    ent_id.unmash(mash_info, this, buffer);
}
